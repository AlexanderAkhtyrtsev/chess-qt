#include "gametimer.h"
#include "board.h"
GameTimer::GameTimer(Board *brd, QWidget *parent) : QWidget(parent)
{
    board = brd;
    passed = 0;
    t_tick = 0;
    timer = new QTimer;
    timer->setInterval(1000);
    QObject::connect(timer, SIGNAL(timeout()), SLOT(tick()));
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    resize(64,32);
}

GameTimer::~GameTimer()
{
    delete timer;
}

quint32 GameTimer::time() const
{
    return passed;
}

void GameTimer::setTime(quint32 time)
{
    passed = time;
    update();
}

GameTimer *GameTimer::start()
{
    t_start = QDateTime::currentMSecsSinceEpoch();
    timer->start();
    this->update();
    return this;
}

GameTimer *GameTimer::stop()
{

    timer->stop();
    passed += t_tick;
    t_tick = 0;
    this->update();
    return this;
}

GameTimer *GameTimer::reset()
{
    if (timer->isActive()) timer->stop();
    passed = 0;
    t_start = 0;
    t_tick = 0;
    this->update();
    return this;
}

void GameTimer::tick()
{
    t_tick = QDateTime::currentMSecsSinceEpoch() - t_start;
    this->update();
}

void GameTimer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFont font("Arial",40);
    font.setPixelSize(this->height());
    painter.setFont(font);
    painter.setPen(QColor(timer->isActive() ? Qt::black : Qt::gray));
    quint32 passed_s = (passed + t_tick) / 1000;
    quint32 passed_m = (passed_s - (passed_s % 60));
    passed_s -= passed_m;
    passed_m /= 60;

    QString str = QString::number(passed_m) + ":" + (passed_s < 10 ? "0" : "" ) + QString::number(passed_s);

    painter.drawText(this->rect(), str);
    painter.end();
}

QSize GameTimer::sizeHint() const
{
    return this->size();
}


