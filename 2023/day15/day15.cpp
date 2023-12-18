#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>

std::vector<std::string> parse_file(const char *filename)
{
    std::vector<std::string> instructions;
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");
    std::string line;
    while (std::getline(file, line, ','))
        instructions.push_back(line);
    return instructions;
}

int64_t hash(std::string instruction)
{
    int64_t val = 0;
    for (char c : instruction)
    {
        val += static_cast<int64_t>(c);
        val *= 17;
        val %= 256;
    }
    return val;
}

void part1(const char *filename)
{
    // std::cout << hash("HASH") << "\n"; // 52
    // return;

    int64_t sum = 0;
    auto instructions = parse_file(filename);
    for (auto instruction : instructions)
        sum += hash(instruction);
    std::cout << "part 1: " << sum << "\n"; // 508498
}

void part2(const char *filename)
{
    auto instructions = parse_file(filename);
    int64_t part2 = 0;
    std::cout << "part 2: " << part2 << "\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    part1(argv[1]);
    // part2(argv[1]);

    return 0;
}
