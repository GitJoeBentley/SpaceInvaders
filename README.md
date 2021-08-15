# SpaceInvaders
This is an C++ implementation using SFML libraries of the traditional Space Invaders game.

Compilation

Linux

Assuming you have a "current" g++ compiler and SFML libraries installed, you can use the approximate compile command to build the application.  This sample assumes that your SFML is installed in the /usr/local/ directories and that you are compiling in the directory that contains the src and include directories.

g++ -std=c++11 src/*.cpp -Iinclude -I/usr/local/include -L/usr/local/lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o space_invaders

Windows

Email me for instructions



