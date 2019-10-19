//
// Created by Romain FOUYER on 2019-09-29.
//

#include "Text.hpp"

b12software::babel::Text::Text(const std::string &text, int fontSize)
: QLabel(),
  _text(text),
  _fontSize(fontSize)
{
    setText(QString::fromUtf8(_text.data()));
    setStyleSheet("font-size: " + QString::fromUtf8(std::to_string(_fontSize).data()) + "px; color: black;");
}