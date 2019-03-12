#pragma once

#include <QtWidgets>
#include <vector>
#include <stack>
#include <algorithm>
#include <cassert>

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
    Board();
    static const int initPiecePos[8][8];
    Grid* grids[8][8];
    bool move;
    vector<PieceMove *> *moves;
    PieceMove *currentMove;
    vector<Piece *> *pieces, *pieces_w, *pieces_b;
    Piece *King[2];


    vector<PieceMove> getAllMoves();
    PieceMove getBestMove(int depth = 1);

    bool is_check(bool w);
    int getCurrentScore();
    int minimax(int depth, int alpha = MIN_SCORE, int beta = MAX_SCORE);

    int check_game();

    void undoMove(); // ??
};


class PieceMove
{
public:
    PieceMove(L::Piece *_piece = nullptr, L::Grid *_from = nullptr, L::Grid *_to = nullptr, L::Piece *rem = nullptr, bool _extra = false);
    L::Piece *lpiece;
    L::Grid *from, *to;
    L::Piece *removed;
    bool isNull() const;
    bool extra;
};


}

class Board : public QGraphicsScene
{
    Q_OBJECT
    bool m_reverse;
    QGraphicsPixmapItem *boardTex;
    unsigned m_size;
public:
    L::Board  *lboard;
    AIThread *aiThread;
    QSize size;
    Board(QWidget* = nullptr);
    ~Board();
    unsigned int spacing;

    void ReplaceElements();
    void undoMove();
    Board *ResetHighligtedGrids();
    bool reverse() const; // getter
    bool reverse(bool reverse);     // setter


    int grid_size;


    bool piecesSelectable;
    Grid* grids[8][8];
    QPixmap* chess_tiles;
    vector<Piece *> *pieces;
    stack<quint32> timerValue[2];
    vector<Piece *> *pieces_w, *pieces_b;
    Piece *selected, *King[2];
    FreePieces *free_pieces[2];
    MainWindow *window;
    Options *options;

    void computerMove();
public slots:
    void computerMoveEnd();
    void newGame();
    void doAutoMove();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class AIThread : public QThread
{
    Board *board;
    public:
    L::PieceMove bestMove;
    AIThread(Board *board);
    void run();
};
