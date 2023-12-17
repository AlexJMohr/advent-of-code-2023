#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <format>

typedef std::vector<std::string> grid;

std::vector<grid> parse_file(const char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<grid> grids;

    grid current_grid;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            grids.push_back(current_grid);
            current_grid.clear();
        }
        else
            current_grid.push_back(line);
    }

    if (!current_grid.empty())
        grids.push_back(current_grid);

    return grids;
}

grid transpose(grid g)
{
    grid t(g[0].length(), std::string(g.size(), ' '));
    for (size_t i = 0; i < g.size(); i++)
        for (size_t j = 0; j < g[i].length(); j++)
            t[j][i] = g[i][j];
    return t;
}

void print_grid(grid grid, int reflected_row_idx, int reflected_col_idx)
{
    if (reflected_row_idx >= 0)
        std::cout << "reflected row: " << (reflected_row_idx + 1) << "\n";
    else if (reflected_col_idx >= 0)
        std::cout << "reflected col: " << (reflected_col_idx + 1) << "\n";
    for (int r = 0; r < grid.size(); r++)
    {
        for (int c = 0; c < grid[r].length(); c++)
        {
            std::cout << "  " << grid[r][c];
        }
        if (reflected_row_idx >= 0)
        {
            if (r == reflected_row_idx)
                std::cout << " v";
            else if (r == reflected_row_idx + 1)
                std::cout << " ^";
            else
                std::cout << "  ";
        }
        std::cout << std::format("{:3}", r + 1);
        std::cout << "\n";
    }
    if (reflected_col_idx >= 0)
    {
        for (int c = 0; c < grid[0].length(); c++)
        {
            if (c == reflected_col_idx)
                std::cout << "  >";
            else if (c == reflected_col_idx + 1)
                std::cout << "  <";
            else
                std::cout << "   ";
        }
    }
    std::cout << "\n";
    for (size_t c = 0; c < grid[0].length(); c++)
        std::cout << std::format("{:3}", c + 1);
    std::cout << "\n";
}

int find_reflected_row(grid const &grid, int prev_row_idx = -1)
{
    // start at the bottom row and work our way up
    for (int r = grid.size() - 2; r >= 0; --r)
    {
        if (prev_row_idx >= 0 && r == prev_row_idx)
            continue;

        bool reflected = true;
        for (int i = r, j = r + 1; i >= 0 && j < grid.size(); --i, ++j)
        {
            if (grid.at(i) != grid.at(j))
            {
                reflected = false;
                break;
            }
        }
        if (reflected)
            return r;
    }
    return -1;
}

int find_unsmudged_reflected_row(grid const &g)
{
    // to make sure we don't find the same reflection twice, we keep track of the
    // previous reflection and skip it
    int prev_row_idx = find_reflected_row(g);

    for (int r = 0; r < g.size(); ++r)
    {
        for (int c = 0; c < g[r].length(); ++c)
        {
            grid grid_copy = g;
            grid_copy[r][c] = (grid_copy[r][c] == '#' ? '.' : '#');

            int reflected_row_idx = find_reflected_row(grid_copy, prev_row_idx);
            if (reflected_row_idx >= 0)
                return reflected_row_idx;
        }
    }

    return -1;
}

void part1(const char *filename)
{
    auto grids = parse_file(filename);

    int part1 = 0;
    for (auto grid : grids)
    {
        int score = 0;
        int reflected_row_idx = find_reflected_row(grid);
        int reflected_col_idx = -1;
        if (reflected_row_idx >= 0)
        {
            score = (reflected_row_idx + 1) * 100; // row numbers start at 1
        }
        else
        {
            reflected_col_idx = find_reflected_row(transpose(grid));
            if (reflected_col_idx >= 0)
                score = reflected_col_idx + 1;
        }
        part1 += score;

        // print_grid(grid, reflected_row_idx, reflected_col_idx);
        // std::cout << "score: " << score << "\n\n";
    }

    std::cout << "part 1: " << part1 << std::endl; // 36015
}

void part2(const char *filename)
{
    auto grids = parse_file(filename);

    int part2 = 0;
    for (auto grid : grids)
    {
        int score = 0;
        int reflected_row_idx = find_unsmudged_reflected_row(grid);
        int reflected_col_idx = -1;
        if (reflected_row_idx >= 0)
        {
            score = (reflected_row_idx + 1) * 100; // row numbers start at 1
        }
        else
        {
            reflected_col_idx = find_unsmudged_reflected_row(transpose(grid));
            if (reflected_col_idx >= 0)
                score = reflected_col_idx + 1;
        }
        part2 += score;

        // print_grid(grid, reflected_row_idx, reflected_col_idx);
        // std::cout << "score: " << score << "\n\n";

        if (reflected_row_idx < 0 && reflected_col_idx < 0)
        {
            std::cout << "PANIC! No reflection found!\n";
            return;
        }
    }

    std::cout << "part 2: " << part2 << std::endl; // 35335
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
