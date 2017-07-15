#ifndef FIGURE_H
#define FIGURE_H

#include "gui/mainwindow.h"
#include "board.h"
#include "grid.h"

class FigureMove;


class Figure : public QGraphicsObject
{
    Q_OBJECT

public:
    enum Type {None, King, Queen, Bishop, Knight, Rook, Pawn};
    bool is_white, in_game;
    Type type;
    Figure(Type = Pawn, bool = true, Grid * = 0, Board * = 0);
    QPropertyAnimation *anim;
    Board *board;
    vector<FigureMove *> *moves;
    Grid *grid, *next_move;
    void placeTo(Grid *);
    void move(Grid *);
    bool move_valid(Grid *);
    void Select();
    void Remove();
    vector<Grid *> getGrids(bool attacked = false);
    bool isProtected();
    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool moved() const;
public slots:
    void moveEnd();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // FIGURE_H
