#pragma once
#include "mainwindow.h"
#include "board.h"
#include "grid.h"


class Piece : public QGraphicsObject
{
    Q_OBJECT
private:
    QPropertyAnimation *anim;
public:
    LPiece *lpiece;
    Piece(LPiece *l_piece, Board * = nullptr);
    ~Piece();
    Board *board;
    Grid *grid;
    void placeTo(Grid *grid_to, bool show = true);
    void makeMove(Grid *);
    void animateTo(Grid *, bool moveEnd = false);
    void select();
    void remove();
    static Piece *get(LPiece *lp, Board *);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool isAnimationRunning() const;
    void stopAnimation();
public slots:
    void moveEnd();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};



const struct { bool extra, diagonal,  straight; }  pieceBehavior[7] = {
    {0,0,0}, // how do u think ?)))
    {1,0,0}, // King
    {0,1,1}, // Queen
    {0,1,0}, // Bishop
    {1,0,0}, // Knight
    {0,0,1}, // Rook
    {1,0,0} // Pawn
};

class LPiece {
    QVector<LGrid *> m_cachedGrids,
                    m_cachedAttackedGrids;
    bool gridsCached {false},
         attackedGridsCached {false};
public:
    enum Type {None, King, Queen, Bishop, Knight, Rook, Pawn};


    LPiece(Type = Pawn, bool white = true, LGrid * = nullptr, LBoard * = nullptr);
    ~LPiece();

    int index {-1};
    Type type;
    bool isWhite, inGame {true};

    LGrid *lgrid;
    LBoard *lboard;
    quint32 moves{0};

    bool isMoveValid(LGrid *);
    void remove();

    void placeTo(LGrid *gridTo); // setGrid
    void clearMoves();
    bool isProtected() const;
    bool isMoved() const;
    void makeMove(LGrid *, bool fake = false);
    QVector<LGrid *> getGrids(bool attacked = false);
};


