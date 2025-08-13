#pragma once
#include <vector>
#include "Connection.h"
#include "Chip.h"

class PathFinder {
public:
    static bool pathExists(int start, int end,
        const std::vector<Connection>& connections,
        const std::vector<Chip>& chips);

    static std::vector<int> findPath(int start, int end,
        const std::vector<Connection>& connections,
        const std::vector<Chip>& chips);
};