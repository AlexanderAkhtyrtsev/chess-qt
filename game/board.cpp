#include "board.h"
#include "piece.h"
#include <time.h>
#include "freepieces.h"

int iter=0;


//*
const int L::Board::initPiecePos[8][8] = {
    { 5, 4, 3, 2, 1, 3, 4, 5 },
    { 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    {-6,-6,-6,-6,-6,-6,-6,-6 },
    {-5,-4,-3,-2,-1,-3,-4,-5 }
};


namespace L {

Board::Board(){
    move                = true; // true - white
    pieces              = new vector<Piece     *>;
    pieces_w            = new vector<Piece     *>;
    pieces_b            = new vector<Piece     *>;
    moves               = new vector<PieceMove *>;
    currentMove         = nullptr;

    Piece *p_piece;
    int pieceIndex = 0;
    for(int i=0; i<8; i++)
    for(int j=0; j<8; j++)
         {
             grids[j][i] = new L::Grid(j, i, this);
             if ( !initPiecePos[i][j] ) continue;
             p_piece = new Piece(static_cast<Piece::Type>(qAbs(initPiecePos[i][j])), initPiecePos[i][j] > 0, grids[j][i], this);
             p_piece->index = pieceIndex++;
             pieces->push_back(p_piece);
             (p_piece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);
         }

    King[1] = pieces->at(4);
    King[0] = pieces->at(28);
}

int Board::check_game()
{
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
        }
    }
    return game_over;
}

void Board::undoMove()
{
    if (!moves->size()) return;
    PieceMove *last = moves->at(moves->size() - 1);


    if (last->extra)
    {
        // Castling
        if (last->lpiece->type == Piece::King)
        {
            if ( last->lpiece->lgrid->x == 2)
            {
                Piece *Rook = last->lpiece->lgrid->offset(1,0)->lpiece;
                Rook->placeTo(Rook->lgrid->offset(-3,0));
            }
            else if ( last->lpiece->lgrid->x == 6)
            {
                Piece *Rook = last->lpiece->lgrid->offset(-1,0)->lpiece;
                Rook->placeTo(Rook->lgrid->offset(2,0));
            }
        }
        else if (last->lpiece->type == Piece::Queen)
        {
            last->lpiece->type = Piece::Pawn;
        }
    }


    last->lpiece->placeTo(last->from);
    //last->to->lpiece = nullptr;

    if ( last->removed )
    {
/*// bool shit
        if (last->extra && last->lpiece->type == Piece::Pawn && last->to->y != ( last->lpiece->isWhite ? 7 : 0 )) {
            offsetY = last->lpiece->isWhite ? -1 : 1;
        }

        */
        // Adding piece to game
        last->removed->inGame = true;
        //last->removed->placeTo(last->to->offset(0, offsetY));
        last->removed->placeTo(last->to);
    }
    last->lpiece->moves->erase(last->lpiece->moves->end()-1);
    moves->erase( moves->end() - 1 );
    move = !move;
}

bool Board::is_check(bool w)
{
    return King[w]->lgrid->is_attacked(!w);
}


int Board::getCurrentScore()
{
    //int Points = {None, King, Queen, Bishop, Knight, Rook, Pawn};
    int points[] = {0,    0,    80,    30,      35,      40,   5};
    int total = 0;

    if (this->check_game() == 1) {
        return this->move ? MIN_SCORE : MAX_SCORE;
    }

    for( L::Piece *piece : *pieces ){
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


    if (depth == 0 || this->check_game() == 1) {
        return this->getCurrentScore();
    }

    vector<PieceMove> moves = this->getAllMoves();
    int bestMove = this->move ? MIN_SCORE : MAX_SCORE;
    for(PieceMove pieceMove : moves) {
iter++;
        pieceMove.lpiece->makeMove(pieceMove.to);
        int eval = this->minimax(depth -1, alpha, beta);
        this->undoMove();

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

            pieceMove.lpiece->makeMove(pieceMove.to);
            int m = minimax(depth - 1);
            this->undoMove();

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
        unsigned int r = (static_cast<unsigned int>(qrand()) % eqMoves.size());
        bestMove = eqMoves.size() > 1 ? eqMoves[r] : eqMoves[0];
        qDebug() << "(one of" << eqMoves.size() << "best moves)";
    }

    else if (bestMove.isNull() && moves.size()){
        unsigned int r = (static_cast<unsigned int>(qrand()) % moves.size());
        bestMove = moves[r];
        qDebug() << "(random move)";
    }

    return bestMove;
}


vector<PieceMove> Board::getAllMoves()
{
    vector<PieceMove> result;
    vector<L::Piece *> availPieces, *currentPieces = (this->move ? pieces_w : pieces_b);

    for (L::Piece *piece : *currentPieces) {
        if(piece->inGame && piece->getGrids().size() > 0 ) {
                availPieces.push_back(piece);
        }
    }

    for(Piece *piece : availPieces) {
        vector<Grid*> moves = piece->getGrids();
        for(Grid *g : moves) {
            result.push_back(PieceMove(piece, piece->lgrid, g));
        }
    }

    return result;
}


PieceMove::PieceMove(L::Piece *_piece, L::Grid *_from, L::Grid *_to, L::Piece *rem, bool _extra)
: lpiece(_piece), from(_from), to(_to), removed(rem), extra(_extra)
{
}

bool PieceMove::isNull() const
{
    return (!this->lpiece || !this->from || !this->to);
}

} // NAMESPACE L

Board::Board(QWidget* parent) :
    QGraphicsScene(parent)
{
    grid_size = parent->width() / int(BOARD_SIZE);
    setSceneRect(0, 0, BOARD_SIZE*quint32(grid_size), BOARD_SIZE*quint32(grid_size));
    chess_tiles = new QPixmap(":/img/chess.png");
    boardTex = new QGraphicsPixmapItem(QPixmap(":/img/board.png"));
    lboard              = new L::Board;
    selected            = nullptr;
    m_reverse           = false;
    piecesSelectable    = true;
    pieces              = new vector<Piece *>;
    pieces_w            = new vector<Piece *>;
    pieces_b            = new vector<Piece *>;
    options = new Options;

    aiThread            = new AIThread(this);
    QObject::connect(aiThread, SIGNAL(finished()), this, SLOT(computerMoveEnd()));

    this->addItem(boardTex);

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            this->grids[i][j] = new Grid(lboard->grids[i][j], this);
            this->addItem(grids[i][j]);
        }
    }

    free_pieces[0] = new FreePieces(this);
    free_pieces[1] = new FreePieces(this); // white

    Piece *p_piece;
     for(quint32 i=0; i<lboard->pieces->size(); i++){
             p_piece = new Piece(lboard->pieces->at(i), this);
             p_piece->grid = Grid::get(p_piece->lpiece->lgrid, this);
             pieces->push_back(p_piece);
             (p_piece->lpiece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);
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
    selected = nullptr;
    lboard->move  = true; // true = white
    piecesSelectable = true;
    lboard->moves->clear();
    reverse(false);
    // ?????
    free_pieces[0] = new FreePieces(this);
    free_pieces[1] = new FreePieces(this);

    Piece *p_piece;
    unsigned int f_count = 0;
    for(int x = 0; x < 8; x++)
    {
        for(int y=0; y<8; y++)
        {
            if ( !L::Board::initPiecePos[x][y] ) continue;
            p_piece = pieces->at(f_count);
            p_piece->lpiece->inGame = true;
            p_piece->placeTo(grids[y][x]);
            p_piece->lpiece->clearMoves();
            if (p_piece->lpiece->type == L::Piece::Queen && qAbs( L::Board::initPiecePos[x][y] ) == L::Piece::Pawn ){
                p_piece->lpiece->type = L::Piece::Pawn;
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


void Board::undoMove()
{
    if (!lboard->moves->size() || aiThread->isRunning()) return;
    L::PieceMove *last = lboard->moves->at(lboard->moves->size()-1);
    Piece *removed = Piece::get(last->removed, this);
    Piece *piece = Piece::get(last->lpiece, this);

    if (removed){
        free_pieces[removed->lpiece->isWhite]->removePiece(removed);
        free_pieces[removed->lpiece->isWhite]->update();
        removed->grid = Grid::get(last->to, this);
    }

    if (aiThread->isRunning()) {
        aiThread->terminate();

    }

    lboard->undoMove();

    // Stopping animation
    if (piece->anim->state() == QAbstractAnimation::Running){
        piece->anim->stop();
        piecesSelectable = true;
        if (last->extra && piece->lpiece->type == L::Piece::King){
            if (piece->grid->lgrid->offset(2, 0) == last->to)
            {
                /*Piece *Rook = */ piece->grid->offset(3, 0)->piece->anim->stop();
            }
            else if (piece->grid->lgrid->offset(-2, 0) == last->to)
            {
                /*Piece *Rook = */ piece->grid->offset(-4, 0)->piece->anim->stop();
            }
        }
    }
    piece->grid = Grid::get(last->from, this);
    ResetHighligtedGrids();
    ReplaceElements();
}

void Board::computerMove()
{
    if (aiThread->isRunning()){
        return;
    }
    aiThread->start();
    piecesSelectable = false;
}

void Board::computerMoveEnd()
{
    if (!aiThread->bestMove.isNull()) {
        Piece::get(aiThread->bestMove.lpiece, this)->makeMove(Grid::get(aiThread->bestMove.to, this));
    }
    piecesSelectable = true;
}

// TODO: optimize
Board *Board::ResetHighligtedGrids()
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            grids[i][j]->Highlight(false);
        }
    }
    if ( lboard->is_check(0) ) King[0]->grid->Highlight();
    else if ( lboard->is_check(1) ) King[1]->grid->Highlight();

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


void Board::ReplaceElements()
{
    for(int i=0; i<8; i++)for(int j=0; j<8; j++)grids[i][j]->setPos(grid_size * (i+1), grid_size * ((m_reverse ? (j+1) : 8-j)) );
    for(unsigned i=0; i<pieces->size(); i++)
    {
        Piece* f = pieces->at(i);
        if ( f->lpiece->inGame ) f->placeTo(f->grid);
    }

    free_pieces[0]->update();
    free_pieces[1]->update();

    double multiplier = double(grid_size*int(BOARD_SIZE))/double(boardTex->pixmap().width());
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

AIThread::AIThread(Board *_board)
{
    board = _board;
}

void AIThread::run()
{
    iter = 0;
    time_t t1, t2;
    float tdiff;
    QApplication::processEvents();
    t1 = QDateTime::currentMSecsSinceEpoch();
    bestMove = board->lboard->getBestMove(board->window->sel1val());
    t2 = QDateTime::currentMSecsSinceEpoch();
    tdiff = t2-t1;
    //---///---//

    //--/f/--/u/--/c/
    qDebug() << "time:" << (tdiff/1000.0f) << "s.";
    qDebug() << iter << "iterations.";
    qDebug() << "iterations/sec:" << (iter/(tdiff/1000.0f));
}
