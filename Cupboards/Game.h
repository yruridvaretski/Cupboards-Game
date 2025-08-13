#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    Board board;
    sf::Font font;
    bool gameWon = false;
    sf::Clock winClock;
    int currentLevel = 1;
    const int MAX_LEVELS = 3;

    void handleEvents();
    void update(float dt);
    void render();
    void loadFont();
    void loadLevel();
    void showWinMessage();
    void resetGame();
    void restartLevel();
    void drawUI();
};