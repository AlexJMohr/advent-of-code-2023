#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <optional>

void part1(const char *input_file)
{
    std::ifstream file(input_file);

    std::string line;
    int total = 0;
    while (std::getline(file, line))
    {
        size_t first_digit = line.find_first_of("0123456789");
        size_t last_digit = line.find_last_of("0123456789");
        std::string number = line.substr(first_digit, 1) + line.substr(last_digit, 1);
        int num = std::stoi(number);
        total += num;
    }
    std::cout << total << std::endl;
}

void part2(const char *input_file)
{
    const std::array<std::pair<const std::string, int>, 10> number_words{{
        {"zero", 0},
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9},
    }};

    std::ifstream file(input_file);
    int total;

    std::string line;
    while (std::getline(file, line))
    {
        int first_digit = -1;
        int last_digit = -1;

        size_t i = 0;
        for (i = 0; i < line.length(); i++)
        {
            // if it's a digit
            if (line[i] >= '0' && line[i] <= '9')
            {
                first_digit = line[i] - '0';
                break;
            }

            // not digit, try the words
            for (auto &[word, num] : number_words)
            {
                if (line.compare(i, word.length(), word) == 0)
                {
                    first_digit = num;
                    break;
                }
            }
            if (first_digit >= 0)
                break;
        }

        for (i = line.length(); i--;)
        {
            // if it's a digit
            if (line[i] >= '0' && line[i] <= '9')
            {
                last_digit = line[i] - '0';
                break;
            }

            // not digit, try the words
            for (auto &[word, num] : number_words)
            {
                int c = line.compare(i, word.length(), word);
                if (c == 0)
                {
                    last_digit = num;
                    break;
                }
            }
            if (last_digit >= 0)
                break;
        }

        total += first_digit * 10 + last_digit;
    }

    std::cout << total << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    part2(argv[1]);

    return 0;
}
