#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void FileParser::loadLevel(const std::string& filename,
    std::vector<sf::Vector2f>& points,
    std::vector<Connection>& connections,
    std::vector<Chip>& chips,
    std::vector<int>& winCondition)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open level file: " + filename);
    }

    int chipCount, pointCount;
    file >> chipCount >> pointCount;

    points.resize(static_cast<size_t>(pointCount) + 1);
    for (int i = 1; i <= pointCount; ++i) {
        char comma;
        file >> points[static_cast<size_t>(i)].x >> comma >> points[static_cast<size_t>(i)].y;
    }

    std::vector<int> initialPositions(static_cast<size_t>(chipCount));
    for (int i = 0; i < chipCount; ++i) {
        char comma;
        file >> initialPositions[static_cast<size_t>(i)];
        if (i < chipCount - 1) file >> comma;
    }

    winCondition.resize(static_cast<size_t>(chipCount));
    for (int i = 0; i < chipCount; ++i) {
        char comma;
        file >> winCondition[static_cast<size_t>(i)];
        if (i < chipCount - 1) file >> comma;
    }

    static const sf::Color colors[] = {
        sf::Color::Red, sf::Color::Green, sf::Color::Blue,
        sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan
    };
    chips.clear();
    for (int i = 0; i < chipCount; ++i) {
        chips.emplace_back(i + 1, colors[i % 6], initialPositions[static_cast<size_t>(i)]);
    }

    int connectionCount;
    file >> connectionCount;
    connections.resize(static_cast<size_t>(connectionCount));
    for (int i = 0; i < connectionCount; ++i) {
        char comma;
        file >> connections[static_cast<size_t>(i)].point1 >> comma >> connections[static_cast<size_t>(i)].point2;
    }
}