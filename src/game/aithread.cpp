#include "aithread.h"


bool AIThread::isInterrupted() const
{
    return interrupted;
}

void AIThread::setInterrupted(bool value)
{
    interrupted = value;
}

AIThread::AIThread(Board *_board)
    : board(_board)
{}



void AIThread::run()
{
    this->setInterrupted(false);
    totalIterations = 0;

    QElapsedTimer timer;
    timer.start();

    LBoard *lboard = new LBoard(*board->lboard);//board->lboard;

    auto playerType = board->options->player[board->lboard->move];
    int level = playerType == 1 ? 2 : playerType == 2 ? 3 : 4;

    //if (lboard->moves->size() < 11) level = 2;
    QVector<PieceMove> bestMoves = getBestMoves(lboard, level);
    PieceMove r_bestMove;

    QRandomGenerator randomGenerator(time(0));

    if (bestMoves.size()) {
        unsigned int r = (randomGenerator.generate() & std::numeric_limits<unsigned>::max() % bestMoves.size());
        r_bestMove = bestMoves.size() > 1 ? bestMoves[r] : bestMoves[0];
        qDebug() << "(random from" << bestMoves.size() << "best moves)";
    }

    else if (r_bestMove.isNull() && bestMoves.size()) {
        unsigned int r = (randomGenerator.generate() & std::numeric_limits<unsigned>::max() % bestMoves.size());
        r_bestMove = bestMoves[r];
        qDebug() << "(random from ALL moves)";
    }

    if (!r_bestMove.isNull()) {
       bestMove = PieceMove(board->lboard->pieces->at(r_bestMove.lpiece->index),
                    board->lboard->grid(r_bestMove.from->x, r_bestMove.from->y),
                    board->lboard->grid(r_bestMove.to->x, r_bestMove.to->y),
                    r_bestMove.removed);

        qreal elapsed = qreal(timer.elapsed()) / 1000;
        qDebug() << totalIterations << "iter. /" << elapsed << "s ="
                 << (qreal(totalIterations)/(elapsed)) << "i/s";
    }

    delete lboard;
    if (timer.elapsed() < 1000) {
        QThread::sleep((randomGenerator.generate() & std::numeric_limits<unsigned>::max()) % 5 + 1);
    }
}



int AIThread::minimax(LBoard *lboard, int depth, int alpha, int beta)
{
    int bestMove = lboard->move ? MIN_SCORE : MAX_SCORE;

    if (isInterruptionRequested()) {
        interrupted = true;
        return bestMove;
    }

    if (depth == 0 || lboard->check_game()) {
        return lboard->getCurrentScore();
    }

    QVector<PieceMove> moves = lboard->getAllMoves();
    for(PieceMove pieceMove : moves) {
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


QVector<PieceMove> AIThread::getBestMoves(LBoard *lboard, int depth)
{
    QVector<PieceMove> eqMoves,
            moves = lboard->getAllMoves();
    if (this->isInterruptionRequested())
    {
#ifdef _DEBUG
        qDebug() << "AIThread::interruption";
#endif
        interrupted = true;
        return moves;
    }

    totalIterations = 0;
    if (depth > 0) {

        if (!moves.size()) {
            qDebug() << "no moves :(";
            return moves;
        }

        int score = lboard->move ? MIN_SCORE : MAX_SCORE;

        for(PieceMove pieceMove : moves)
        {
            totalIterations++;
            pieceMove.lpiece->makeMove(pieceMove.to);
            int m = minimax(lboard, depth - 1);
            lboard->undoMove();

            if (isInterruptionRequested()) {
                interrupted = true;
                return moves;
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

    return eqMoves;
}
