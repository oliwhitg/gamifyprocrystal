# gamifyprocrystal

Simple Gamify of procrystal systems

Compile with:
//
cmake .
make
g++ main.cpp -o main.x -lsfml-graphics -lsfml-window -lsfml-system
//

You may need to change the CMakeCache.txt file, and install SFML for your system (I used https://www.sfml-dev.org/tutorials/2.5/start-linux.php)

Use the input files to control mc parameters
Configuration chosen by ./main.x [fileno] where fileno is a number between 2-15

In the app:\
*tab      saves the config connections\
*left     click on node rotates it\
*right    click converts between bent/linear forms of 2 coordinate nodes\
*enter    runs mc for number of steps OR until energy is halved\
