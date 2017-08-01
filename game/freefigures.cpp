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
    int sz = figures->size();
    if (!sz) {
        figures->push_back(figure);
    } else {
        bool f = false;
        for(int i=0; i<sz; i++) {
            if(figures->at(i)->type == figure->type) {
                figures->insert(figures->begin()+1+i, figure);
                f = true;
                break;
            }
        }
        if (!f) {
            figures->push_back(figure);
        }
    }
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

    int empty = 0;
    for(int i = 0; i < sz; i++)
    {
        figure = figures->at(i);
        int top = 1,
         bottom = 10;
        int y = board->reverse() ? (figure->is_white ? bottom : top) : (figure->is_white ? top : bottom);
        if ( i > 0 && figure->type != figures->at(i-1)->type ) empty+=2;
        figure->setPos( (empty + i) * board->grid_size / 3.5, y * board->grid_size );
    }
}
