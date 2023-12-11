#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <vector>

typedef std::vector<char> Row;
typedef std::vector<Row> Grid;

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

Grid parse_file(const char *input_file)
{
    std::ifstream file(input_file);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    Grid grid;
    std::string line;
    while (std::getline(file, line))
    {
        Row row;
        for (size_t i = 0; i < line.length(); i++)
            row.push_back(line[i]);
        grid.push_back(row);
    }
    return grid;
}

void part1(Grid &grid)
{
    std::array<char, 10> SYMBOLS = {'+', '-', '*', '/', '=', '#', '&', '@', '$', '%'};
    int sum = 0;
    for (size_t r = 0; r < grid.size(); r++)
    {
        auto row = grid[r];
        int n = 0;
        bool has_symbol = false;
        for (size_t c = 0; c < row.size(); c++)
        {
            if (is_digit(row[c]))
            {
                n = n * 10 + (row[c] - '0');

                // look for symbol around the digit
                if (!has_symbol)
                {
                    for (size_t y = (r == 0 ? 0 : r - 1); y <= std::min(r + 1, grid.size() - 1); y++)
                    {
                        for (size_t x = (c == 0 ? 0 : c - 1); x <= std::min(c + 1, row.size() - 1); x++)
                        {
                            has_symbol = std::find(SYMBOLS.begin(), SYMBOLS.end(), grid[y][x]) != SYMBOLS.end();
                            if (has_symbol)
                                break;
                        }
                        if (has_symbol)
                            break;
                    }
                }
            }
            // if we've reached the end of the row or the current char is not a digit
            if (c == row.size() - 1 || row[c] < '0' || row[c] > '9')
            {
                if (has_symbol)
                    sum += n;
                n = 0;
                has_symbol = false;
            }
        }
    }

    std::cout << "Part 1: " << sum << '\n'; // 533784
}

void part2(Grid &grid)
{
    int sum = 0;

    for (size_t r = 0; r < grid.size(); r++)
    {
        auto row = grid[r];
        for (size_t c = 0; c < row.size(); c++)
        {
            if (row[c] == '*')
            {

                int second_number = 0;
                size_t second_number_x = 0;
                size_t second_number_y = 0;
                // look for second number, starting at the bottom right
                for (size_t y = std::min(r + 1, grid.size() - 1); y >= (r == 0 ? 0 : r - 1); y--)
                {
                    for (size_t x = std::min(c + 1, row.size() - 1); x >= (c == 0 ? 0 : c - 1); x--)
                    {
                        if (x == c && y == r)
                            continue;

                        if (grid[y][x] >= '0' && grid[y][x] <= '9')
                        {
                            second_number_y = y;
                            size_t i = x;
                            while (i > 0 && is_digit(grid[y][i - 1]))
                                i--;
                            second_number_x = i;
                            for (; i < grid[0].size() && is_digit(grid[y][i]); i++)
                                second_number = second_number * 10 + (grid[y][i] - '0');
                            break;
                        }
                    }
                    if (second_number)
                        break;
                }

                // if we didn't find a second number, it's not a gear
                if (!second_number)
                    continue;

                int first_number = 0;
                // assume it's a gear for now
                bool is_gear = true;

                // look for first number, starting at the top left
                for (size_t y = (r == 0 ? 0 : r - 1); y <= std::min(r + 1, grid.size()); y++)
                {
                    for (size_t x = (c == 0 ? 0 : c - 1); x <= std::min(c + 1, row.size()); x++)
                    {
                        if (grid[y][x] >= '0' && grid[y][x] <= '9')
                        {
                            size_t i = x;
                            while (i > 0 && is_digit(grid[y][i - 1]))
                                i--;
                            if (y == second_number_y && i == second_number_x)
                            {
                                // it's the same number, so it's not a gear
                                is_gear = false;
                                break;
                            }
                            for (; i < grid[0].size() && is_digit(grid[y][i]); i++)
                                first_number = first_number * 10 + (grid[y][i] - '0');
                            break;
                        }
                        if (!is_gear)
                            break;
                    }
                    if (!is_gear || first_number != 0)
                        break;
                }

                if (!first_number)
                    continue;

                sum += first_number * second_number;
            }
        }
    }

    std::cout << "Part 2: " << sum << '\n'; // 78826761
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }
    auto grid = parse_file(argv[1]);
    part1(grid);
    part2(grid);
    return 0;
}
