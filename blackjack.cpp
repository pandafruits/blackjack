#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTextEdit>
#include "blackjack.h"

/**
 * The Blackjack class constructor.
 * Everything is set up in the body of this function.
 */
Blackjack::Blackjack(QWidget *parent) : QMainWindow(parent)
{
	// Layout the UI and style them first
	setupUi();
	setStyle();
	// ... then initialize data
	resetData();
	readSettings();
	// ... and update the UI with the data.
	updateUi();
	
	// Now shuffle the card deck
	m_deck.shuffle();
	// ... and finally connect everything up.
	connect(newGameAct, SIGNAL(triggered()),
	        this, SLOT(resetGame()));
	connect(quitAct, SIGNAL(triggered()),
	        this, SLOT(close()));
	connect(aboutAct, SIGNAL(triggered()),
	        this, SLOT(about()));
	connect(ruleAct, SIGNAL(triggered()),
	        this, SLOT(rule()));
	        
	connect(&m_dealerHand, SIGNAL(handChanged()),
	        m_dealerHandView, SLOT(refresh()));
	connect(&m_playerHand, SIGNAL(handChanged()),
	        m_playerHandView, SLOT(refresh()));	
	          
	connect(m_betButtons, SIGNAL(buttonClicked(int)),
	        this, SLOT(updateBet(int)));
	connect(m_dealButton, SIGNAL(clicked()),
	        this, SLOT(deal()));
	connect(m_hitButton, SIGNAL(clicked()),
			this, SLOT(hit()));
	connect(m_stayButton, SIGNAL(clicked()),
			this, SLOT(dealerPlays()));	
}

/**
 * Member function to reset all non-ui data.
 * This function is called when game first starts and when user restarts 
 * the game.
 */
void Blackjack::resetData()
{
	m_cardsLeft = 52;
	m_currentBet = 0;
	m_balance = 1000;
	
	m_isBetting = true;
	m_mainInfo = QString("Dealer stands on all 17s");
	m_mainInfoStyleStr = QString("padding-left: 10px; font-weight: normal; color: #ffffff;");
}

/**
 * Member function that reads and loads settings from last game.
 */
void Blackjack::readSettings()
{
	QSettings settings("pandafruits", "blackjack");
	
	resize(settings.value("size", QSize(400, 400)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	m_balance = settings.value("balance", 1000).toInt();
}

/**
 * Member function that updates UI with the latest game data.
 * This function is called whenever the UI needs updating. It enables or 
 * disables various buttons as appropriate; it also updates various displays.
 */
void Blackjack::updateUi()
{
	// Game is in betting mode
	if(m_isBetting)
	{
		m_betFiveButton->setDisabled(false);
		m_betTwentyfiveButton->setDisabled(false);
		m_betFiftyButton->setDisabled(false);
		
		m_hitButton->setDisabled(true);
		m_stayButton->setDisabled(true);
		
		if(m_currentBet == 0)
		{
			m_clearBetBtn->setDisabled(true);
			m_dealButton->setDisabled(true);
		}
		else
		{
			m_clearBetBtn->setDisabled(false);
			m_dealButton->setDisabled(false);
		}
	}
	// Game is in playing mode
	else
	{
		m_clearBetBtn->setDisabled(true);
		m_betFiveButton->setDisabled(true);
		m_betTwentyfiveButton->setDisabled(true);
		m_betFiftyButton->setDisabled(true);
		m_dealButton->setDisabled(true);
		
		m_hitButton->setDisabled(false);
		m_stayButton->setDisabled(false);
	}
	
	m_cardsLeftDisp->setValue(m_cardsLeft);
	m_mainInfoLabel->setText(m_mainInfo);
	m_mainInfoLabel->setStyleSheet(m_mainInfoStyleStr);
	
	m_betDisp->display(m_currentBet);
	m_balanceDisp->display(m_balance);	
}

/**
 * Member function that resets the game.
 * This function is called when the user clicks "New Game", or when the 
 * user has no money left to continue the game.
 */
void Blackjack::resetGame()
{
	// Voluntary restart of a game (when player still has money but 
	// "New Game" is clicked)
	if(m_balance > 0 || m_currentBet > 0)
	{
		QMessageBox::StandardButton ans;
		
		ans = QMessageBox::warning(this, "New game?", 
		      "If you start a new game, all data will be lost.\n"
		      "Are you sure?", 
		      QMessageBox::Yes | QMessageBox::No);
		
		if(ans == QMessageBox::No)
		{
			return;
		}
	}

	// Work to restart a game
	m_dealerHand.clear();
	m_playerHand.clear();
	resetData();
	updateUi();
	m_deck.reset();
	m_deck.shuffle();
}

/**
 * Overloaded close event handler.
 * This function is called to save game settings before close.
 */
void Blackjack::closeEvent(QCloseEvent *event)
{
	if(userReallyWantsToQuit())
	{
		writeSettings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

/**
 * Member function that asks user whether or not sure to quit in the middle of a hand.
 * @return true: user wants to quit; false: user doesn't want to quit
 */
bool Blackjack::userReallyWantsToQuit()
{
	// In the middle of a hand - user loses the bet if quit
	if(m_hitButton->isEnabled())
	{
		QMessageBox::StandardButton ans;
		
		ans = QMessageBox::warning(this, "Quit?", 
		      "You are in the middle of a hand.\n"
		      "Quit now will make you automatically lose the current hand.\n"
		      "Are you sure to quit now?", 
		      QMessageBox::Yes | QMessageBox::No);
		
		if(ans == QMessageBox::Yes)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// Still betting - take back the bet
	else
	{
		m_balance += m_currentBet;
		return true;
	}
}

/**
 * Member function that saves game settings.
 */
void Blackjack::writeSettings()
{
	QSettings settings("pandafruits", "blackjack");
	
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.setValue("balance", m_balance);
}

/**
 * Member function that shows the About message box.
 */
void Blackjack::about()
{
	QMessageBox::about(this, "About Blackjack", 
	                   "By pandafruits.com\n"
	                   "v1.0");
}

/**
 * Member function that shows the game rule box.
 */
void Blackjack::rule()
{
	static bool ruleBoxAlreadyCreated = false;
	static QTextEdit *ruleBox = new QTextEdit(this);
	
	if(!ruleBoxAlreadyCreated)
	{	
		ruleBoxAlreadyCreated = true;
		
		ruleBox->setWindowFlags(Qt::Window);
		ruleBox->setWindowModality(Qt::WindowModal);
		ruleBox->setWindowTitle("Blackjack rules");
		ruleBox->move(pos().x()+150, pos().y());
		ruleBox->resize(400, 550);
		
		ruleBox->setReadOnly(true);
		ruleBox->setHtml("<h2>Rules of the game</h2>"
						 "<p>Dealer stands on all 17s. This means that regardless of the score \
						 player has, dealer must keep asking for more cards until the score is \
						 17 or above, at which time he must stop. Aces count as 11 whenever possible.</p>"
						 "<P><b>Example 1:</b></p>"
						 "<p><img src=\":/images/dealer_15.jpg\"></P>"
						 "<P>Dealer is 15 points. That's less than 17. He must take more cards even if \
						 player has busted.</p>"
						 "<P><b>Example 2:</b></p>"
						 "<p><img src=\":/images/dealer_18.jpg\"></P>"
						 "<P>Dealer is 18 points. That's more than 17. He must stay even if \
						 player has higher points. Also note ace here counts as 11, not 1.</p>");
	}

	ruleBox->show();
}

/**
 * Member function that updates the bet and balance.
 * This function is called when the user increases of clear the bet.
 * @param bet Bet value - 0 if the user clicks the clear bet button
 */
void Blackjack::updateBet(int bet)
{
	// The clear bet button is clicked
	if(bet == 0)
	{
		m_balance += m_currentBet;
		m_currentBet = 0;
	}
	// Other bet value buttons are clicked
	else
	{	
		if(m_balance < bet) return;

		m_currentBet += bet;
		m_balance -= bet;
	}
	
	updateUi();
}

/**
 * Member function that deals a hand to dealer and player.
 * This function is called when the user clicks the deal button.
 */
void Blackjack::deal()
{
	// Switch off betting mode
	m_isBetting = false;
	
	// Clear both hands first if necessary
	if(m_dealerHand.numCards() != 0)
	{
		m_dealerHand.clear();
		m_playerHand.clear();
	}
	
	// Reset the deck if there is less than 10 cards left in the deck
	if(m_deck.cardsLeft() < 10)
	{
		m_deck.reset();
		m_deck.shuffle();
		m_cardsLeft = 52;
	}
	
	// Deal 2 cards to dealer and player
	// Keep the second card of player face down
	m_dealerHand << m_deck.deal() << m_deck.deal()->setFacedown(true);
	m_playerHand << m_deck.deal() << m_deck.deal();
	
	// Update game data
	m_cardsLeft -= 4;
	m_mainInfo = QString("Dealer stands on all 17s");
	m_mainInfoStyleStr = QString("padding-left: 10px; font-weight: normal; color: #ffffff;");
	
	updateUi();
}

/**
 * Member function that deals one more card to the user's hand.
 * This function is called when the user clicks the hit button.
 */
void Blackjack::hit()
{
	m_playerHand << m_deck.deal();
	--m_cardsLeft;
	
	if(m_playerHand.busted())
	{
		dealerPlays();
	}
	else
	{
		updateUi();
	}
}

/**
 * Member function that drives the dealer's action.
 * This function is called when the user clicks the stay button, or 
 * when the user's hand has busted.
 */
void Blackjack::dealerPlays()
{
	// Show dealer's second card that was previously hidden
	m_dealerHand.cards()[1]->setFacedown(false);
	
	// Keep dealing until 17 points or more
	while(m_dealerHand.score() < 17)
	{
		m_dealerHand << m_deck.deal();
		--m_cardsLeft;
	}
	
	// Updates game data and UI according to the results
	// of the hand counting
	countHands();
	m_isBetting = true;
	updateUi();
	
	// Force a new game when the user has lost all money
	if(m_currentBet == 0)
	{
		QMessageBox::information(this, "You're bankrupt!",
		                         "The casino has advanced you some more money to keep you going!");
		resetGame();
	}
}

/**
 * Member function that counts the hands and updates game data accordingly.
 * This function is called after dealer has finished his play.
 */
void Blackjack::countHands()
{
	bool playerWins = false;
	bool dealerWins = false;
	
	// Determine who wins
	if(m_playerHand.busted())
	{
		if(!m_dealerHand.busted())
			dealerWins = true;
	}
	else if(m_playerHand.isBlackjack())
	{
		if(!m_dealerHand.isBlackjack())
			playerWins = true;
	}
	else if(m_dealerHand.busted())
	{
		playerWins = true;
	}
	else if(m_dealerHand.isBlackjack())
	{
		dealerWins = true;
	}
	else if(m_playerHand.score() > m_dealerHand.score())
	{
		playerWins = true;
	}	
	else if(m_playerHand.score() < m_dealerHand.score())
	{
		dealerWins = true;
	}
	
	// Update game data depending on who wins
	if(playerWins)
	{
		m_balance += m_currentBet;
		m_mainInfo = QString("You won.");
		m_mainInfoStyleStr = QString("padding-left: 10px;\
		                              font-weight: bold;\
		                              color: #89c403;");
	}
	else if(dealerWins)
	{
		if(m_balance == 0)
		{
			m_currentBet = 0;
		}
		else if(m_balance < m_currentBet)
		{
			m_currentBet = m_balance;
			m_balance = 0;
		}
		else
		{
			m_balance -= m_currentBet;
		}
		m_mainInfo = QString("You lost.");
		m_mainInfoStyleStr = QString("padding-left: 10px;\
		                              font-weight: bold;\
		                              color: #f24537;");
	}
	else
	{
		m_mainInfo = QString("Draw");
		m_mainInfoStyleStr = QString("padding-left: 10px;\
		                              font-weight: bold;\
		                              color: #3d94f6;");
	}
}

/**
 * Member function that lays out all widgets of the game.
 * This function only lays out the widgets, it does not involve any 
 * game data manipulation.
 */
void Blackjack::setupUi()
{	
	///////////////////////////////////////////////////////////////////////////
	// Set up window title, menu, toolbar and status bar 
	///////////////////////////////////////////////////////////////////////////
	setWindowTitle("Blackjack");
	
	gameMenu = menuBar()->addMenu("&Game");
	helpMenu = menuBar()->addMenu("&Help");
	
	newGameAct = gameMenu->addAction(QIcon(":/images/new.png"), "&New Game");
	newGameAct->setStatusTip("Start a new game");
	quitAct = gameMenu->addAction(QIcon(":/images/quit.png"), "&Quit");
	quitAct->setStatusTip("Quit the game");
	ruleAct = helpMenu->addAction("&Rule");
	ruleAct->setStatusTip("Show rules of the game");
	aboutAct = helpMenu->addAction("&About Blackjack");
	aboutAct->setStatusTip("Show About dialog of Blackjack");
	
	toolBar = addToolBar("Game Control");
	toolBar->addAction(newGameAct);
	toolBar->addAction(quitAct);
	
	statusBar()->showMessage("Ready");
	 	
	////////////////////////////////////////////////////////////////////////////
	// Layout central widget UI
	////////////////////////////////////////////////////////////////////////////
	
	m_centralWidget = new QWidget(this);
	m_centralWidget->setObjectName("CentralWidget");
	setCentralWidget(m_centralWidget);
	m_centralLayout = new QVBoxLayout(m_centralWidget);
	
	// Dealer's cards display
	m_dealerCardsGroup = new QGroupBox("Dealer", m_centralWidget);
	m_dealerCardsGroup->setFixedHeight(120);
	m_dealerHandView = new HandView(&m_dealerHand, m_dealerCardsGroup);
	m_centralLayout->addWidget(m_dealerCardsGroup);
	// end dealer's cards display
	
	// Main info area
	m_infoGroup = new QGroupBox(m_centralWidget);
	QHBoxLayout *infoLayout = new QHBoxLayout();
	infoLayout->setContentsMargins(0, 0, 0, 0);
	infoLayout->setAlignment(Qt::AlignLeft);
	m_cardsLeftLabel = new QLabel("Cards Left", m_infoGroup);
	m_cardsLeftDisp = new QSpinBox(m_infoGroup);
	m_cardsLeftDisp->setDisabled(true);
	m_cardsLeftLabel->setBuddy(m_cardsLeftDisp);
	m_mainInfoLabel = new QLabel(m_infoGroup);
	infoLayout->addWidget(m_cardsLeftLabel);
	infoLayout->addWidget(m_cardsLeftDisp);
	infoLayout->addWidget(m_mainInfoLabel);
	m_infoGroup->setLayout(infoLayout);
	m_centralLayout->addWidget(m_infoGroup);
	// end main info area

	// Player's cards display
	m_playerCardsGroup = new QGroupBox("Player", m_centralWidget);
	m_playerCardsGroup->setFixedHeight(120);
	m_playerHandView = new HandView(&m_playerHand, m_playerCardsGroup);
	m_centralLayout->addWidget(m_playerCardsGroup);
	// end player's cards display
	
	// + Control area
	QHBoxLayout *ctrlAreaLayout = new QHBoxLayout();
	//   - Bet area
	m_betGroup = new QGroupBox(m_centralWidget);
	QVBoxLayout *betGroupLayout = new QVBoxLayout();
	
	m_betLabel = new QLabel("Bet", m_betGroup);
	m_betDisp = new QLCDNumber(4, m_betGroup);
	m_betDisp->setFixedSize(50, 25);
	m_betLabel->setBuddy(m_betDisp);
	m_clearBetBtn = new QPushButton(QIcon(":/images/clearbet.png"), "", m_betGroup); // Really bet 0 button
	m_clearBetBtn->setStatusTip("Clear current bet");
	m_clearBetBtn->setFlat(true);
	m_clearBetBtn->setFixedSize(25, 25);
	m_clearBetBtn->setIconSize(QSize(25, 25));
	QHBoxLayout *betInfoLayout = new QHBoxLayout();
	betInfoLayout->setAlignment(Qt::AlignLeft);
	betInfoLayout->addWidget(m_betLabel);
	betInfoLayout->addWidget(m_betDisp);
	betInfoLayout->addWidget(m_clearBetBtn);
	betGroupLayout->addLayout(betInfoLayout);
	
	m_betButtons = new QButtonGroup(this);
	m_betFiveButton = new QPushButton(QIcon(":/images/chip5.png"), "", m_betGroup);
	m_betFiveButton->setStatusTip("Bet $5");
	m_betFiveButton->setFlat(true);
	m_betFiveButton->setFixedSize(40, 40);
	m_betFiveButton->setIconSize(QSize(40, 40));
	m_betTwentyfiveButton = new QPushButton(QIcon(":/images/chip25.png"), "", m_betGroup);
	m_betTwentyfiveButton->setStatusTip("Bet $25");
	m_betTwentyfiveButton->setFlat(true);
	m_betTwentyfiveButton->setFixedSize(40, 40);
	m_betTwentyfiveButton->setIconSize(QSize(40, 40));
	m_betFiftyButton = new QPushButton(QIcon(":/images/chip50.png"), "", m_betGroup);
	m_betFiftyButton->setStatusTip("Bet $50");
	m_betFiftyButton->setFlat(true);
	m_betFiftyButton->setFixedSize(40, 40);
	m_betFiftyButton->setIconSize(QSize(40, 40));
	m_betButtons->addButton(m_clearBetBtn, 0); // Clear bet button as bet 0
	m_betButtons->addButton(m_betFiveButton, 5);
	m_betButtons->addButton(m_betTwentyfiveButton, 25);
	m_betButtons->addButton(m_betFiftyButton, 50);
	QHBoxLayout *betBtnLayout = new QHBoxLayout();
	betBtnLayout->setAlignment(Qt::AlignLeft);
	betBtnLayout->addWidget(m_betFiveButton);
	betBtnLayout->addWidget(m_betTwentyfiveButton);
	betBtnLayout->addWidget(m_betFiftyButton);
	betGroupLayout->addLayout(betBtnLayout);
	
	m_balanceLabel = new QLabel("Balance", m_betGroup);
	m_balanceDisp = new QLCDNumber(4, m_betGroup);
	m_balanceDisp->setFixedSize(50, 25);
	m_balanceLabel->setBuddy(m_balanceDisp);
	QHBoxLayout *balInfoLayout = new QHBoxLayout();
	balInfoLayout->setAlignment(Qt::AlignLeft);
	balInfoLayout->addWidget(m_balanceLabel);
	balInfoLayout->addWidget(m_balanceDisp);
	betGroupLayout->addLayout(balInfoLayout);
	
	m_betGroup->setLayout(betGroupLayout);
	ctrlAreaLayout->addWidget(m_betGroup);
	//   - end bet area
	
	//   - Play area
	m_playGroup = new QGroupBox(m_centralWidget);
	QVBoxLayout *playGroupLayout = new QVBoxLayout();
	m_dealButton = new QPushButton("Deal", m_playGroup);
	m_dealButton->setStatusTip("Deal a new hand");
	m_dealButton->setFixedSize(120, 30);
	playGroupLayout->addWidget(m_dealButton);
	
	m_hitButton = new QPushButton("Hit", m_playGroup);
	m_hitButton->setStatusTip("Hit me");
	m_hitButton->setFixedSize(58, 30);
	m_stayButton = new QPushButton("Stay", m_playGroup);
	m_stayButton->setStatusTip("I'm staying");
	m_stayButton->setFixedSize(58, 30);	
	QHBoxLayout *decisionAreaLayout = new QHBoxLayout();
	decisionAreaLayout->setAlignment(Qt::AlignLeft);
	decisionAreaLayout->addWidget(m_hitButton);
	decisionAreaLayout->addWidget(m_stayButton);
	playGroupLayout->addLayout(decisionAreaLayout);
	
	m_playGroup->setLayout(playGroupLayout);
	ctrlAreaLayout->addWidget(m_playGroup);
	//   - end play area
	
	m_centralLayout->addLayout(ctrlAreaLayout);
	// + end control area
}

/**
 * Member function that sets the style of various UI widgets.
 */
void Blackjack::setStyle()
{
	// Game background and default font color
	setStyleSheet("#CentralWidget {background-image: url(:/images/bg.jpg);}");
	m_centralWidget->setStyleSheet("color: white;");
	
	// Dealer cards area
	m_dealerCardsGroup->setStyleSheet("padding-left: 3px; \
	                                   padding-top: 3px;\
	                                   font-weight: bold;");
	
	// Main information area in the middle                                   
	m_cardsLeftDisp->setStyleSheet("color: #000000;");
	m_mainInfoLabel->setStyleSheet("padding-left: 10px;");
	
	// Player cards area                           
	m_playerCardsGroup->setStyleSheet("padding-left: 3px; \
	                                   padding-top: 3px;\
	                                   font-weight: bold;");
	
	// The LCD displays                                   
	m_betGroup->setStyleSheet("QLCDNumber {border-color: white;\
	                                       border-style: solid; \
	                                       border-width: 1px; \
	                                       color: red; \
	                                       background-color: black;}");
    
    // The deal button                                  
	m_dealButton->setStyleSheet("QPushButton {font-weight: bold;\
											  border-radius:6px;\
	                                          color: #ffffff;\
	                                          background-color: #3d94f6;}\
	                             QPushButton:disabled {color: #767373;\
	                                                   background-color: #A9A5A5;}\
	                             QPushButton:hover {background-color: #1e62d0;}");                                                              

	// The hit button
	m_hitButton->setStyleSheet("QPushButton {font-weight: bold;\
											  border-radius:6px;\
	                                          color: #ffffff;\
	                                          background-color: #89c403;}\
	                             QPushButton:disabled {color: #767373;\
	                                                   background-color: #A9A5A5;}\
	                             QPushButton:hover {background-color: #77a809;}"); 
	
	// The stay button                             
	m_stayButton->setStyleSheet("QPushButton {font-weight: bold;\
											  border-radius:6px;\
	                                          color: #ffffff;\
	                                          background-color: #f24537;}\
	                             QPushButton:disabled {color: #767373;\
	                                                   background-color: #A9A5A5;}\
	                             QPushButton:hover {background-color: #c62d1f;}"); 
}
