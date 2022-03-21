//----------------------------------------------------------------
// Name        : spiral_galaxies.cpp
// Author      : Remco Hoogenboezem
// Version     :
// Copyright   :
// Description :
//----------------------------------------------------------------
#include <iostream>
#include <cstring>
#include <chrono>
#include "spiral_galaxies.h"
//----------------------------------------------------------------
void SpiralGalaxies::Clear(void)
{
    if(cells!=nullptr)
    {
        delete [] cells;
        cells=nullptr;
    }

    if(paddedBoard!=nullptr)
    {
        delete [] paddedBoard;
        paddedBoard=nullptr;
    }

    if(centres!=nullptr)
    {
        delete [] centres;
        centres=nullptr;
    }

    if(histogram!=nullptr)
    {
        delete [] histogram;
        histogram=nullptr;
    }
}
//----------------------------------------------------------------
void SpiralGalaxies::PrintSolution(const string & message=string())
{
    if(message.empty()==false) cout << message << "\n\n";

    for(int row=nRows-1;row>=0;row--)
    {
        int y=(row<<1)+1;

        for(int col=0;col<nCols;col++)
        {
            int x=(col<<1)+1;
            int id=board[row][col].id;
            int xCentre=centres[id].x;
            int yCentre=centres[id].y;

            if(y+1==yCentre)
            {
                if(x-1==xCentre)
                {
                    cout << "O   ";
                    continue;
                }

                if(x==xCentre)
                {
                    cout << "+ O ";
                    continue;
                }
            }

            if(board[row+1][col].id==board[row][col].id)
            {
                cout << "+   ";
                continue;
            }

            cout << "+---";
        }

        cout << "+\n";

        for(int col=0;col<nCols;col++)
        {
            int x=(col<<1)+1;
            int id=board[row][col].id;
            int xCentre=centres[id].x;
            int yCentre=centres[id].y;

            if(y==yCentre)
            {
                if(x-1==xCentre)
                {
                    cout << "O   ";
                    continue;
                }

                if(x==xCentre)
                {
                    if(board[row][col-1].id==board[row][col].id)
                    {
                        cout << "  O ";
                        continue;
                    }

                    cout << "| O ";
                    continue;
                }
            }

            if(board[row][col-1].id==board[row][col].id)
            {
                cout << "    ";
                continue;
            }

            cout << "|   ";
        }

        cout << "|\n";
    }

    for(int col=0;col<nCols;col++) cout << "+---"; cout << "+\n";

    cout << flush;
}
//----------------------------------------------------------------
bool SpiralGalaxies::Init(int nRows,int nCols,char * pCentres)
{
    //----------------------------------------------------------------
    //Check input arguments
    //----------------------------------------------------------------

    if(nRows<4)
    {
        cerr << "Error: Number of rows in universe should be at least 4" << endl;
        return false;
    }

    if(nCols<4)
    {
        cerr << "Error: Number of cols in universe should be at least 4" << endl;
        return false;
    }

    if(pCentres[0]=='\0')
    {
        cerr << "Error: Please specify a semicolon separated list of galaxy centre coordinates (x1,y1;x2,y2...)" << endl;
        return false;
    }

    //----------------------------------------------------------------
    //Create padded board
    //----------------------------------------------------------------

    int nPaddedRows=1 + nRows + 1;
    int nPaddedCols=1 + nCols + 1;

    Cell * cells=new Cell[nPaddedRows * nPaddedCols];    //All cells initialized to BORDER
    Cell ** paddedBoard=new Cell*[nPaddedRows];

    paddedBoard[0]=&cells[1];
    for(int row=1;row<nPaddedRows;row++) paddedBoard[row]=paddedBoard[row-1]+nPaddedCols;

    Cell ** board=&paddedBoard[1];

    for(int row=0;row<nRows;row++)
    {
        for(int col=0;col<nCols;col++) board[row][col].id=EMPTY;
    }

    //----------------------------------------------------------------
    //Parse galaxy centres
    //----------------------------------------------------------------

    int xMax=nCols<<1;
    int yMax=nRows<<1;

    vector<Centre> vCentres;

    for(char * pCentre=strsep(&pCentres,";");pCentre!=nullptr;pCentre=strsep(&pCentres,";"))
    {
        char * pX=strsep(&pCentre,",");
        char * pY=strsep(&pCentre,",");

        if(pY==nullptr || pCentre!=nullptr)
        {
            cerr << "Error: Two fields required (x,y) for each galaxy center" << endl;
            delete [] cells;
            delete [] paddedBoard;
            return false;
        }

        int x=int(atof(pX)*2.0 + 0.5);
        int y=int(atof(pY)*2.0 + 0.5);

        if(x<=0 || x>=xMax)
        {
            cerr << "Error: Galaxy center x coordinate can't be equal/smaller than zero or equal/larger than the maximum possible x coordinate" << endl;
            delete [] cells;
            delete [] paddedBoard;
            return false;
        }

        if(y<=0 || y>=yMax)
        {
            cerr << "Error: Galaxy center y coordinate can't be equal/smaller than zero or equal/larger than the maximum possible y coordinate" << endl;
            delete [] cells;
            delete [] paddedBoard;
            return false;
        }

        int isEven=(y & 1) == 0;
        int row1=(y-isEven)>>1;
        int row2=(y+isEven)>>1;

        isEven=(x & 1) == 0;
        int col1=(x-isEven)>>1;
        int col2=(x+isEven)>>1;

        if(board[row1][col1].id!=EMPTY || board[row1][col2].id!=EMPTY || board[row2][col1].id!=EMPTY || board[row2][col2].id!=EMPTY)
        {
            cerr << "Error: Overlapping galaxies in begin state!" << endl;
            delete [] cells;
            delete [] paddedBoard;
            return false;
        }

        int id=vCentres.size();

        if(id>63)
        {
            cerr << "Error: Currently this implementation only supports 64 galaxies!" << endl;
            delete [] cells;
            delete [] paddedBoard;
            return false;
        }

        board[row1][col1].id=id;
        board[row1][col2].id=id;
        board[row2][col1].id=id;
        board[row2][col2].id=id;

        vCentres.push_back(Centre(x,y));
    }

    int nGalaxies=vCentres.size();

    //----------------------------------------------------------------
    //Copy centres
    //----------------------------------------------------------------

    Centre * centres=new Centre[nGalaxies];
    for(int i=0;i<nGalaxies;i++) centres[i]=vCentres[i];

    //----------------------------------------------------------------
    //Init histogram
    //----------------------------------------------------------------

    int * histogram=new int[nGalaxies];
    for(int i=0;i<nGalaxies;i++) histogram[i]=0;

    for(int row=0;row<nRows;row++)
    {
        for(int col=0;col<nCols;col++)
        {
            int id=board[row][col].id;
            if(id>=0) histogram[id]++;
        }
    }

    //----------------------------------------------------------------
    //Store all in object
    //----------------------------------------------------------------

    Clear();

    this->nRows=nRows;
    this->nCols=nCols;
    this->stride=nPaddedCols;
    this->nGalaxies=nGalaxies;

    this->cells=cells;
    this->paddedBoard=paddedBoard;
    this->board=board;
    this->centres=centres;
    this->histogram=histogram;

    //----------------------------------------------------------------
    //Done
    //----------------------------------------------------------------

    return true;
}
//----------------------------------------------------------------
void SpiralGalaxies::Count(int id,uint64_t mask,Cell * pC1,Cell * pC2,int & count)
{
    //----------------------------------------------------------------
    //c1 and c2 visited
    //----------------------------------------------------------------

    pC1->mask|=mask;
    pC2->mask|=mask;

    //----------------------------------------------------------------
    //Increase count for current id
    //----------------------------------------------------------------

    if(pC1->id==id) count+=1+int(pC1!=pC2);

    //----------------------------------------------------------------
    //Move c1 down and c2 up
    //----------------------------------------------------------------

    Cell * pC1New=pC1-stride;
    Cell * pC2New=pC2+stride;

    if((pC1New->mask & mask)==0ULL && pC1New->id==pC2New->id && (pC1New->id==EMPTY || pC1New->id==id)) Count(id,mask,pC1New,pC2New,count);

    //----------------------------------------------------------------
    //Move c1 up and c2 down
    //----------------------------------------------------------------

    pC1New=pC1+stride;
    pC2New=pC2-stride;

    if((pC1New->mask & mask)==0ULL && pC1New->id==pC2New->id && (pC1New->id==EMPTY || pC1New->id==id)) Count(id,mask,pC1New,pC2New,count);

    //----------------------------------------------------------------
    //Move c1 left and c2 right
    //----------------------------------------------------------------

    pC1New=pC1-1;
    pC2New=pC2+1;

    if((pC1New->mask & mask)==0ULL && pC1New->id==pC2New->id && (pC1New->id==EMPTY || pC1New->id==id)) Count(id,mask,pC1New,pC2New,count);

    //----------------------------------------------------------------
    //Move c1 right and c2 left
    //----------------------------------------------------------------

    pC1New=pC1+1;
    pC2New=pC2-1;

    if((pC1New->mask & mask)==0ULL && pC1New->id==pC2New->id && (pC1New->id==EMPTY || pC1New->id==id)) Count(id,mask,pC1New,pC2New,count);
}
//----------------------------------------------------------------
void SpiralGalaxies::Recursion(void)
{
    //----------------------------------------------------------------
    //Clear masks
    //----------------------------------------------------------------

    for(int row=0;row<nRows;row++)
    {
        for(int col=0;col<nCols;col++) board[row][col].mask=0ULL;
    }

    //----------------------------------------------------------------
    //Test current board
    //----------------------------------------------------------------

    for(int id=0;id<nGalaxies;id++)
    {
        int x=centres[id].x;
        int y=centres[id].y;
        int xIsEven=(x & 1) == 0;
        int yIsEven=(y & 1) == 0;

        Cell * pC1=&board[(y-yIsEven)>>1][(x-xIsEven)>>1];
        Cell * pC2=&board[(y+yIsEven)>>1][(x+xIsEven)>>1];

        int count=0;
        Count(id,1ULL<<id,pC1,pC2,count);

        if(count!=histogram[id]) return;
    }

    //----------------------------------------------------------------
    //Find cell with least number of options
    //----------------------------------------------------------------

    int rowMin,colMin;
    int nOptionsMin=nGalaxies+1;

    for(int row=0;row<nRows;row++)
    {
        for(int col=0;col<nCols;col++)
        {
            if(board[row][col].id!=EMPTY) continue;

            int nOptions=__builtin_popcountll(board[row][col].mask);

            if(nOptions==0) return;
            if(nOptions>=nOptionsMin) continue;

            rowMin=row;
            colMin=col;
            nOptionsMin=nOptions;
        }
    }

    if(nOptionsMin==nGalaxies+1)
    {
        PrintSolution("Solution found!");
        return;
    }

    //----------------------------------------------------------------
    //Recursion
    //----------------------------------------------------------------

    int x=(colMin<<1)+1;
    int y=(rowMin<<1)+1;

    uint64_t mask=board[rowMin][colMin].mask;

    for(;mask!=0ULL;mask&=mask-1)
    {
        int id=__builtin_ctzll(mask);
        int row=((centres[id].y<<1)-y)>>1;
        int col=((centres[id].x<<1)-x)>>1;

        board[rowMin][colMin].id=id;
        board[row][col].id=id;

        histogram[id]+=2;

        Recursion();

        board[row][col].id=EMPTY;

        histogram[id]-=2;
    }

    board[rowMin][colMin].id=EMPTY;
}
//----------------------------------------------------------------
SpiralGalaxies::SpiralGalaxies(void) : cells(nullptr) , paddedBoard(nullptr) , centres(nullptr) , histogram(nullptr)
{
}
//----------------------------------------------------------------
SpiralGalaxies::~SpiralGalaxies(void)
{
    Clear();
}
//----------------------------------------------------------------
bool SpiralGalaxies::Run(int nRows,int nCols,const string & sCentres)
{
    //----------------------------------------------------------------
    //Init object (Create local copy of string)
    //----------------------------------------------------------------

    char * pCentres=strcpy(new char[sCentres.size()+1],sCentres.c_str());
    bool ret=Init(nRows,nCols,pCentres);
    delete [] pCentres;

    if(ret==false) return false;

    //----------------------------------------------------------------
    //Recursion
    //----------------------------------------------------------------

    auto start = chrono::steady_clock::now();

    Recursion();

    auto end = chrono::steady_clock::now();

    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;

    //----------------------------------------------------------------
    //Done
    //----------------------------------------------------------------

    return true;
}
//----------------------------------------------------------------





/*
void SpiralGalaxies::PrintState(const string & message=string())
{
    if(message.empty()==false) cout << message << "\n\n";

    for(int row=nRows-1;row>=0;row--)
    {
        for(int col=0;col<nCols;col++) cout << "+---"; cout << "+\n";

        for(int col=0;col<nCols;col++)
        {
            int id=board[row][col].id;

            cout << '|';
            if(id<100) cout << ' ';
            if(id<10) cout << ' ';
            if(id>=0) cout << id;
            if(id==EMPTY) cout << ' ';
        }

        cout << "|\n";
    }

    for(int col=0;col<nCols;col++) cout << "+---"; cout << "+\n";

    cout << flush;
}
//----------------------------------------------------------------
void SpiralGalaxies::PrintOptions(const string & message=string())
{
    if(message.empty()==false) cout << message << "\n\n";

    for(int row=nRows-1;row>=0;row--)
    {
        for(int col=0;col<nCols;col++) cout << "+---"; cout << "+\n";

        for(int col=0;col<nCols;col++)
        {
            if(board[row][col].id!=EMPTY)
            {
                cout << "|   ";
                continue;
            }

            int nOptions=__builtin_popcountll(board[row][col].mask);

            cout << '|';
            if(nOptions<100) cout << ' ';
            if(nOptions<10) cout << ' ';
            cout << nOptions;
        }

        cout << "|\n";
    }

    for(int col=0;col<nCols;col++) cout << "+---"; cout << "+\n";

    cout << flush;
}
//----------------------------------------------------------------
*/
