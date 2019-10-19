
/* Created by Romain FOUYER on 05/10/2019 at 14:47. */


#include "RequestCallDialog.hpp"
#include "protocol/BabelPacket.hpp"

b12software::babel::RequestCallDialog::RequestCallDialog(b12software::babel::BabelClientUser &user, network::NetworkClient &client)
: QDialog(),
  _user(user),
  _client(client),
  _text(new b12software::babel::Text(_user.getUsername() + " tries to call you", 18)),
  _layout(new QGridLayout),
  _buttonLayout(new QGridLayout),
  _acceptButton(new QPushButton("Accept")),
  _denyButton(new QPushButton("Deny"))
{
    // Layout configuration

    _buttonLayout->addWidget(_acceptButton, 0, 0);
    _buttonLayout->addWidget(_denyButton, 0, 1);
    _layout->addWidget(_text, 0, 0);
    _layout->addLayout(_buttonLayout, 1, 0);

    // Style

    _text->setStyleSheet("color: white;");
    _acceptButton->setStyleSheet("background-color: #2ecc71;");
    _denyButton->setStyleSheet("background-color: #e74c3c;");
    setStyleSheet("background-color: #74b9ff;");

    setLayout(_layout);

    // Callbacks

    connect(_denyButton, &QPushButton::clicked, this, &b12software::babel::RequestCallDialog::deny);
}

b12software::babel::RequestCallDialog::~RequestCallDialog()
{
    delete _acceptButton;
    delete _denyButton;
    delete _buttonLayout;
    delete _layout;
}

QPushButton* b12software::babel::RequestCallDialog::getAcceptButton()
{
    return _acceptButton;
}

b12software::babel::BabelClientUser &b12software::babel::RequestCallDialog::getUser()
{
    return _user;
}


void b12software::babel::RequestCallDialog::deny()
{
    b12software::babel::BabelPacket callPacketAccept;

    callPacketAccept.createSendResponseCallPacket(_user.getId(), false);
    if (!callPacketAccept.isValid())
        return;
    _client.getClient()->send(callPacketAccept);
}

