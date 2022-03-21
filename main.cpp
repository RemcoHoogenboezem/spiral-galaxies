//----------------------------------------------------------------
// Name        : main.cpp
// Author      : Remco Hoogenboezem
// Version     :
// Copyright   :
// Description : spiral galaxies puzzle solver
//----------------------------------------------------------------
#include <iostream>
#include <getopt.h>
#include "spiral_galaxies.h"
//----------------------------------------------------------------
#define ROWS            'r'
#define COLS            'c'
#define CENTRES         'C'
#define HELP            'h'
#define SHORT_OPTIONS   "r:c:C:h"
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
struct option longOptions[] =
{
    {"nRows",required_argument,nullptr,ROWS},
    {"nCols",required_argument,nullptr,COLS},
    {"centers",required_argument,nullptr,CENTRES},
    {"help",no_argument,nullptr,HELP}
};
//----------------------------------------------------------------
int main(int argc,char * argv[])
{
    //----------------------------------------------------------------
    //Get input arguments
    //----------------------------------------------------------------

    bool showHelp=(argc==1);
    int option,optionIndex;

    int nRows=0;
    int nCols=0;

    string centres;

    while((option=getopt_long(argc,argv,SHORT_OPTIONS,longOptions,&optionIndex))>=0)
    {
        switch(option)
        {
            case ROWS:

                nRows=atoi(optarg);
                break;

            case COLS:

                nCols=atoi(optarg);
                break;

            case CENTRES:

                centres=string(optarg);
                break;

            case HELP:

                showHelp=true;
                break;
        }
    }

    //----------------------------------------------------------------
    //Show help
    //----------------------------------------------------------------

    if(showHelp)
    {
        cerr << "spiral_galaxies [options] > result (output always to std::out)"                                    << endl;
        cerr                                                                                                        << endl;
        cerr << "-r --nRows <int>       Number of rows in puzzle (required)"                                        << endl;
        cerr << "-c --nCols <int>       Number of cols in puzzle (required)"                                        << endl;
        cerr << "-C --centres <string>  Semicolon separated list of galaxy centre coordinates (required)"           << endl;
        cerr << "-h --help <void>       This help"                                                                  << endl;
        cerr                                                                                                        << endl;

        return 0;
    }

    //----------------------------------------------------------------
    //Lets go
    //----------------------------------------------------------------

    SpiralGalaxies spiralGalaxies;
    spiralGalaxies.Run(nRows,nCols,centres);


    //----------------------------------------------------------------
    //Done
    //----------------------------------------------------------------

    return 0;
}
//----------------------------------------------------------------

//spiralGalaxies.Run(18,10,string("0.5,0.5;9.0,0.5;3.5,1.0;9.5,1.5;8.0,2.0;1.5,2.5;6.0,2.5;1.0,3.5;3.5,3.5;5.0,4.0;0.5,4.5;8.5,5.5;9.5,6.0;2.0,7.0;6.5,7.0;4.5,8.5;9.5,8.5;2.0,10.0;4.0,10.5;0.5,11.5;5.0,11.5;7.0,12.0;1.0,13.0;9.0,13.0;3.5,13.5;7.0,13.5;0.5,15.0;9.5,15.0;3.5,16.0;7.5,16.0;2.5,17.0;8.5,17.0;1.0,17.5;6.5,17.5;9.5,17.5"));
//spiralGalaxies.Run(7,7,"5.0,0.5;0.5,1.0;1.5,1.5;4.5,2.0;1.5,2.5;6.5,2.5;2.5,4.5;5.0,5.0;0.5,6.0;3.0,6.5;6.5,6.5");
//spiralGalaxies.Run(11,11,"4.5,0.5;6.5,0.5;8.5,1.0;2.0,2.0;4.5,2.5;6.5,2.5;7.5,2.5;9.5,2.5;0.5,3.5;3.5,3.5;1.0,4.5;7.5,4.5;1.5,5.5;2.5,5.5;10.5,6.0;1.0,6.5;7.5,7.5;5.5,8.5;1.5,9.0;3.5,9.0;0.5,9.5;5.5,10.5");

