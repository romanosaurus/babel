//
// Created by Romain FOUYER on 2019-09-29.
//

#include "Image.hpp"

b12software::babel::Image::Image(const std::string &path)
: QLabel(),
  _path(path),
  _imageReader(new QImageReader(QString::fromUtf8(path.data()))),
  _image(new QImage(_imageReader->read()))
{
    setPixmap(QPixmap::fromImage(*_image));
}

b12software::babel::Image::Image(const std::string &path, int width, int height)
: QLabel(),
  _path(path),
  _imageReader(new QImageReader(QString::fromUtf8(path.data()))),
  _image(new QImage(_imageReader->read()))
{
    setPixmap(QPixmap::fromImage(_image->scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}

b12software::babel::Image::~Image()
{
    delete _imageReader;
    delete _image;
}