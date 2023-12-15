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

std::vector<std::pair<coord, coord>> get_galaxy_pairs(std::vector<coord> galaxy_positions)
{
    // permutations of all galaxy pairs
    std::vector<std::pair<coord, coord>> pairs;
    for (size_t i = 0; i < galaxy_positions.size(); ++i)
        for (size_t j = i + 1; j < galaxy_positions.size(); ++j)
            pairs.push_back({galaxy_positions[i], galaxy_positions[j]});
    return pairs;
}

int manhattan_distance(coord c1, coord c2)
{
    return std::abs(c1.first - c2.first) + std::abs(c1.second - c2.second);
}

void part1(const char *filename)
{
    auto grid = parse_file(filename);

    std::vector<int> empty_row_indices;
    std::vector<int> empty_col_indices;
    // find empty rows and galaxy positions
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
            empty_row_indices.push_back(y);
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
            empty_col_indices.push_back(x);
    }

    // clone empty rows and columns.
    // reverse so that the insertions don't mess up the indices.
    std::reverse(empty_row_indices.begin(), empty_row_indices.end());
    std::reverse(empty_col_indices.begin(), empty_col_indices.end());
    for (auto const &col : empty_col_indices)
        for (auto &row : grid)
            row.insert(row.begin() + col, '.');
    for (auto const &row : empty_row_indices)
        grid.insert(grid.begin() + row, std::string(grid[0].size(), '.'));

    // find galaxy positions ('#')
    std::vector<coord> galaxy_positions;
    for (size_t y = 0; y < grid.size(); ++y)
        for (size_t x = 0; x < grid[y].size(); ++x)
            if (grid[y][x] == '#')
                galaxy_positions.push_back({x, y});

    auto pairs = get_galaxy_pairs(galaxy_positions);

    int total_distance = 0;
    for (const auto &[g1, g2] : pairs)
        total_distance += manhattan_distance(g1, g2);

    std::cout << "part 1: " << total_distance << std::endl; // 9550717
}

void part2(const char *filename)
{
    auto grid = parse_file(filename);

    std::vector<int> empty_row_indices;
    std::vector<int> empty_col_indices;
    // find empty rows and galaxy positions
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
            empty_row_indices.push_back(y);
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
            empty_col_indices.push_back(x);
    }

    // print empty rows and columns
    std::cout << "empty rows: ";
    for (auto const &row : empty_row_indices)
        std::cout << row << ' ';
    std::cout << std::endl;
    std::cout << "empty cols: ";
    for (auto const &col : empty_col_indices)
        std::cout << col << ' ';
    std::cout << std::endl;

    // find galaxy positions ('#') before the expansion
    std::vector<coord> galaxy_positions;
    for (size_t y = 0; y < grid.size(); ++y)
        for (size_t x = 0; x < grid[y].size(); ++x)
            if (grid[y][x] == '#')
                galaxy_positions.push_back({x, y});

    auto pairs = get_galaxy_pairs(galaxy_positions);

    const int expansion = 10; // for test, and total distance should be 1030
    // const int expansion = 1'000'000; // for input

    int total_distance = 0;
    for (const auto &[g1, g2] : pairs)
    {
        int x1 = g1.first;
        int y1 = g1.second;
        int x2 = g2.first;
        int y2 = g2.second;

        // make sure x1 < x2 and y1 < y2
        if (x1 > x2)
            std::swap(x1, x2);
        if (y1 > y2)
            std::swap(y1, y2);

        // find number of empty columns between x and g2.first
        auto first_empty_col = std::lower_bound(empty_col_indices.begin(), empty_col_indices.end(), x1);
        auto last_empty_col = std::lower_bound(empty_col_indices.begin(), empty_col_indices.end(), x2);
        int num_empty_cols = std::distance(first_empty_col, last_empty_col);

        auto first_empty_row = std::lower_bound(empty_row_indices.begin(), empty_row_indices.end(), y1);
        auto last_empty_row = std::lower_bound(empty_row_indices.begin(), empty_row_indices.end(), y2);
        int num_empty_rows = std::distance(first_empty_row, last_empty_row);

        std::cout << "num_empty_cols: " << num_empty_cols << std::endl;
        std::cout << "num_empty_rows: " << num_empty_rows << std::endl;

        int horizontal_distance = x2 - x1 - num_empty_cols + (num_empty_cols * expansion);
        int vertical_distance = y2 - y1 - num_empty_rows + (num_empty_rows * expansion);
        std::cout << "horizontal_distance: " << horizontal_distance << std::endl;
        std::cout << "vertical_distance: " << vertical_distance << std::endl;
        total_distance += horizontal_distance + vertical_distance;
        std::cout << "---\n";
        // break;
    }

    // 58000090 too low
    // 82000210 too low
    std::cout << "part 2: " << total_distance << std::endl;
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
