#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Chip {
public:
    Chip(int id, const sf::Color& color, int position);
    void update(float dt);
    void draw(sf::RenderWindow& window, const std::vector<sf::Vector2f>& points);
    void setTargetPosition(int pos);
    void setMovementPath(const std::vector<int>& path, const std::vector<sf::Vector2f>& points);
    int getPosition() const;
    void setHighlight(bool highlight);
    bool isMoving() const;

private:
    int id;
    sf::Color color;
    int currentPosition;
    int targetPosition;
    std::vector<sf::Vector2f> movementPath;
    size_t currentPathIndex;
    float moveProgress;
    bool isHighlighted;
};