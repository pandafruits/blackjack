#ifndef DECK_H
#define DECK_H

#include <QList>
#include "card.h"

/**
 * Class that represents a card deck.
 * This class only has one data member - a Card list.
 */
class Deck
{
public:
	Deck();
	~Deck();
	
	/**
	 * Member function that returns the number of cards left in the deck.
	 * @return Number of cards left in the deck
	 */
	int cardsLeft() const {return m_cards.count();}
	void shuffle();
	void reset();
	Card * deal();
	QList<Card *> deal(int numcards);
	
private:
	QList<Card *> m_cards;
};

#endif
