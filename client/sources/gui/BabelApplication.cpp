//
// Created by Romain FOUYER on 2019-09-18.
//

#include "BabelApplication.hpp"


b12software::babel::BabelApplication::BabelApplication(int &argc, char **argv, b12software::network::NetworkClient &client)
: _application(argc, argv),
  _window(new b12software::babel::BabelWindow(client)),
  _client(client)
{
    _client.run();
}

b12software::babel::BabelApplication::~BabelApplication()
{
    delete _window;
}

int b12software::babel::BabelApplication::run()
{
    _window->show();
    return _application.exec();
}