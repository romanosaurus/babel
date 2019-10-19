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

# Information:
Please document each part of your code.

All of the babel work should be done in the b12software::babel namespace.

# Documentation:
You can access the babel protocol RFC and the doxygen documentation in the docs folder or [here](babel.b12powered.com).