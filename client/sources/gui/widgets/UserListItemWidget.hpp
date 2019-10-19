//
// Created by Romain FOUYER on 2019-09-30.
//

#ifndef BABEL_USERLISTITEMWIDGET_HPP
#define BABEL_USERLISTITEMWIDGET_HPP

#include "gui/utilities/Image.hpp"
#include "gui/utilities/Text.hpp"
#include "gui/BabelClientUser.hpp"
#include "gui/widgets/ChatWidget.hpp"
#include "gui/widgets/PendingFriendRequestWidget.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QListWidgetItem>

namespace b12software {

    namespace babel {

        class UserListItemWidget : public QListWidgetItem {
        public:
            UserListItemWidget(BabelClientUser &appUser, BabelClientUser user, network::NetworkClient &client,  QTimer *udpTimer);
            ~UserListItemWidget() override;

            QWidget *getFriendWidget();
            PendingFriendRequestWidget *getFriendRequestWidget();
            ChatWidget *getChatWidget();

            void setAdded(bool newStatus);
            bool getAdded();

            BabelClientUser &getTargetUser();

            void refreshUser();
        private:
            QGridLayout *_friendLayout;

            BabelClientUser &_appUser;
            BabelClientUser _user;
            network::NetworkClient &_client;

            ChatWidget *_chatWidget;
            QWidget *_friendWidget;
            PendingFriendRequestWidget *_friendRequestWidget;

            Image *_avatar;
            Text *_username;
            bool _added;
        };

    }

}


#endif //BABEL_USERLISTITEMWIDGET_HPP
