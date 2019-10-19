
/* Created by Romain FOUYER on 05/10/2019 at 14:47. */


#ifndef BABEL_REQUESTCALLDIALOG_HPP
#define BABEL_REQUESTCALLDIALOG_HPP

#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

#include "network/NetworkClient.hpp"
#include "gui/utilities/Text.hpp"
#include "gui/BabelClientUser.hpp"

namespace b12software {
    namespace babel {
        class RequestCallDialog : public QDialog {
        public:
            RequestCallDialog(b12software::babel::BabelClientUser &user, network::NetworkClient &client);
            ~RequestCallDialog();

            BabelClientUser &getUser();

            QPushButton *getAcceptButton();
        private slots:
            void deny();
        private:
            BabelClientUser &_user;
            network::NetworkClient &_client;

            b12software::babel::Text *_text;
            QGridLayout *_layout;
            QGridLayout *_buttonLayout;

            QPushButton *_acceptButton;
            QPushButton *_denyButton;
        };
    }
}


#endif //BABEL_REQUESTCALLDIALOG_HPP
