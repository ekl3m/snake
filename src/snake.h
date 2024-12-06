#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "vector"

class CSnake : public CFramedWindow {
private:
// ints
    int score;
    int direction;
    int currMove;
    int moveCounter;

// bools 
    bool isLost;
    bool isWon;
    bool isPaused;
    bool isMenu;
    bool isReset;
    
// objects
    CPoint head;
    vector<CPoint> body;
    CPoint food;

public:
// Set default values
    CSnake(CRect r, char _c = ' ');

// Display functions
    void paint();
    void displayMenu();
    void displayScore();
    void displayLost();
    void displayWon();
    void displaySnake();
    inline void displayFood();
    void displayInfoWindow();
    void displayPause();
    void undisplayMenu();
    void clearScreen();

// Game parameters
	void resetGame();
	void move(const CPoint &delta);
	inline void setHead(CPoint h);
	CPoint generateFood();
	bool checkHitBody();
	bool checkWin();
    void changeMenuVisibility(bool isHelp);

// Handle events
	void handleEatFood();
	bool handleMove();
    void handleWall();
    bool handleEvent(int c);
};


#endif
