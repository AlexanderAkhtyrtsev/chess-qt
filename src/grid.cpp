#include "grid.h"

namespace L {

Grid::Grid(int _x, int _y, L::Board *_board) : x(_x), y(_y), lboard(_board)
{
    lpiece = nullptr;
}

Grid *Grid::offset(int dx, int dy)
{
    assert(this);
    int xn = x+dx;
    int yn = y+dy;
    if ( xn < 0 || xn > 7 || yn < 0 || yn > 7) {
        return nullptr;
    }
    return lboard->grids[xn][yn];
}

QString Grid::name()
{
    return QString(char(0x61+x)) + QString::number(y+1);
}

bool Grid::is_attacked(bool w)
{

    L::Piece* p_piece;
    vector<L::Grid *> grids;

    vector<L::Piece *> *pieces =
            w ? lboard->pieces_w
              : lboard->pieces_b;
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
    return lpiece == nullptr;
}

vector<Piece *> Grid::attackedBy(bool w)
{
    vector<Piece *> *pieces = w ? lboard->pieces_w : lboard->pieces_b;
    vector<Grid *> grids;
    vector<Piece *> result;
    for(Piece *piece: *pieces)
    {
        if (!piece->inGame) continue;
        grids = piece->getGrids(true);
        if ( std::find(grids.begin(), grids.end(), this) != grids.end() )
        {
            result.push_back(piece);
        }
    }
    return result;
}


} // NAMESPACE L


Grid::Grid(L::Grid *l_grid, Board *_board) : QGraphicsItem(), lgrid(l_grid), board(_board)
{
    light = false;
    piece = nullptr;
    setZValue(0);
    setPos(lgrid->x*board->grid_size, (board->reverse() ? lgrid->y : 7-lgrid->y)*board->grid_size);
}

Grid::~Grid()
{

}

void Grid::highlight(int lightType)
{
    if (light == lightType) return; // dont change ath cause no needed

    board->highlightedGrids.push(this);
    light = lightType;
    update(0, 0, board->grid_size,board->grid_size);
}

Grid::operator L::Grid() const
{
    return *lgrid;
}

Grid *Grid::offset(int dx, int dy)
{
    int xn = lgrid->x+dx,
        yn = lgrid->y+dy;
    if ( xn < 0 || xn > 7 || yn < 0 || yn > 7) {
        return nullptr;
    }
    return board->grids[xn][yn];
}

Grid *Grid::get(L::Grid *l_grid, Board *board)
{
    if (!l_grid) return nullptr;
    return board->grids [l_grid->x][l_grid->y];
}


QRectF Grid::boundingRect() const
{
    return QRectF(0, 0, board->grid_size, board->grid_size);
}


void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{

    if (light)
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setOpacity(0.8);
         int p =  static_cast<int>(board->grid_size * 0.05);

        QRect rect(p, p, board->grid_size-p*2, board->grid_size-p*2);

        QColor color;
        switch (light) {
        case 1:
            color = this->lgrid->empty() ? Qt::green :
                    lgrid->lpiece->isWhite != board->lboard->move ? Qt::red :
                    board->selected && lgrid->lpiece == board->selected->lpiece ? Qt::blue : Qt::red ;
            break;
        case 2:
            color = Qt::darkBlue;
            break;
        };

        color.setAlpha(80);
        QBrush brush2(color);
        painter->setPen(QPen(brush2, p, Qt::SolidLine));
        painter->setBrush(brush2);
        painter->drawRect(rect);
        painter->restore();
    }
}


void Grid::mousePressEvent(QGraphicsSceneMouseEvent *pe)
{
    if( board->selected && light && lgrid->lpiece != board->selected->lpiece ) {
        board->selected->makeMove(this);

    } else if( lgrid->empty() && board->selected) {
        board->resetHighligtedGrids()->selected = nullptr;
    }

    QGraphicsItem::mousePressEvent(pe);
}
