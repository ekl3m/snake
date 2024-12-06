#include "snake.h"
#include "screen.h"

#define MAX_SCORE 50   // change this value so as to meet your requirements, max 99
#define START_SPEED 60 // change this value so as to meet your requirements

// Set default values

CSnake::CSnake(CRect r, char _c) : CFramedWindow(r, _c) {
	// ints
	score = 0;
    direction = 2;
    currMove = 0;
    moveCounter = 0;
	
	// bools
	isLost = false;
    isWon = false;
    isPaused = true;
    isMenu = true;
    isReset = false;
	
	// functions
    setHead({geom.topleft.x + geom.size.x / 2, geom.topleft.y + geom.size.y / 2});
    generateFood();
}

// Display functions

void CSnake::paint() {
    CFramedWindow::paint();
    displayInfoWindow();
	displayMenu();

	if (isReset) {
		clearScreen();
		resetGame();
	}

	if (isPaused) {
		displayPause();
		displayFood();
		displaySnake();
		displayScore();
		return;
	}

	displayFood();
	displaySnake();
	displayScore();

	if (isWon) {
		clearScreen();
		displayWon();
		return;
	}

	if (isLost) {
		clearScreen();
		displayLost();
		return;
	}

	displayPause();
}

void CSnake::displayMenu() {
    if (!isMenu) {
        undisplayMenu();
        return;
    }

    for (int i = 0; i < 43 /*MENU WIDTH*/; i++) {
        gotoyx(geom.topleft.y + geom.size.y, geom.topleft.x + i + 1);
        printl("-");
    }

    for (int i = 0; i < 43 /*MENU WIDTH*/; i++) {
        gotoyx(geom.topleft.y + geom.size.y + 8 /*MENU HEIGHT*/, geom.topleft.x + i + 1);
        printl("-");
    }

    for (int i = 0; i < 8 /*MENU HEIGHT*/; i++) {
        gotoyx(geom.topleft.y + geom.size.y + i, geom.topleft.x + geom.size.x - 1);
        printl("|");
    }
    
    gotoyx(geom.topleft.y + geom.size.y + 8, geom.topleft.x);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y, geom.topleft.x);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y, geom.topleft.x + geom.size.x - 1);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y + 8 /*MENU HEIGHT*/, geom.topleft.x + geom.size.x - 1);
    printl("+");

    gotoyx(geom.topleft.y + geom.size.y + 1, geom.topleft.x);
    printl("| h - toggle help information               ");
    gotoyx(geom.topleft.y + geom.size.y + 2, geom.topleft.x);
    printl("| p - toggle pause/play mode                ");
    gotoyx(geom.topleft.y + geom.size.y + 3, geom.topleft.x);
    printl("| r - restart game                          ");
    gotoyx(geom.topleft.y + geom.size.y + 4, geom.topleft.x);
    printl("| q - quit                                  ");
    gotoyx(geom.topleft.y + geom.size.y + 5, geom.topleft.x);
    printl("| arrows - move snake (in play mode)        ");
    gotoyx(geom.topleft.y + geom.size.y + 6, geom.topleft.x);
    printl("|          or                               ");
    gotoyx(geom.topleft.y + geom.size.y + 7, geom.topleft.x);
    printl("|          move window (in pause mode)      ");
}

void CSnake::displayScore() {
    gotoyx(geom.topleft.y - 1, geom.topleft.x + 1);
    printl("| SCORE: %2d | | OBJECTIVE: %2d| | LEFT: %2d |", score, MAX_SCORE, MAX_SCORE - score);
}

void CSnake::displayLost() {
    gotoyx(geom.topleft.y + geom.size.y / 2, geom.topleft.x + geom.size.x / 2 - 5);
    printl("| GAME OVER |");
    gotoyx(geom.topleft.y + geom.size.y / 2 + 1, geom.topleft.x + geom.size.x / 2 - 11);
    printl("| YOU REACHED SCORE: %d |", score);
    gotoyx(geom.topleft.y + geom.size.y / 2 + 2, geom.topleft.x + geom.size.x / 2 - 11);
    printl("| PRESS R TO PLAY AGAIN |");

    string defeat = "YOU LOST";
    for (int i = 0; i < (int)defeat.length(); i++) {
        gotoyx(geom.topleft.y + geom.size.y / 2 - defeat.length() / 2 + i, geom.topleft.x - 3);
        printl("%c", defeat[i]);
    }
}

void CSnake::displayWon() {
    gotoyx(geom.topleft.y + geom.size.y / 2, geom.topleft.x + geom.size.x / 2 - 5);
    printl("| YOU WON |");
    gotoyx(geom.topleft.y + geom.size.y / 2 + 1, geom.topleft.x + geom.size.x / 2 - 12);
    printl("| YOU REACHED SCORE: %d |", score);
    gotoyx(geom.topleft.y + geom.size.y / 2 + 2, geom.topleft.x + geom.size.x / 2 - 12);
    printl("| PRESS R TO PLAY AGAIN |");

    string win = "!!! GAME WON !!!";
    for (int i = 0; i < (int)win.length(); i++) {
        gotoyx(geom.topleft.y + geom.size.y / 2 - win.length() / 2 + i, geom.topleft.x - 3);
        printl("%c", win[i]);
    }
}

void CSnake::displaySnake() {
    for (auto &it: body) {
        gotoyx(it.y, it.x);
        if (it.x == head.x && it.y == head.y) printl("x"); else printl("o");
	}
}

inline void CSnake::displayFood() {
    gotoyx(food.y, food.x);
    printl("*");
}

void CSnake::displayInfoWindow() {
    for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++) {
        gotoyx(y, geom.topleft.x - 2);
        printl("|");
        gotoyx(y, geom.topleft.x - 3);
        printl(" ");
        gotoyx(y, geom.topleft.x - 4);
        printl("|");
    }
    gotoyx(geom.topleft.y + geom.size.y - 1, geom.topleft.x - 3);
    printl("-");
    gotoyx(geom.topleft.y + geom.size.y - 1, geom.topleft.x - 4);
    printl("+");
    gotoyx(geom.topleft.y, geom.topleft.x - 3);
    printl("-");
    gotoyx(geom.topleft.y, geom.topleft.x - 4);
    printl("+");
    gotoyx(geom.topleft.y + geom.size.y - 1, geom.topleft.x - 2);
    printl("+");
    gotoyx(geom.topleft.y, geom.topleft.x - 2);
    printl("+");
}

void CSnake::displayPause() {
    string pause = "PAUSED";
    if (!isPaused) pause = "RUNNING";
    
    for (unsigned long int i = 0; i < pause.length(); i++) {
        gotoyx(geom.topleft.y + geom.size.y / 2 - pause.length() / 2 + i, geom.topleft.x - 3);
        printl("%c", pause[i]);
    }
}

void CSnake::undisplayMenu() {
    for (int y = geom.topleft.y + geom.size.y + 1; y < geom.topleft.y + geom.size.y + 6; y++)
        for (int x = geom.topleft.x; x < geom.topleft.x + geom.size.x + 1; x++) {
            gotoyx(y, x);
            printl(".");
        }
}

void CSnake::clearScreen() {
    for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++)
        for (int x = geom.topleft.x + 1; x < geom.topleft.x + geom.size.x - 1; x++) {
            gotoyx(y, x);
            printl(" ");
        }
}

// Game parameters

void CSnake::resetGame() {
	// ints
    score = 0;
    direction = 2;
    moveCounter = 0;
    
    
   // bools
    isLost = false;
    isWon = false;
    isPaused = true;
    isReset = false;
    isMenu = true;
   
   // functions
    setHead({geom.topleft.x + geom.size.x / 2, geom.topleft.y + geom.size.y / 2});
    body.clear();
    body.push_back(head);
    generateFood();
    displayMenu();
}

void CSnake::move(const CPoint &delta) {
    CView::move(delta);
    for (auto &it: body) it += delta;
    food += delta;
    head += delta;
}

inline void CSnake::setHead(CPoint h) {
    head = h;
    body.push_back(head);
}

CPoint CSnake::generateFood() {
    while (true) {
        food.x = rand() % (geom.size.x - 2) + geom.topleft.x + 1;
        food.y = rand() % (geom.size.y - 2) + geom.topleft.y + 1;
        
        bool isOnSnake = false;
        for (auto &it: body) {
            if (it.x == food.x && it.y == food.y) {
                isOnSnake = true;
                break;
            }
        }
        if (!isOnSnake) break;
    }
    
    return {food.x, food.y};
}

bool CSnake::checkHitBody() {
    for (auto it = body.begin(); it != body.end(); it++) {
        if (it->x == head.x && it->y == head.y && it != body.begin()) {
            isLost = true;
            return true;
        }
    }
    return false;
}

bool CSnake::checkWin() {
    if (score == MAX_SCORE) {
        isWon = true;
        return true;
    }
    return false;
}

void CSnake::changeMenuVisibility(bool isMenuTmp) {
	isMenu = isMenuTmp ? false : true;
}

// Handle events

void CSnake::handleEatFood() {
    if (head.x == food.x && head.y == food.y) {
        generateFood();
        score++;
    } else {
        body.erase(body.begin());
    }
}

bool CSnake::handleMove() {
    moveCounter++;
    if (moveCounter > START_SPEED / (score + 1)) {
        moveCounter = 0;
        return true;
    }
    return false;
}

void CSnake::handleWall() {
    if (head.x == geom.topleft.x + geom.size.x - 1) head.x = geom.topleft.x + 1;
    if (head.x == geom.topleft.x) head.x = geom.topleft.x + geom.size.x - 2;
    if (head.y == geom.topleft.y + geom.size.y - 1) head.y = geom.topleft.y + 1;
    if (head.y == geom.topleft.y) head.y = geom.topleft.y + geom.size.y - 2;
}

bool CSnake::handleEvent(int c) {
    currMove = c;
    if (isPaused || isLost || isWon) {
        if (CFramedWindow::handleEvent(c)) return true;
        switch (c) {
            case 'p':
                if (isLost || isWon) return true;
                if (isPaused) isPaused = false; else isPaused = true;
                isMenu = true;
                break;
            case 'r':
                isReset = true;
            case 'h':
                changeMenuVisibility(isMenu);
                break;
            default:
                return false;
        }
        return true;
    }

    switch (c) {
        case KEY_UP:
            if (direction != 2) direction = 1; else return false;
            break;
        case KEY_DOWN:
            if (direction != 1) direction = 2; else return false;
            break;
        case KEY_RIGHT:
            if (direction != 4) direction = 3; else return false;
            break;
        case KEY_LEFT:
            if (direction != 3) direction = 4; else return false;
            break;
        case 'h':
            changeMenuVisibility(isMenu);
            return true;
        case 'p':
            isMenu = true;
            isPaused = true;
            return true;
        case 'r':
            isReset = true;
            return true;
        case ERR:
            if (!handleMove()) return false;
            break;
        default:
            break;
    }
    switch (direction) {
        case 1:
            head.y--;
            break;
        case 2:
            head.y++;
            break;
        case 3:
            head.x++;
            break;
        case 4:
            head.x--;
            break;
    }

    if (checkHitBody()) return true;
    handleWall();
    body.push_back(head);
    handleEatFood();
    checkWin();
    return true;
}
