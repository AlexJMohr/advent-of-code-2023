#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>

struct MaxGame
{
    MaxGame(int id) : id(id) {}

    bool is_possible(int red, int green, int blue) const
    {
        return num_seen_red <= red && num_seen_green <= green && num_seen_blue <= blue;
    }

    void update_red(int red)
    {
        num_seen_red = std::max(num_seen_red, red);
    }

    void update_green(int green)
    {
        num_seen_green = std::max(num_seen_green, green);
    }

    void update_blue(int blue)
    {
        num_seen_blue = std::max(num_seen_blue, blue);
    }

    int id;
    int num_seen_red = 0;
    int num_seen_green = 0;
    int num_seen_blue = 0;
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    std::string line;

    std::vector<MaxGame> games;

    while (std::getline(file, line))
    {
        std::stringstream game_stream(line);
        // throw away "Game "
        game_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');

        // parse game id
        int game_id;
        game_stream >> game_id;
        MaxGame game(game_id);

        std::cout << "Game " << game_id << ":" << std::endl;

        // throw away ": "
        game_stream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        game_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');

        std::string handful;
        while (std::getline(game_stream, handful, ';'))
        {
            std::stringstream hand_stream(handful);

            std::string pair;
            while (std::getline(hand_stream, pair, ','))
            {
                std::stringstream pair_stream(pair);
                int count;
                std::string color;

                pair_stream >> count;
                pair_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
                std::getline(pair_stream, color, ',');
                pair_stream >> color;

                if (color == "red")
                    game.update_red(count);
                else if (color == "green")
                    game.update_green(count);
                else if (color == "blue")
                    game.update_blue(count);

                // throw away " "
                hand_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            }

            // throw away " "
            game_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        }

        games.push_back(game);
    }

    const int MAX_RED = 12;
    const int MAX_GREEN = 13;
    const int MAX_BLUE = 14;
    int game_id_sum = 0;
    int total_power = 0;
    for (auto &game : games)
    {
        if (game.is_possible(MAX_RED, MAX_GREEN, MAX_BLUE))
            game_id_sum += game.id;
        total_power += (game.num_seen_red * game.num_seen_green * game.num_seen_blue);
    }

    std::cout << "Part 1: " << game_id_sum << '\n'; // 2771
    std::cout << "Part 2: " << total_power << '\n'; // 70924

    return 0;
}
