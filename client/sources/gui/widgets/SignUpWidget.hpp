//
// Created by Romain FOUYER on 2019-09-23.
//

#ifndef BABEL_SIGNUPWIDGET_HPP
#define BABEL_SIGNUPWIDGET_HPP

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDateEdit>

namespace b12software {

    namespace babel {

        class SignUpWidget : public QWidget {
        public:
            explicit SignUpWidget(QWidget *parent);
            ~SignUpWidget() override;

            [[nodiscard]] const QPushButton *getSubmitButton() const;
            [[nodiscard]] const QPushButton *getBackButton() const;

            [[nodiscard]] const QLineEdit *getUsernameField() const;
            [[nodiscard]] const QLineEdit *getEmailField() const;
            [[nodiscard]] const QLineEdit *getPasswordField() const;
            [[nodiscard]] const QLineEdit *getConfirmPasswordField() const;

            [[nodiscard]] const QDateEdit *getDateField() const;

        private:
            QFormLayout *_formLayout;

            QLineEdit *_usernameField;
            QLineEdit *_emailField;
            QLineEdit *_passwordField;
            QLineEdit *_confirmPasswordField;

            QDateEdit *_birthField;

            QPushButton *_submitButton;
            QPushButton *_backButton;
        };

    }

}


#endif //BABEL_SIGNUPWIDGET_HPP
