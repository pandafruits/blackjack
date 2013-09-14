#include <QtGlobal>
#include <QDateTime>
#include "deck.h"

/**
 * The Deck class constructor.
 * The card list data member of the class is populated with 
 * unshuffled cards.
 */
Deck::Deck()
{
	qsrand(QDateTime::currentDateTime().toTime_t());
	
	foreach(QChar suit, Card::CardSuits)
	{
		foreach(QChar value, Card::CardValues)
		{
			QString card = QString("%1%2").arg(value).arg(suit);
			m_cards << new Card(card);
		}
	}
}

/**
 * The Deck class destructor.
 * Delete cards that stills remains in the deck (not delt to anybody). 
 * The desturctor is required as the Deck is not derived from QObject 
 * so it does not have a parent to manage it.
 */
Deck::~Deck()
{
	int numCards = m_cards.count();
	
	for(int i = 0; i < numCards; ++i)
	{
		delete m_cards[i];
	}
	
	m_cards.clear();
}

/**
 * Member function that shuffles the deck.
 * This function must be called when the deck is still in full i.e. 
 * having 52 cards.
 */
void Deck::shuffle()
{
	// Randomly swap cards 500 times
	for(int i = 0; i < 500; ++i)
	{
		m_cards.swap(qrand()%52, qrand()%52);
    }
}

/**
 * Member function that resets the deck
 * The deck is reset to an untouched state i.e. having 52 cards and 
 * unshuffled.
 */
void Deck::reset()
{
	// Delete all remaining cards
	int numCards = m_cards.count();
	for(int i = 0; i < numCards; ++i)
	{
		delete m_cards[i];
	}
	m_cards.clear();
	
	// Then re-constructs the deck
	foreach(QChar suit, Card::CardSuits)
	{
		foreach(QChar value, Card::CardValues)
		{
			QString card = QString("%1%2").arg(value).arg(suit);
			m_cards << new Card(card);
		}
	}	
}

/**
 * Member function that deals one card from the deck.
 * This function deals (removes) one card from the deck. It returns 0 if
 * the deck is empty.
 * @return Pointer to the card delt, 0 if deck is empty
 */
Card * Deck::deal()
{
	if(m_cards.isEmpty())
	{
		return 0;
	}
	else
	{
		return m_cards.takeFirst();
	}
}

/**
 * Member function that deals any number of cards.
 * This function deals (removes) any number of cards from the deck. 
 * @param numcards Number of cards to be delt
 * @return A list of Card, the list is empty if the deck has less cards than desired
 */
QList<Card *> Deck::deal(int numcards)
{
	QList<Card *> retval;
	
	if(m_cards.count() < numcards)
	{
		return retval;
	}
	else
	{
		while(0 < numcards--)
		{
			retval << m_cards.takeFirst();
		}
		
		return retval;
	}
}
