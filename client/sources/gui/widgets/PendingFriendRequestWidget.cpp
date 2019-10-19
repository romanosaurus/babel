
/* Created by Romain FOUYER on 03/10/2019 at 03:44. */


#include <iostream>
#include "PendingFriendRequestWidget.hpp"
#include "protocol/BabelPacket.hpp"

b12software::babel::PendingFriendRequestWidget::PendingFriendRequestWidget(b12software::babel::BabelClientUser &user, network::NetworkClient &client)
: _user(user),
  _client(client),
  _layout(new QGridLayout),
  _buttonLayout(new QGridLayout),
  _acceptButton(new QPushButton("Accept")),
  _denyButton(new QPushButton("Deny")),
  _avatar(new b12software::babel::Image(_user.getAvatarPath(), 40, 40)),
  _username(new b12software::babel::Text(_user.getUsername(), 12))
{
    // Button Layout configuration

    _buttonLayout->addWidget(_acceptButton, 0, 0);
    _buttonLayout->addWidget(_denyButton, 1, 0);

    // Main Layout configuration

    _layout->addWidget(_avatar, 0, 0);
    _layout->addWidget(_username, 0, 1);
    _layout->addLayout(_buttonLayout, 0, 2);
    _layout->setSizeConstraint(QLayout::SetFixedSize);
    _layout->setSpacing(50);
    _layout->setVerticalSpacing(0);

    // Style

    _acceptButton->setStyleSheet(":hover {color:#2ecc71;}");
    _denyButton->setStyleSheet(":hover {color:#e74c3c;}");
    setStyleSheet("background-color:rgba(0, 0, 255, 0);");

    setLayout(_layout);

    // Connect

    connect(_acceptButton, &QPushButton::clicked, this, &b12software::babel::PendingFriendRequestWidget::accept);
    connect(_denyButton, &QPushButton::clicked, this, &b12software::babel::PendingFriendRequestWidget::deny);
}

b12software::babel::PendingFriendRequestWidget::~PendingFriendRequestWidget()
{
    delete _acceptButton;
    delete _denyButton;
    delete _avatar;
    delete _username;
    delete _layout;
}

void b12software::babel::PendingFriendRequestWidget::refreshUser()
{
    _username->setText(QString::fromUtf8(_user.getUsername().data()) + "#" + QString::fromUtf8(std::to_string(_user.getId()).data()));
}

void b12software::babel::PendingFriendRequestWidget::accept()
{
    b12software::babel::BabelPacket acceptPacket;

    acceptPacket.createSendResponseContactRequestPacket(_user.getId(), true);
    if (!acceptPacket.isValid())
        return;
    _client.getClient()->send(acceptPacket);
}

void b12software::babel::PendingFriendRequestWidget::deny()
{
    b12software::babel::BabelPacket denyPacket;

    denyPacket.createSendResponseContactRequestPacket(_user.getId(), false);
    if (!denyPacket.isValid())
        return;
    _client.getClient()->send(denyPacket);
}