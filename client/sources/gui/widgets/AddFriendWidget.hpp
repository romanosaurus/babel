
/* Created by Romain FOUYER on 03/10/2019 at 01:19. */


#ifndef BABEL_ADDFRIENDWIDGET_HPP
#define BABEL_ADDFRIENDWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>

#include "gui/BabelClientUser.hpp"

namespace b12software {

    namespace babel {

        class AddFriendWidget : public QWidget {
        public:
            AddFriendWidget(BabelClientUser &user, b12software::network::NetworkClient &client);
            ~AddFriendWidget() override;

        private slots:
            void addFriend();

        private:
            BabelClientUser &_user;
            network::NetworkClient &_client;

            QGridLayout *_layout;
            QLineEdit *_searchField;
            QPushButton *_addButton;
        };

    }

}


#endif //BABEL_ADDFRIENDWIDGET_HPP
