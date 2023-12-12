#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>
#include <stack>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    std::string line;

    int64_t part1 = 0;
    int64_t part2 = 0;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::vector<int64_t> numbers;
        for (int64_t number; ss >> number;)
            numbers.push_back(number);

        std::stack<int64_t> first_numbers;
        std::stack<int64_t> last_numbers;
        while (true)
        {
            first_numbers.push(numbers.front());
            last_numbers.push(numbers.back());

            std::vector<int64_t> new_numbers;
            new_numbers.reserve(numbers.size() - 1);

            bool all_zero = true;
            for (size_t i = 1; i < numbers.size(); ++i)
            {
                int64_t diff = numbers[i] - numbers[i - 1];
                new_numbers.push_back(diff);
                if (diff != 0)
                    all_zero = false;
            }
            numbers = new_numbers;
            if (all_zero || numbers.size() == 1)
                break;
        }

        while (!last_numbers.empty())
        {
            int64_t num = last_numbers.top();
            last_numbers.pop();
            part1 += num;
        }

        int64_t x = 0;
        while (!first_numbers.empty())
        {
            int64_t first_num = first_numbers.top();
            first_numbers.pop();
            x = first_num - x;
        }
        part2 += x;
    }

    std::cout << "part1: " << part1 << "\n"; // 2174807968
    std::cout << "part2: " << part2 << "\n"; // 1208

    return 0;
}
