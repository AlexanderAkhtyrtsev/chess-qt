#pragma once
#include "figure.h"
#include <map>
using std::map;

class FreeFigures
{
public:

    FreeFigures(Board *);
    ~FreeFigures();


    Board *board;
    Figure *addFigure(Figure *);
    Figure *removeFigure(Figure *);
    vector<Figure *> *figures;
    void update();
};
