#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <deque>
#include <map>

std::vector<std::string> parse_file(const char *filename)
{
    std::vector<std::string> grid;
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");
    std::string line;
    while (std::getline(file, line))
        grid.push_back(line);
    return grid;
}

void dump(std::vector<std::string> grid)
{
    for (auto row : grid)
        std::cout << row << "\n";
}

void tilt_north(std::vector<std::string> &grid)
{
    for (int col = 0; col < grid.at(0).size(); col++)
    {
        std::deque<int> free_rows;
        for (int row = 0; row < grid.size(); row++)
        {
            switch (grid[row][col])
            {
            case '.': // free space
                free_rows.push_back(row);
                break;
            case '#': // immovable rock
                // no rocks below this one can move above it. Clear the free rows.
                free_rows = {};
                break;
            case 'O': // movable rock
                if (free_rows.size() > 0)
                {
                    int free_row = free_rows.front();
                    free_rows.pop_front();
                    // move the rock up to the free spot
                    std::swap(grid[row][col], grid[free_row][col]);
                    // this row is now free
                    free_rows.push_back(row);
                }
                // else: no free rows, so the rock stays where it is.
                break;
            default:
                __builtin_unreachable();
            }
        }
    }
}

void rotate_clockwise(std::vector<std::string> &grid)
{
    int squares = grid.size() / 2;
    for (int i = 0; i < squares; i++)
    {
        int last = grid.size() - 1 - i;
        for (int j = i; j < last; j++)
        {
            std::swap(grid[i][j], grid[j][last]);
            std::swap(grid[i][j], grid[last][last - j + i]);
            std::swap(grid[i][j], grid[last - j + i][i]);
        }
    }
}

int calculate_load(std::vector<std::string> grid)
{
    int height = grid.size();
    int load = 0;
    for (int row = 0; row < grid.size(); row++)
        for (int col = 0; col < grid.at(0).size(); col++)
            if (grid[row][col] == 'O')
                load += height - row;
    return load;
}

void part1(const char *filename)
{
    auto grid = parse_file(filename);

    tilt_north(grid);

    // calculate the total load of the rocks.
    // the load of each rock is the distance to the bottom of the grid.
    // TODO: this might be improved by working it into the loop above.
    int part1 = calculate_load(grid);

    std::cout << "part 1: " << part1 << "\n"; // 109385
}

void spin(std::vector<std::string> &grid)
{
    for (int i = 0; i < 4; i++)
    {
        tilt_north(grid);
        rotate_clockwise(grid);
    }
}

void part2(const char *filename)
{
    auto grid = parse_file(filename);

    std::map<std::vector<std::string>, int> grid_idx_map;
    int cycle_idx = 0;
    int cycle_length = -1;
    while (true)
    {
        spin(grid);
        if (grid_idx_map.contains(grid))
        {
            cycle_length = cycle_idx - grid_idx_map[grid];
            break;
        }
        grid_idx_map[grid] = cycle_idx++;
    }
    if (cycle_length == -1)
        throw std::runtime_error("Could not find cycle");

    int additional_cycle_count = (1'000'000'000 - cycle_idx) % cycle_length - 1;
    for (int i = 0; i < additional_cycle_count; i++)
        spin(grid);

    int part2 = calculate_load(grid);

    std::cout << "part 2: " << part2 << "\n"; // 93102
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    part1(argv[1]);
    part2(argv[1]);

    return 0;
}
