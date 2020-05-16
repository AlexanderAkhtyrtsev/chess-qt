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
    unsigned sz = m_pieces->size();
    if (!sz) {
        m_pieces->push_back(piece);
    } else {
        bool samePieceFound = false;
        for(unsigned i=0; i<sz; i++) {
            if(m_pieces->at(i)->lpiece->type == piece->lpiece->type) {
                m_pieces->insert(m_pieces->begin()+i+1, piece);
                samePieceFound = true;
                break;
            }
        }

        if (!samePieceFound) {
            m_pieces->push_back(piece);
        }
    }
    update();
    return piece;
}



Piece *FreePieces::removePiece(Piece *piece)
{
    /*vector<Piece *>::iterator i*/
    auto i = std::find(m_pieces->begin(), m_pieces->end(), piece);
    if ( i != m_pieces->end() ) m_pieces->erase(i);
    return piece;
}

void FreePieces::reset()
{
    m_pieces->clear();
    update();
}

void FreePieces::update() const
{
    unsigned int sz = m_pieces->size();
    Piece *piece;

    unsigned int empty = 0;
    for(unsigned i = 0; i < sz; i++)
    {
        piece = m_pieces->at(i);
        int top = 0,
         bottom = 9;
        int y = m_board->reverse() ? (piece->lpiece->isWhite ? bottom : top) :
                                     (piece->lpiece->isWhite ? top : bottom);
        if ( i > 0 && piece->lpiece->type != m_pieces->at(i-1)->lpiece->type ) {
            empty+=2;
        }
        piece->setPos( (empty + i) * (m_board->grid_size) / 3.5,
                       y * m_board->grid_size );
    }
}
