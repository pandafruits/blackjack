#include "hand.h"

/**
 * Default constructor for the Hand class.
 * The function does nothing so the hand will be empty.
 */
Hand::Hand(QObject *parent) : QObject(parent)
{}

/**
 * Hand class constuctor.
 * This constuctor takes a card list to initialize the hand.
 * @param cards A card list to initialize the hand
 */
Hand::Hand(QList<Card *> cards, QObject *parent) : 
QObject(parent), m_cards(cards)
{}

/**
 * Member function that adds a card to the hand.
 * @param card Card to be added to the hand
 * @return Reference to the hand object
 */
Hand& Hand::operator<<(Card *card)
{
	m_cards << card;
	emit handChanged();
	return *this;
}

/**
 * Member function that calculates the best score of the hand.
 * This function calculates the score of the hand. It has some 
 * intelligence to determine whether an Ace should be counted as 10 or 1.
 * @return Best possible score of the hand
 */
int Hand::score() const
{
	int score = 0;
	int numcards = m_cards.count();
	bool hasAce = false;
	
	// All Aces (if any) are counted as 1 first
	for(int i = 0; i < numcards; ++i)
	{
		if(m_cards[i]->isAce())
		{
			hasAce = true;
		}
		score += cardPoints(m_cards[i]);
	}
	
	// If the hand does contains Aces and the score is less than 11,
	// then changes one Ace's from 1 to 11.
	if(hasAce && score<=11)
	{
		score += 10;
	}

	return score;
}

/**
 * Member function that check whether is hand is a Blackjack.
 * @return true: the hand is a Blackjack; false: the hand is not a Blackjack
 */
bool Hand::isBlackjack() const
{
	if((m_cards.count()==2) && (score()==21))
	{
		return true;
	}
	return false;
}

/**
 * Member function that check whether is hand has busted.
 * @return true: the hand has busted; false: the hand has not busted
 */
bool Hand::busted() const
{
	if(score() > 21)
	{
		return true;
	}
	return false;
}
	
/**
 * Helper function to check a card's point.
 * Note Aces are always counted as 1 in here.
 * @param cd A Card pointer
 * @return Score of the card
 */
int Hand::cardPoints(Card *cd) const
{
	QChar c = cd->value();
	
	if(c.isDigit())
	{
		return c.digitValue();
	}
	else if(c == 'a')
	{
		return 1;
	}
	else
	{
		return 10;
	}
}

/**
 * Member function that deletes all cards in the hand.
 */
void Hand::clear()
{
	int numCards = m_cards.count();
	
	for(int i = 0; i < numCards; ++i)
	{
		delete m_cards[i];
	}
	
	m_cards.clear();
	emit handChanged();
}

