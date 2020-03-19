#include "board.h"
#include "piece.h"
//#include <time.h>
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

Board::Board(const Board &origBoard)
{
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
            if ( !initPiecePos[i][j] ) {
                   continue;
            }
            p_piece = new Piece(static_cast<Piece::Type>(qAbs(initPiecePos[i][j])), initPiecePos[i][j] > 0, grids[j][i], this);
            grids[j][i]->lpiece = p_piece;
            p_piece->index = pieceIndex++;
            pieces->push_back(p_piece);
            (p_piece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);
        }
    }
    King[1] = pieces->at(4);
    King[0] = pieces->at(28);

    for(PieceMove *pieceMove : *origBoard.moves)
    {
        Piece *p = pieces->at( quint32(pieceMove->lpiece->index) );
        Grid *gridTo = grids[pieceMove->to->x][pieceMove->to->y];
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
        return !this->move ? MIN_SCORE : MAX_SCORE;
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

int Board::getPiecePosEval(Piece *piece)
{

#include "poseval.h"

        bool w = piece->isWhite;
        int p = posEval[piece->type] [ (w ? piece->lgrid->y : 7 - (piece->lgrid->y)) * 8 + piece->lgrid->x ];
        return w ? p : -p;

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

 /* PieceMove::isNull returns lpiece=1, grid_from=2, grid_to=3
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

    //options->flipBoard = true;
    // Board texture
    this->addItem(boardTex);


    for(int i=0; i<8; i++)
    {
        // Coords
        coords[i]       = new QGraphicsTextItem(QString::number(i+1));
        coords[8+i]     = new QGraphicsTextItem(QString::number(i+1));
        coords[16+i]    = new QGraphicsTextItem(QString(char(65+i)));
        coords[24+i]    = new QGraphicsTextItem(QString(char(65+i)));
        for(int j=0; j<8; j++)
        {
            this->grids[i][j] = new Grid(lboard->grids[i][j], this);
            this->addItem(grids[i][j]);
        }
    }

    free_pieces[0] = new FreePieces(this);
    free_pieces[1] = new FreePieces(this); // white

    Piece *p_piece;
    for(quint32 i=0; i<lboard->pieces->size(); i++) {
        p_piece = new Piece(lboard->pieces->at(i), this);
        p_piece->grid = Grid::get(p_piece->lpiece->lgrid, this);
        pieces->push_back(p_piece);
        (p_piece->lpiece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);

        this->addItem(pieces->at(i));

        coords[i]->setDefaultTextColor(Qt::black);
        coords[i]->setOpacity(0.6);
        this->addItem(coords[i]);
    }

    King[1] = pieces->at(4);
    King[0] = pieces->at(28);

     // timers
     timer[0] = new GameTimer(this);
     timer[1] = new GameTimer(this);

     timer[0]->setAlignment(Qt::AlignRight);
     timer[1]->setAlignment(Qt::AlignRight);

     pw_timer[0] = this->addWidget(timer[0]);
     pw_timer[1] = this->addWidget(timer[1]);

     this->options->player[0] = Options::AI_Stupid;
     this->options->player[1] = Options::AI_Hard;
     this->newGame();
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
    delete timer[0];
    delete timer[1];
}

// TODO: check this one
void Board::newGame()
{
    while (aiThread->isRunning()) {
        aiThread->requestInterruption();
    }

    Piece *p;
    if (lboard->currentMove && (p = Piece::get(lboard->currentMove->lpiece, this)) &&
            p->anim->state() == QAbstractAnimation::Running)
    {
        p->anim->stop();
    }


    this->timer[0]->reset();
    this->timer[1]->reset()->start();

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
            if (p_piece->lpiece->type == L::Piece::Queen &&
                    qAbs( L::Board::initPiecePos[x][y] ) == L::Piece::Pawn )
            {
                p_piece->lpiece->type = L::Piece::Pawn;
            }
            f_count ++;
        }
    }
    resetHighligtedGrids();

    if (options->player[0] == Options::Human){
        this->reverse(!reverse());
    }

    if (options->player[1] != Options::Human) {
        this->computerMove();
    }
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
    if (options->player[lboard->move] != Options::Human) {
        computerMove();
   }
}


void Board::undoMove()
{
    if (!lboard->moves->size()) {
        return;
    }

    if (aiThread->isRunning()) {
        aiThread->requestInterruption();
    }


    if (m_endGame) {
        piecesSelectable = true;
        m_endGame = false;
        this->timer[lboard->move]->start();
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

        this->timer[lboard->move]->stop();
        this->timer[!lboard->move]->start();

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

        this->timer[lboard->move]->stop()->setTime(!timerValue[lboard->move].empty() ? timerValue[lboard->move].top() : 0);
        timerValue[lboard->move].pop();
        this->timer[!lboard->move]->start();
    }

    lboard->undoMove();
    resetHighligtedGrids();

    // If playing against computer
    if ( options->player[lboard->move] != Options::Human ) {
        // To avoid endless recursion
        if (options->player[!lboard->move] != Options::Human && lboard->moves->size()) {
            this->undoMove();
        } else {
            this->computerMove();
        }
    }
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
Board *Board::resetHighligtedGrids()
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
    replaceElements();
    free_pieces[0]->update();
    free_pieces[1]->update();
    return m_reverse;
}

void Board::endGame()
{
    piecesSelectable = false;
    m_endGame = true;
    this->timer[lboard->move]->stop();
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


void Board::replaceElements()
{
    int iter = 0;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            grids[i][j]->setPos(grid_size * i, grid_size * ((m_reverse ? (j+1) : 8-j)) );
        }
    }
    for(unsigned i=0; i<pieces->size(); i++)
    {
        Piece* f = pieces->at(i);
        if ( f->lpiece->inGame ) f->placeTo(f->grid);

        QFont font("Arial");
        QFontMetrics fm(font);
        font.setPixelSize(grid_size / 6);

        int offsetY = (grid_size/2) - coords[iter]->boundingRect().height()/2;
        int offsetX = (grid_size/2) - coords[iter]->boundingRect().width()/2;

        if (iter > 23) {
            // bottom
            coords[iter]->setPos((iter%8) * grid_size + offsetX, 9*grid_size);
        } else if (iter > 15) {
            // top
             coords[iter]->setPos((iter%8) * grid_size + offsetX, grid_size - fm.height()*0.7);
        } else if (iter > 7) {
            // left
            coords[iter]->setPos(-coords[iter]->boundingRect().width(), (m_reverse ? (iter%8)+1 : 8-(iter%8)) * grid_size + offsetY);
        } else {
            // right
            coords[iter]->setPos(grid_size*8, (m_reverse ? iter+1 : 8-iter)*grid_size + offsetY);
        }

        coords[iter]->setFont(font);

        iter ++;

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


    // timers
    qreal xpos = this->sceneRect().width() - pw_timer[0]->rect().width();
    pw_timer[1]->setPos(xpos, this->reverse() ? 0 : this->sceneRect().height() - pw_timer[1]->rect().height());
    pw_timer[0]->setPos(xpos, this->reverse() ? this->sceneRect().height() - pw_timer[0]->rect().height() : 0);
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

    totalIterations = 0;

    QElapsedTimer timer;
    timer.start();
    L::Board *lboard = new L::Board(*board->lboard);//board->lboard;

    Options::PlayerType playerType = board->options->player[board->lboard->move];
    int level = playerType == 1 ? 2 : playerType == 2 ? 3 : 4;


    L::PieceMove b_bestMove = getBestMove(lboard, level);
    if (b_bestMove.isNull()) return;
    bestMove = L::PieceMove(board->lboard->pieces->at( b_bestMove.lpiece->index ),
                            board->lboard->grid(b_bestMove.from->x, b_bestMove.from->y),
                            board->lboard->grid(b_bestMove.to->x, b_bestMove.to->y),
                             b_bestMove.removed);

    qreal elapsed = qreal(timer.elapsed()) / 1000;
    qDebug() << totalIterations << "iter. / " << elapsed << " s = " << (qreal(totalIterations)/(elapsed)) << " i/s";
    delete lboard;

    //QThread::sleep(qrand()%5);
}



int AIThread::minimax(L::Board *lboard, int depth, int alpha, int beta)
{
    int bestMove = lboard->move ? MIN_SCORE : MAX_SCORE;

    if (isInterruptionRequested()) {
        return bestMove;
    }

    if (depth == 0 || lboard->check_game()) {
        return lboard->getCurrentScore();
    }

    vector<L::PieceMove> moves = lboard->getAllMoves();
    for(L::PieceMove pieceMove : moves) {
        totalIterations++; // for iter per sec counting.

        pieceMove.lpiece->makeMove(pieceMove.to);
        int eval = this->minimax(lboard, depth -1, alpha, beta);
        lboard->undoMove();

        if (lboard->move) { // is maximizing player
            bestMove = qMax (bestMove, eval);
            alpha = qMax(alpha, eval);
        }
        else {
            beta = qMin(beta, eval);
            bestMove = qMin (bestMove, eval);
        }
        if (beta <= alpha) {
            break;
        }
    }
    return bestMove;
}


L::PieceMove AIThread::getBestMove(L::Board *lboard, int depth)
{
    L::PieceMove bestMove;
    if (this->isInterruptionRequested())
    {
        qDebug() << "INT";
        return bestMove;
    }

    vector<L::PieceMove> eqMoves, moves = lboard->getAllMoves();
    totalIterations = 0;
    if (depth > 0) {

        if (!moves.size()) {
            qDebug() << "no moves :(";
            return bestMove;
        }

        int score = lboard->move ? MIN_SCORE : MAX_SCORE;


        for(L::PieceMove pieceMove : moves)
        {
            totalIterations++;
            pieceMove.lpiece->makeMove(pieceMove.to);
            int m = minimax(lboard, depth - 1);
            lboard->undoMove();

            if (isInterruptionRequested()) {
                return bestMove;
            }

            if (lboard->move){
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
        qDebug() << "(random from" << eqMoves.size() << "best moves)";
    }

    else if (bestMove.isNull() && moves.size()) {
        unsigned int r = (static_cast<unsigned int>(qrand()) % moves.size());
        bestMove = moves[r];

        qDebug() << "(random from ALL moves)";
    }

    return bestMove;
}

