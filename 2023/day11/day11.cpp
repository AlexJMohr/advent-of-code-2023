#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>

typedef std::pair<int, int> coord;
typedef std::vector<std::string> grid;

grid parse_file(const char *filename)
{
    grid grid;
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");
    std::string line;
    while (std::getline(file, line))
        grid.push_back(line);
    return grid;
}

std::vector<coord> find_galaxy_positions(grid const &grid)
{
    std::vector<coord> galaxy_positions;
    for (size_t y = 0; y < grid.size(); ++y)
        for (size_t x = 0; x < grid[y].size(); ++x)
            if (grid[y][x] == '#')
                galaxy_positions.push_back({x, y});
    return galaxy_positions;
}

std::vector<std::pair<coord, coord>> get_galaxy_pairs(std::vector<coord> galaxy_positions)
{
    // permutations of all galaxy pairs
    std::vector<std::pair<coord, coord>> pairs;
    for (size_t i = 0; i < galaxy_positions.size(); ++i)
        for (size_t j = i + 1; j < galaxy_positions.size(); ++j)
            pairs.push_back({galaxy_positions[i], galaxy_positions[j]});
    return pairs;
}

std::vector<std::vector<int>> get_expansion_grid(const grid &grid, int expansion)
{
    std::vector<std::vector<int>> expansion_grid(grid.size(), std::vector<int>(grid[0].size(), 1));
    // find empty rows
    for (size_t y = 0; y < grid.size(); ++y)
    {
        bool empty_row = true;
        for (size_t x = 0; x < grid[y].size(); ++x)
        {
            if (grid[y][x] == '#')
            {
                empty_row = false;
                break;
            }
        }
        if (empty_row)
            expansion_grid[y] = std::vector<int>(grid[0].size(), expansion);
    }
    // find empty columns
    for (size_t x = 0; x < grid[0].size(); ++x)
    {
        bool empty_col = true;
        for (size_t y = 0; y < grid.size(); ++y)
        {
            if (grid[y][x] == '#')
            {
                empty_col = false;
                break;
            }
        }
        if (empty_col)
            for (size_t y = 0; y < grid.size(); ++y)
                expansion_grid[y][x] = expansion;
    }
    return expansion_grid;
}

int64_t get_total_distance(std::vector<coord> const &galaxy_positions, std::vector<std::vector<int>> const &expansion_grid)
{
    auto galaxy_pairs = get_galaxy_pairs(galaxy_positions);

    int64_t total_distance = 0;
    for (const auto &pair : galaxy_pairs)
    {
        auto g1 = pair.first;
        auto g2 = pair.second;

        int64_t horizontal_distance = 0;
        // start on east-most galaxy
        if (g1.first > g2.first)
            std::swap(g1, g2);
        for (int x = g1.first; x < g2.first; ++x)
            horizontal_distance += expansion_grid[g1.second][x];
        int64_t vertical_distance = 0;
        // start on north-most galaxy
        if (g1.second > g2.second)
            std::swap(g1, g2);
        for (int y = g1.second; y < g2.second; ++y)
            vertical_distance += expansion_grid[y][g2.first];
        total_distance += horizontal_distance + vertical_distance;
    }
    return total_distance;
}

void part1(const char *filename)
{
    auto grid = parse_file(filename);
    auto galaxy_positions = find_galaxy_positions(grid);
    auto expansion_grid = get_expansion_grid(grid, 2);
    auto total_distance = get_total_distance(galaxy_positions, expansion_grid);
    std::cout << "part 1: " << total_distance << std::endl; // 9550717
}

void part2(const char *filename)
{
    // const int EXPANSION = 10; // for test, and total distance should be 1030
    // const int EXPANSION = 100; // for test, and total distance should be 8410
    const int EXPANSION = 1000000; // for input
    auto grid = parse_file(filename);
    auto galaxy_positions = find_galaxy_positions(grid);
    auto expansion_grid = get_expansion_grid(grid, EXPANSION);
    auto total_distance = get_total_distance(galaxy_positions, expansion_grid);
    std::cout << "part 2: " << total_distance << std::endl; // 648458253817
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
