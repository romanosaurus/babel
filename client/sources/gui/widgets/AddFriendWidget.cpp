
/* Created by Romain FOUYER on 03/10/2019 at 01:19. */

#include <network/NetworkClient.hpp>
#include "gui/widgets/AddFriendWidget.hpp"
#include "protocol/BabelPacket.hpp"

b12software::babel::AddFriendWidget::AddFriendWidget(b12software::babel::BabelClientUser &user, b12software::network::NetworkClient &client)
: QWidget(),
  _user(user),
  _client(client),
  _layout(new QGridLayout),
  _searchField(new QLineEdit),
  _addButton(new QPushButton("Add"))
{
    _searchField->setPlaceholderText("username#id");
    _searchField->setAutoFillBackground(true);
    _searchField->setTextMargins(5, 0, 0, 0);
    _searchField->setStyleSheet("background-color: #ecf0f1; color: #2c3e50; border: 1px solid #2c3e50;");

    _addButton->setStyleSheet("QPushButton {color: #2c3e50;} QPushButton:hover {color:#2ecc71;}");
    _layout->addWidget(_searchField, 0, 0);
    _layout->addWidget(_addButton, 0, 1);

    setLayout(_layout);

    connect(_addButton, &QPushButton::clicked, this, &b12software::babel::AddFriendWidget::addFriend);
    connect(_searchField, &QLineEdit::returnPressed, this, &b12software::babel::AddFriendWidget::addFriend);
}

b12software::babel::AddFriendWidget::~AddFriendWidget()
{
    delete _addButton;
    delete _searchField;
    delete _layout;
}

void b12software::babel::AddFriendWidget::addFriend()
{
    const std::string &userToAdd = _searchField->text().toUtf8().constData();

    if (userToAdd.empty())
        return;
    if (userToAdd.find('#') == std::string::npos) {
        QMessageBox errorBox;

        errorBox.setText("Please precise the id of your future friend");
        errorBox.exec();
        return;
    }

    long idToAdd = std::stol(userToAdd.substr(userToAdd.find('#') + 1, userToAdd.length()));

    b12software::babel::BabelPacket addFriendPacket;
    addFriendPacket.createAddUserContactListPacket(idToAdd);

    if (!addFriendPacket.isValid())
        return;

    _client.getClient()->send(addFriendPacket);

    _searchField->clear();
}