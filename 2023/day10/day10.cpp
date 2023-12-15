#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>
#include <unordered_map>
#include <optional>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    typedef std::pair<int64_t, int64_t> coord;
    typedef std::pair<coord, coord> pipe;

    std::vector<std::vector<pipe>> grid;

    const std::unordered_map<char, pipe> pipe_directions = {
        // north = {0, -1}
        // south = {0, 1}
        // east = {1, 0}
        // west = {-1, 0}
        // {{x1, y1}, {x2, y2}}
        {'|', {{0, -1}, {0, 1}}},  // north and south
        {'-', {{1, 0}, {-1, 0}}},  // east and west
        {'L', {{0, -1}, {1, 0}}},  // north and east
        {'J', {{0, -1}, {-1, 0}}}, // north and west
        {'7', {{0, 1}, {-1, 0}}},  // south and west
        {'F', {{0, 1}, {1, 0}}},   // south and east
    };
    const pipe ground = {{0, 0}, {0, 0}};

    // get first line
    std::string line;
    coord start;
    for (size_t j = 0; std::getline(file, line); j++)
    {
        std::vector<pipe> row;
        // for char in line
        for (int i = 0; i < line.size(); i++)
        {
            char c = line[i];
            switch (c)
            {
            case 'S':
                start = {i, j};
                row.push_back(ground);
                break;
            case '.':
                row.push_back(ground);
                break;
            default:
                pipe p = pipe_directions.at(c);
                auto [offset1, offset2] = p;
                row.push_back({{i + offset1.first, j + offset1.second}, {i + offset2.first, j + offset2.second}});
                break;
            }
        }
        std::cout << line << std::endl;
        grid.push_back(row);
    }

    coord previous_coord = start;
    coord current_coord;
    // find the two directions off of the start
    auto [x, y] = start;
    bool found = false;
    for (int64_t j = std::max(y - 1, 0L); j <= std::min(y + 1, static_cast<int64_t>(grid.size()) - 1); j++)
    {
        for (int64_t i = std::max(y - 1, 0L); i <= std::min(y + 1, static_cast<int64_t>(grid.size()) - 1); i++)
        {
            if (i == x && j == y || grid[j][i] == ground)
                continue;
            std::cout << i << ", " << j << ": ";
            pipe p = grid[j][i];
            std::cout << p.first.first << ", " << p.first.second << std::endl;
            if (p.first == start || p.second == start)
            {
                current_coord = {i, j};
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    for (auto &row : grid)
    {

        for (auto &pipe : row)
        {
            auto [c1, c2] = pipe;
            std::cout << c1.first << "," << c1.second << "-" << c2.first << "," << c2.second << " | ";
        }

        std::cout << "\n---------------------\n";
    }

    std::cout << "Start: " << start.first << ", " << start.second << std::endl;

    // follow along the path away from the start, until we get back to the start
    int steps = 0;
    for (; current_coord != start; steps++)
    {
        auto [x, y] = current_coord;
        std::cout << "Current: " << x << ", " << y << std::endl;
        auto current_pipe = grid[y][x];
        // next pipe is the one that is not at the previous coord
        current_coord = current_pipe.first == previous_coord ? current_pipe.second : current_pipe.first;
        std::cout << "next: " << current_coord.first << ", " << current_coord.second << std::endl;

        if (steps == 10)
            break;
    }

    std::cout << "Steps: " << steps << std::endl;

    return 0;
}
