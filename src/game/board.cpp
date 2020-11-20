#include "board.h"
#include "piece.h"
#include "freepieces.h"
#include "aithread.h"
#include "piecemove.h"

bool Board::isPiecesSelectable() const
{
    return piecesSelectable;
}

void Board::setPiecesSelectable(bool value)
{
    piecesSelectable = value;
}

Board::Board(QWidget* parent) : QGraphicsScene(parent)
{
    m_endGame           = false;
    size                = QSize(8, 10);
    grid_size = parent->width() / size.width();
    setSceneRect(0, 0, size.width()*grid_size, size.height()*grid_size);

    piecesTileset = new QPixmap(":/img/pieces.png");
    boardTexture = new QGraphicsPixmapItem(QPixmap(":/img/board.png"));

    lboard              = new LBoard;
    selected            = nullptr;
    m_reverse           = false;
    piecesSelectable    = true;
    pieces              = new QVector<Piece *>;
    pieces_w            = new QVector<Piece *>;
    pieces_b            = new QVector<Piece *>;
    options             = new Options;
    timersValue[0]       = QStack<quint32>();
    timersValue[1]       = QStack<quint32>();
    aiThread            = new AIThread(this);
    QObject::connect(aiThread, SIGNAL(finished()), this, SLOT(computerMoveEnd()));


    highlightedGrids = QStack<Grid *>();

    //options->flipBoard = true;
    // Board texture
    this->addItem(boardTexture);


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
    for(int i=0; i<lboard->pieces->size(); i++) {
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

     this->options->player[1] = Options::Human;
     this->options->player[0] = Options::AI_Medium;
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
    delete boardTexture;
    delete piecesTileset;
    delete timer[0];
    delete timer[1];
}

// TODO: check this one
void Board::newGame()
{

    Piece *p;
    if (lboard->currentMove && lboard->currentMove->lpiece &&
            (p = Piece::get(lboard->currentMove->lpiece, this)) &&
            p->isAnimationRunning())
    {
        p->stopAnimation();
    }

    while (aiThread->isRunning()) {
        aiThread->requestInterruption();
    }

    this->timer[0]->reset();
    this->timer[1]->reset()->start();

    selected = nullptr;
    lboard->move  = true; // true = white
    this->setPiecesSelectable(true);
    lboard->moves->clear();

    timersValue[0] = QStack<quint32>();
    timersValue[1] = QStack<quint32>();
    reverse(false);


    free_pieces[0]->reset();
    free_pieces[1]->reset();

    Piece *p_piece;
    unsigned int f_count = 0;
    for(int x = 0; x < 8; x++)
    {
        for(int y=0; y<8; y++)
        {
            if ( !LBoard::initPiecePos[x][y] ) continue;

            p_piece = pieces->at(f_count);
            p_piece->lpiece->inGame = true;
            p_piece->placeTo(grids[y][x]);
            p_piece->lpiece->clearMoves();
            if (p_piece->lpiece->type == LPiece::Queen &&
                    qAbs( LBoard::initPiecePos[x][y] ) == LPiece::Pawn )
            {
                p_piece->lpiece->type = LPiece::Pawn;
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
    // Check if AI is running
    if (aiThread->isRunning()) {
        aiThread->requestInterruption();
    }

    if (m_endGame) {
        this->setPiecesSelectable(true);
        m_endGame = false;
        this->timer[lboard->move]->start();
    }

    PieceMove *last = lboard->moves->back();
    Piece *piece = Piece::get(last->lpiece, this);

    // Stopping animation
    if (piece->isAnimationRunning())
    {
        this->setPiecesSelectable(true);
        piece->stopAnimation();

        Grid *offset_grid, *grid_from = Grid::get(last->from, this);
        piece->animateTo(grid_from);
        piece->placeTo(grid_from, false);

        // Castling
        if (last->extra && piece->lpiece->type == LPiece::King)
        {
            Piece *rook = nullptr;
            if (piece->grid->lgrid->offset(2, 0) == last->to)
            {
                rook = piece->grid->offset(1, 0)->piece;
                offset_grid = rook->grid->offset(2, 0);
                rook->stopAnimation();
                rook->animateTo(offset_grid);
                rook->placeTo(offset_grid, false);
            }

            else //if (piece->grid->lgrid->offset(-2, 0) == last->to)
            {
                rook = piece->grid->offset(-1, 0)->piece;
                offset_grid = rook->grid->offset(-3, 0);
                rook->stopAnimation();
                rook->animateTo(offset_grid);
                rook->placeTo(offset_grid, false);
            }
        }

        this->timer[lboard->move]->stop();
        this->timer[!lboard->move]->start();

    } else {

        if (last->removed){
            Piece *removed = Piece::get(last->removed, this);
            free_pieces[removed->lpiece->isWhite]->removePiece(removed);
            free_pieces[removed->lpiece->isWhite]->update();
            Grid *prevPos_grid =
                  Grid::get(last->extra && last->removed->type == LPiece::Pawn ?
                        last->to->offset(0, last->removed->isWhite ? 1 : -1)
                              : last->to, this);
            removed->animateTo(prevPos_grid);
            removed->placeTo(prevPos_grid, false);
        }

        // Castling
        if (last->extra && piece->lpiece->type == LPiece::King)
        {
            Piece *rook;
            Grid *g_offset;
            // Short
            if (piece->grid->lgrid->offset(-2, 0) == last->from)
            {
                rook = piece->grid->offset(-1, 0)->piece;
                g_offset = rook->grid->offset(2,0);
                rook->animateTo(g_offset);
                rook->placeTo(g_offset, false);
            }
            // Long
            else //if (piece->grid->lgrid->offset(-2, 0) == last->to)
            {
                rook = piece->grid->offset(1, 0)->piece;
                g_offset = rook->grid->offset(-3, 0);
                rook->animateTo(g_offset);
                rook->placeTo(g_offset, false);
            }
        }
        Grid *g_from = Grid::get(last->from, this);
        piece->animateTo(g_from);
        piece->placeTo(g_from, false);

        this->timer[lboard->move]->stop()->setTime(!timersValue[lboard->move].empty() ? timersValue[lboard->move].top() : 0);
        timersValue[lboard->move].pop();
        this->timer[!lboard->move]->start();
    }

    lboard->undoMove();
    resetHighligtedGrids();

    // If playing against computer
    if ( options->player[lboard->move] != Options::Human  ) {

        if (options->player[!lboard->move] == Options::Human && lboard->moves->size()) {
            this->undoMove();
        }
        if (options->player[lboard->move] != Options::Human) {
            this->computerMove();
        }
    }
}

void Board::computerMove()
{
    if (aiThread->isRunning()){
        return;
    }
    this->setPiecesSelectable(false);

    /*
    // Show as cpu thinking

    LBoard *copyBoard = new LBoard(*lboard);
    auto possibleMoves = aiThread->getBestMoves(copyBoard, 2);

    PieceMove possibleMove;
    if (possibleMoves.size() > 0) {
        possibleMove = possibleMoves[0];
    }

     if (!possibleMove.isNull()) {
         Grid::get(possibleMove.from, this)->highlight(2);
     }

    delete copyBoard;
    //*/
    aiThread->start();
}


void Board::computerMoveEnd()
{
    if (!aiThread->bestMove.isNull() && !aiThread->isInterrupted()) {
        Piece::get(aiThread->bestMove.lpiece, this)->makeMove(Grid::get(aiThread->bestMove.to, this));
        aiThread->bestMove = nullptr;
    }
    this->setPiecesSelectable(true);
}

// TODO: optimize
Board *Board::resetHighligtedGrids()
{

    while (!this->highlightedGrids.empty()) {
        this->highlightedGrids.top()->highlight(0);
        this->highlightedGrids.pop();
    }

    if ( lboard->isCheck(0) ) King[0]->grid->highlight(2);
    else if ( lboard->isCheck(1) ) King[1]->grid->highlight(2);

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
    this->setPiecesSelectable(false);
    m_endGame = true;
    this->timer[lboard->move]->stop();

    if (this->lboard->check_game() == 2) return;

    // Highlighting figures in check mate
    for(int i=-1; i<2; i++)
    {
        for(int j=-1; j<2; j++)
        {
            Grid *g = King[lboard->move]->grid->offset(i,j);

            if (g) // grid exists
            {
                auto attackingPieces = g->lgrid->attackedBy(!lboard->move);
                if (!(i || j)) {
                    for(LPiece *attackingPiece: attackingPieces)
                       Grid::get(attackingPiece->lgrid, this)->highlight(2);
                }

                else  if (g->lgrid->empty() // grid empty
                        || (g->lgrid->lpiece->isWhite != lboard->move))
                       // or grid not empty & piece is enemie's
                {
                    if (attackingPieces.size()){
                        Grid::get(attackingPieces.at(0)->lgrid,
                                  this) -> highlight(2);
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
            grids[i][j]->setPos(grid_size * i,
                                grid_size * ((m_reverse ? (j+1) : 8-j)) );
        }
    }
    for(int i=0; i<pieces->size(); i++)
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

        iter++;

    }

    free_pieces[0]->update();
    free_pieces[1]->update();

    double multiplier = double(grid_size * size.height()) /
                        double(boardTexture->pixmap().width());
    boardTexture->setScale(multiplier);
    boardTexture->setPos(-1 * grid_size, boardTexture->pos().y());
    if(this->reverse()) {
        auto t = QTransform()
                .rotate(180, Qt::YAxis)
                .translate(-boardTexture->pixmap().width()*multiplier, 0);
        boardTexture->setTransform(t);
    } else {
        boardTexture->resetTransform();
    }


    // timers
    auto sceneHeight = this->sceneRect().height();
    qreal xpos = this->sceneRect().width() - pw_timer[0]->rect().width();
    pw_timer[1]->setPos(xpos, this->reverse() ?
                            0 : sceneHeight - pw_timer[1]->rect().height());
    pw_timer[0]->setPos(xpos, this->reverse() ?
                            sceneHeight - pw_timer[0]->rect().height() : 0);
}


void Board::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}




// LBOARD


//*
const int LBoard::initPiecePos[8][8] = {
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



LBoard::LBoard() {
    move                = true; // true - white
    pieces              = new QVector<LPiece     *>;
    pieces_w            = new QVector<LPiece     *>;
    pieces_b            = new QVector<LPiece     *>;
    moves               = new QVector<PieceMove *>;
    currentMove         = nullptr;

    LPiece *p_piece;
    int pieceIndex = 0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            grids[j][i] = new LGrid(j, i, this);
            if ( !initPiecePos[i][j] ) continue;
            p_piece = new LPiece(static_cast<LPiece::Type>(qAbs(initPiecePos[i][j])), initPiecePos[i][j] > 0, grids[j][i], this);
            p_piece->index = pieceIndex++;
            pieces->push_back(p_piece);
            (p_piece->isWhite ? pieces_w : pieces_b)->push_back(p_piece);
        }
    }
    King[1] = pieces->at(4);
    King[0] = pieces->at(28);
}

LBoard::LBoard(const LBoard &origBoard)
{
    move                = true; // true - white
    pieces              = new QVector<LPiece     *>;
    pieces_w            = new QVector<LPiece     *>;
    pieces_b            = new QVector<LPiece     *>;
    moves               = new QVector<PieceMove *>;
    currentMove         = nullptr;

    LPiece *p_piece;
    int pieceIndex = 0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            grids[j][i] = new LGrid(j, i, this);
            if ( !initPiecePos[i][j] ) {
                   continue;
            }
            p_piece = new LPiece(static_cast<LPiece::Type>(qAbs(initPiecePos[i][j])), initPiecePos[i][j] > 0, grids[j][i], this);
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
        LPiece *p = pieces->at( quint32(pieceMove->lpiece->index) );
        LGrid *gridTo = grids[pieceMove->to->x][pieceMove->to->y];
        p->makeMove(gridTo);
    }
}


LBoard::~LBoard()
{
    for(LPiece *p: *this->pieces)
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

LGrid *LBoard::grid(int x, int y) const
{
    if (x < 0 || x > 7 || y < 0 || y > 7) return nullptr;
    return this->grids[x][y];
}

LPiece *LBoard::piece(int index) const
{
    if (index >= this->pieces->size() ) return nullptr;
    return this->pieces->at(quint32(index));
}


/**
 * returns: 0 if in game,
 *  check mate = 1, draw = 2
 *
*/

int LBoard::check_game() const
{
    int game_over = 2; // 0 - the game isn't over; 1 - check mate; 2 - draw;
    QVector<LPiece *> *pieces = move ? pieces_w : pieces_b;
    LPiece *temp;

    // Checkig for possible moves
    // IF not possible moves is mean draw
    for(int i = 0; i < pieces->size(); i++)
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
        if( this->isCheck(this->move) ){
            game_over = 1;
#ifdef _DEBUG
                    qDebug() << "Check mate!";
#endif
        }
    }
    else {
        // Check for repeating moves
        auto movesCount = this->moves->size();
        if (!game_over && movesCount > 6)
        {
            // IF last move is equal previous
            if (*moves->at(movesCount-1) == *moves->at(movesCount-5)) {
                if (*moves->at(movesCount-2) == *moves->at(movesCount-6)) {
                    game_over = 2;
#ifdef _DEBUG
                    qDebug() << "Draw!";
#endif
                }
            }
        }
    }

    return game_over;
}

void LBoard::undoMove()
{
    if (!moves->size()) return;
    PieceMove *last = moves->back();

    if (last->extra)
    {
        // Castling
        if (last->lpiece->type == LPiece::King)
        {
            if ( last->lpiece->lgrid->x == 2)
            {
                LPiece *Rook = last->lpiece->lgrid->offset(1,0)->lpiece;
                Rook->placeTo(Rook->lgrid->offset(-3,0));
            }
            else if ( last->lpiece->lgrid->x == 6)
            {
                LPiece *Rook = last->lpiece->lgrid->offset(-1,0)->lpiece;
                Rook->placeTo(Rook->lgrid->offset(2,0));
            }
        }
        else if (last->lpiece->type == LPiece::Queen)
        {
            last->lpiece->type = LPiece::Pawn;
        }
    }


    last->lpiece->placeTo(last->from);
    if ( last->removed )
    {
        last->removed->inGame = true;
        if (last->extra && last->removed->type == LPiece::Pawn){
            last->removed->placeTo(last->to->offset(0, last->removed->isWhite ? 1 : -1));
        }
        else {
            last->removed->placeTo(last->to);
        }
    }

    if (!last->fake) {
        positionChanged = true;
    }
    last->lpiece->moves--;
    moves->erase( moves->end() - 1 );
    delete last;
    this->currentMove = nullptr;
    move = !move;
}


/***
 *  Returns true if check
 */
bool LBoard::isCheck(bool w) const
{
    return King[w]->lgrid->is_attacked(!w);
}



int LBoard::getCurrentScore()
{
    //int Points =              {None, King,     Queen,   Bishop,  Knight, Rook,  Pawn};
    const static int points[] = {0,    10000,    1000,    500,     510,    800,   100};


    int gameState = this->check_game();

    if (gameState == 1) {
        return this->move ? MIN_SCORE : MAX_SCORE;
    } else if (gameState == 2){
        return !this->move ? MIN_SCORE : MAX_SCORE;
    }

    int total = 0;
    for( LPiece *piece : *pieces ){
        if (piece->inGame)
        {
            total += piece->isWhite ? points[piece->type] : -points[piece->type];
            total += getPiecePosEval(piece);
        }
    }

    return total ;

}

int LBoard::getPiecePosEval(LPiece *piece) const
{
// Position evaluations
#include "poseval.h"

    bool w = piece->isWhite;
    int p = posEval[piece->type] [ (w ? piece->lgrid->y : 7 - (piece->lgrid->y)) * 8 + piece->lgrid->x ];
    return w ? p : -p;
}


QVector<PieceMove> LBoard::getAllMoves() const
{
    QVector<PieceMove> result;
    QVector<LPiece *> availPieces, *currentPieces = (this->move ? pieces_w : pieces_b);

    for (LPiece *piece : *currentPieces) {
        if(piece->inGame) {
            availPieces.push_back(piece);
            QVector<LGrid*> moves = piece->getGrids();
            for(LGrid *g : moves) {
                result.push_back(PieceMove(piece, piece->lgrid, g));
            }
        }
    }

    return result;
}
