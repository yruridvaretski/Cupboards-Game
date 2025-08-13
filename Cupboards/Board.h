#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Chip.h"
#include "Connection.h"
#include "PathFinder.h"

class Board {
public:
    Board();
    void loadFromFile(const std::string& filename);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);
    bool checkWinCondition() const;
    void setFont(const sf::Font& font);
    bool isAnyChipMoving() const;

private:
    std::vector<sf::Vector2f> points;
    std::vector<Connection> connections;
    std::vector<Chip> chips;
    std::vector<int> winCondition;
    int selectedChip = -1;
    sf::Font font;

    std::vector<int> findPath(int start, int end) const;
    bool pathExists(int start, int end) const;
    bool isConnected(int p1, int p2) const;
    int findNearestPoint(const sf::Vector2f& pos) const;
    void resetSelection();
};