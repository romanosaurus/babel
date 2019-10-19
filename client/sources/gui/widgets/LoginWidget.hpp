//
// Created by Romain FOUYER on 2019-09-18.
//

#ifndef BABEL_LOGINWIDGET_HPP
#define BABEL_LOGINWIDGET_HPP

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

namespace b12software {

    namespace babel {

        class LoginWidget : public QWidget {
        public:
            explicit LoginWidget(QWidget *parent);
            ~LoginWidget() override;

            QPushButton *getLoginButton();
            QPushButton *getSignUpButton();

            std::string getUsername();
            std::string getPassword();

            void setupFormLayout();
            void setupField();
            void setupTitle();
            void setupButtons();
        private:
            QFormLayout *_formLayout;
            QGridLayout *_mainLayout;
            QGridLayout *_buttonsLayout;

            QLineEdit *_usernameField;
            QLineEdit *_passwordField;

            QPushButton *_loginButton;
            QPushButton *_signUpButton;

            QLabel *_titleLabel;
        };

    }

}

#endif //BABEL_LOGINWIDGET_HPP
