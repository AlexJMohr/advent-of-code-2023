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

coord find_start(const grid &grid)
{
    for (int j = 0; j < grid.size(); ++j)
        for (int i = 0; i < grid[j].size(); ++i)
            if (grid[j][i] == 'S')
                return {i, j};
    throw std::runtime_error("Could not find start");
}

coord next_pos(const grid &grid, coord prev, coord current)
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

    std::string line;

    grid grid;
    while (std::getline(file, line))
        grid.push_back(line);

    coord start = find_start(grid);
    coord prev1 = start;
    coord prev2 = start;
    // next positions from start are east and west in both test and input
    coord pos1 = {prev1.first + 1, prev1.second};
    coord pos2 = {prev2.first, prev2.second + 1};
    int step_count = 1;

    // std::cout << "Step: " << step_count << '\n';
    // std::cout << "1: " << prev1.first << ", " << prev1.second << " -> " << pos1.first << ", " << pos1.second << '\n';
    // std::cout << "2: " << prev2.first << ", " << prev2.second << " -> " << pos2.first << ", " << pos2.second << '\n';
    // std::cout << "---\n";

    while (pos1 != pos2)
    {
        auto temp1 = pos1;
        auto temp2 = pos2;
        pos1 = next_pos(grid, prev1, pos1);
        pos2 = next_pos(grid, prev2, pos2);
        prev1 = temp1;
        prev2 = temp2;
        ++step_count;
        // std::cout << "Step: " << step_count << '\n';
        // std::cout << "1: " << prev1.first << ", " << prev1.second << " -> " << pos1.first << ", " << pos1.second << '\n';
        // std::cout << "2: " << prev2.first << ", " << prev2.second << " -> " << pos2.first << ", " << pos2.second << '\n';
        // std::cout << "---\n";
    }

    std::cout << "Part 1: " << step_count << std::endl; // 6968

    return 0;
}
