#pragma once

#include <QtWidgets>
#include <QVector>
#include <QStack>
#include <algorithm>


#ifdef _DEBUG
#include <cassert>
#endif

const int MIN_SCORE =  INT_MIN;
const int MAX_SCORE =  INT_MAX;
const int MIN_GRID_SIZE = 24;


class Piece;
class Grid;
class FreePieces;
class MainWindow;
class Options;
class AIThread;

class PieceMove;
class LGrid;
class LPiece;
class GameTimer;

class LBoard {
public:
    static const int initPiecePos[8][8];
    LGrid *grids[8][8];
    QVector<PieceMove *> *moves;
    PieceMove *currentMove;
    QVector<LPiece *> *pieces, *pieces_w, *pieces_b;
    LPiece *King[2];
    bool move;
    bool positionChanged {true};

    LBoard();
    LBoard(const LBoard& b);
    ~LBoard();

    LGrid *grid(int x, int y) const;
    LPiece *piece(int index) const;
    bool isCheck(bool) const;
    int check_game() const;
    void undoMove();

    QVector<PieceMove> getAllMoves() const;
    int getCurrentScore() ;
    int getPiecePosEval(LPiece *piece) const;
};

class Board : public QGraphicsScene
{
    Q_OBJECT
private:
    bool m_reverse;
    QGraphicsPixmapItem *boardTexture;
    unsigned m_size;
    bool m_endGame;
    QGraphicsProxyWidget *pw_timer[2];
    QGraphicsTextItem *coords[32];
    bool piecesSelectable;
public:
    LBoard  *lboard;
    AIThread *aiThread;
    GameTimer *timer[2];
    QSize size;

    QStack<Grid *> highlightedGrids;

    Board(QWidget* = nullptr);
    ~Board();


    Board *resetHighligtedGrids();
    bool reverse() const; // getter
    bool reverse(bool reverse);     // setter
    QPixmap *piecesTileset;
    int grid_size;
    Grid* grids[8][8];
    QVector<Piece *> *pieces;
    QStack<quint32> timersValue[2];
    QVector<Piece *> *pieces_w, *pieces_b;
    Piece *selected, *King[2];
    FreePieces *free_pieces[2];
    MainWindow *window;
    Options *options;
    void endGame();
    void computerMove();
    bool isPiecesSelectable() const;
    void setPiecesSelectable(bool value);
    void replaceElements();
    void undoMove();
public slots:
    void computerMoveEnd();
    void newGame();
    void doAutoMove();
    void pieceMoveCompleted();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};


