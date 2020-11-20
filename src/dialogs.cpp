#include "dialogs.h"
#include "mainwindow.h"

NewGameDialog::NewGameDialog(MainWindow *mwnd, QWidget *parent) : QWidget(parent)
{
    mainWindow = mwnd;
    QStringList cpuLevels{"Low", "Medium", "Hard", "Very hard"};
    cpuLevel = new ListSelect("CPU Level", cpuLevels);
    playerSide = new ListSelect("Player's side", QStringList{"White","Black"});
    QVBoxLayout *vbl = new QVBoxLayout;

    btn_play = new QPushButton("PLAY",this);
    btn_play->setStyleSheet("font-size: 18pt; border: none; color: black; margin: 15px;");
    QObject::connect(btn_play, SIGNAL(clicked()), SLOT(playBtnPressed()));

    vbl->addWidget(cpuLevel);
    vbl->addWidget(playerSide);
    vbl->addWidget(btn_play);
    vbl->setAlignment(Qt::AlignVCenter);
    setLayout(vbl);
}

NewGameDialog::~NewGameDialog()
{}

void NewGameDialog::playBtnPressed()
{
   auto options = mainWindow->view->board->options;
   options->player[!playerSide->selected()] = Options::Human;
   options->player[playerSide->selected()] = static_cast<Options::PlayerType>(cpuLevel->selected()+1);

   mainWindow->setCentralWidget(mainWindow->view);
   mainWindow->view->board->newGame();
   mainWindow->view->fadeIn();
}

ListSelect::ListSelect(QString header, QStringList list_options, QWidget *parent) : QWidget(parent)
{
    vbl = new QVBoxLayout;
    hbl = new QHBoxLayout;
    lbl_header = new QLabel(header, this);
    lbl_header->setStyleSheet("border:none;border-right:2px solid gray;");
    lbl_header->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    list_selections = new QList<QPushButton *>;

    for(auto str : list_options) {
        QPushButton *btn = new QPushButton(str, this);
        list_selections->append(btn);
        vbl->addWidget(btn);
        QObject::connect(btn, SIGNAL(clicked()), this, SLOT(select()));
    }

    setStyleSheet("QLabel, QPushButton {color: #666; text-align:left; border: none; font-size: 18pt;}");

    vbl->setAlignment(Qt::AlignLeft);

    hbl->addWidget(lbl_header);
    hbl->addLayout(vbl);
    setLayout(hbl);
    hbl->setAlignment(Qt::AlignVCenter);
    select(list_selections->at(0));
}


ListSelect::~ListSelect()
{

}

void ListSelect::select(QPushButton *btn)
{
    if (btn == optionSelected){
        return;
    }

    optionSelected = btn;

    for(auto i: *list_selections) {
        i->setStyleSheet("color: #666; border: none; font-size: 18pt;");
    }

    btn->setStyleSheet("color: black; border: none; font-size: 18pt;");
}

void ListSelect::select()
{
    QPushButton *btn = qobject_cast<QPushButton*> (sender ());
    select(btn);
    qDebug() << selected();
}

unsigned ListSelect::selected()
{
    unsigned i=0;

    for(auto it: *list_selections) {
        if (it == optionSelected) {
            break;
        }

        i++;
    }

    return i;
}
