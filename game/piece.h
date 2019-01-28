#pragma once
#include "gui/mainwindow.h"
#include "board.h"
#include "grid.h"

class PieceMove;


class Piece : public QGraphicsObject
{
    Q_OBJECT

public:
    enum Type {None, King, Queen, Bishop, Knight, Rook, Pawn};
    bool isWhite, inGame;
    Type type;
    Piece(Type = Pawn, bool = true, Grid * = 0, Board * = 0);
    QPropertyAnimation *anim;
    Board *board;
    vector<PieceMove *> *moves;
    Grid *grid;
    PieceMove *currentMove;
    void placeTo(Grid *, bool show = true);
    void makeMove(Grid *, bool show = true);
    void animateTo(Grid *, bool place = false);
    bool isMoveValid(Grid *);
    void select();
    void remove(bool show = true);
    vector<Grid *> getGrids(bool attacked = false);
    void clearMoves();
    bool isProtected();

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool moved() const;
public slots:
    void moveEnd(bool show = true);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

