//
// Created by Romain FOUYER on 2019-09-29.
//

#include "ProfileDialog.hpp"

b12software::babel::ProfileDialog::ProfileDialog(QWidget *parent, b12software::babel::BabelClientUser &user)
: QDialog(parent),
  _layout(new QGridLayout),
  _textLayout(new QGridLayout),
  _avatar(new b12software::babel::Image(user.getAvatarPath(), 200, 200)),
  _username(new b12software::babel::Text(user.getUsername() + "#" + std::to_string(user.getId()), 42)),
  _emailAddress(new b12software::babel::Text(user.getEmailAddress(), 34))
{
    _layout->setSpacing(50);
    if (_username && _emailAddress && _avatar) {
        _textLayout->addWidget(_username, 0, 0);
        _textLayout->addWidget(_emailAddress, 1, 0);
        _layout->addWidget(_avatar, 0, 0);
    }
    _layout->addLayout(_textLayout, 0, 1);
    setStyleSheet("background-color:#f1f2f6; color:black;");
    setLayout(_layout);
}

b12software::babel::ProfileDialog::~ProfileDialog()
{
    delete _username;
    delete _avatar;
    delete _emailAddress;
    delete _textLayout;
    delete _layout;
}
