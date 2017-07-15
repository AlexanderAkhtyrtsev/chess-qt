#include "freefigures.h"

FreeFigures::FreeFigures(Board *brd)
{
    board = brd;
    figures = new vector<Figure *>;
}

FreeFigures::~FreeFigures()
{
    delete figures;
}


Figure *FreeFigures::addFigure(Figure *figure)
{
    figures->push_back(figure);
    update();
    return figure;
}



Figure *FreeFigures::removeFigure(Figure *figure)
{
    vector<Figure *>::iterator i = std::find(figures->begin(), figures->end(), figure);
    if ( i != figures->end() ) figures->erase(i);
    return figure;
}

void FreeFigures::update()
{
    int sz = figures->size();
    Figure *figure;
    for(int i = 0; i < sz; i++)
    {
        figure = figures->at(i);
        int x = (i % 8);
        int t = i > 7;
        bool r = board->reverse();
        int top = 1 - t,
         bottom = 10 + t;
        int y = r ? (figure->is_white ? bottom : top) : (figure->is_white ? top : bottom);

        figure->setPos( x * board->grid_size, y * board->grid_size );
    }
}
