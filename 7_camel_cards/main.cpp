#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm>

enum Type
{
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OAK,
    FULL_HOUSE,
    FOUR_OAK,
    FIVE_OAK
};

struct Game
{
    std::string hand;
    int bid = -1;

    Type type() const
    {
        std::unordered_map<char, int> char_counts;
        for (const auto c : hand)
            char_counts[c]++;
        
        if (char_counts.size() == 1)
            return FIVE_OAK;
        
        if (char_counts.size() == 2)
        {
            int first_count = char_counts.begin()->second;
            if (first_count == 1 || first_count == 4)
                return FOUR_OAK;
            else
                return FULL_HOUSE;
        }

        if (char_counts.size() == 3)
        {
            for (const auto [ch, count] : char_counts)
                if (count == 3)
                    return THREE_OAK;
            
            return TWO_PAIR;
        }

        if (char_counts.size() == 4)
            return ONE_PAIR;

        return HIGH_CARD;
    }

    Type best_type() const
    {
        Type best = HIGH_CARD;

        for (const auto [to_swap, _] : char_strengths)
        {
            if (to_swap == 'J')
                continue;

            std::string hand_jokered = hand;

            for (char& c : hand_jokered)
                if (c == 'J')
                    c = to_swap;

            if (hand == hand_jokered)
                return type();

            Game swapped {hand_jokered};
            best = std::max(best, swapped.type());
        }
        return best;
    }

    static const std::unordered_map<char, int> char_strengths;
};

const std::unordered_map<char, int> Game::char_strengths = {
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    {'J', 11},
    {'Q', 12},
    {'K', 13},
    {'A', 14}
};

std::vector<Game> load_games_from_file(const std::string& path)
{
    std::ifstream file(path);

    if (!file)
    {
        throw std::runtime_error("Input file could not be opened");
    }
    
    std::vector<Game> games;

    std::string h;
    int b = -1;
    while(file >> h >> b)
    {
        games.push_back({h, b});
    }

    return games;
}

void cames_cards(const std::string& path, int part)
{
    auto games = load_games_from_file(path);

    std::sort(games.begin(), games.end(), [part](const Game& a, const Game& b) {
        Type a_type = part == 1 ? a.type() : a.best_type();
        Type b_type = part == 1 ? b.type() : b.best_type();

        if (a_type != b_type)
            return a_type < b_type;
        
        for (int i = 0; i < a.hand.length(); i++)
        {
            int a_strength = Game::char_strengths.at(a.hand[i]);
            int b_strength = Game::char_strengths.at(b.hand[i]);

            if (part == 2)
            {
                a_strength *= a.hand[i] != 'J';
                b_strength *= b.hand[i] != 'J';
            }

            if (a_strength != b_strength)
                return a_strength < b_strength;
        }

        return false;
    });

    for (const auto& g : games)
        std::cout << g.hand << " " << (part == 1 ? g.type() : g.best_type()) << std::endl;

    size_t total_winnings = 0;

    for (size_t i = 0; i < games.size(); i++)
        total_winnings += games[i].bid * (i+1);
    
    std::cout << total_winnings << std::endl;
}

int main(int argc, char** argv)
{
    std::string path = "../input.txt";

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path>" << std::endl;
        std::cout << "Falling back to default: " << path << std::endl;
    }
    else
    {
        path = argv[1];
    }
    
    cames_cards(path, 1);
    cames_cards(path, 2);
}
