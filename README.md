# Informations
The babel is an Epitech project who's goal is to create a VoIP protocol and implement it in c++ using the following libraries:
* Qt (client side)
* Boost(server/client side)
* BoostAsio(server side)
* PortAudio(client side)
* Opus(client side)

The project should be cross platform (windows/linux) and consists of a server and a client. The server connects clients between them and handle messages/commands, but the voice communication should be done in p2p using udp between two clients.

# Authors
[Julian Frabel](https://github.com/Eldriann)

[Charlie Jeanneau](https://github.com/JeSuisCharlie1)

[Brian Guitteny](https://github.com/briangtn)

[Romain Fouyer](https://github.com/romanosaurus)

# How to install

## Before install:
* You must have **conan** and **cmake** installed.
* You must have a compiler installed (windows: **Visual Studio 15 2017**, unix: **g++**)

Of course you should have git and have cloned this repository.

## Windows:
First add the conan remote that have all the dependencies required:

    conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
    conan remote add epitech https://api.bintray.com/conan/epitech/public-conan
    
Then make build folder and go into it:
    
    mkdir build
    cd build
    
Run conan to install all dependencies to your computer:

    conan install ..
    
Run CMake:

    cmake .. -G "Visual Studio 15 2017 Win64"
    
You need to change this line if you want to compile on 32bits windows or if you want to use an other version of visual studio (then i will let you search how to change the command).

At this point the .sln will be generated and you can freely change the code by opening it.

Finally if nothing went wrong you just need to build the exe file:

    cmake --build . --config Release
    
If you want to change the config to debug simply use Debug instead of Release.

## Linux:
Simply go to the folder setup and run setup_unix.sh (or run all the commands from setup_unix.sh manually)

# Documentation:
You can access the babel protocol RFC and the doxygen documentation in the docs folder or [here](https://babel.b12powered.com).
