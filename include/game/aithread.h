#ifndef AITHREAD_H
#define AITHREAD_H

#include <QThread>

#include "board.h"
#include "piecemove.h"
#include "piece.h"


class AIThread : public QThread
{
    Board *board;
    qint64 totalIterations;
    bool interrupted {false};
public:
    PieceMove bestMove;
    AIThread(Board *board);
    /* virtual */ void run();
    int minimax(LBoard *lboard,
                int depth,
                int alpha = MIN_SCORE,
                int beta = MAX_SCORE);
    QVector<PieceMove> getBestMoves(LBoard *lboard, int depth);
    bool isInterrupted() const;
    void setInterrupted(bool value);
};


#endif // AITHREAD_H
