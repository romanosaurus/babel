//
// Created by Romain FOUYER on 2019-09-30.
//

#ifndef BABEL_CALLDIALOG_HPP
#define BABEL_CALLDIALOG_HPP

#include "gui/BabelClientUser.hpp"
#include "gui/utilities/Image.hpp"
#include "gui/utilities/Text.hpp"
#include "network/NetworkClient.hpp"

#include <QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QTimer>

namespace b12software {

    namespace babel {

        class CallDialog : public QDialog {

        public:
            CallDialog(BabelClientUser &appUser, BabelClientUser &chatUser, network::NetworkClient &client, QTimer *udpTimer);
            ~CallDialog() override;

            void refreshUser(BabelClientUser &userRefreshed);
        protected:
            void closeEvent(QCloseEvent *e) override;

        private slots:
            void hangUp();

        private:
            QBoxLayout *_layout;

            Image *_callingAvatar;
            QPushButton *_hangUpButton;

            BabelClientUser &_appUser;
            BabelClientUser &_chatUser;
            network::NetworkClient &_client;

            QTimer *_udpTimer;
        };

    }

}


#endif //BABEL_CALLDIALOG_HPP
