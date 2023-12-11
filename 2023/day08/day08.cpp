#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <numeric>

struct ParseResult
{
    std::string directions;
    std::unordered_map<std::string, std::pair<std::string, std::string>> nodes_map;
};

ParseResult parse(const char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open input file");

    std::string directions;
    std::getline(file, directions);

    std::unordered_map<std::string, std::pair<std::string, std::string>> nodes_map;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream ss(line);

        std::string name;
        std::getline(ss, name, ' ');

        // skip "= ("
        ss.ignore(3);

        std::string left;
        std::getline(ss, left, ',');

        // ignore space
        ss.ignore();

        std::string right;
        std::getline(ss, right, ')');

        nodes_map[name] = {left, right};
    }

    return {directions, nodes_map};
}

int part1(const char *filename)
{
    const std::string target("ZZZ");
    std::string current("AAA");
    size_t i = 0;
    int64_t steps = 0;

    auto [directions, nodes_map] = parse(filename);

    while (current != target)
    {
        ++steps;

        switch (directions[i])
        {
        case 'L':
            current = nodes_map[current].first;
            break;
        case 'R':
            current = nodes_map[current].second;
            break;
        default:
            __builtin_unreachable();
        }

        i = (i + 1) % directions.length();
    }

    std::cout << "part 1: " << steps << '\n'; // 19631

    return 0;
}

void part2(const char *filename)
{
    auto [directions, nodes_map] = parse(filename);

    std::vector<std::string> first_nodes;
    for (auto &[name, node] : nodes_map)
        if (name.back() == 'A')
            first_nodes.push_back(name);

    std::vector<int64_t> step_counts;
    step_counts.reserve(first_nodes.size());
    for (auto &node : first_nodes)
    {
        size_t i = 0;
        int64_t steps = 0;
        auto current = node;
        while (current.back() != 'Z')
        {
            switch (directions[i])
            {
            case 'L':
                current = nodes_map[current].first;
                break;
            case 'R':
                current = nodes_map[current].second;
                break;
            default:
                __builtin_unreachable();
            }
            ++steps;
            i = (i + 1) % directions.length();
        }
        step_counts.push_back(steps);
    }

    int64_t part2 = std::accumulate(step_counts.begin(), step_counts.end(), 1LL, [](auto a, auto b)
                                    { return std::lcm(a, b); });

    std::cout << "part 2: " << part2 << '\n'; // 21003205388413
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << "<part1|part2> <input file>" << std::endl;
        return 1;
    }

    if (std::string(argv[1]) == "part1")
        part1(argv[2]);
    else if (std::string(argv[1]) == "part2")
        part2(argv[2]);
    else
        std::cerr << "Invalid part: " << argv[1] << std::endl;
}