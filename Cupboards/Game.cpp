#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(400, 400), "Cupboards") {
    loadFont();
    board.setFont(font);
    loadLevel();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (!gameWon && event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left && !board.isAnyChipMoving()) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.y });
            board.handleClick(mousePos);
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R && !board.isAnyChipMoving()) {
                restartLevel();
            }
        }

        if (gameWon && event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            resetGame();
        }
    }
}

void Game::update(float dt) {
    if (!gameWon && board.checkWinCondition()) {
        gameWon = true;
        winClock.restart();
    }
    board.update(dt);
}

void Game::render() {
    window.clear(sf::Color(30, 30, 40));
    board.draw(window);
    drawUI();

    if (gameWon) {
        showWinMessage();
    }

    window.display();
}

void Game::showWinMessage() {
    sf::Text winText;
    winText.setFont(font);
    winText.setString(currentLevel < MAX_LEVELS ?
        "Level Complete!\nClick to continue" :
        "You Win All Levels!\nClick to restart");
    winText.setCharacterSize(20);
    winText.setFillColor(sf::Color::Green);
    winText.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = winText.getLocalBounds();
    winText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    winText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    float pulse = std::sin(winClock.getElapsedTime().asSeconds() * 3.0f) * 0.1f + 1.0f;
    winText.setScale(pulse, pulse);

    window.draw(winText);
}

void Game::loadLevel() {
    try {
        board.loadFromFile("level" + std::to_string(currentLevel) + ".txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading level: " << e.what() << std::endl;
        currentLevel = 1;
        try {
            board.loadFromFile("level1.txt");
        }
        catch (...) {
            std::cerr << "Critical error: cannot load initial level" << std::endl;
            window.close();
        }
    }
}

void Game::loadFont() {
    if (!font.loadFromFile("arial.ttf")) {
        if (!font.loadFromFile("C:/Windows/Fonts/Arial.ttf")) {
            sf::Font defaultFont;
            if (!defaultFont.loadFromMemory(nullptr, 0)) {
                std::cerr << "Failed to create default font" << std::endl;
            }
            font = defaultFont;
        }
    }
}

void Game::resetGame() {
    gameWon = false;
    currentLevel = (currentLevel < MAX_LEVELS) ? currentLevel + 1 : 1;
    loadLevel();
}

void Game::restartLevel() {
    loadLevel();
    gameWon = false;
}

void Game::drawUI() {
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Press R to restart");
    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(10, 10);
    window.draw(restartText);
}