//
// Created by Romain FOUYER on 2019-09-29.
//

#ifndef BABEL_IMAGE_HPP
#define BABEL_IMAGE_HPP

#include <QtWidgets/QLabel>
#include <QtGui/QImageReader>

namespace b12software {

    namespace babel {

        class Image : public QLabel {
        public:
            explicit Image(const std::string &path);
            Image(const std::string &path, int width, int height);
            ~Image() override;
        private:
            std::string _path;
            QImageReader *_imageReader;
            QImage *_image;
        };

    }

}


#endif //BABEL_IMAGE_HPP
