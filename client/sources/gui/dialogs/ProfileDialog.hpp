//
// Created by Romain FOUYER on 2019-09-29.
//

#ifndef BABEL_PROFILEDIALOG_HPP
#define BABEL_PROFILEDIALOG_HPP

#include <QDialog>
#include <QtWidgets/QGridLayout>

#include "gui/utilities/Text.hpp"
#include "gui/utilities/Image.hpp"
#include "gui/BabelClientUser.hpp"

namespace b12software {
    namespace babel {
        class ProfileDialog : public QDialog {
        public:
            ProfileDialog(QWidget *parent, BabelClientUser &user);

            ~ProfileDialog() override;

        private:
            QGridLayout *_layout;
            QGridLayout *_textLayout;

            b12software::babel::Image *_avatar;
            b12software::babel::Text *_username;
            b12software::babel::Text *_emailAddress;
        };
    }
}


#endif //BABEL_PROFILEDIALOG_HPP
