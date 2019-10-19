//
// Created by Romain FOUYER on 2019-09-29.
//

#ifndef BABEL_TEXT_HPP
#define BABEL_TEXT_HPP

#include <QtWidgets/QLabel>

namespace b12software {

    namespace babel {

        class Text : public QLabel {
        public:
            Text(const std::string &text, int fontSize);
            ~Text() = default;
        private:
            std::string _text;
            int _fontSize;
        };

    }

}


#endif //BABEL_TEXT_HPP
