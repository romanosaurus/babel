//
// Created by Romain FOUYER on 2019-09-26.
//

#include <iostream>

#include "gui/widgets/ChatWidget.hpp"
#include "gui/utilities/AssetsManager.hpp"
#include "protocol/BabelPacket.hpp"
#include "gui/utilities/UserUtilities.hpp"

#include <QtWidgets/QScrollBar>
#include <QtWidgets/QHeaderView>

b12software::babel::ChatWidget::ChatWidget(QWidget *parent, b12software::babel::BabelClientUser &appUser, b12software::babel::BabelClientUser &chatUser, b12software::network::NetworkClient &client, QTimer *udpTimer)
: QWidget(parent),
  _userInfoLayout(new QGridLayout),
  _chatLayout(new QGridLayout),
  _inputLayout(new QGridLayout),
  _buttonLayout(new QGridLayout),
  _inputBox(new QLineEdit),
  _tableWidget(new QTableWidget),
  _username(new b12software::babel::Text(chatUser.getUsername(), 24)),
  _avatar(new b12software::babel::Image(chatUser.getAvatarPath(), 50, 50)),
  _callButton(new QPushButton),
  _deleteFriendButton(new QPushButton),
  _callDialog(new b12software::babel::CallDialog(appUser, chatUser, client, udpTimer)),
  _appUser(appUser),
  _chatUser(chatUser),
  _client(client),
  _currentMessageIndex(0)
{
    auto &assetsManager = b12software::babel::AssetsManager::getInstance();

    _callButton->setIcon(QIcon(QString::fromUtf8(assetsManager["call_button"].data())));
    _callButton->setIconSize(QSize(35, 35));
    _callButton->setFixedSize(35, 35);
    _callButton->setFlat(true);
    _callButton->setToolTip("Call this user.");

    _deleteFriendButton->setIcon(QIcon(QString::fromUtf8(assetsManager["delete_button"].data())));
    _deleteFriendButton->setIconSize(QSize(35, 35));
    _deleteFriendButton->setFixedSize(35, 35);
    _deleteFriendButton->setFlat(true);
    _deleteFriendButton->setToolTip("Delete this friend.");

    _buttonLayout->addWidget(_callButton, 0, 0);
    _buttonLayout->addWidget(_deleteFriendButton, 0, 1);

    _userInfoLayout->addWidget(_avatar, 0, 0);
    _userInfoLayout->addWidget(_username, 0, 1);
    if (_chatUser.getUsername() != "tapadami")
        _userInfoLayout->addLayout(_buttonLayout, 0, 2);
    _inputLayout->addWidget(_inputBox, 0, 0);

    _chatLayout->addLayout(_userInfoLayout, 0, 0);
    _chatLayout->addWidget(_tableWidget, 1, 0);
    _chatLayout->addLayout(_inputLayout, 2, 0);

    _inputBox->setMinimumHeight(30);
    _inputBox->setPlaceholderText("Your message....");
    _inputBox->setTextMargins(5, 0, 0, 0);
    _inputBox->setStyleSheet("background-color: #ecf0f1; color: #2c3e50; border-radius: 5px;");
    setLayout(_chatLayout);

    _tableWidget->setFrameStyle(QFrame::NoFrame);
    _tableWidget->verticalHeader()->setVisible(false);
    _tableWidget->horizontalHeader()->setVisible(false);
    _tableWidget->setColumnCount(2);
    _tableWidget->setRowCount(1);
    _tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if (_chatUser.getId() != 99999 && _chatUser.getId() != -1)
        appendMessages();
    _tableWidget->setShowGrid(false);
    _tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    _tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    _tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _tableWidget->setStyleSheet("background-color: #ecf0f1; color: #2c3e50; border-radius: 5px;");
    _tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    _tableWidget->scrollToBottom();

    connect(_inputBox, &QLineEdit::returnPressed, this, &b12software::babel::ChatWidget::returnPressed);
    connect(_callButton, &QPushButton::clicked, this, &b12software::babel::ChatWidget::call);
    connect(_deleteFriendButton, &QPushButton::clicked, this, &b12software::babel::ChatWidget::deleteFriend);
}

b12software::babel::ChatWidget::~ChatWidget()
{
    delete _inputBox;
    delete _inputLayout;
    delete _tableWidget;
    delete _avatar;
    delete _callButton;
    delete _username;
    delete _deleteFriendButton;
    delete _buttonLayout;
    delete _userInfoLayout;
    delete _chatLayout;
}

void b12software::babel::ChatWidget::appendMessages()
{
    std::chrono::seconds actualTimestamp = std::chrono::duration_cast<std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());

    b12software::babel::BabelPacket retrieveMessagePacket;

    retrieveMessagePacket.createRetrieveMessageListPacket(_chatUser.getId(), actualTimestamp.count() - (3600 * 24 * 30), actualTimestamp.count());

    if (!retrieveMessagePacket.isValid())
        return;

    _client.getClient()->send(retrieveMessagePacket);
}

b12software::babel::CallDialog *b12software::babel::ChatWidget::getCallDialog()
{
    return _callDialog;
}

void b12software::babel::ChatWidget::returnPressed()
{
    std::string message = _inputBox->text().toUtf8().data();

    if (message.empty())
        return;

    b12software::babel::BabelPacket messagePacket;
    messagePacket.createSendMessagePacket(_chatUser.getId(), message);

    if (!messagePacket.isValid())
        return;
    _client.getClient()->send(messagePacket);

    auto *messageItem = new QTableWidgetItem(QString::fromUtf8(message.data()));
    messageItem->setTextAlignment(Qt::AlignRight);
    messageItem->setBackgroundColor(QColor::fromRgb(116, 185, 255));
    messageItem->setTextColor(QColor::fromRgb(255, 255, 255));

    _messages.emplace_back(std::pair<uint64_t, QTableWidgetItem *>({static_cast<uint64_t>(_appUser.getId()), messageItem}));
    _tableWidget->insertRow(_currentMessageIndex);
    _tableWidget->setItem(_currentMessageIndex, 1, messageItem);
    _currentMessageIndex++;

    _inputBox->clear();
}

void b12software::babel::ChatWidget::call()
{
    if (!_appUser.isInCall()) {
        b12software::babel::BabelPacket callPacket;
        callPacket.createStartCallPacket(_chatUser.getId());

        if (!callPacket.isValid())
            return;

        _client.getClient()->send(callPacket);

        b12software::babel::UserUtilities::getInstance().setCalledId(_chatUser.getId());
    }
}

void b12software::babel::ChatWidget::deleteFriend()
{
    b12software::babel::BabelPacket deleteFriend;
    deleteFriend.createDeleteUserContactListPacket(_chatUser.getId());

    _client.getClient()->send(deleteFriend);
}

void b12software::babel::ChatWidget::setUser(b12software::babel::BabelClientUser &newUser)
{
    _chatUser = newUser;
    _username->setText(QString::fromUtf8(_chatUser.getUsername().data()));
    _callDialog->refreshUser(_chatUser);
}

std::vector<std::pair<uint64_t, std::string>> &b12software::babel::ChatWidget::getRawMessages()
{
    return _rawMessages;
}

int &b12software::babel::ChatWidget::getCurrentMessageIndex()
{
    return _currentMessageIndex;
}

QTableWidget *b12software::babel::ChatWidget::getTableWidget()
{
    return _tableWidget;
}

void b12software::babel::ChatWidget::addMessages(std::vector<std::pair<uint64_t, std::string>> messageList)
{
    _rawMessages.clear();
    for (auto &message : messageList) {
        _rawMessages.emplace_back(message);
    }

    for (auto &it : _rawMessages) {
        if (it.first == _appUser.getId()) {
            auto *messageItem = new QTableWidgetItem(QString::fromUtf8(it.second.data()));
            messageItem->setTextAlignment(Qt::AlignRight);
            messageItem->setBackgroundColor(QColor::fromRgb(116, 185, 255));
            messageItem->setTextColor(QColor::fromRgb(255, 255, 255));
            _tableWidget->insertRow(_currentMessageIndex);
            _tableWidget->setItem(_currentMessageIndex, 1, messageItem);
        } else {
            auto *messageItem = new QTableWidgetItem(QString::fromUtf8(it.second.data()));
            messageItem->setTextAlignment(Qt::AlignLeft);
            _tableWidget->insertRow(_currentMessageIndex);
            _tableWidget->setItem(_currentMessageIndex, 0, messageItem);
        }
        _currentMessageIndex++;
    }
}

std::vector<std::pair<uint64_t, QTableWidgetItem *>> b12software::babel::ChatWidget::getMessages()
{
    return _messages;
}

