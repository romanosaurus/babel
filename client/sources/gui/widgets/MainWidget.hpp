//
// Created by Romain FOUYER on 2019-09-18.
//

#ifndef BABEL_MAINWIDGET_HPP
#define BABEL_MAINWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QSplitter>
#include <map>

#include "gui/widgets/ProfileWidget.hpp"
#include "gui/widgets/UserListItemWidget.hpp"
#include "gui/widgets/AddFriendWidget.hpp"
#include "gui/BabelClientUser.hpp"
#include "network/NetworkClient.hpp"

namespace b12software {

    namespace babel {

        class MainWidget : public QWidget {
        public:
            MainWidget(QWidget *parent, BabelClientUser &user, network::NetworkClient &client, QTimer *udpTimer);
            ~MainWidget() override;

            void setUser(BabelClientUser &user);

            void fillFriendsList();
            void fillFriendRequestList();

            void fillPendingListWidget();
            void fillListWidget();
            void setupLeftBar();

            std::vector<UserListItemWidget *> &getFriendList();
            std::vector<UserListItemWidget *> &getPendingFriendList();

            QListWidget *getListWidget();
        private slots:
            void onListItemClicked(QListWidgetItem *item);

        private:
            BabelClientUser &_currentUser;
            network::NetworkClient &_client;

            QGridLayout *_mainLayout;
            QSplitter *_splitter;

            QStackedWidget *_stack;
            QListWidget *_listWidget;

            std::vector<UserListItemWidget *> _friendList;
            std::vector<UserListItemWidget *> _pendingRequestList;

            QWidget *_leftBar;
            QGridLayout *_leftBarLayout;

            ProfileWidget *_profileWidget;
            AddFriendWidget *_addFriendWidget;

            QTimer *_udpTimer;
        };

    }

}

#endif //BABEL_MAINWIDGET_HPP
