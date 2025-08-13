#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Connection.h"
#include "Chip.h"

class FileParser {
public:
    static void loadLevel(const std::string& filename,
        std::vector<sf::Vector2f>& points,
        std::vector<Connection>& connections,
        std::vector<Chip>& chips,
        std::vector<int>& winCondition);
};