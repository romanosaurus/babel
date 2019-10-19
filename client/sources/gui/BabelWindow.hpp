//
// Created by Romain FOUYER on 2019-09-18.
//

#ifndef BABEL_BABELWINDOW_HPP
#define BABEL_BABELWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <codec/OpusCodecAPI.hpp>
#include <audio/IAudioAPI.hpp>

#include "network/NetworkClient.hpp"
#include "gui/widgets/LoadingWidget.hpp"
#include "gui/widgets/LoginWidget.hpp"
#include "gui/widgets/MainWidget.hpp"
#include "gui/widgets/SignUpWidget.hpp"
#include "protocol/BabelPacket.hpp"
#include "gui/BabelClientUser.hpp"
#include "gui/dialogs/RequestCallDialog.hpp"

namespace b12software {

    namespace babel {

        class BabelWindow : public QMainWindow {
        public:
            explicit BabelWindow(b12software::network::NetworkClient &client);
            ~BabelWindow() override;

            void openCall();

        protected:
            void keyPressEvent(QKeyEvent *pe) override;

        private slots:
            void loginButtonClicked();
            void signUpButtonClicked();
            void backButtonClicked();
            void registerButtonClicked();
            void updateOnRequest();
            void udpResponse();
            void acceptCallButton();
            void denyCallButton();

        private:
            void handleBinaryResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleAuthResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleContactInfoResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleCallUserResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleListContactResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleAddUserResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleFriendRequestResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleAllFriendRequestResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleFriendListResponse(b12software::babel::BabelPacket &receivedPacket);
            void handleReceiveCall(b12software::babel::BabelPacket &receivedPacket);
            void handleDeleteContact(b12software::babel::BabelPacket &receivedPacket);
            void handleHangUp(b12software::babel::BabelPacket &receivedPacket);
            void handleReceiveMessage(b12software::babel::BabelPacket &receivedPacket);
            void handleRetrieveAllMessages(b12software::babel::BabelPacket &receivedPacket);

        private:
            void setupCallbacks();

            BabelClientUser _user;
            QStackedWidget *_parentWidget;
            LoginWidget *_loginWidget;
            MainWidget *_mainWidget;
            SignUpWidget *_signUpWidget;
            LoadingWidget *_loadingWidget;
            RequestCallDialog *_requestCallDialog;

            b12software::network::NetworkClient &_client;
            QTimer *_timer;

            QTimer *_udpTimer;
            OpusCodecAPI _opusAPI;
            IAudioAPI *_audioApi;
            IAudioAPI::Sample *_inputBuffer;
            size_t _frameSize;
            size_t _bufferDynamicSize;
            size_t _inputBufferSize;
            long _lastReceivedTimestamp;
        };

    }
}


#endif //BABEL_BABELWINDOW_HPP
