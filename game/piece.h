#pragma once
#include "gui/mainwindow.h"
#include "board.h"
#include "grid.h"


class Piece : public QGraphicsObject
{
    Q_OBJECT

public:
    L::Piece *lpiece;
    Piece(L::Piece *l_piece, Board * = nullptr);
    ~Piece();
    QPropertyAnimation *anim;
    Board *board;
    Grid *grid;
    void placeTo(Grid *grid_to, bool show = true);
    void makeMove(Grid *);
    void animateTo(Grid *, bool moveEnd = false);
    void select();
    void remove();
    static Piece *get(L::Piece *lp, Board *);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void moveEnd();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

namespace L {

class Piece {

public:
    enum Type {None, King, Queen, Bishop, Knight, Rook, Pawn};

    Piece(Type = Pawn, bool white = true, L::Grid * = nullptr, L::Board * = nullptr);
    ~Piece();
    int index;
    Type type;
    bool isWhite, inGame;

    L::Grid *lgrid;
    L::Board *lboard;
    vector<PieceMove *> *moves;

    bool isMoveValid(L::Grid *);
    void remove();

    void placeTo(L::Grid *gridTo); // setGrid
    void clearMoves();
    bool isProtected();
    bool moved() const;
    void makeMove(L::Grid *);
    vector<Grid *> getGrids(bool attacked = false);
};

}// NAMESPACE L

