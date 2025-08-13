#include "PathFinder.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>

bool PathFinder::pathExists(int start, int end,
    const std::vector<Connection>& connections,
    const std::vector<Chip>& chips)
{
    if (start == end) return false;

    std::unordered_set<int> occupied;
    for (const auto& chip : chips) {
        if (chip.getPosition() != start) {
            occupied.insert(chip.getPosition());
        }
    }

    std::queue<int> queue;
    std::unordered_set<int> visited;
    queue.push(start);
    visited.insert(start);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (const auto& conn : connections) {
            int neighbor = -1;
            if (conn.point1 == current) neighbor = conn.point2;
            if (conn.point2 == current) neighbor = conn.point1;

            if (neighbor == -1 || visited.count(neighbor) || occupied.count(neighbor)) {
                continue;
            }

            if (neighbor == end) {
                return true;
            }

            visited.insert(neighbor);
            queue.push(neighbor);
        }
    }

    return false;
}

std::vector<int> PathFinder::findPath(int start, int end,
    const std::vector<Connection>& connections,
    const std::vector<Chip>& chips)
{
    std::unordered_set<int> occupied;
    for (const auto& chip : chips) {
        if (chip.getPosition() != start) {
            occupied.insert(chip.getPosition());
        }
    }

    std::queue<int> queue;
    std::unordered_map<int, int> came_from;
    queue.push(start);
    came_from[start] = -1;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (current == end) break;

        for (const auto& conn : connections) {
            int neighbor = -1;
            if (conn.point1 == current) neighbor = conn.point2;
            if (conn.point2 == current) neighbor = conn.point1;

            if (neighbor != -1 && came_from.find(neighbor) == came_from.end() &&
                occupied.find(neighbor) == occupied.end()) {
                queue.push(neighbor);
                came_from[neighbor] = current;
            }
        }
    }

    std::vector<int> path;
    if (came_from.find(end) != came_from.end()) {
        for (int at = end; at != -1; at = came_from[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());
    }
    return path;
}