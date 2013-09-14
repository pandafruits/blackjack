#ifndef HANDVIEW_H
#define HANDVIEW_H

#include <QHBoxLayout>
#include "hand.h"

/**
 * Class that represents view of a hand.
 */
class HandView : public QWidget
{
	Q_OBJECT
	
public:
	HandView(Hand *hand, QWidget *parent = 0);
		
public slots:
	void refresh();

private:
	Hand *m_hand;
	int m_numCards;
};

#endif
