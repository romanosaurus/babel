//
// Created by Romain FOUYER on 2019-09-18.
//

#include "LoginWidget.hpp"

b12software::babel::LoginWidget::LoginWidget(QWidget *parent)
: QWidget(parent),
  _formLayout(new QFormLayout),
  _mainLayout(new QGridLayout),
  _buttonsLayout(new QGridLayout),
  _usernameField(new QLineEdit),
  _passwordField(new QLineEdit),
  _loginButton(new QPushButton("Login")),
  _signUpButton(new QPushButton("Sign Up")),
  _titleLabel(new QLabel)
{
    setupField();
    setupFormLayout();
    setupTitle();
    setupButtons();

    _buttonsLayout->addWidget(_loginButton, 0, 0);
    _buttonsLayout->addWidget(_signUpButton, 0, 1);

    _mainLayout->addWidget(_titleLabel, 0, 0, Qt::AlignHCenter);
    _mainLayout->addLayout(_formLayout, 1, 0);
    _mainLayout->addLayout(_buttonsLayout, 2, 0);

    setStyleSheet("background-color:#74b9ff; color:white;");

    setLayout(_mainLayout);
}

b12software::babel::LoginWidget::~LoginWidget()
{
    delete _titleLabel;
    delete _signUpButton;
    delete _loginButton;
    delete _passwordField;
    delete _usernameField;
    delete _buttonsLayout;
    delete _formLayout;
    delete _mainLayout;
}

QPushButton *b12software::babel::LoginWidget::getLoginButton()
{
    return _loginButton;
}

QPushButton* b12software::babel::LoginWidget::getSignUpButton()
{
    return _signUpButton;
}

std::string b12software::babel::LoginWidget::getUsername()
{
    return _usernameField->text().toUtf8().constData();
}

std::string b12software::babel::LoginWidget::getPassword()
{
    return _passwordField->text().toUtf8().constData();
}

void b12software::babel::LoginWidget::setupFormLayout()
{
    _formLayout->addRow(tr("&Username:"), _usernameField);
    _formLayout->addRow(tr("&Password:"), _passwordField);
    _formLayout->setFormAlignment(Qt::AlignVCenter | Qt::AlignHCenter | Qt::AlignTop);
}

void b12software::babel::LoginWidget::setupField()
{
    _usernameField->setAutoFillBackground(true);
    _usernameField->setStyleSheet("background-color: #ecf0f1; color: #2c3e50;");
    _passwordField->setEchoMode(QLineEdit::Password);
    _passwordField->setStyleSheet("background-color: #ecf0f1; color: #2c3e50;");
}

void b12software::babel::LoginWidget::setupTitle()
{
    _titleLabel->setText("BABEL");
    _titleLabel->setMaximumHeight(100);
    _titleLabel->setStyleSheet("font-size: 48px; font-weight: bold;");
}

void b12software::babel::LoginWidget::setupButtons()
{
    _loginButton->setFlat(true);
    _loginButton->setAutoFillBackground(true);
    _signUpButton->setFlat(true);
    _signUpButton->setAutoFillBackground(true);
}