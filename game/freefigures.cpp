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
        int x = (i % 8) * board->grid_size;
        int t = i > 7;
        bool r = board->reverse();
        int y = board->grid_size * ((figure->is_white ? (r ? 10 : 1 ) : (r ? 1 : 10) ) + (figure->is_white ? -t : t));
        figure->setPos( x, y );
    }
}
