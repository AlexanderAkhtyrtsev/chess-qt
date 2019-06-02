#include "board.h"
#include "piece.h"
#include <time.h>
#include "freepieces.h"


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
//*/



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
    {
        for(int j=0; j<8; j++)
        {
            grids[j][i] = new L::Grid(j, i, this);
            if ( !initPiecePos[i][j] ) continue;
            p_piece = new Piece(static_cast<Piece::Type>(qAbs(initPiecePos[i][j])), initPiecePos[i][j] > 0, grids[j][i], this);
            p_piece->index = pieceIndex++;
            pieces->push_back(p_piece);
            (p_piece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);
        }
    }
    King[1] = pieces->at(4);
    King[0] = pieces->at(28);

}

Board::Board(const Board &b)
{
    move                = true; // true - white
    pieces              = new vector<Piece     *>;
    pieces_w            = new vector<Piece     *>;
    pieces_b            = new vector<Piece     *>;
    moves               = new vector<PieceMove *>;
    currentMove         = nullptr;
       totalIterations = 0;

    Piece *p_piece;
    int pieceIndex = 0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            grids[j][i] = new L::Grid(j, i, this);
            if ( !initPiecePos[i][j] ) continue;
            p_piece = new Piece(static_cast<Piece::Type>(qAbs(initPiecePos[i][j])), initPiecePos[i][j] > 0, grids[j][i], this);
            p_piece->index = pieceIndex++;
            pieces->push_back(p_piece);
            (p_piece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);
        }
    }
    King[1] = pieces->at(4);
    King[0] = pieces->at(28);

    for(PieceMove *pm : *b.moves)
    {
        Piece *p = this->pieces->at( quint32(pm->lpiece->index) );
        Grid *gridTo = this->grids[pm->to->x][pm->to->y];
        p->makeMove(gridTo);
    }

}


Board::~Board()
{
    for(Piece *p: *this->pieces)
    {
        delete p;
    }
    for(quint32 i=0; i<8; i++){
        for(quint32 j=0; j<8; j++){
            delete this->grids[i][j];
        }
    }
    for(PieceMove *pm: *this->moves){
        delete pm;
    }
    delete pieces_b;
    delete pieces_w;
    delete pieces;
}


Grid *Board::grid(int x, int y) const
{
    if (x < 0 || x > 7 || y < 0 || y > 7) return nullptr;
    return this->grids[x][y];
}

Piece *Board::piece(int index) const
{
    if (quint32(index) >= this->pieces->size() ) return nullptr;
    return this->pieces->at(quint32(index));
}


/**
 * returns 1 if check mate, 2 if draw, 0 if in game
 */

int Board::check_game()
{
    int game_over = 2; // 0 - the game isn't over; 1 - check mate; 2 - draw;
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
    PieceMove *last = moves->back();


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
    if ( last->removed )
    {
        last->removed->inGame = true;
        if (last->extra && last->removed->type == L::Piece::Pawn){
            last->removed->placeTo(last->to->offset(0, last->removed->isWhite ? 1 : -1));
        }
        else {
            last->removed->placeTo(last->to);
        }
    }

    last->lpiece->moves--;
    moves->erase( moves->end() - 1 );
    this->posChanged();
    delete last;
    move = !move;
}



/**
 *  Returns true if check
 */
bool Board::is_check(bool w)
{
    return King[w]->lgrid->is_attacked(!w);
}



int Board::getCurrentScore()
{
    //int Points =              {None, King, Queen, Bishop, Knight, Rook, Pawn};
    const static int points[] = {0,    10000,    1000,    500,     510,  800,   100};

    int gameState = this->check_game();
    if (gameState == 1) {
        return this->move ? MIN_SCORE : MAX_SCORE;
    } else if (gameState == 2){
        return 0;
    }

    int total = 0;
    for( L::Piece *piece : *pieces ){
        if (piece->inGame)
        {
            total += piece->isWhite ? points[piece->type] : -points[piece->type];
            total += getPiecePosEval(piece);
        }
    }
    return total ;
}

void Board::posChanged()
{
    for(Piece *p: *this->pieces)
    {
        p->resetAvailMoves();
    }
}

int Board::minimax(int depth, int alpha, int beta)
{


    if (depth == 0 || this->check_game()) {
        return this->getCurrentScore();
    }

    vector<PieceMove> moves = this->getAllMoves();
    int bestMove = this->move ? MIN_SCORE : MAX_SCORE;
    for(PieceMove pieceMove : moves) {
        this->totalIterations++; // for iter per sec counting.

        pieceMove.lpiece->makeMove(pieceMove.to);
        int eval = this->minimax(depth -1, alpha, beta);
        this->undoMove();

        if (this->move) { // is maximizing player
            bestMove = qMax (bestMove, eval);
            alpha = qMax(alpha, eval);
        }
        else {
            beta = qMin(beta, eval);
            bestMove = qMin (bestMove, eval);
        }
        if (beta <= alpha) {
          //  break;
        }
    }
    return bestMove;
}

int Board::getPiecePosEval(Piece *piece)
{
    const static int pawnPosEval[8][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 0},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        { 2,-1,-1, 2, 2, 0, 2, 1},
        { 2, 2, 2, 3, 3, 3, 3, 4},
        { 2, 2, 2, 3, 3, 4, 4, 4},
        { 3, 2, 2, 4, 4, 4, 4, 4},
        { 0, 0, 0, 0, 0, 0, 0, 0}
    };



    if (piece->type == L::Piece::Pawn)
    {
        bool w = piece->isWhite;
        int p = pawnPosEval[piece->lgrid->x][w ? piece->lgrid->y : 7 - (piece->lgrid->y)];
        return w ? p : -p;
    }
    return 0;
}

PieceMove Board::getBestMove(int depth)
{

    PieceMove bestMove;
    vector<PieceMove> eqMoves, moves = this->getAllMoves();
    this->totalIterations = 0;
    if (depth > 0) {

        if (!moves.size()) {
            qDebug() << "no moves :(";
            return bestMove;
        }

        int score = this->move ? MIN_SCORE : MAX_SCORE;


        for(PieceMove pieceMove : moves)
        {
            this->totalIterations++;
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

    if (eqMoves.size()) {
        unsigned int r = (static_cast<unsigned int>(qrand()) % eqMoves.size());
        bestMove = eqMoves.size() > 1 ? eqMoves[r] : eqMoves[0];
        qDebug() << "(random from best moves)";
    }

    else if (bestMove.isNull() && moves.size()) {
        unsigned int r = (static_cast<unsigned int>(qrand()) % moves.size());
        bestMove = moves[r];

                qDebug() << "(random from ALL moves)";
    }

    return bestMove;
}


vector<PieceMove> Board::getAllMoves()
{
    vector<PieceMove> result;
    vector<L::Piece *> availPieces, *currentPieces = (this->move ? pieces_w : pieces_b);

    for (L::Piece *piece : *currentPieces) {
        if(piece->inGame) {
            availPieces.push_back(piece);
            vector<Grid*> moves = piece->getGrids();
            for(Grid *g : moves) {
                result.push_back(PieceMove(piece, piece->lgrid, g));
            }
        }
    }

    return result;
}


PieceMove::PieceMove(L::Piece *_piece, L::Grid *_from, L::Grid *_to, L::Piece *rem, bool _extra)
: lpiece(_piece), from(_from), to(_to), removed(rem), extra(_extra)
{
}

/**
 * PieceMove::isNull returns lpiece=1, grid_from=2, grid_to=3
 */

int PieceMove::isNull() const
{
    return (!this->lpiece ? 1 : !this->from ? 2 : !this->to ? 3 : 0);
}

} // NAMESPACE L

Board::Board(QWidget* parent) : QGraphicsScene(parent)
{
    m_endGame           = false;
    size                = QSize(8, 10);
    grid_size = parent->width() / size.width();
    setSceneRect(0, 0, size.width()*grid_size, size.height()*grid_size);

    chess_tiles = new QPixmap(":/img/chess.png");
    boardTex = new QGraphicsPixmapItem(QPixmap(":/img/board.png"));

    lboard              = new L::Board;
    selected            = nullptr;
    m_reverse           = false;
    piecesSelectable    = true;
    pieces              = new vector<Piece *>;
    pieces_w            = new vector<Piece *>;
    pieces_b            = new vector<Piece *>;
    options             = new Options;
    timerValue[0]       = stack<quint32>();
    timerValue[1]       = stack<quint32>();
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

     for(unsigned i=0; i<pieces->size(); i++)
     {
         this->addItem(pieces->at(i));
     }
     //window->timer[1]->start();
     /*
     for(auto p : *pieces)
     {
         if (p->lpiece->type > 2){
             p->lpiece->remove();
             p->remove();
         }
     }
     //*/
}

Board::~Board()
{
    for(Piece *p: *this->pieces)
    {
        delete p;
    }

    delete free_pieces[0];
    delete free_pieces[1];

    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++){
            delete this->grids[i][j];
        }
    }
    delete aiThread;
    delete options;
    delete pieces  ;
    delete pieces_w;
    delete pieces_b;
    delete lboard;
    delete boardTex;
    delete chess_tiles;
}

// TODO: check this one
void Board::newGame()
{
    Piece *p;
    if (lboard->currentMove && (p = Piece::get(lboard->currentMove->lpiece, this)) && p->anim->state() == QAbstractAnimation::Running)
    p->anim->stop();
    if(aiThread->isRunning()) {
        aiThread->terminate();
    }
    window->timer[0]->reset();
    window->timer[1]->reset()->start();

    selected = nullptr;
    lboard->move  = true; // true = white
    piecesSelectable = true;
    lboard->moves->clear();

    timerValue[0] = stack<quint32>();
    timerValue[0] = stack<quint32>();
    reverse(false);


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
    lboard->posChanged();
    ResetHighligtedGrids();
}

void Board::doAutoMove()
{
    this->computerMove();
}

void Board::pieceMoveCompleted()
{
    if (lboard->check_game()){
        this->endGame();
        return;
    }
    if (!lboard->move) {
        QTimer::singleShot(500, this, &Board::computerMove);
    }
}


void Board::undoMove()
{
    if (!lboard->moves->size()) return;

    if (aiThread->isRunning())
    {
        aiThread->terminate();
        return;
    }


    if (m_endGame)
    {
        piecesSelectable = true;
        m_endGame = false;
        this->window->timer[lboard->move]->start();
    }

    L::PieceMove *last = lboard->moves->back();
    Piece *piece = Piece::get(last->lpiece, this);

    // Stopping animation
    if (piece->anim->state() == QAbstractAnimation::Running)
    {
        piecesSelectable = true;
        piece->anim->stop();
        piece->placeTo(Grid::get(last->from, this));

        // Castling
        if (last->extra && piece->lpiece->type == L::Piece::King)
        {
            Piece *rook = nullptr;
            if (piece->grid->lgrid->offset(2, 0) == last->to)
            {
                rook = piece->grid->offset(1, 0)->piece;
                rook->anim->stop();
                rook->placeTo(rook->grid->offset(2, 0));
            }

            else //if (piece->grid->lgrid->offset(-2, 0) == last->to)
            {
                rook = piece->grid->offset(-1, 0)->piece;
                rook->anim->stop();
                rook->placeTo(rook->grid->offset(-3, 0));
            }
        }

        window->timer[lboard->move]->stop();
        window->timer[!lboard->move]->start();

    } else {

        Piece *removed = Piece::get(last->removed, this);
        if (removed){
            free_pieces[removed->lpiece->isWhite]->removePiece(removed);
            free_pieces[removed->lpiece->isWhite]->update();
            Grid *prevPos_grid = Grid::get(last->extra && last->removed->type == L::Piece::Pawn? last->to->offset(0, last->removed->isWhite ? 1 : -1) : last->to, this);
            removed->placeTo(prevPos_grid);
        }

        // Castling
        if (last->extra && piece->lpiece->type == L::Piece::King)
        {
            Piece *rook;
            // Short
            if (piece->grid->lgrid->offset(-2, 0) == last->from)
            {
                rook = piece->grid->offset(-1, 0)->piece;
                rook->placeTo(rook->grid->offset(2,0));
            }
            // Long
            else //if (piece->grid->lgrid->offset(-2, 0) == last->to)
            {
                rook = piece->grid->offset(1, 0)->piece;
                rook->placeTo(rook->grid->offset(-3,0));
            }
        }
        piece->placeTo(Grid::get(last->from, this));

        window->timer[lboard->move]->stop()->setTime(!timerValue[lboard->move].empty() ? timerValue[lboard->move].top() : 0);
        timerValue[lboard->move].pop();
        window->timer[!lboard->move]->start();
    }

    lboard->undoMove();
    ResetHighligtedGrids();
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
        aiThread->bestMove = nullptr;
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

void Board::endGame()
{
    piecesSelectable = false;
    m_endGame = true;
    this->window->timer[lboard->move]->stop();
    for(int i=-1; i<2; i++)
    {
        for(int j=-1; j<2; j++)
        {
            Grid *g = King[lboard->move]->grid->offset(i,j);

            if (g) // grid exists
            {
                vector<L::Piece *> attackingPieces = g->lgrid->attackedBy(!lboard->move);
                if (!(i || j)) {
                    for(L::Piece *attackingPiece: attackingPieces)
                       Grid::get(attackingPiece->lgrid, this)->Highlight();
                }

                else  if (g->lgrid->empty() // grid empty
                        || (g->lgrid->lpiece->isWhite != lboard->move)) // or grid not empty & piece is enemie's
                {
                    if (attackingPieces.size()){
                        Grid::get(attackingPieces.at(0)->lgrid, this)->Highlight();
                    }
                }
            }
        }
    }
}


void Board::ReplaceElements()
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            grids[i][j]->setPos(grid_size * i, grid_size * ((m_reverse ? (j+1) : 8-j)) );
        }
    }
    for(unsigned i=0; i<pieces->size(); i++)
    {
        Piece* f = pieces->at(i);
        if ( f->lpiece->inGame ) f->placeTo(f->grid);
    }

    free_pieces[0]->update();
    free_pieces[1]->update();

    double multiplier = double(grid_size*size.height()) / double(boardTex->pixmap().width());
    boardTex->setScale(multiplier);
    boardTex->setPos(-1 * grid_size, boardTex->pos().y());
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
    qsrand(static_cast<quint32>(time(nullptr)));
    QElapsedTimer timer;
    timer.start();
    L::Board *lboard = board->lboard;
                    //new L::Board(*board->lboard);
    bestMove = lboard->getBestMove(board->window->sel1val());
    qreal elapsed = qreal(timer.elapsed()) / 1000;


    qDebug() << lboard->totalIterations << "iter. /" << elapsed << "s =" << (qreal(lboard->totalIterations)/(elapsed)) << "i/s";

}
