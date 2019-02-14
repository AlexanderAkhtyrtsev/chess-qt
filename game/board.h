#pragma once

#include <QtWidgets>
#include <vector>
#include <algorithm>

const int BOARD_SIZE = 10;
const int MIN_SCORE = -999999;
const int MAX_SCORE =  999999;


using std::vector;
class Piece;
class Grid;
class PieceMove;
class FreePieces;
class MainWindow;
class Options;


class Board : public QGraphicsScene
{
    Q_OBJECT
    bool m_reverse;
    QGraphicsPixmapItem *boardTex;
    unsigned m_size;
public:
    Board(QWidget* = 0);
    ~Board();
    static const int initPiecePos[8][8];
    unsigned int spacing;

    void PaintGrids();
    void ReplaceElements();
    bool is_check(bool w);
    int check_game(bool show = true);
    void undoMove(bool show = true);
    void computerMove();
    int getCurrentScore();
    int minimax(int depth, int alpha = MIN_SCORE, int beta = MAX_SCORE);
    PieceMove getBestMove(int depth = 1);
    PieceMove getFirstMove();
    vector<PieceMove> getAllMoves();
    Board *ResetHighligtedGrids();

    int grid_size;

    bool move;
    bool reverse() const; // getter
    bool reverse(bool reverse);     // setter
    bool pieces_selectable;
    Grid* grids[8][8];
    QPixmap* chess_tiles;
    vector<PieceMove *> *moves;
    vector<Piece *> *pieces;
    vector<Piece *> *pieces_w, *pieces_b;
    Piece *selected, *King[2];
    FreePieces *free_pieces[2];
    MainWindow *window;

    Options *options;
public slots:
    void newGame();
    void doAutoMove();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};


class PieceMove
{
public:
    PieceMove(Piece *_piece = 0, Grid *_from = 0, Grid *_to = 0, Piece *rem = 0, bool _extra = false);
    Piece *piece;
    Grid *from, *to;
    Piece *removed;
    bool isNull() const;
    bool extra;
};
