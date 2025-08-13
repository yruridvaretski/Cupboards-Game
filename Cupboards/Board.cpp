#include "Board.h"
#include "FileParser.h"
#include <algorithm>
#include <cmath>

Board::Board() : selectedChip(-1) {}

void Board::loadFromFile(const std::string& filename) {
    FileParser::loadLevel(filename, points, connections, chips, winCondition);
    resetSelection();
}

void Board::update(float dt) {
    for (auto& chip : chips) {
        chip.update(dt);
    }
}

bool Board::isAnyChipMoving() const {
    for (const auto& chip : chips) {
        if (chip.isMoving()) {
            return true;
        }
    }
    return false;
}

bool Board::isConnected(int p1, int p2) const {
    for (const auto& conn : connections) {
        if ((conn.point1 == p1 && conn.point2 == p2) ||
            (conn.point1 == p2 && conn.point2 == p1)) {
            return true;
        }
    }
    return false;
}

std::vector<int> Board::findPath(int start, int end) const {
    return PathFinder::findPath(start, end, connections, chips);
}

bool Board::pathExists(int start, int end) const {
    return PathFinder::pathExists(start, end, connections, chips);
}

void Board::handleClick(const sf::Vector2f& mousePos) {
    if (isAnyChipMoving()) {
        return;
    }

    int pointIndex = findNearestPoint(mousePos);
    if (pointIndex == -1) return;

    for (size_t i = 0; i < chips.size(); ++i) {
        if (chips[i].getPosition() == pointIndex) {
            resetSelection();
            selectedChip = static_cast<int>(i);
            chips[i].setHighlight(true);
            return;
        }
    }

    if (selectedChip != -1) {
        int currentPos = chips[selectedChip].getPosition();
        auto path = findPath(currentPos, pointIndex);

        if (!path.empty()) {
            chips[selectedChip].setMovementPath(path, points);
        }

        resetSelection();
    }
}

void Board::draw(sf::RenderWindow& window) {
    sf::VertexArray lines(sf::Lines, connections.size() * 2);
    for (size_t i = 0; i < connections.size(); ++i) {
        lines[i * 2].position = points[connections[i].point1];
        lines[i * 2 + 1].position = points[connections[i].point2];
        lines[i * 2].color = sf::Color(100, 100, 100);
        lines[i * 2 + 1].color = sf::Color(100, 100, 100);
    }
    window.draw(lines);

    for (int i = 1; i < static_cast<int>(points.size()); ++i) {
        sf::CircleShape circle(8);
        circle.setPosition(points[i] - sf::Vector2f(8, 8));
        circle.setFillColor(sf::Color(70, 70, 70));
        window.draw(circle);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(i));
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(points[i] - sf::Vector2f(5, 10));
        window.draw(text);
    }

    for (auto& chip : chips) {
        chip.draw(window, points);
    }

    if (selectedChip != -1 && !isAnyChipMoving()) {
        int currentPos = chips[selectedChip].getPosition();
        for (int i = 1; i < static_cast<int>(points.size()); ++i) {
            if (i != currentPos && pathExists(currentPos, i)) {
                bool targetFree = true;
                for (const auto& chip : chips) {
                    if (chip.getPosition() == i) {
                        targetFree = false;
                        break;
                    }
                }

                if (targetFree) {
                    sf::CircleShape marker(15);
                    marker.setPosition(points[i] - sf::Vector2f(15, 15));
                    marker.setFillColor(sf::Color(0, 255, 0, 100));
                    window.draw(marker);
                }
            }
        }
    }
}

int Board::findNearestPoint(const sf::Vector2f& pos) const {
    float minDist = 25.f;
    int nearest = -1;
    for (int i = 1; i < static_cast<int>(points.size()); ++i) {
        float dist = std::hypot(pos.x - points[i].x, pos.y - points[i].y);
        if (dist < minDist) {
            minDist = dist;
            nearest = i;
        }
    }
    return nearest;
}

bool Board::checkWinCondition() const {
    for (size_t i = 0; i < chips.size(); ++i) {
        if (chips[i].getPosition() != winCondition[i])
            return false;
    }
    return true;
}

void Board::setFont(const sf::Font& font) {
    this->font = font;
}

void Board::resetSelection() {
    selectedChip = -1;
    for (auto& chip : chips) {
        chip.setHighlight(false);
    }
}