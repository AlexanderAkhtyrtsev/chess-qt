#include "board.h"
#include "piece.h"
#include <time.h>
#include "freepieces.h"



//*
const int Board::initPiecePos[8][8] = {
    { 5, 4, 3, 2, 1, 3, 4, 5 },
    { 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    {-6,-6,-6,-6,-6,-6,-6,-6 },
    {-5,-4,-3,-2,-1,-3,-4,-5 }
};

/*/

const int Board::initPiecePos[8][8] = {
    { 5, 4, 3, 0, 1, 0, 4, 5 },
    { 6, 6, 6, 6, 0, 6, 6, 6 },
    { 0, 0, 0, 0, 0, 2, 0, 0 },
    {-6, 0, 3, 0, 6, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0,-6, 0, 0, 0, 0, 0 },
    { 0,-6, 0,-6,-6,-6,-6,-6 },
    {-5,-4,-3,-2,-1,-3,-4,-5 }
};
//*/


Board::Board(QWidget* parent) :
    QGraphicsScene(parent)
{
    grid_size = parent->width() / BOARD_SIZE;
    setSceneRect(0, 0, BOARD_SIZE*grid_size, BOARD_SIZE*grid_size);
    chess_tiles = new QPixmap(":/img/chess.png");
    boardTex = new QGraphicsPixmapItem(QPixmap(":/img/board.png"));

    selected            = 0;
    move                = true; // true - white
    m_reverse           = false;
    pieces_selectable   = true;
    pieces              = new vector<Piece     *>;
    pieces_w            = new vector<Piece     *>;
    pieces_b            = new vector<Piece     *>;
    moves               = new vector<PieceMove *>;
    options = new Options;

    this->addItem(boardTex);

    PaintGrids();
    free_pieces[0] = new FreePieces(this);
    free_pieces[1] = new FreePieces(this); // white

    Piece *p_piece;
     for(int i=0; i<8; i++)
         for(int j=0; j<8; j++)
         {
             if ( !initPiecePos[i][j] ) continue;
             p_piece = new Piece((Piece::Type) (initPiecePos[i][j] > 0 ? initPiecePos[i][j] : -initPiecePos[i][j]), initPiecePos[i][j] > 0, grids[j][i], this);
             pieces->push_back(p_piece);
             (p_piece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);
         }

    King[1] = pieces->at(4);
    King[0] = pieces->at(28);

     for(unsigned i=0; i<pieces->size(); i++){
         addItem(pieces->at(i));
         //if( pieces->at(i)->type != Piece::King )  pieces->at(i)->remove();
     }


}

Board::~Board()
{
    delete chess_tiles;
    delete pieces;
    delete pieces_w;
    delete pieces_b;
    // delete grids;
}

// TODO: check this one
void Board::newGame()
{
    selected = 0;
    move  = true; // true = white
    pieces_selectable = true;
    moves->clear();
    reverse(!move);
    free_pieces[0] = new FreePieces(this);
    free_pieces[1] = new FreePieces(this);

    Piece *p_piece;
    int f_count = 0;
    for(int x = 0; x < 8; x++)
    {
        for(int y=0; y<8; y++)
        {
            if ( !initPiecePos[x][y] ) continue;
            p_piece = pieces->at(f_count);
            p_piece->inGame = true;
            p_piece->placeTo(grids[y][x]);
            p_piece->clearMoves();
            if (p_piece->type == Piece::Queen && qAbs( initPiecePos[x][y] ) == Piece::Pawn ){
                p_piece->type = Piece::Pawn;
            }
            f_count ++;
        }
    }
    ResetHighligtedGrids();
}

void Board::doAutoMove()
{
    this->computerMove();
}


bool Board::is_check(bool w)
{
    return King[w]->grid->is_attacked(!w);
}

int Board::check_game(bool show)
{
    if (show){
        if ( is_check(0) ) King[0]->grid->Highlight();
        else if ( is_check(1) ) King[1]->grid->Highlight();
    }

    bool game_over = 2; // 0 - the game isn't over; 1 - check mate; 2 - draw;
    vector<Piece *> *pieces = move ? pieces_w : pieces_b;
    Piece *temp;
    for(unsigned i = 0; i < pieces->size(); i++)
    {
        temp = pieces->at(i);
        if ( temp->inGame && temp->getGrids().size() > 0 )
        {
            game_over = 0;
            break;
        }
    }
    if (game_over) {
        // the game is already over.
        // loser's move (that isn't possible)
        if( this->is_check(this->move) ){
            game_over = 1;
            if (show)
            qDebug() << "Check mate!" << (this->move ? "black" : "white") << "win!";
        }
        else {
            if (show)
            qDebug() << "Draw!";
        }
    }
    return game_over;
}

void Board::undoMove(bool show)
{
    if (!moves->size()) return;
    PieceMove *last = moves->at(moves->size() - 1);


    if (last->extra)
    {
        // Castling
        if (last->piece->type == Piece::King)
        {
            if ( last->piece->grid->x == 2)
            {
                Piece *Rook = last->piece->grid->Offset(1,0)->piece;
                Rook->placeTo(Rook->grid->Offset(-3,0), show);
            }
            else if ( last->piece->grid->x == 6)
            {
                Piece *Rook = last->piece->grid->Offset(-1,0)->piece;
                Rook->placeTo(Rook->grid->Offset(2,0), show);
            }
        }
        else
        {
            last->piece->type = Piece::Pawn;
        }
    }


    last->piece->placeTo(last->from, show);
    last->to->piece = 0;

    if ( last->removed )
    {
        int offsetY = 0;

        if (last->extra && last->piece->type == Piece::Pawn && last->to->y != ( last->piece->isWhite ? 7 : 0 )) {
            offsetY = last->piece->isWhite ? -1 : 1;
        }
        // Adding piece to game
        last->removed->inGame = true;
        last->removed->placeTo(last->to->Offset(0, offsetY));
        free_pieces[last->removed->isWhite]->removePiece(last->removed);
    }
   last->piece->moves->erase(last->piece->moves->end()-1);
   moves->erase( moves->end() - 1 );
   move = !move;
   if (show){
       if (!moves->size()) reverse( !move );
       ResetHighligtedGrids();
   }
}
int iter = 0;
void Board::computerMove()
{
    //---///---//
    iter = 0;
    time_t t1, t2;
    float tdiff;
    QApplication::processEvents();
    t1 = QDateTime::currentMSecsSinceEpoch();
    PieceMove bestMove = this->getBestMove(this->window->sel1val());
    t2 = QDateTime::currentMSecsSinceEpoch();
    tdiff = t2-t1;
    //---///---//


    if (!bestMove.isNull()) {
        bestMove.piece->makeMove(bestMove.to);
    }

    //--/f/--/u/--/c/
    qDebug() << "time:" << (tdiff/1000.0f) << "s.";
    qDebug() << iter << "iterations.";
    qDebug() << "iterations/sec:" << (iter/(tdiff/1000.0f));
}

int Board::getCurrentScore()
{
    //int Points = {None, King, Queen, Bishop, Knight, Rook, Pawn};
    int points[] = {0,    0,    80,    30,      35,      40,   5};
    int total = 0;

    if (this->check_game(false) == 1) {
        return this->move ? MIN_SCORE : MAX_SCORE;
    }

    for( auto piece : *pieces ){
        if (piece->inGame) {
            total += piece->isWhite ? points[piece->type] : -points[piece->type];
          /*  too low
           * if (piece->type > Piece::King && piece->type < Piece::Pawn) {

                if (piece->getGrids(false).size()){
                    total += piece->isWhite ? 1 : -1;
                }
            }/*/
        }
    }
    return total ;//+ this->getAllMoves().size();
}

int Board::minimax(int depth, int alpha, int beta)
{

    iter++;


    if (depth == 0 || this->check_game(false) == 1) {
        return this->getCurrentScore();
    }

    vector<PieceMove> moves = this->getAllMoves();
    int bestMove = this->move ? MIN_SCORE : MAX_SCORE;
    for(PieceMove pieceMove : moves) {

        pieceMove.piece->makeMove(pieceMove.to, false);
        int eval = this->minimax(depth -1, alpha, beta);
        this->undoMove(false);

        if (this->move) { // is maximizing player

            bestMove = qMax (bestMove, eval);
            alpha = qMax(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        else {
            beta = qMin(beta, eval);
            bestMove = qMin (bestMove, eval);
            if (beta <= alpha) {
                break;
            }
        }
    }
    return bestMove;
}

PieceMove Board::getBestMove(int depth)
{

    PieceMove bestMove;
    vector<PieceMove> eqMoves, moves = this->getAllMoves();

    if (depth > 0) {

        if (!moves.size()) {
            qDebug() << "no moves :(";
            return bestMove;
        }

        int score = this->move ? MIN_SCORE : MAX_SCORE;


        for(PieceMove pieceMove : moves) {

            pieceMove.piece->makeMove(pieceMove.to, false);
            int m = minimax(depth - 1);
            this->undoMove(false);

            if (this->move){
                if (m > score) {
                    bestMove = pieceMove;
                    score = m;
                    eqMoves.clear();
                    eqMoves.push_back(pieceMove);
                } else if (m == score) {
                    eqMoves.push_back(pieceMove);
                }
            } else {
                if (m < score) {
                    bestMove = pieceMove;
                    score = m;
                    eqMoves.clear();
                    eqMoves.push_back(pieceMove);
                } else if (m == score) {
                    eqMoves.push_back(pieceMove);
                }
            }
        }
    }

    if (eqMoves.size()){
            bestMove = eqMoves.size() > 1 ? eqMoves[(qrand() % eqMoves.size())] : eqMoves[0];
            qDebug() << "(one of" << eqMoves.size() << "best moves)";
    }

    else if (bestMove.isNull() && moves.size()){
            bestMove = moves[(qrand() % moves.size())];
            qDebug() << "(random move)";
    }

    return bestMove;
}

PieceMove Board::getFirstMove()
{

}

vector<PieceMove> Board::getAllMoves()
{
    vector<PieceMove> result;
    vector<Piece *> availPieces, *currentPieces = (this->move ? pieces_w : pieces_b);

    for (auto piece : *currentPieces) {
        if(piece->inGame && piece->getGrids().size() > 0 ) {
                availPieces.push_back(piece);
        }
    }

    for(Piece *piece : availPieces) {
        vector<Grid*> moves = piece->getGrids();
        for(Grid *g : moves) {
            result.push_back(PieceMove(piece, piece->grid, g));
        }
    }

    return result;
}



Board *Board::ResetHighligtedGrids()
{
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            grids[i][j]->Highlight(false);
        }
    }
    return this;
}


bool Board::reverse() const
{
    return m_reverse;
}

bool Board::reverse(bool reverse)
{
    m_reverse = reverse;
    ReplaceElements();
    free_pieces[0]->update();
    free_pieces[1]->update();
    return m_reverse;
}

void Board::PaintGrids()
{
    bool w = 0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            grids[i][j] = new Grid(w, i, j, this);
            addItem(grids[i][j]);
            w = !w;
        }
        w = !w;
    }
}

void Board::ReplaceElements()
{
    for(unsigned i=0; i<8; i++)
        for(unsigned j=0; j<8; j++)
            grids[i][j]->setPos(grid_size * (i+1), grid_size * ((m_reverse ? (j+1) : 8-j)) );

    for(unsigned i=0; i<pieces->size(); i++)
    {
        Piece* f = pieces->at(i);
        if ( f->inGame ) f->placeTo(f->grid);
    }
    free_pieces[0]->update();
    free_pieces[1]->update();
    float multiplier = (float)(grid_size*BOARD_SIZE)/(float)(boardTex->pixmap().width());
    boardTex->setScale(multiplier);
    if(this->reverse()) {
        QTransform t = QTransform().rotate(180, Qt::YAxis).translate(-boardTex->pixmap().width()*multiplier, 0);
        boardTex->setTransform(t);
    } else {
        boardTex->resetTransform();
    }
}


void Board::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}


PieceMove::PieceMove(Piece *_piece, Grid *_from, Grid *_to, Piece *rem, bool _extra)
: piece(_piece), from(_from), to(_to), removed(rem), extra(_extra)
{

}

bool PieceMove::isNull() const
{
    return (!this->piece || !this->from || !this->to);
}
