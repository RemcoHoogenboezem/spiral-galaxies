# spiral-galaxies
Simple program to solve spiral galaxy puzzles from the command line
The program requires you to give the board dimensions and galaxy centres
For example:

$ ./spiral_galaxies --help

spiral_galaxies [options] > result (output always to std::out)

-r --nRows <int>       Number of rows in puzzle (required)
-c --nCols <int>       Number of cols in puzzle (required)
-C --centres <string>  Semicolon separated list of galaxy centre coordinates (required)
-h --help <void>       This help


$ ./spiral_galaxies -r 7 -c 7 -C 5.0,0.5;0.5,1.0;1.5,1.5;4.5,2.0;1.5,2.5;6.5,2.5;2.5,4.5;5.0,5.0;0.5,6.0;3.0,6.5;6.5,6.5

Solution found!

"+---+---+---+---+---+---+---+"
"|   |       O       |   | O |"
"+ O +---+---+---+---+   +---+"
"|   |           |           |"
"+---+---+   +---+   O   +---+"
"|       | O |           |   |"
"+   +---+   +---+   +---+   +"
"|   |           |   |   |   |"
"+   +---+---+---+---+   +   +"
"|     O     |           | O |"
"+---+---+   +   + O +   +   +"
"|   | O |   |           |   |"
"+ O +---+   +   +---+---+   +"
"|   |       |   |   O   |   |"
"+---+---+---+---+---+---+---+"
Elapsed time: 160 us

