#!/usr/bin/env bash

PS3='Do you want to install bincrafters remote: '
options=("Yes" "No")
select opt in "${options[@]}"
do
    case $opt in
        "Yes")
            conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
            break
            ;;
        "No")
            break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done
PS3='Do you want to install epitech remote: '
options=("Yes" "No")
select opt in "${options[@]}"
do
    case $opt in
        "Yes")
            conan remote add epitech https://api.bintray.com/conan/epitech/public-conan
            break
            ;;
        "No")
            break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done
mkdir ../build
cd ../build
conan install .. --build missing
cmake ..
cmake -build .
make
echo "You can find the binary inside ../build/bin/"