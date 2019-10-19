//
// Created by Romain FOUYER on 2019-09-18.
//

#include "BabelWindow.hpp"
#include "audio/PortAudioAPI.hpp"
#include "gui/utilities/AssetsManager.hpp"
#include "gui/utilities/UserUtilities.hpp"
#include "network/Datagram.hpp"
#include <map>
#include <QtWidgets/QMessageBox>
#include <QTimer>
#include <protocol/BabelDatagram.hpp>
#include <codec/OpusCodecAPI.hpp>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	#include <ws2tcpip.h>
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif 

b12software::babel::BabelWindow::BabelWindow(b12software::network::NetworkClient &client)
: QMainWindow(),
  _user("", ""),
  _parentWidget(new QStackedWidget),
  _loginWidget(new b12software::babel::LoginWidget(_parentWidget)),
  _mainWidget(nullptr),
  _signUpWidget(new b12software::babel::SignUpWidget(_parentWidget)),
  _loadingWidget(new b12software::babel::LoadingWidget(_parentWidget)),
  _requestCallDialog(nullptr),
  _client(client),
  _timer(new QTimer),
  _udpTimer(new QTimer),
  _opusAPI(),
  _audioApi(new PortAudioAPI(2, 2)),
  _inputBuffer(nullptr),
  _frameSize(),
  _bufferDynamicSize(),
  _inputBufferSize(0),
  _lastReceivedTimestamp(0)
{
    setMinimumSize(800, 600);
    setWindowTitle("Babel");

    setupCallbacks();
    _timer->start(100);
    _parentWidget->addWidget(_loginWidget);
    _parentWidget->addWidget(_signUpWidget);
    _parentWidget->addWidget(_loadingWidget);
    _parentWidget->setCurrentWidget(_loginWidget);
    setCentralWidget(_parentWidget);

    // Audio api initialisation

    _frameSize = _audioApi->getNbInputChannels() * sizeof(b12software::babel::IAudioAPI::Sample);
    _bufferDynamicSize = 10 * static_cast<int>(_audioApi->getSampleRate()) * _frameSize;

    _inputBuffer = new b12software::babel::IAudioAPI::Sample[_bufferDynamicSize];
    memset(_inputBuffer, 0, _bufferDynamicSize * sizeof(*_inputBuffer));

    _opusAPI.setBitRate(64000);
    _opusAPI.setSamplingRate(48000);
    _opusAPI.initializeEncoder(2);
    _opusAPI.initializeDecoder(2);
    _audioApi->setSampleRate(48000);

}

b12software::babel::BabelWindow::~BabelWindow()
{
    delete _loginWidget;
    delete _signUpWidget;
    delete _parentWidget;
    delete _audioApi;
    delete [] _inputBuffer;
}

void b12software::babel::BabelWindow::openCall()
{
    return;
}

void b12software::babel::BabelWindow::keyPressEvent(QKeyEvent *pe)
{
    if (pe->key() == Qt::Key_Return && _parentWidget->currentWidget() == _loginWidget) loginButtonClicked();
    if (pe->key() == Qt::Key_Return && _parentWidget->currentWidget() == _signUpWidget) registerButtonClicked();
    if (pe->key() == Qt::Key_Escape && _parentWidget->currentWidget() == _signUpWidget) backButtonClicked();
}

void b12software::babel::BabelWindow::loginButtonClicked()
{
    std::map<std::string, std::string> account({
        {"username", _loginWidget->getUsername()},
        {"password", _loginWidget->getPassword()}
    });

    if (account["username"].empty() || account["password"].empty()) {
        QMessageBox msgBox;
        msgBox.setText("Incorrect username or password.\nTry again.");
        msgBox.exec();
        return;
    }

    b12software::babel::BabelPacket loginPacket;
    loginPacket.createLoginUserPacket(account["username"], account["password"]);

    if (!loginPacket.isValid())
        return;

    _client.getClient()->send(loginPacket);
}

void b12software::babel::BabelWindow::signUpButtonClicked()
{
    _parentWidget->setCurrentWidget(_signUpWidget);
}

void b12software::babel::BabelWindow::backButtonClicked()
{
    _parentWidget->setCurrentWidget(_loginWidget);
}

void b12software::babel::BabelWindow::registerButtonClicked()
{
    std::map<std::string, std::string> accountRegistered({
            {"username", _signUpWidget->getUsernameField()->text().toUtf8().constData()},
            {"password", _signUpWidget->getPasswordField()->text().toUtf8().constData()},
            {"confirm_password", _signUpWidget->getConfirmPasswordField()->text().toUtf8().constData()},
            {"email_address", _signUpWidget->getEmailField()->text().toUtf8().constData()},
            {"birth_date", _signUpWidget->getDateField()->text().toUtf8().constData()}
    });

    if (accountRegistered["username"].empty() || accountRegistered["password"].empty() ||
        accountRegistered["confirm_password"].empty() || accountRegistered["confirm_password"].empty()) {
        QMessageBox errorBox;
        errorBox.setText("One field is empty. Please fill it.");
        errorBox.exec();
        return;
    }

    if (accountRegistered["password"] != accountRegistered["confirm_password"]) {
        QMessageBox errorBox;
        errorBox.setText("Password doesn't match");
        errorBox.exec();
        return;
    }

    b12software::babel::BabelPacket signUpPacket;
    signUpPacket.createRegisterNewUserPacket(
            accountRegistered["username"],
            _signUpWidget->getDateField()->dateTime().toTime_t(),
            accountRegistered["email_address"],
            accountRegistered["password"]
    );

    if (!signUpPacket.isValid())
        return;

    _client.getClient()->send(signUpPacket);
}

void b12software::babel::BabelWindow::updateOnRequest()
{
    std::map<b12software::babel::BabelPacket::BabelProtocolOpCode, void (b12software::babel::BabelWindow::*)(b12software::babel::BabelPacket &)> callback = {
            {b12software::babel::BabelPacket::BPOpCodeSendBinaryResponse, &b12software::babel::BabelWindow::handleBinaryResponse},
            {b12software::babel::BabelPacket::BPOpCodeUserAuthResponse, &b12software::babel::BabelWindow::handleAuthResponse},
            {b12software::babel::BabelPacket::BPOpCodeListContactInfoResponse, &b12software::babel::BabelWindow::handleContactInfoResponse},
            {b12software::babel::BabelPacket::BPOpCodeCallUserResponse, &b12software::babel::BabelWindow::handleCallUserResponse},
            {b12software::babel::BabelPacket::BPOpCodeListContactInfoResponse, &b12software::babel::BabelWindow::handleListContactResponse},
            {b12software::babel::BabelPacket::BPOpCodeRequestUserContactResponse, &b12software::babel::BabelWindow::handleAddUserResponse},
            {b12software::babel::BabelPacket::BPOpCodeRequestContactRequest, &b12software::babel::BabelWindow::handleFriendRequestResponse},
            {b12software::babel::BabelPacket::BPOpCodeListContactRequestListResponse, &b12software::babel::BabelWindow::handleAllFriendRequestResponse},
            {b12software::babel::BabelPacket::BPOpCodeListContactListResponse, &b12software::babel::BabelWindow::handleFriendListResponse},
            {b12software::babel::BabelPacket::BPOpCodeReceivedCall, &b12software::babel::BabelWindow::handleReceiveCall},
            {b12software::babel::BabelPacket::BPOpCodeDeletedUserContactListResponse, &b12software::babel::BabelWindow::handleDeleteContact},
            {b12software::babel::BabelPacket::BPOpCodeUserHangedUp, &b12software::babel::BabelWindow::handleHangUp},
            {b12software::babel::BabelPacket::BPOpCodeReceivedMessage, &b12software::babel::BabelWindow::handleReceiveMessage},
            {b12software::babel::BabelPacket::BPOpCodeRetrieveMessageListResponse, &b12software::babel::BabelWindow::handleRetrieveAllMessages}
    };
    b12software::babel::BabelPacket receivePacket;

    _client.getClient()->receive(receivePacket);

    if (receivePacket.isValid()) {
        auto it = callback.find(receivePacket.getPacketOpCode());
        if (it != callback.end())
            (*this.*(it->second))(receivePacket);
    }
}

void b12software::babel::BabelWindow::handleBinaryResponse(b12software::babel::BabelPacket &receivedPacket)
{
    bool response = false;
    std::string description;

    receivedPacket.extractSendBinaryResponsePacket(response, description);

    std::cout << "description: " << description << std::endl;
    if (_parentWidget->currentWidget() == _mainWidget) {
        if (!response) {
            QMessageBox msgBox;

            msgBox.setText(QString::fromUtf8(description.data()));
            msgBox.exec();
        } else {

            if (description == "Message sended successfully.")
                return;

            if (!_user.isInCall()) {
                int size = _mainWidget->getListWidget()->count();

                for (int i = 0; i < size; i++)
                    _mainWidget->getListWidget()->takeItem(0);
                _mainWidget->fillFriendsList();
                _mainWidget->fillFriendRequestList();
            }

            if (b12software::babel::UserUtilities::getInstance().getCalledId() != -1) {
                for (auto &it : _mainWidget->getFriendList()) {
                    if (it->getTargetUser().getId() == b12software::babel::UserUtilities::getInstance().getCalledId()) {
                        it->getChatWidget()->getCallDialog()->show();
                        _user.setInCall(true);
                    }
                }
            }

            if (description == "Friend request sended successfully.") {
                QMessageBox msgBox;

                msgBox.setText(QString::fromUtf8(description.data()));
                msgBox.exec();
            }
        }
    } else if (_parentWidget->currentWidget() == _signUpWidget) {
        if (!response) {
            QMessageBox msgBox;

            msgBox.setText(QString::fromUtf8(description.data()));
            msgBox.exec();
        } else {
            _parentWidget->setCurrentWidget(_loginWidget);
        }
    } else {
        QMessageBox msgBox;

        msgBox.setText(QString::fromUtf8(description.data()));
        msgBox.exec();
    }
}

void b12software::babel::BabelWindow::handleAuthResponse(b12software::babel::BabelPacket &receivedPacket)
{
    if (_parentWidget->currentWidget() == _loginWidget) {
        long userId = 0;

        receivedPacket.extractUserAuthResponsePacket(userId);
        _user.setId(userId);


        b12software::babel::BabelPacket profilePacket;
        profilePacket.createListContactInfoPacket(userId);

        if (!profilePacket.isValid())
            return;

        _client.getClient()->send(profilePacket);

        _loadingWidget->getMovie()->start();
        _parentWidget->setCurrentWidget(_loadingWidget);
    }
}

void b12software::babel::BabelWindow::handleContactInfoResponse(b12software::babel::BabelPacket &receivedPacket)
{
    std::tuple<long, std::string, long, std::string> userInfo;

    receivedPacket.extractListContactInfoResponsePacket(std::get<0>(userInfo), std::get<1>(userInfo), std::get<2>(userInfo), std::get<3>(userInfo));

    if (_parentWidget->currentWidget() == _loadingWidget) {
        _user.setId(std::get<0>(userInfo));
        _user.setUsername(std::get<1>(userInfo));
        _user.setAvatarPath(b12software::babel::AssetsManager::getInstance()["avatar"]);
        _user.setEmailAddress(std::get<3>(userInfo));

        _mainWidget = new b12software::babel::MainWidget(0, _user, _client, _udpTimer);
        _parentWidget->addWidget(_mainWidget);
        _parentWidget->setCurrentWidget(_mainWidget);
    } else {
        b12software::babel::UserUtilities &utilities = b12software::babel::UserUtilities::getInstance();

        if (utilities.getCalledUser().getId() == std::get<0>(userInfo) && b12software::babel::UserUtilities::getInstance().getWaitingCall()) {
            utilities.getCalledUser().setUsername(std::get<1>(userInfo));
            utilities.getCalledUser().setAvatarPath(b12software::babel::AssetsManager::getInstance()["avatar"]);
            utilities.getCalledUser().setEmailAddress(std::get<3>(userInfo));
            _requestCallDialog = new b12software::babel::RequestCallDialog(utilities.getCalledUser(), _client);
            connect(_requestCallDialog->getAcceptButton(), &QPushButton::clicked, this, &b12software::babel::BabelWindow::acceptCallButton);
            _requestCallDialog->show();
            return;
        }
        for (auto &fr : _mainWidget->getPendingFriendList()) {
            if (fr->getTargetUser().getId() == std::get<0>(userInfo)) {
                fr->getTargetUser().setUsername(std::get<1>(userInfo));
                fr->getTargetUser().setAvatarPath(b12software::babel::AssetsManager::getInstance()["avatar"]);
                fr->getTargetUser().setEmailAddress(std::get<3>(userInfo));
                fr->getFriendRequestWidget()->refreshUser();
                fr->getChatWidget()->setUser(fr->getTargetUser());
                _mainWidget->fillPendingListWidget();
                return;
            }
        }
        for (auto &fr : _mainWidget->getFriendList()) {
            if (fr->getTargetUser().getId() == std::get<0>(userInfo)) {
                fr->getTargetUser().setUsername(std::get<1>(userInfo));
                fr->getTargetUser().setAvatarPath(b12software::babel::AssetsManager::getInstance()["avatar"]);
                fr->getTargetUser().setEmailAddress(std::get<3>(userInfo));
                fr->refreshUser();
                _mainWidget->fillListWidget();
                return;
            }
        }
    }
}

void b12software::babel::BabelWindow::handleCallUserResponse(b12software::babel::BabelPacket &receivedPacket)
{
    std::tuple<long, bool, int, int> response;

    receivedPacket.extractCallUserResponsePacket(
            std::get<0>(response),
            std::get<1>(response),
            std::get<2>(response),
            std::get<3>(response)
    );

    if (!std::get<1>(response)) {
        for (auto &it : _mainWidget->getFriendList()) {
            if (it->getTargetUser().getId() == b12software::babel::UserUtilities::getInstance().getCalledId()) {
                it->getChatWidget()->getCallDialog()->close();
                _user.setInCall(false);
            }
        }
    } else {
        for (auto &it : _mainWidget->getFriendList()) {
            if (it->getTargetUser().getId() == b12software::babel::UserUtilities::getInstance().getCalledId()) {
                struct in_addr ip_addr;
                ip_addr.s_addr = std::get<2>(response);
                it->getChatWidget()->getCallDialog()->show();
                _client.getUdpSocket()->registerHost({inet_ntoa(ip_addr), std::get<3>(response)});
                _client.getUdpSocket()->bind(b12software::babel::BabelPacket::udpPort);
                _lastReceivedTimestamp = std::time(nullptr);
                _udpTimer->start(1);
                connect(_udpTimer, &QTimer::timeout, this, &b12software::babel::BabelWindow::udpResponse);
                _user.setInCall(true);
            }
        }
    }
}

void b12software::babel::BabelWindow::handleListContactResponse([[maybe_unused]]b12software::babel::BabelPacket &receivedPacket)
{
    b12software::babel::BabelPacket callUserResponsePacket;

    std::vector<long> _idList;

    callUserResponsePacket.extractListContactListResponsePacket(_idList);

    std::vector<long> unknownId;

    for (auto &id : _idList) {
        for (auto &it : _mainWidget->getFriendList()) {
            if (id == it->getTargetUser().getId()) {
                unknownId.push_back(id);
            }
        }
    }
    for (auto &id : unknownId) {
        b12software::babel::BabelPacket contactInfoPacket;

        contactInfoPacket.createListContactInfoPacket(id);
        _client.getClient()->send(contactInfoPacket);
    }
}

void b12software::babel::BabelWindow::handleAddUserResponse([[maybe_unused]]b12software::babel::BabelPacket &receivedPacket)
{
    int size = _mainWidget->getListWidget()->count();

    for (int i = 0; i < size; i++)
        _mainWidget->getListWidget()->takeItem(0);

    _mainWidget->fillFriendsList();
    _mainWidget->fillFriendRequestList();
}

void b12software::babel::BabelWindow::handleFriendRequestResponse(b12software::babel::BabelPacket &receivedPacket)
{
    long userId = 0;

    receivedPacket.extractRequestContactRequestPacket(userId);
    b12software::babel::BabelClientUser client("", b12software::babel::AssetsManager::getInstance()["avatar"], "", userId);
    _mainWidget->getPendingFriendList().push_back(new b12software::babel::UserListItemWidget(_user, client, _client, _udpTimer));

    b12software::babel::BabelPacket profilePacket;
    profilePacket.createListContactInfoPacket(userId);

    _client.getClient()->send(profilePacket);
}

void b12software::babel::BabelWindow::handleAllFriendRequestResponse(b12software::babel::BabelPacket &receivedPacket)
{
    std::vector<long> _userIds;

    receivedPacket.extractListContactRequestListResponsePacket(_userIds);
    for (auto &id : _userIds) {
        b12software::babel::BabelClientUser client("", b12software::babel::AssetsManager::getInstance()["avatar"], "", id);
        _mainWidget->getPendingFriendList().push_back(new b12software::babel::UserListItemWidget(_user, client, _client, _udpTimer));

        b12software::babel::BabelPacket profilePacket;
        profilePacket.createListContactInfoPacket(id);

        _client.getClient()->send(profilePacket);
    }
}

void b12software::babel::BabelWindow::handleFriendListResponse(b12software::babel::BabelPacket &receivedPacket)
{
    std::vector<long> ids;
    receivedPacket.extractListContactListResponsePacket(ids);

    for (auto &id : ids) {
        b12software::babel::BabelClientUser client("", b12software::babel::AssetsManager::getInstance()["avatar"], "", id);
        _mainWidget->getFriendList().push_back(new b12software::babel::UserListItemWidget(_user, client, _client, _udpTimer));

        b12software::babel::BabelPacket profilePacket;
        profilePacket.createListContactInfoPacket(id);

        _client.getClient()->send(profilePacket);
    }
}

void b12software::babel::BabelWindow::handleReceiveCall(b12software::babel::BabelPacket &receivedPacket)
{
    std::tuple<long, int, int> receivedInfo;
    receivedPacket.extractReceivedCallPacket(std::get<0>(receivedInfo), std::get<1>(receivedInfo), std::get<2>(receivedInfo));

    b12software::babel::UserUtilities::getInstance().setCommunicationInfo(std::get<1>(receivedInfo), std::get<2>(receivedInfo));
    b12software::babel::UserUtilities::getInstance().getCalledUser().setId(std::get<0>(receivedInfo));
    b12software::babel::UserUtilities::getInstance().setCalledId(std::get<0>(receivedInfo));

    b12software::babel::BabelPacket profilePacket;
    profilePacket.createListContactInfoPacket(std::get<0>(receivedInfo));

    if (!profilePacket.isValid())
        return;

    _client.getClient()->send(profilePacket);

    struct in_addr ip_addr;
    ip_addr.s_addr = std::get<1>(receivedInfo);
    _client.getUdpSocket()->registerHost({inet_ntoa(ip_addr), std::get<2>(receivedInfo)});
    b12software::babel::UserUtilities::getInstance().setWaitingCall(true);
}

void b12software::babel::BabelWindow::acceptCallButton()
{
    _requestCallDialog->close();
    b12software::babel::BabelPacket callPacketAccept;

    callPacketAccept.createSendResponseCallPacket(_requestCallDialog->getUser().getId(), true);
    if (!callPacketAccept.isValid())
        return;
    _client.getClient()->send(callPacketAccept);

    for (auto &it : _mainWidget->getFriendList()) {
        if (it->getTargetUser().getId() == b12software::babel::UserUtilities::getInstance().getCalledId()) {
            it->getChatWidget()->getCallDialog()->show();
            _client.getUdpSocket()->bind(b12software::babel::BabelPacket::udpPort);
            _udpTimer->start(1);
            connect(_udpTimer, &QTimer::timeout, this, &b12software::babel::BabelWindow::udpResponse);
            _user.setInCall(true);
        }
    }
}

void b12software::babel::BabelWindow::udpResponse()
{
    // Send Voice

    while (_audioApi->getNumberOfInputBufferedFrames() >= 480) {
        _inputBufferSize = _audioApi->extractInputFrames(480, _inputBuffer) * _frameSize;
        auto compressed = new char[_bufferDynamicSize];
        memset(compressed, 0, _bufferDynamicSize * sizeof(*compressed));
        int compressedRealSize = _opusAPI.encode(_inputBuffer, 480, reinterpret_cast<unsigned char *>(compressed), _bufferDynamicSize);
        BabelDatagram sendDatagram;
        sendDatagram.createBabelAudioDatagram(_user.getId(), std::time(nullptr), compressedRealSize, compressed);
        _client.getUdpSocket()->send(sendDatagram);
        memset(_inputBuffer, 0, _bufferDynamicSize * sizeof(*_inputBuffer));
        _inputBufferSize = 0;
        delete [] compressed;
    }

    // Receive voice

    while (_client.getUdpSocket()->hasPendingDatagrams()) {
        b12software::network::Datagram datagram = _client.getUdpSocket()->receive();
        BabelDatagram babelDatagram;

        long userId = 0;
        long timeStamp = 0;
        size_t audioDataSize = 0;
        char *compressedAudio = nullptr;

        babelDatagram.setData(datagram.getData(), datagram.getDatagramSize());
        babelDatagram.setDestination(datagram.getHostInfos());
        babelDatagram.extractBabelAudioDatagram(userId, timeStamp, audioDataSize, compressedAudio);

        if (timeStamp < _lastReceivedTimestamp) {
            delete [] compressedAudio;
            return;
        }
        _lastReceivedTimestamp = timeStamp;
        auto outputBuffer = new b12software::babel::IAudioAPI::Sample[_bufferDynamicSize];
        _opusAPI.decode(reinterpret_cast<const unsigned char *>(compressedAudio), audioDataSize, outputBuffer, 480);
        delete [] compressedAudio;
        _audioApi->insertOutputFrames(480, outputBuffer) * _frameSize;
        delete [] outputBuffer;
    }
}

void b12software::babel::BabelWindow::handleDeleteContact([[maybe_unused]]b12software::babel::BabelPacket &receivedPacket)
{
    int size = _mainWidget->getListWidget()->count();

    for (int i = 0; i < size; i++)
        _mainWidget->getListWidget()->takeItem(0);

    _mainWidget->fillFriendsList();
    _mainWidget->fillFriendRequestList();
}

void b12software::babel::BabelWindow::handleHangUp(b12software::babel::BabelPacket &receivedPacket)
{
    long id;

    receivedPacket.extractHangupCallPacket(id);

    for (auto &it : _mainWidget->getFriendList()) {
        if (it->getTargetUser().getId() == id) {
            _user.setInCall(false);
            b12software::babel::UserUtilities::getInstance().setCalledId(-1);
            b12software::babel::UserUtilities::getInstance().setWaitingCall(false);
            it->getChatWidget()->getCallDialog()->close();
            _client.getUdpSocket()->unbind();
        }
    }
}

void b12software::babel::BabelWindow::setupCallbacks()
{
    connect(_timer, &QTimer::timeout, this, &b12software::babel::BabelWindow::updateOnRequest);
    connect(_loginWidget->getLoginButton(), &QPushButton::clicked, this, &b12software::babel::BabelWindow::loginButtonClicked);
    connect(_loginWidget->getSignUpButton(), &QPushButton::clicked, this, &b12software::babel::BabelWindow::signUpButtonClicked);
    connect(_signUpWidget->getBackButton(), &QPushButton::clicked, this, &b12software::babel::BabelWindow::backButtonClicked);
    connect(_signUpWidget->getSubmitButton(), &QPushButton::clicked, this, &b12software::babel::BabelWindow::registerButtonClicked);
}

void b12software::babel::BabelWindow::handleReceiveMessage(b12software::babel::BabelPacket &receivedPacket)
{
    long userId;
    std::string message;

    receivedPacket.extractReceivedMessagePacket(userId, message);

    auto messageItem = new QTableWidgetItem(QString::fromUtf8(message.data()));

    for (auto &fren : _mainWidget->getFriendList()) {
        if (fren->getTargetUser().getId() == userId) {
            fren->getChatWidget()->getMessages().emplace_back(std::pair<uint64_t, QTableWidgetItem *>({static_cast<uint64_t>(userId), messageItem}));
            fren->getChatWidget()->getTableWidget()->insertRow(fren->getChatWidget()->getCurrentMessageIndex());
            fren->getChatWidget()->getTableWidget()->setItem(fren->getChatWidget()->getCurrentMessageIndex(), 0, messageItem);
            fren->getChatWidget()->getCurrentMessageIndex()++;
        }
    }
}

void b12software::babel::BabelWindow::handleRetrieveAllMessages(b12software::babel::BabelPacket &receivedPacket)
{
    long userId;
    std::vector<std::pair<uint64_t, std::string>> messageList;

    receivedPacket.extractRetrieveMessageListResponsePacket(userId, messageList);

    for (auto &it : _mainWidget->getFriendList()) {
        if (it->getTargetUser().getId() == userId) {
            it->getChatWidget()->addMessages(messageList);
            return;
        }
    }
}
