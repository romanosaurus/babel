//
// Created by Romain FOUYER on 2019-09-18.
//

#include "gui/widgets/MainWidget.hpp"
#include "gui/utilities/AssetsManager.hpp"
#include "protocol/BabelPacket.hpp"

b12software::babel::MainWidget::MainWidget(QWidget *parent, BabelClientUser &user, b12software::network::NetworkClient &client, QTimer *udpTimer)
: QWidget(parent),
  _currentUser(user),
  _client(client),
  _mainLayout(new QGridLayout),
  _splitter(new QSplitter(parent)),
  _stack(new QStackedWidget),
  _listWidget(new QListWidget),
  _leftBar(new QWidget),
  _leftBarLayout(new QGridLayout),
  _profileWidget(new b12software::babel::ProfileWidget(user)),
  _addFriendWidget(new b12software::babel::AddFriendWidget(user, _client)),
  _udpTimer(udpTimer)
{
    fillFriendRequestList();
    fillFriendsList();
    fillListWidget();
    fillPendingListWidget();
    setupLeftBar();

    _stack->setStyleSheet("background-color: #f5f6fa;");
    _splitter->addWidget(_leftBar);
    _splitter->addWidget(_stack);
    _splitter->setStretchFactor(0, 1);
    _splitter->setStretchFactor(1, 2);
    _splitter->setHandleWidth(1);

    _mainLayout->addWidget(_splitter, 0, 0);
    _mainLayout->setContentsMargins(0,0,0,0);

    setStyleSheet("background-color: #f1f2f6; color: black;");
    _listWidget->setStyleSheet("QListWidget::item:selected {background-color:#74b9ff; color:white;} QListWidget::item:hover{background-color:#dedfe6;} QListWidget::item:selected:hover {background-color:#74b9ff; color:white;}");

    setLayout(_mainLayout);

    connect(_listWidget, &QListWidget::itemClicked, this, &b12software::babel::MainWidget::onListItemClicked);
}

b12software::babel::MainWidget::~MainWidget()
{
    for (auto &it : _friendList)
        delete it;
    for (auto &it : _pendingRequestList)
        delete it;
    delete _listWidget;
    delete _leftBarLayout;
    delete _leftBar;
    delete _stack;
    delete _splitter;
    delete _mainLayout;
    delete _udpTimer;
}

void b12software::babel::MainWidget::setUser(b12software::babel::BabelClientUser &user)
{
    _currentUser = user;
}

void b12software::babel::MainWidget::fillFriendsList()
{
    _friendList.clear();
    b12software::babel::BabelPacket fetchFriendList;
    fetchFriendList.createListContactListPacket();

    if (!fetchFriendList.isValid())
        return;

    _client.getClient()->send(fetchFriendList);
}

void b12software::babel::MainWidget::fillFriendRequestList()
{
    _pendingRequestList.clear();
    b12software::babel::BabelPacket fetchPendingRequest;
    fetchPendingRequest.createListContactRequestListPacket();

    if (!fetchPendingRequest.isValid())
        return;

    _client.getClient()->send(fetchPendingRequest);
}

void b12software::babel::MainWidget::fillListWidget()
{
    int index = 0;
    for (auto &it : _friendList) {
        if (!it->getAdded() && !it->getTargetUser().getUsername().empty()) {
            it->setSizeHint(
                    QSize(it->getFriendWidget()->sizeHint().width(), it->getFriendWidget()->sizeHint().height() - 15));
            _listWidget->insertItem(index, it);
            _listWidget->setItemWidget(it, it->getFriendWidget());
            _stack->addWidget(it->getChatWidget());
            it->setAdded(true);
            ++index;
        }
    }
    if (!_friendList.empty())
        _stack->setCurrentWidget(_friendList.front()->getChatWidget());
    else {
        b12software::babel::BabelClientUser voidUser("tapadami", b12software::babel::AssetsManager::getInstance()["avatar"], "tapadami", -1);
        auto *voidWidget = new b12software::babel::ChatWidget(0, _currentUser, voidUser, _client, _udpTimer);
        _stack->addWidget(voidWidget);
        _stack->setCurrentWidget(voidWidget);
    }
}

void b12software::babel::MainWidget::fillPendingListWidget()
{
    int index = 0;
    for (auto &it : _pendingRequestList) {
        if (!it->getAdded() && !it->getTargetUser().getUsername().empty()) {
            it->setSizeHint(QSize(it->getFriendRequestWidget()->sizeHint().width() - 10,
                                  it->getFriendRequestWidget()->sizeHint().height() - 15));
            _listWidget->insertItem(index, it);
            _listWidget->setItemWidget(it, it->getFriendRequestWidget());
            _stack->addWidget(it->getChatWidget());
            it->setAdded(true);
            ++index;
        }
    }
}

void b12software::babel::MainWidget::onListItemClicked(QListWidgetItem *item)
{
    for (auto &it : _friendList) {
        if (it == item)
            _stack->setCurrentWidget(it->getChatWidget());
    }
}

void b12software::babel::MainWidget::setupLeftBar()
{
    _leftBarLayout->addWidget(_profileWidget, 0, 0);
    _leftBarLayout->addWidget(_listWidget, 1, 0);
    _leftBarLayout->addWidget(_addFriendWidget, 2, 0);
    _leftBar->setStyleSheet("border: 0px;");
    _leftBar->setLayout(_leftBarLayout);
    _listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

std::vector<b12software::babel::UserListItemWidget *> &b12software::babel::MainWidget::getFriendList()
{
    return _friendList;
}

std::vector<b12software::babel::UserListItemWidget *>& b12software::babel::MainWidget::getPendingFriendList()
{
    return _pendingRequestList;
}

QListWidget *b12software::babel::MainWidget::getListWidget()
{
    return _listWidget;
}