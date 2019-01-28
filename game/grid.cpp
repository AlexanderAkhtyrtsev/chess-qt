#include "grid.h"

Grid::Grid(bool w, int _x, int _y, Board *_board) : QGraphicsItem()
{
    is_white = w;
    light = false;
    piece = 0;
    board = _board;
    x = _x; y = _y;

    if( x < 0 ) x = 0;
    else if (x > 7) x = 7;

    if( y < 0 ) y = 0;
    else if (y > 7) y = 7;

    setZValue(0);
    setPos(x*board->grid_size, (board->reverse() ? y : 7-y)*board->grid_size);
}

Grid::~Grid()
{

}

void Grid::Highlight(int lightType)
{
    if (light == lightType) return; // dont change ath cause no needed
    light = lightType;
    update(0, 0, board->grid_size,board->grid_size);
}



QRectF Grid::boundingRect() const
{
    return QRectF(0, 0, board->grid_size, board->grid_size);
}

Grid *Grid::Offset(int dx, int dy)
{
    int xn = x+dx,
            yn = y+dy;
    if ( xn < 0 || xn > 7 || yn < 0 || yn > 7) return 0;
    return board->grids[xn][yn];
}

QString Grid::name()
{
    return QString((char)(0x61+x)) + QString::number(y+1);
}

bool Grid::is_attacked(bool w)
{

    Piece* p_piece;
    vector<Grid *> grids;

    vector<Piece *> *pieces =
            w ? board->pieces_w
              : board->pieces_b;
    unsigned pieces_count = pieces->size();
    for(unsigned i = 0; i < pieces_count; i++)
    {
         p_piece = pieces->at(i);
         if( !p_piece->inGame ) continue;
         grids = p_piece->getGrids(true);
         if ( std::find(grids.begin(), grids.end(), this) != grids.end() ) return true;
    }
    return false;
}



bool Grid::empty() const
{
    return piece == 0;
}


void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{

    /*
    QBrush brush(QColor(is_white ? QColor(200, 200, 200) : QColor(120, 120, 120)));
    painter->save();
        painter->setBrush(brush);
        painter->setPen(QPen(Qt::NoPen)); // no border
        painter->drawRect(QRect(0, 0, board->grid_size, board->grid_size));
        painter->setRenderHint(QPainter::Antialiasing, true);
    painter->restore();
*/

    if (light)
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setOpacity(0.8);
        int p =  board->grid_size * 0.09;

        QRect rect(p, p, board->grid_size-p*2, board->grid_size-p*2);

        QColor color;
        switch (light) {
        case 1:
            color = this->empty() ? Qt::green :
                    piece->isWhite != board->move ? Qt::red :
                    board->selected && piece == board->selected ? Qt::blue : Qt::red ;
            break;
        case 2:
            color = Qt::gray;
            break;
        };
        QBrush brush2(color);
               painter->setPen(QPen(brush2, p, Qt::SolidLine, Qt::RoundCap));
               painter->drawRect(rect);
        painter->restore();
    }

}


void Grid::mousePressEvent(QGraphicsSceneMouseEvent *pe)
{
    if( board->selected && light && piece != board->selected ) board->selected->makeMove(this);
    else if( this->empty() )  board->ResetHighligtedGrids()->selected = 0;
    QGraphicsItem::mousePressEvent(pe);
}
