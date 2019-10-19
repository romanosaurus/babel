//
// Created by Romain FOUYER on 2019-09-18.
//

#ifndef BABEL_BABELAPPLICATION_HPP
#define BABEL_BABELAPPLICATION_HPP

#include <QtWidgets/QApplication>

#include "BabelWindow.hpp"
#include "network/NetworkClient.hpp"

namespace b12software {

    namespace babel {

        class BabelApplication {
        public:
            BabelApplication(int &argc, char **argv, b12software::network::NetworkClient &_client);
            ~BabelApplication();

            int run();
        private:
            QApplication _application;
            b12software::babel::BabelWindow *_window;
            b12software::network::NetworkClient &_client;
        };

    }

}

#endif //BABEL_BABELAPPLICATION_HPP
