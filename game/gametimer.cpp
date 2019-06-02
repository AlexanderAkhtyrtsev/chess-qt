#include "gametimer.h"
#include "board.h"
GameTimer::GameTimer(Board *brd, QWidget *parent) : QLabel(parent)
{
    board = brd;
    passed = 0;
    t_tick = 0;
    timer = new QTimer;
    timer->setInterval(1000);
    QObject::connect(timer, SIGNAL(timeout()), SLOT(tick()));
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    resize(400,32);
    this->setText("--:--");
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
    this->showTime();
    return this;
}

GameTimer *GameTimer::stop()
{

    timer->stop();
    passed += t_tick;
    t_tick = 0;
    this->showTime();
    return this;
}

GameTimer *GameTimer::reset()
{
    if (timer->isActive()) timer->stop();
    passed = 0;
    t_start = 0;
    t_tick = 0;
    this->showTime();
    return this;
}

void GameTimer::tick()
{
    t_tick = QDateTime::currentMSecsSinceEpoch() - t_start;

    this->showTime();
}

void GameTimer::showTime()
{
    quint32 passed_s = (passed + t_tick) / 1000;
    quint32 passed_m = (passed_s - (passed_s % 60));
    passed_s -= passed_m;
    passed_m /= 60;

    QString str = QString::number(passed_m) + ":" + (passed_s < 10 ? "0" : "" ) + QString::number(passed_s);
    this->setText(str);
    this->setStyleSheet(timer->isActive() ? "color: black" :"color: silver");
}
QSize GameTimer::sizeHint() const
{
    return this->size();
}

void GameTimer::resizeEvent(QResizeEvent *)
{
    QFont font = this->font();
    font.setPixelSize(this->height());
    this->setFont(font);
}


