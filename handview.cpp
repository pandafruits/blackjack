#include <QDebug>

#include "handview.h"

/**
 * The HandView class constructor.
 * Show cards (if any) of a hand in a horizontal layout.
 * @see refresh()
 */
HandView::HandView(Hand *hand, QWidget *parent) :
QWidget(parent), m_hand(hand), m_numCards(hand->numCards())
{
	setFixedSize(200, 120);
	refresh();
}

/**
 * Hand view refresh slot.
 * This slot is called when the hand is changed.
 * @see Hand::handchanged()
 */
void HandView::refresh()
{
	QList<Card *> cards = m_hand->cards();
	m_numCards = m_hand->numCards();
	
	for(int i = 0; i < m_numCards; ++i)
	{
		Card *c = cards[i];
					
		c->setParent(this);
		c->setGeometry(20*i, 20, 72, 96);
		c->show();
	}		
}
