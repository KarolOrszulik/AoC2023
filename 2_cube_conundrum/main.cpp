// https://adventofcode.com/2023/day/2

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

enum Color
{
    INVALID = -1,
    RED,
    GREEN,
    BLUE,
};

struct Hand
{
    int count;
    Color color_index;
};

struct Turn
{
    int counts[3] = {};
};

struct Game
{
    size_t ID;
    std::vector<Turn> turns;
};

Hand get_hand(const std::string& hand_string)
{
    std::istringstream iss(hand_string);

    Hand result;

    std::string color_string;

    iss >> result.count >> color_string;

    if (color_string.back() == ',' || color_string.back() == ';')
        color_string.pop_back();

    if (color_string == "red")
        result.color_index = RED;
    else if (color_string == "green")
        result.color_index = GREEN;
    else if (color_string == "blue")
        result.color_index = BLUE;
    else
        throw std::runtime_error("Invalid color provided: " + color_string);
    
    return result;
}

Turn get_turn(const std::string& turn_string)
{
    std::istringstream iss(turn_string);

    Turn result;

    std::string hand_line;
    while(std::getline(iss, hand_line, ','))
    {
        Hand hand = get_hand(hand_line);
        result.counts[hand.color_index] = hand.count;
    }
    
    return result;
}

Game get_game(const std::string game_string)
{
    std::istringstream iss(game_string);

    Game result;

    std::string null_s;
    char null_c;

    iss >> null_s >> result.ID >> null_c;

    std::string turn_string;
    while(std::getline(iss, turn_string, ';'))
    {
        result.turns.push_back(get_turn(turn_string));
    }

    return result;
}

std::vector<Game> parse_input(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Input file not found!" << std::endl;
        return {};
    }

    std::vector<Game> result;

    std::string line;
    while(std::getline(file, line))
    {
        result.push_back(get_game(line));
    }

    return result;
}

bool is_possible(const Game& game, const Turn& maxima)
{
    for (const Turn& turn : game.turns)
    {
        for (int i = 0; i < 3; i++)
        {
            if (turn.counts[i] > maxima.counts[i])
                return false;
        }
    }

    return true;
}

void cube_conundrum_pt1()
{
    static const Turn maxima {{12,13,14}};

    std::vector<Game> games = parse_input("../input.txt");

    size_t sum = 0;

    for (const Game& game : games)
    {
        if (is_possible(game, maxima))
        {
            // std::cout << game.ID << std::endl;
            sum += game.ID;
        }
    }

    std::cout << sum << std::endl;
}

void cube_conundrun_pt2()
{
    std::vector<Game> games = parse_input("../input.txt");

    size_t sum = 0;

    for (const Game& game : games)
    {
        Turn minimum;

        for (const Turn& turn : game.turns)
        {
            for (int i = 0; i < 3; i++)
            {
                minimum.counts[i] = std::max(minimum.counts[i], turn.counts[i]);
            }
        }

        size_t power = 1;
        for (int i = 0; i < 3; i++)
            power *= minimum.counts[i];

        // std::cout << power << std::endl;
        sum += power;
    }

    std::cout << sum << std::endl;
}

int main(int, char**)
{
    cube_conundrum_pt1();
    cube_conundrun_pt2();
}
