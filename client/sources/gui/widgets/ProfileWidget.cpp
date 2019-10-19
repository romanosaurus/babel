//
// Created by Romain FOUYER on 2019-09-28.
//

#include "ProfileWidget.hpp"

b12software::babel::ProfileWidget::ProfileWidget(b12software::babel::BabelClientUser &user)
: QWidget(0),
  _user(user),
  _profileLayout(new QGridLayout),
  _textLayout(new QGridLayout),
  _avatar(new b12software::babel::Image(_user.getAvatarPath(), 50, 50)),
  _username(new b12software::babel::Text(_user.getUsername() + "#" + std::to_string(_user.getId()), 24)),
  _emailAddress(new b12software::babel::Text(_user.getEmailAddress(), 12)),
  _profileDialog(new b12software::babel::ProfileDialog(this, _user))
{

    _emailAddress->setStyleSheet("color: #2c3e50");

    _textLayout->addWidget(_username, 0, 0, Qt::AlignHCenter);
    _textLayout->addWidget(_emailAddress, 1, 0, Qt::AlignHCenter);
    _profileLayout->addWidget(_avatar, 0, 0);

    _profileLayout->addLayout(_textLayout, 0, 1);

    setLayout(_profileLayout);
    setStyleSheet(":hover {background-color:#dedfe6;}");
}

b12software::babel::ProfileWidget::~ProfileWidget()
{
    delete _avatar;
    delete _username;
    delete _emailAddress;
    delete _profileLayout;
    delete _profileDialog;
}

void b12software::babel::ProfileWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        _profileDialog->show();
    }
}