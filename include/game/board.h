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



class PieceMove;
class LGrid;
class LPiece;

class LBoard {
public:
    static const int initPiecePos[8][8];
    LGrid *grids[8][8];
    vector<PieceMove *> *moves;
    PieceMove *currentMove;
    vector<LPiece *> *pieces, *pieces_w, *pieces_b;
    LPiece *King[2];
    bool move;

    LBoard();
    LBoard(const LBoard& b);
    ~LBoard();
    LGrid *grid(int x, int y) const;
    LPiece *piece(int index) const;
    bool isCheck(bool) const;
    int check_game();
    void undoMove();

    vector<PieceMove> getAllMoves();

    int getCurrentScore();

    int getPiecePosEval(LPiece *piece);
};


class PieceMove
{
public:
    PieceMove(LPiece *_piece = nullptr,
              LGrid *_from = nullptr,
              LGrid *_to = nullptr,
              LPiece *rem = nullptr,
              bool _extra = false);

    LPiece *lpiece;
    LGrid *from, *to;
    LPiece *removed;
    int isNull() const;
    bool extra;
    bool operator==(PieceMove &);
};



class Board : public QGraphicsScene
{
    Q_OBJECT
    bool m_reverse;
    QGraphicsPixmapItem *boardTexture;
    unsigned m_size;
    bool m_endGame;
    QGraphicsProxyWidget *pw_timer[2];
    QGraphicsTextItem *coords[32];

public:
    LBoard  *lboard;
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

    QPixmap *piecesTileset;

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
    PieceMove bestMove;
    AIThread(Board *board);
    /* virtual */ void run();
    int minimax(LBoard *lboard,
                int depth,
                int alpha = MIN_SCORE,
                int beta = MAX_SCORE);
    PieceMove getBestMove(LBoard *lboard, int depth);
};

