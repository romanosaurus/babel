//
// Created by Romain FOUYER on 2019-09-24.
//

#ifndef BABEL_LOADINGWIDGET_HPP
#define BABEL_LOADINGWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QMovie>

namespace b12software {

    namespace babel {

        class LoadingWidget : public QWidget {
        public:
            explicit LoadingWidget(QWidget *parent);
            ~LoadingWidget() override;

            QMovie *getMovie();

        private:
            QFormLayout *_spinnerLayout;
            QMovie *_movie;
            QGridLayout *_layout;
            QLabel *_label;
        };

    }

}


#endif //BABEL_LOADINGWIDGET_HPP
