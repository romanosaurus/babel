
/* Created by Romain FOUYER on 03/10/2019 at 03:44. */


#ifndef BABEL_PENDINGFRIENDREQUESTWIDGET_HPP
#define BABEL_PENDINGFRIENDREQUESTWIDGET_HPP

#include "gui/BabelClientUser.hpp"
#include "gui/utilities/Text.hpp"
#include "gui/utilities/Image.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include "network/NetworkClient.hpp"

namespace b12software {

    namespace babel {

        class PendingFriendRequestWidget : public QWidget {
        public:
            PendingFriendRequestWidget(BabelClientUser &user, network::NetworkClient &client);
            ~PendingFriendRequestWidget() override;

            void refreshUser();

        private slots:
            void accept();
            void deny();

        private:
            BabelClientUser &_user;
            network::NetworkClient &_client;

            QGridLayout *_layout;
            QGridLayout *_buttonLayout;

            QPushButton *_acceptButton;
            QPushButton *_denyButton;
            Image *_avatar;
            Text *_username;
        };

    }

}


#endif //BABEL_PENDINGFRIENDREQUESTWIDGET_HPP
