#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std::literals;

std::vector<std::pair<std::string, std::vector<int64_t>>> parse_file(const char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<std::pair<std::string, std::vector<int64_t>>> springs;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string spring;
        ss >> spring;

        std::vector<int64_t> broken_lengths;
        int64_t length;
        while (ss >> length)
        {
            broken_lengths.push_back(length);
            if (ss.peek() == ',')
                ss.ignore(1);
        }

        springs.push_back({spring, broken_lengths});
    }

    return springs;
}

int64_t count_valid_patterns(std::string spring, std::vector<int64_t> broken_lengths, int64_t current_broken_length = 0)
{
    static std::map<std::tuple<std::string, std::vector<int64_t>, int64_t>, int64_t> memo_table;
    auto key = std::make_tuple(spring, broken_lengths, current_broken_length);
    if (auto it = memo_table.find(key); it != memo_table.end())
        return it->second;
    if (spring.length() == 0)
    {
        // We've reached the end of the spring.
        // It's valid if we've used up all the broken lengths, and we're not in the middle of a broken segment...
        if (broken_lengths.size() == 0 && current_broken_length == 0)
            return 1;
        // ... or if we're on the last broken segment and it matches the last broken length.
        else if (broken_lengths.size() == 1 && broken_lengths[0] == current_broken_length)
            return 1;
        // Otherwise it's invalid
        return 0;
    }

    char current = spring.at(0);
    std::string rest_spring = spring.substr(1); // copy

    if (current == '?')
    {
        auto result = count_valid_patterns("." + rest_spring, broken_lengths, current_broken_length) //
                      + count_valid_patterns("#" + rest_spring, broken_lengths, current_broken_length);
        memo_table[key] = result;
        return result;
    }

    int64_t expected_length = broken_lengths.size() > 0 ? broken_lengths.at(0) : 0;

    switch (current)
    {
    case '.':
        // If previous char was part of a broken segment...
        if (current_broken_length > 0)
        {
            // ... and it's the incorrect length, we're on an invalid path. abort.
            if (current_broken_length != expected_length)
                return 0;
            // Otherwise we're keep going, and we're not on a broken segment anymore.
            broken_lengths.erase(broken_lengths.begin());
            return count_valid_patterns(rest_spring, broken_lengths, 0);
        }
        // We're still not on a broken segment. Keep going.
        return count_valid_patterns(rest_spring, broken_lengths, 0);
    case '#':
        // If current broken segment is too long, we're on an invalid path. abort.
        if (current_broken_length > expected_length)
            return 0;
        // Otherwise keep going
        return count_valid_patterns(rest_spring, broken_lengths, current_broken_length + 1);
    default:
        __builtin_unreachable();
    }
}

void part1(const char *filename)
{
    auto springs = parse_file(filename);
    int64_t part1 = 0;
    for (auto &[spring, lengths] : springs)
        part1 += count_valid_patterns(spring, lengths);
    std::cout << "part 1: " << part1 << std::endl; // 7169
}

void part2(const char *filename)
{
    auto springs = parse_file(filename);
    int64_t part2 = 0;
    for (auto &[spring, lengths] : springs)
    {
        // concatenate spring with itself five times with a '?' in between
        spring = spring + "?" + spring + "?" + spring + "?" + spring + "?" + spring;

        // repeat the lengths array five times
        size_t old_count = lengths.size();
        lengths.reserve(old_count * 5);
        std::copy_n(lengths.begin(), old_count * 4, std::back_inserter(lengths));

        auto valid_pattern_count = count_valid_patterns(spring, lengths);
        part2 += valid_pattern_count;

        // dump
        // std::cout << spring << " (";
        // for (size_t i = 0; auto length : lengths)
        // {
        //     std::cout << length;
        //     if (i++ < lengths.size() - 1)
        //         std::cout << ", ";
        // }
        // std::cout << ") : " << valid_pattern_count << "\n";
    }

    std::cout << "part 2: " << part2 << std::endl;
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
