#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <QMainWindow>
#include <QToolBar>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QLCDNumber>
#include <QButtonGroup>
#include <QPushButton>
#include "card.h"
#include "deck.h"
#include "hand.h"
#include "handview.h"

/**
 * Class that represents a Blackjack game.
 * The class contains all UI and logic of the Blackjack game.
 */
class Blackjack : public QMainWindow
{
	Q_OBJECT

public:
	Blackjack(QWidget *parent = 0);
	
protected:
	void closeEvent(QCloseEvent *event);	
    
private slots:
	void updateBet(int bet);
	void deal();
	void hit();
	void dealerPlays();
	void resetGame();
	void about();
	void rule();
	
private:
	void resetData();
	void setupUi();
	void setStyle();
	void updateUi();
	void countHands();
	void readSettings();
	void writeSettings();
	bool userReallyWantsToQuit();
	
private:
	// UI member data
	QAction *newGameAct;
	QAction *quitAct;
	QAction *ruleAct;
	QAction *aboutAct;
	QMenu *gameMenu;
	QMenu *helpMenu;
	QToolBar *toolBar;

	QWidget *m_centralWidget;
	QVBoxLayout *m_centralLayout;
	
	QGroupBox *m_dealerCardsGroup;
	HandView *m_dealerHandView;
	
	QGroupBox *m_infoGroup;
	QLabel *m_cardsLeftLabel;
	QSpinBox *m_cardsLeftDisp;
	QLabel *m_mainInfoLabel;
	
	QGroupBox *m_playerCardsGroup;
	HandView *m_playerHandView;
	
	QGroupBox *m_betGroup;
	QLabel *m_betLabel;
	QLCDNumber *m_betDisp;
	QPushButton *m_clearBetBtn;
	QButtonGroup *m_betButtons;
	QPushButton *m_betFiveButton;
	QPushButton *m_betTwentyfiveButton;
	QPushButton *m_betFiftyButton;
	QLabel *m_balanceLabel;
	QLCDNumber *m_balanceDisp;
	
	QGroupBox *m_playGroup;
	QPushButton *m_dealButton;
	QPushButton *m_hitButton;
	QPushButton *m_stayButton;
	// end UI member data
	
	Deck m_deck;
	Hand m_dealerHand;
	Hand m_playerHand;
	
	int m_cardsLeft;
	int m_currentBet;
	int m_balance;
	QString m_mainInfo;
	QString m_mainInfoStyleStr;
	
	bool m_isBetting;
};


#endif
