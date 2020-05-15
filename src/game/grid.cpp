#include "grid.h"

LGrid::LGrid(int _x, int _y, LBoard *_board) : x(_x), y(_y), lboard(_board)
{
    lpiece = nullptr;
}

LGrid *LGrid::offset(int dx, int dy)
{
    int xn = x+dx;
    int yn = y+dy;
    if ( xn < 0 || xn > 7 || yn < 0 || yn > 7) {
        return nullptr;
    }
    return lboard->grids[xn][yn];
}

QString LGrid::name() const
{
    return QString(char(0x61+x)) + QString::number(y+1);
}

bool LGrid::is_attacked(bool w)
{
    LPiece* p_piece;
    vector<LGrid *> grids;

    vector<LPiece *> *pieces =
            w ? lboard->pieces_w
              : lboard->pieces_b;
    unsigned pieces_count = pieces->size();
    for(unsigned i = 0; i < pieces_count; i++)
    {
         p_piece = pieces->at(i);
         if( !p_piece->inGame ) continue;
         grids = p_piece->getGrids(true);
         if ( std::find(grids.begin(), grids.end(), this) != grids.end() ) {
             return true;
         }
    }
    return false;
}



bool LGrid::empty() const
{
    return lpiece == nullptr;
}

vector<LPiece *> LGrid::attackedBy(bool w)
{
    vector<LPiece *> *pieces = w ? lboard->pieces_w : lboard->pieces_b;
    vector<LGrid *> grids;
    vector<LPiece *> result;
    for(LPiece *piece: *pieces)
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



Grid::Grid(LGrid *l_grid, Board *_board)
    : QGraphicsItem(), lgrid(l_grid), board(_board)
{
    light = false;
    piece = nullptr;

    setZValue(0);
    setPos(lgrid->x * board->grid_size,
           (board->reverse() ? lgrid->y : 7 - lgrid->y) * board->grid_size);
}

Grid::~Grid()
{

}

void Grid::highlight(int lightType)
{
    if (light == lightType) return; // dont change anything cause no needed

    board->highlightedGrids.push(this);

    light = lightType;
    update(0, 0, board->grid_size,board->grid_size); // repaint
}

Grid::operator LGrid() const
{
    return *lgrid;
}

Grid *Grid::offset(int dx, int dy) const
{
    int xn = lgrid->x+dx,
        yn = lgrid->y+dy;
    if ( xn < 0 || xn > 7 || yn < 0 || yn > 7) {
        return nullptr;
    }
    return board->grids[xn][yn];
}

Grid *Grid::get(LGrid *l_grid, Board *board)
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
            color =  (lgrid->lpiece && lgrid->lpiece->isWhite != board->lboard->move ) ? Qt::red : Qt::green;
            break;
        case 2:
            color = Qt::red;
            break;
        case 3:
            color = Qt::darkBlue;
            break;
        case 4:
            color = Qt::blue;
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
