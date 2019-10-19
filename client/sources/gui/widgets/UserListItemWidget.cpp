//
// Created by Romain FOUYER on 2019-09-30.
//

#include "UserListItemWidget.hpp"

#include <QSpacerItem>

b12software::babel::UserListItemWidget::UserListItemWidget(b12software::babel::BabelClientUser &appUser, b12software::babel::BabelClientUser user, network::NetworkClient &client, QTimer *udpTimer)
: QListWidgetItem(),
  _friendLayout(new QGridLayout),
  _appUser(appUser),
  _user(user),
  _client(client),
  _chatWidget(new b12software::babel::ChatWidget(0, _appUser, _user, _client, udpTimer)),
  _friendWidget(new QWidget),
  _friendRequestWidget(new b12software::babel::PendingFriendRequestWidget(_user, client)),
  _avatar(new b12software::babel::Image(_user.getAvatarPath(), 40, 40)),
  _username(new b12software::babel::Text(_user.getUsername() + "#" + std::to_string(_user.getId()), 12)),
  _added(false)
{
    _friendWidget->setLayout(_friendLayout);
    _friendLayout->setSpacing(50);
    _friendLayout->addWidget(_avatar, 0, 0);
    _friendLayout->addWidget(_username, 0, 1);
    _friendLayout->setSizeConstraint(QLayout::SetFixedSize);
    _friendWidget->setStyleSheet("background-color:rgba(0, 0, 255, 0);");
}

b12software::babel::UserListItemWidget::~UserListItemWidget()
{
    //delete _avatar;
    //delete _username;
    //delete _layout;
}

QWidget *b12software::babel::UserListItemWidget::getFriendWidget()
{
    return _friendWidget;
}

b12software::babel::PendingFriendRequestWidget *b12software::babel::UserListItemWidget::getFriendRequestWidget()
{
    return _friendRequestWidget;
}

b12software::babel::ChatWidget *b12software::babel::UserListItemWidget::getChatWidget()
{
    return _chatWidget;
}

b12software::babel::BabelClientUser &b12software::babel::UserListItemWidget::getTargetUser()
{
    return _user;
}

void b12software::babel::UserListItemWidget::refreshUser()
{
    _username->setText(QString::fromUtf8(_user.getUsername().data()) + "#" + QString::fromUtf8(std::to_string(_user.getId()).data()));
    _chatWidget->setUser(_user);
}

void b12software::babel::UserListItemWidget::setAdded(bool newStatus)
{
    _added = newStatus;
}

bool b12software::babel::UserListItemWidget::getAdded()
{
    return _added;
}