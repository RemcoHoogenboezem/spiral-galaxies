//----------------------------------------------------------------
#ifndef SPIRAL_GALAXIES_H
#define SPIRAL_GALAXIES_H
//----------------------------------------------------------------
#include <string>
#include <vector>
//----------------------------------------------------------------
#define EMPTY   -1
#define BORDER  -2
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class Cell
{
    private:
    public:

        int id;
        uint64_t mask;

        Cell(void) : id(BORDER) , mask(0){}
};
//----------------------------------------------------------------
class Centre
{
    private:
    public:

        int x;
        int y;

        Centre(void) : x(0) , y(0) {}
        Centre(int x,int y) : x(x) , y(y) {}
};
//----------------------------------------------------------------
class SpiralGalaxies
{
    private:

        int nRows,nCols,stride,nGalaxies;

        Cell * cells;
        Cell ** paddedBoard;
        Cell ** board;
        Centre * centres;
        int * histogram;

        void Clear(void);
        void PrintSolution(const string & message);
        bool Init(int nRows,int nCols,char * pCentres);
        void Count(int id,uint64_t mask,Cell * pC1,Cell * pC2,int & count);
        void Recursion(void);

    public:

        SpiralGalaxies(void);
        ~SpiralGalaxies(void);

        bool Run(int nRows,int nCols,const string & sCentres);
};
//----------------------------------------------------------------
#endif // SPIRAL_GALAXIES_H
