#pragma once

#include <QtWidgets>
#include <vector>
#include <stack>
#include <algorithm>
#include <cassert>
#include "gametimer.h"

const int MIN_SCORE = -999999;
const int MAX_SCORE =  999999;


using std::vector;
using std::stack;

class Piece;
class Grid;
class FreePieces;
class MainWindow;
class Options;
class AIThread;

namespace L {

class PieceMove;
class Grid;
class Piece;

class Board {
public:
    static const int initPiecePos[8][8];
    Grid *grids[8][8];
    vector<PieceMove *> *moves;
    PieceMove *currentMove;
    vector<Piece *> *pieces, *pieces_w, *pieces_b;
    Piece *King[2];
    bool move;

    Board();
    Board(const Board& b);
    ~Board();
    Grid *grid(int x, int y) const;
    Piece *piece(int index) const;
    bool isCheck(bool) const;
    int check_game();
    void undoMove();

    vector<PieceMove> getAllMoves();

    int getCurrentScore();

    int getPiecePosEval(Piece *piece);
};


class PieceMove
{
public:
    PieceMove(L::Piece *_piece = nullptr,
              L::Grid *_from = nullptr,
              L::Grid *_to = nullptr,
              L::Piece *rem = nullptr,
              bool _extra = false);

    L::Piece *lpiece;
    L::Grid *from, *to;
    L::Piece *removed;
    int isNull() const;
    bool extra;
};


} // L NAMESPACE

class Board : public QGraphicsScene
{
    Q_OBJECT
    bool m_reverse;
    QGraphicsPixmapItem *boardTex;
    unsigned m_size;
    bool m_endGame;
    QGraphicsProxyWidget *pw_timer[2];
    QGraphicsTextItem *coords[32];

public:
    L::Board  *lboard;
    AIThread *aiThread;
    GameTimer *timer[2];
    QSize size;

    std::stack<Grid *> highlightedGrids;

    Board(QWidget* = nullptr);
    ~Board();

    void replaceElements();
    void undoMove();
    Board *resetHighligtedGrids();
    bool reverse() const; // getter
    bool reverse(bool reverse);     // setter

    QPixmap *chess_tiles;

    int grid_size;
    bool piecesSelectable;
    Grid* grids[8][8];
    vector<Piece *> *pieces;
    stack<quint32> timersValue[2];
    vector<Piece *> *pieces_w, *pieces_b;
    Piece *selected, *King[2];
    FreePieces *free_pieces[2];
    MainWindow *window;
    Options *options;
    void endGame();
    void computerMove();
public slots:
    void computerMoveEnd();
    void newGame();
    void doAutoMove();
    void pieceMoveCompleted();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class AIThread : public QThread
{
    Board *board;
    qint64 totalIterations;
public:
    L::PieceMove bestMove;
    AIThread(Board *board);
    void run();
    int minimax(L::Board *lboard, int depth, int alpha = MIN_SCORE, int beta = MAX_SCORE);
    L::PieceMove getBestMove(L::Board *lboard, int depth);
};

