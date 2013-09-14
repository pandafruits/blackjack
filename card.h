#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPixmap>
#include <QChar>
#include <QImage>
#include <QMap>

/**
 * Class that represents a card.
 * This class represents a playing card that's characterized by:
 * - a name that consists of a "value + suit" string such as "5s"
 * - a pixmap picture
 * - a flag denoting whether the card is facing down or up
 */ 
class Card : public QLabel
{
	Q_OBJECT
	
public:
	/**
	 * enum type representing card face up or down.
	 */
	enum CardFaceDirection {
		                       CardFaceUp = 0,    /**< enum value CardFaceUp. */
		                       CardFaceDown = 1   /**< enum value CardFaceDown. */
		                   }; 
	
	static const QString CardValues; /**< static member containing all card values. */
	static const QString CardSuits;  /**< static member containing all card suits. */
	static QMap<QString, QImage> CardImageMap; /**< static member containing <card, image> mapping. */
	
	Card(QString name, CardFaceDirection facedir = CardFaceUp, QWidget *parent = 0);
	
	/**
	 * Member function that checks whether card is facing down.
	 * @return true: card is facing down; false: card is facing up
	 */
	bool isFaceDown() const {return m_faceDown;}
	
	/**
	 * Member function that checks whether card is Ace.
	 * @return true: card is Ace; false: card is not Ace
	 */
	bool isAce() const {return m_name[0] == 'a';}
	
	/**
	 * Member function that returns card name.
	 * @return Card name e.g. "5s"
	 */
	QString name() const {return m_name;}
	
	/**
	 * Member function that returns card value.
	 * @return Card value - one of "23456789tjqka"
	 */
	QChar value() const {return m_name[0];}
	
	/**
	 * Member function that returns card suit.
	 * @return Card suit - one of "cdhs"
	 */
	QChar suit() const {return m_name[1];}
	
	Card *setFacedown(bool wantFacedown);
	
private:
	void initCardImages();
	
private:
	QString m_name;
	QPixmap m_pixmap;
	bool m_faceDown;
};

#endif
