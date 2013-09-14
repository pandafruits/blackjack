#ifndef HAND_H
#define HAND_H

#include "card.h"

/**
 * Class that represents a hand of cards.
 * This class only has one data member - a Card list.
 */
class Hand : public QObject
{
	Q_OBJECT
	
public:
	Hand(QObject *parent = 0);
	Hand(QList<Card *> cards, QObject *parent = 0);
	
	Hand& operator<<(Card *card);
	int score() const;
	bool isBlackjack() const;
	bool busted() const;
	
	/**
	 * Member function that returns the hand.
	 * @return The card list of the hand
	 */
	QList<Card *> cards() const {return m_cards;}
	
	/**
	 * Member function that returns the number of cards in the hand.
	 * @return Number of cards in the hand
	 */
	int numCards() const {return m_cards.count();}
	void clear();
	
signals:
	/**
	 * Hand changed signal.
	 * This signal is emitted when the hand is changed.
	 * @see HandView::refresh()
	 */
	void handChanged();
	
private:
	int cardPoints(Card *c) const;

private:
	QList<Card *> m_cards;
};

#endif
