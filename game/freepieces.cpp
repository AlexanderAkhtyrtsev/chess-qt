#include "freepieces.h"

FreePieces::FreePieces(Board *brd)
{
    m_board = brd;
    m_pieces = new vector<Piece *>;
}

FreePieces::~FreePieces()
{
    delete m_pieces;
}


Piece *FreePieces::addPiece(Piece *piece)
{
    int sz = m_pieces->size();
    if (!sz) {
        m_pieces->push_back(piece);
    } else {
        bool f = false;
        for(int i=0; i<sz; i++) {
            if(m_pieces->at(i)->type == piece->type) {
                m_pieces->insert(m_pieces->begin()+1+i, piece);
                f = true;
                break;
            }
        }
        if (!f) {
            m_pieces->push_back(piece);
        }
    }
    update();
    return piece;
}



Piece *FreePieces::removePiece(Piece *piece)
{
    vector<Piece *>::iterator i = std::find(m_pieces->begin(), m_pieces->end(), piece);
    if ( i != m_pieces->end() ) m_pieces->erase(i);
    return piece;
}

void FreePieces::update()
{
    int sz = m_pieces->size();
    Piece *piece;

    int empty = 0;
    for(int i = 0; i < sz; i++)
    {
        piece = m_pieces->at(i);
        int top = 0,
         bottom = 9;
        int y = m_board->reverse() ? (piece->isWhite ? bottom : top) : (piece->isWhite ? top : bottom);
        if ( i > 0 && piece->type != m_pieces->at(i-1)->type ) empty+=2;
        piece->setPos( (empty + i) * (m_board->grid_size) / 3.5 + m_board->grid_size, y * m_board->grid_size );
    }
}
