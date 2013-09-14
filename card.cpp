#include "card.h"

const QString Card::CardValues = "23456789tjqka";
const QString Card::CardSuits = "cdhs";
QMap<QString, QImage> Card::CardImageMap = QMap<QString, QImage>();

/**
 * Member function that reads in card images and populate the card image map.
 * This function is only called once when the first Card is constructed.
 */
void Card::initCardImages()
{
	foreach(QChar suit, CardSuits)
	{
		foreach(QChar value, CardValues)
		{
			QString card = QString("%1%2").arg(value).arg(suit);
			QString imageFileName = QString(":/images/cards/%1.png").arg(card);
			QImage image(imageFileName);
			CardImageMap[card] = image;
		}
	}
	
	// Card back image
	CardImageMap["cb"] = QImage(":/images/cards/cb.png");
}

/**
 * The Card class constructor.
 * Within the function the image map initializer is called 
 * on first Card construction.
 * @see initCardImages()
 */
Card::Card(QString name, CardFaceDirection facedir, QWidget *parent) :
QLabel(parent), m_name(name), m_faceDown(facedir)
{
	static bool cardImageMapInitialized = false;
	if(cardImageMapInitialized == false)
	{
		initCardImages();
		cardImageMapInitialized = true;
	}
	
	if(facedir == CardFaceUp)
	{
		m_pixmap = QPixmap::fromImage(CardImageMap[name]);
	}
	else
	{
		m_pixmap = QPixmap::fromImage(CardImageMap["cb"]);
	}
	
	setPixmap(m_pixmap);
}

/**
 * Member function that sets card face down or up.
 * @param wantFacedown = true: set the card face down; wantFacedown = false: set the card face up
 * @return Pointer to the Card object
 */
Card *Card::setFacedown(bool wantFacedown)
{
	if(wantFacedown && !m_faceDown)
	{
		m_pixmap = QPixmap::fromImage(CardImageMap["cb"]);
		setPixmap(m_pixmap);
		m_faceDown = true;
	}
	if(!wantFacedown && m_faceDown)
	{
		m_pixmap = QPixmap::fromImage(CardImageMap[m_name]);
		setPixmap(m_pixmap);
		m_faceDown = false;
	}
	
	return this;
}


