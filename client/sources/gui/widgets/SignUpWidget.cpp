//
// Created by Romain FOUYER on 2019-09-23.
//

#include "SignUpWidget.hpp"

b12software::babel::SignUpWidget::SignUpWidget(QWidget *parent)
: QWidget(new QWidget(parent)),
  _formLayout(new QFormLayout),
  _usernameField(new QLineEdit),
  _emailField(new QLineEdit),
  _passwordField(new QLineEdit),
  _confirmPasswordField(new QLineEdit),
  _birthField(new QDateEdit),
  _submitButton(new QPushButton("Submit")),
  _backButton(new QPushButton("Back"))
{
    _passwordField->setEchoMode(QLineEdit::Password);
    _confirmPasswordField->setEchoMode(QLineEdit::Password);
    _formLayout->addRow(tr("&Username:"), _usernameField);
    _formLayout->addRow(tr("&Password:"), _passwordField);
    _formLayout->addRow(tr("&Confirm password:"), _confirmPasswordField);
    _formLayout->addRow(tr("&Email:"), _emailField);
    _formLayout->addRow(tr("&Birth Date:"), _birthField);

    _formLayout->addRow(_submitButton, _backButton);
    _formLayout->setFormAlignment(Qt::AlignVCenter | Qt::AlignHCenter | Qt::AlignTop);

    _submitButton->setFlat(true);
    _submitButton->setAutoFillBackground(true);
    _backButton->setFlat(true);
    _backButton->setAutoFillBackground(true);
    setStyleSheet("background-color:#74b9ff; color: white;");
    setLayout(_formLayout);
}

b12software::babel::SignUpWidget::~SignUpWidget()
{
    delete _usernameField;
    delete _emailField;
    delete _passwordField;
    delete _birthField;
    delete _submitButton;
    delete _backButton;
    delete _formLayout;
}

const QPushButton *b12software::babel::SignUpWidget::getBackButton() const
{
    return _backButton;
}

const QPushButton *b12software::babel::SignUpWidget::getSubmitButton() const
{
    return _submitButton;
}

const QLineEdit *b12software::babel::SignUpWidget::getUsernameField() const
{
    return _usernameField;
}

const QLineEdit *b12software::babel::SignUpWidget::getEmailField() const
{
    return _emailField;
}

const QLineEdit *b12software::babel::SignUpWidget::getPasswordField() const
{
    return _passwordField;
}

const QDateEdit *b12software::babel::SignUpWidget::getDateField() const
{
    return _birthField;
}

const QLineEdit *b12software::babel::SignUpWidget::getConfirmPasswordField() const
{
    return _confirmPasswordField;
}
