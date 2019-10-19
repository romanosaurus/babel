//
// Created by Romain FOUYER on 2019-09-26.
//

#ifndef BABEL_CHATWIDGET_HPP
#define BABEL_CHATWIDGET_HPP

#include "gui/BabelClientUser.hpp"
#include "gui/utilities/Image.hpp"
#include "gui/utilities/Text.hpp"
#include "gui/dialogs/CallDialog.hpp"
#include "network/NetworkClient.hpp"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>

namespace b12software {

    namespace babel {

        class ChatWidget : public QWidget {
        public:
            ChatWidget(QWidget *parent, BabelClientUser &appUser, BabelClientUser &chatUser, network::NetworkClient &client, QTimer *udpTimer);
            ~ChatWidget() override;

            CallDialog *getCallDialog();

            std::vector<std::pair<uint64_t, QTableWidgetItem *>> getMessages();
            std::vector<std::pair<uint64_t, std::string>> &getRawMessages();

            int &getCurrentMessageIndex();
            QTableWidget *getTableWidget();

            void setUser(BabelClientUser &newUser);
            void appendMessages();

            void addMessages(std::vector<std::pair<uint64_t, std::string>> messageList);

        private slots:
            void returnPressed();
            void call();
            void deleteFriend();

        private:
            QGridLayout *_userInfoLayout;
            QGridLayout *_chatLayout;
            QGridLayout *_inputLayout;
            QGridLayout *_buttonLayout;

            QLineEdit *_inputBox;
            QTableWidget *_tableWidget;

            Text *_username;
            Image *_avatar;

            QPushButton *_callButton;
            QPushButton *_deleteFriendButton;
            b12software::babel::CallDialog *_callDialog;

            BabelClientUser &_appUser;
            BabelClientUser &_chatUser;
            network::NetworkClient &_client;

            std::vector<std::pair<uint64_t, QTableWidgetItem *>> _messages;
            std::vector<std::pair<uint64_t, std::string>> _rawMessages;
            int _currentMessageIndex;
        };

    }

}


#endif //BABEL_CHATWIDGET_HPP
