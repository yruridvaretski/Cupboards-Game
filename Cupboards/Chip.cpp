#include "Chip.h"
#include <algorithm>

Chip::Chip(int id, const sf::Color& color, int position)
    : id(id), color(color), currentPosition(position),
    targetPosition(position), currentPathIndex(0),
    moveProgress(0.f), isHighlighted(false) {}

void Chip::update(float dt) {
    if (!movementPath.empty() && currentPathIndex < movementPath.size() - 1) {
        moveProgress += dt * 2.f;
        if (moveProgress >= 1.f) {
            moveProgress = 0.f;
            currentPathIndex++;
            if (currentPathIndex == movementPath.size() - 1) {
                currentPosition = targetPosition;
                movementPath.clear();
            }
        }
    }
}

void Chip::draw(sf::RenderWindow& window, const std::vector<sf::Vector2f>& points) {
    sf::Vector2f pos;
    if (!movementPath.empty() && currentPathIndex < movementPath.size() - 1) {
        const auto& p1 = movementPath[currentPathIndex];
        const auto& p2 = movementPath[currentPathIndex + 1];
        pos = p1 * (1.f - moveProgress) + p2 * moveProgress;
    }
    else {
        pos = points[currentPosition];
    }

    sf::CircleShape shape(20);
    shape.setPosition(pos - sf::Vector2f(20, 20));
    shape.setFillColor(color);
    shape.setOutlineThickness(isHighlighted ? 3.f : 0.f);
    shape.setOutlineColor(sf::Color::White);
    window.draw(shape);

    sf::Text text;
    text.setString(std::to_string(id));
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setPosition(pos - sf::Vector2f(5, 10));
    window.draw(text);
}

void Chip::setTargetPosition(int pos) {
    targetPosition = pos;
}

void Chip::setMovementPath(const std::vector<int>& path, const std::vector<sf::Vector2f>& points) {
    movementPath.clear();
    for (int pointIdx : path) {
        movementPath.push_back(points[pointIdx]);
    }
    currentPathIndex = 0;
    moveProgress = 0.f;
    targetPosition = path.back();
}

int Chip::getPosition() const {
    return currentPosition;
}

void Chip::setHighlight(bool highlight) {
    isHighlighted = highlight;
}

bool Chip::isMoving() const {
    return !movementPath.empty() && currentPathIndex < movementPath.size() - 1;
}