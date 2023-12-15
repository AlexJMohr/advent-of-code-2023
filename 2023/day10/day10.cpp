#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>
#include <unordered_map>
#include <optional>

typedef std::pair<int, int> coord;
typedef std::vector<std::string> grid;

grid parse_file(const char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");
    grid grid;
    std::string line;
    while (std::getline(file, line))
        grid.push_back(line);
    return grid;
}

coord find_start(grid const &grid)
{
    for (int j = 0; j < grid.size(); ++j)
        for (int i = 0; i < grid[j].size(); ++i)
            if (grid[j][i] == 'S')
                return {i, j};
    throw std::runtime_error("Could not find start");
}

coord next_pos(grid const &grid, coord prev, coord current)
{
    auto &[px, py] = prev;
    auto &[cx, cy] = current;
    switch (grid[cy][cx])
    {
    case '|': // north/south
        return coord{cx, cy - 1} == prev ? coord{cx, cy + 1} : coord{cx, cy - 1};
    case '-': // east/west
        return coord{cx - 1, cy} == prev ? coord{cx + 1, cy} : coord{cx - 1, cy};
    case 'L': // north/east
        return coord{cx, cy - 1} == prev ? coord{cx + 1, cy} : coord{cx, cy - 1};
    case 'J': // north/west
        return coord{cx, cy - 1} == prev ? coord{cx - 1, cy} : coord{cx, cy - 1};
    case '7': // south/west
        return coord{cx, cy + 1} == prev ? coord{cx - 1, cy} : coord{cx, cy + 1};
    case 'F': // south/east
        return coord{cx, cy + 1} == prev ? coord{cx + 1, cy} : coord{cx, cy + 1};
    default:
        __builtin_unreachable();
    }
}

void part1(const char *filename)
{
    auto grid = parse_file(filename);
    coord start = find_start(grid);
    coord prev1 = start;
    coord prev2 = start;
    // next positions from start are east and west in both test and input
    coord pos1 = {prev1.first + 1, prev1.second};
    coord pos2 = {prev2.first, prev2.second + 1};
    int step_count = 1;

    while (pos1 != pos2)
    {
        auto temp1 = pos1;
        auto temp2 = pos2;
        pos1 = next_pos(grid, prev1, pos1);
        pos2 = next_pos(grid, prev2, pos2);
        prev1 = temp1;
        prev2 = temp2;
        ++step_count;
    }

    std::cout << "Part 1: " << step_count << std::endl; // 6968
}

std::vector<std::vector<bool>> find_loop_positions(grid const &grid, coord start)
{
    std::vector<std::vector<bool>> loop_positions(grid.size(), std::vector<bool>(grid[0].size(), false));
    coord prev = start;
    // next positions from start are south and east in both test and input.
    // arbitrary choice of which to start with.
    coord current = {start.first + 1, start.second};
    loop_positions[start.second][start.first] = true;
    while (true)
    {
        loop_positions[current.second][current.first] = true;
        auto next = next_pos(grid, prev, current);
        if (next == start)
            break;
        prev = current;
        current = next;
    }
    return loop_positions;
}

void part2(const char *input)
{
    auto grid = parse_file(input);

    coord start = find_start(grid);
    // positions off of start are south and east in both test and input.
    // so start must be 'F'
    grid[start.second][start.first] = 'F';

    // get grid of booleans where true means that the position is part of the loop
    auto loop_positions_grid = find_loop_positions(grid, start);

    // ray casting algorithm.
    // for each point, cast a ray in any direction and count the number of intersections.
    // if the number of intersections is odd, then the point is outside of the polygon.

    int points_within = 0;
    // loop over all points in the grid
    for (int j = 0; j < grid.size(); ++j)
    {
        for (int i = 0; i < grid[j].size(); ++i)
        {
            // loop positions are not contained by the loop
            if (loop_positions_grid[j][i])
                continue;
            // fire ray to the east and count the number of intersections.
            // it is an intersection if the pipe has a south bend (i.e '|', 'F', '7')
            int num_intersections = 0;
            for (int x = i; x < grid[j].size(); ++x)
            {
                if (!loop_positions_grid[j][x])
                    continue;
                char pipe = grid[j][x];
                if (pipe == '|' || pipe == 'F' || pipe == '7')
                    ++num_intersections;
            }
            if (num_intersections % 2 == 1)
                ++points_within;
        }
    }

    std::cout << "Part 2: " << points_within << std::endl; // 413
}

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

    part1(argv[1]);
    part2(argv[1]);
    return 0;
}
