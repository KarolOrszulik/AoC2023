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

        if (char_counts.size() == 5)
            return HIGH_CARD;
    }
};

std::vector<Game> load_games_from_file(const std::string& path)
{
    std::ifstream file("../test.txt");

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

int main(int, char**)
{
    auto games = load_games_from_file("../test.txt");

    // for (const auto& g : games)
    //     std::cout << g.hand << " " << g.bid << " " << static_cast<int>(g.type()) << std::endl;

    std::sort(games.begin(), games.end(), [](const Game& a, const Game& b) {
        Type a_type = a.type();
        Type b_type = b.type();

        // if (a_type != b_type)
            return a_type < b_type;
        
        // for (int i = 0; i < a.hand.length(); i++)
        // {

        // }
    });

    for (const auto& g : games)
        std::cout << g.hand << " " << g.bid << " " << static_cast<int>(g.type()) << std::endl;
}
