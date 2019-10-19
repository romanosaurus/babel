//
// Created by Romain FOUYER on 2019-09-30.
//

#include "gui/utilities/UserUtilities.hpp"
#include "CallDialog.hpp"
#include "gui/utilities/AssetsManager.hpp"
#include "protocol/BabelPacket.hpp"

b12software::babel::CallDialog::CallDialog(b12software::babel::BabelClientUser &appUser, b12software::babel::BabelClientUser &chatUser, network::NetworkClient &client, QTimer *udpTimer)
: QDialog(),
  _layout(new QBoxLayout(QBoxLayout::TopToBottom)),
  _callingAvatar(new b12software::babel::Image(chatUser.getAvatarPath(), 100, 100)),
  _hangUpButton(new QPushButton),
  _appUser(appUser),
  _chatUser(chatUser),
  _client(client),
  _udpTimer(udpTimer)
{
    auto &assetsManager = b12software::babel::AssetsManager::getInstance();

    setWindowTitle(QString::fromUtf8(chatUser.getUsername().data()) + " - call");
    setMinimumSize(500, 500);

    _hangUpButton->setIcon(QIcon(QString::fromUtf8(assetsManager["hangup_button"].data())));
    _hangUpButton->setIconSize(QSize(75, 75));
    _hangUpButton->setFixedSize(100, 100);
    _hangUpButton->setFlat(true);
    _hangUpButton->setStyleSheet("border: none;");

    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setAlignment(Qt::AlignHCenter);
    _layout->addWidget(_callingAvatar);
    _layout->addWidget(_hangUpButton);

    setLayout(_layout);
    setStyleSheet("background-color: #74b9ff; color: white;");

    connect(_hangUpButton, &QPushButton::clicked, this, &b12software::babel::CallDialog::hangUp);
}

b12software::babel::CallDialog::~CallDialog()
{
    delete _layout;
}

void b12software::babel::CallDialog::closeEvent([[maybe_unused]]QCloseEvent *e)
{
    if (_appUser.isInCall()) {
        b12software::babel::BabelPacket closeEventPacket;
        closeEventPacket.createHangupCallPacket(_chatUser.getId());
        _appUser.setInCall(false);
        _client.getClient()->send(closeEventPacket);
        _udpTimer->stop();
        b12software::babel::UserUtilities::getInstance().setWaitingCall(false);
        _client.getUdpSocket()->unbind();
    }
}

void b12software::babel::CallDialog::hangUp()
{
    close();
}

void b12software::babel::CallDialog::refreshUser(b12software::babel::BabelClientUser &userRefreshed)
{
    _chatUser = userRefreshed;
    setWindowTitle(QString::fromUtf8(_chatUser.getUsername().data()) + " - call");
}