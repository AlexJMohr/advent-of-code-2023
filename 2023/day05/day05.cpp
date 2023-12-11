#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <optional>
#include <sstream>
#include <numeric>
#include <limits>
#include <format>

struct Range
{
    int64_t min;
    int64_t max;
};

struct Mapping
{
    int64_t src_start;
    int64_t dst_start;
    int64_t length;

    Mapping(int64_t src_start, int64_t dst_start, int64_t length) : src_start(src_start), dst_start(dst_start), length(length) {}

    bool contains(int64_t n) const
    {
        return n >= src_start && n < src_start + length;
    }

    int64_t map(int64_t src_val) const
    {
        return src_val + (dst_start - src_start);
    }
};

struct Mappings
{
    std::vector<Mapping> mappings;

    void add_mapping(Mapping mapping)
    {
        mappings.push_back(mapping);
    }

    int64_t map(int64_t src_val) const
    {
        for (const auto &mapping : mappings)
        {
            if (mapping.contains(src_val))
                return mapping.map(src_val);
        }
        // If no mapping found, value is identity mapped
        return src_val;
    }

    void dump() const
    {
        for (const auto &mapping : mappings)
            std::cout << mapping.src_start << "-" << mapping.src_start + mapping.length - 1
                      << " -> "
                      << mapping.dst_start << "-" << mapping.dst_start + mapping.length - 1
                      << " (" << std::format("{0:+}", mapping.dst_start - mapping.src_start) << ")"
                      << "\n";
    }
};

Mappings parse_mappings(std::ifstream &file)
{
    // ignore first line
    std::string line;
    std::getline(file, line);

    Mappings mappings;
    while (std::getline(file, line))
    {
        if (line.empty())
            break;

        std::stringstream ss(line);
        int64_t src_start;
        int64_t dst_start;
        int64_t range_length;

        // Read dst range
        ss >> dst_start;
        // Skip " "
        ss.ignore(1);

        // Read src range
        ss >> src_start;
        // Skip " "
        ss.ignore(1);

        // Read range
        ss >> range_length;

        mappings.add_mapping({src_start, dst_start, range_length});
    }

    return mappings;
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

    std::vector<int64_t> part1_seeds;
    // ignore "Seeds: "
    file.ignore(6);
    while (file.peek() != '\n')
    {
        int64_t n;
        file >> n;
        part1_seeds.push_back(n);
        // // Skip " " (or "\n" if it's the last one)
        file.ignore(1);
    }

    std::vector<Range> part2_ranges;
    for (size_t i = 0; i < part1_seeds.size(); i += 2)
        part2_ranges.push_back({part1_seeds.at(i), part1_seeds.at(i) + part1_seeds.at(i + 1) - 1});

    // skip empty line
    file.ignore(1);

    Mappings seed_to_soil_mappings = parse_mappings(file);
    Mappings soil_to_fertilizer_mappings = parse_mappings(file);
    Mappings fertilizer_to_water_mappings = parse_mappings(file);
    Mappings water_to_light_mappings = parse_mappings(file);
    Mappings light_to_temperature_mappings = parse_mappings(file);
    Mappings temperature_to_humidity_mappings = parse_mappings(file);
    Mappings humidity_to_location_mappings = parse_mappings(file);

    std::vector<Mappings> chain{
        seed_to_soil_mappings,
        soil_to_fertilizer_mappings,
        fertilizer_to_water_mappings,
        water_to_light_mappings,
        light_to_temperature_mappings,
        temperature_to_humidity_mappings,
        humidity_to_location_mappings};

    // make sure mapping are sorted by src range min
    for (auto &mappings : chain)
        std::sort(mappings.mappings.begin(), mappings.mappings.end(), [](const Mapping &a, const Mapping &b)
                  { return a.src_start < b.src_start; });

    int64_t lowest_location = std::numeric_limits<int64_t>::max();
    for (auto seed : part1_seeds)
    {
        auto val = seed;
        for (const auto &mapping : chain)
            val = mapping.map(val);
        lowest_location = std::min(lowest_location, val);
    }
    std::cout << "Part 1: " << lowest_location << '\n'; // 621354867

    std::vector<Range> ranges = part2_ranges;
    for (auto &mappings : chain)
    {
        std::vector<Range> new_ranges;
        for (auto &range : ranges)
        {
            auto val = range.min;
            while (val < range.max)
            {
                auto mapping = std::find_if(mappings.mappings.begin(), mappings.mappings.end(), [val](const Mapping &mapping)
                                            { return mapping.contains(val); });
                if (mapping != mappings.mappings.end())
                {
                    auto last_val = std::min(mapping->src_start + mapping->length, range.max);
                    new_ranges.push_back({mapping->map(val), mapping->map(last_val)});
                    val = last_val;
                }
                else
                {
                    new_ranges.push_back({val, range.max});
                    break;
                }
            }
        }
        ranges = new_ranges;
    }

    auto part2 = *std::min_element(ranges.begin(), ranges.end(), [](const Range &a, const Range &b)
                                   { return a.min < b.min; });
    std::cout << "Part 2: " << part2.min << '\n'; // 15880236

    return 0;
}
