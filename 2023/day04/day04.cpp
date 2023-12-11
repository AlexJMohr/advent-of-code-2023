#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <numeric>

struct Card
{
    int id;
    std::vector<int> winning_numbers;
    std::vector<int> numbers;

    int winning_count() const
    {
        return std::accumulate(numbers.begin(), numbers.end(), 0, [&](int a, int b)
                               { return a + (std::find(winning_numbers.begin(), winning_numbers.end(), b) != winning_numbers.end() ? 1 : 0); });
    }
};

std::vector<Card> parse_cards(const char *input_file)
{
    std::ifstream file(input_file);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<Card> cards;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);

        // skip "Card"
        ss.ignore(4);
        // Read id
        int id;
        ss >> id;
        // Skip ':'
        ss.ignore(1);

        // read winning numbers until "|"
        std::vector<int> winning_numbers;
        while (ss.peek() != '|')
        {
            int n;
            ss >> n;
            winning_numbers.push_back(n);
            // Skip " "
            ss.ignore(1);
        }

        // Skip "|"
        ss.ignore(1);

        // Read numbers
        std::vector<int> numbers;
        for (int n; ss >> n;)
            numbers.push_back(n);

        cards.push_back({id, winning_numbers, numbers});
    }

    return cards;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    auto cards = parse_cards(argv[1]);
    // Part 1
    int part1 = std::accumulate(cards.begin(), cards.end(), 0, [](int a, Card &b)
                                { return a + std::max(1 << (b.winning_count() - 1), 0); });
    std::cout << "Part 1: " << part1 << '\n'; // 21821

    // Part 2
    std::vector<int> card_counts(cards.size(), 1);
    for (auto &card : cards)
    {
        int winning_count = card.winning_count();
        for (int i = 1; i <= winning_count; i++)
            card_counts[card.id + i - 1] += card_counts[card.id - 1];
    }
    int part2 = std::accumulate(card_counts.begin(), card_counts.end(), 0);

    std::cout << "Part 2: " << part2 << '\n'; // 5539496

    return 0;
}
