//
// Created by Romain FOUYER on 2019-09-28.
//

#ifndef BABEL_PROFILEWIDGET_HPP
#define BABEL_PROFILEWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QMouseEvent>

#include "gui/dialogs/ProfileDialog.hpp"

namespace b12software {

    namespace babel {

        class ProfileWidget : public QWidget {
        public:
            explicit ProfileWidget(BabelClientUser &_user);
            ~ProfileWidget() override;

        protected:
            void mousePressEvent(QMouseEvent *event) override;
        private:
            BabelClientUser &_user;

            QGridLayout *_profileLayout;
            QGridLayout *_textLayout;

            b12software::babel::Image *_avatar;
            b12software::babel::Text *_username;
            b12software::babel::Text *_emailAddress;

            ProfileDialog *_profileDialog;
        };

    }

}


#endif //BABEL_PROFILEWIDGET_HPP
