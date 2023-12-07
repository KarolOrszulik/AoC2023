#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

struct Card
{
    std::unordered_set<int> winning;
    std::unordered_set<int> numbers;
};

int count_matches(const Card& card)
{
    int matches = 0;

    for (const auto w : card.winning)
        if (card.numbers.count(w))
            matches++;
    
    return matches;
}

size_t matches_to_score(int matches)
{
    if (matches == 0)
        return 0;
    
    return 1UL << (matches-1);
}

std::vector<Card> load_cards_from_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("File could not be opened");
    }

    std::vector<Card> cards;

    std::string line;
    while(std::getline(file, line))
    {
        std::string null;
        int ID;
        std::istringstream iss(line);
        iss >> null >> ID >> null;

        Card card;
        bool reached_numbers = false;

        std::string str;
        while (iss >> str)
        {
            if (str == "|")
            {
                reached_numbers = true;
                continue;
            }

            if (!reached_numbers)
                card.winning.insert(std::stoi(str));
            else
                card.numbers.insert(std::stoi(str));
        }

        cards.push_back(std::move(card));
    }

    return cards;
}

void scratchcards()
{
    auto cards = load_cards_from_file("../input.txt");
    std::unordered_map<int, int> match_count_cache;

// =========== PART 1 ===========

    size_t total = 0;

    for (int i = 0; i < cards.size(); i++)
    {
        int matches = count_matches(cards[i]);
        match_count_cache[i] = matches;

        total += matches_to_score(matches);
    }

    std::cout << total << std::endl;


// =========== PART 2 ===========

    std::queue<int> to_process;
    for (size_t i = 0; i < cards.size(); ++i)
        to_process.push(i);
    
    size_t processed = 0;
    
    while (!to_process.empty())
    {
        // int matches = count_matches(cards[to_process.front()]);
        int matches = match_count_cache[to_process.front()];
        for (int i = 0; i < matches; i++)
        {
            to_process.push(to_process.front() + i + 1);
        }
        to_process.pop();
        ++processed;

        // if (processed % 100'000 == 0)
        //     std::cout << "processed = " << processed << "; to_process.size() = " << to_process.size() << std::endl;
    }

    std::cout << processed << std::endl;
}

int main(int, char**)
{
    scratchcards();
}