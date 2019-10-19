//
// Created by Romain FOUYER on 2019-09-24.
//

#include "gui/widgets/LoadingWidget.hpp"
#include "gui/utilities/AssetsManager.hpp"

b12software::babel::LoadingWidget::LoadingWidget(QWidget *parent)
: QWidget(parent),
  _spinnerLayout(new QFormLayout),
  _movie(new QMovie(QString::fromUtf8(b12software::babel::AssetsManager::getInstance()["loading"].data()))),
  _layout(new QGridLayout),
  _label(new QLabel)
{
    _label->setMovie(_movie);
    _spinnerLayout->setFormAlignment(Qt::AlignVCenter | Qt::AlignHCenter | Qt::AlignTop);

    _layout->addWidget(_label, 0, 0, Qt::AlignHCenter);
    _layout->addLayout(_spinnerLayout, 1, 0);
    setStyleSheet("background-color:#74b9ff;");
    setLayout(_layout);
}

b12software::babel::LoadingWidget::~LoadingWidget()
{
    delete _movie;
    delete _label;
    delete _spinnerLayout;
    delete _layout;
}

QMovie* b12software::babel::LoadingWidget::getMovie()
{
    return _movie;
}