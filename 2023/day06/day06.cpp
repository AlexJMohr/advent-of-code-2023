#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <numeric>

struct Race
{
    int64_t time;
    int64_t distance_to_beat;
};

std::vector<int64_t> parse_line(std::ifstream &file, bool ignore_spaces)
{
    std::string line;
    std::getline(file, line);

    if (ignore_spaces)
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

    std::istringstream ss(line);
    while (ss.peek() != ':')
        ss.ignore();
    ss.ignore();

    std::vector<int64_t> values;
    for (int64_t value; ss >> value;)
        values.push_back(value);
    return values;
}

std::vector<Race> parse_file(const char *filename, bool ignore_spaces = false)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<int64_t> times = parse_line(file, ignore_spaces);
    std::vector<int64_t> distances = parse_line(file, ignore_spaces);

    std::vector<Race> races;
    races.reserve(times.size());
    std::transform(times.begin(), times.end(), distances.begin(), std::back_inserter(races), [](auto time, auto dist)
                   { return Race{time, dist}; });

    return races;
}

void find_roots(int64_t a, int64_t b, int64_t c, double &root1, double &root2)
{
    auto determinant = b * b - 4 * a * c;
    if (determinant < 0)
    {
        throw std::runtime_error("No real roots");
    }
    else if (determinant == 0)
    {
        root1 = -b / (2 * a);
        root2 = root1;
    }
    else
    {
        auto sqrt_det = std::sqrt(determinant);
        root1 = (-b + sqrt_det) / (2 * a);
        root2 = (-b - sqrt_det) / (2 * a);
    }
}

int64_t solve_races(const std::vector<Race> &races)
{
    int result = 1;
    for (auto &race : races)
    {
        // std::cout << "race: " << race.time << " " << race.distance_to_beat << '\n';
        // distance = hold_time * (race.time - hold_time)
        // Expanded quadratic equation:
        // -hold_time * hold_time + race.time * hold_time - distance = 0
        double root1, root2;
        find_roots(-1LL, race.time, -race.distance_to_beat, root1, root2);

        // std::cout << "previous record of " << race.distance_to_beat << " held for between " << root1 << " and " << root2 << '\n';

        int min_hold_time = static_cast<int64_t>(std::ceil(root1) == root1 ? root1 + 1 : std::ceil(root1));
        int max_hold_time = static_cast<int64_t>(std::floor(root2) == root2 ? root2 - 1 : std::floor(root2));

        result *= (max_hold_time - min_hold_time + 1);
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    auto part1_races = parse_file(argv[1]);
    std::cout << "part 1: " << solve_races(part1_races) << '\n'; // 500346

    auto part2_races = parse_file(argv[1], true);
    std::cout << "part 2: " << solve_races(part2_races) << '\n'; // 42515755

    return 0;
}
