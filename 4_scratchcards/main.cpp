#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <sstream>

struct Card
{
    int ID = -1;
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

size_t card_score(const Card& card)
{
    int matches = count_matches(card);
    
    if (matches == 0)
        return 0;
    
    return 1UL << (matches-1);
}

std::unordered_map<int, Card> load_cards_from_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("File could not be opened");
    }

    std::unordered_map<int, Card> cards;

    std::string line;
    while(std::getline(file, line))
    {
        std::string null;
        int ID;
        std::istringstream iss(line);
        iss >> null >> ID >> null;

        Card card {ID};
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

        cards[ID] = std::move(card);
    }

    return cards;
}

void scratchcards()
{
    auto cards = load_cards_from_file("../input.txt");

// =========== PART 1 ===========

    size_t total = 0;

    for (const auto& [ID, card] : cards)
        total += card_score(card);

    std::cout << total << std::endl;


// =========== PART 2 ===========

    std::queue<int> to_process;
    for (const auto& [ID, card] : cards)
        to_process.push(ID);
    
    size_t processed = 0;
    
    while (!to_process.empty())
    {
        int matches = count_matches(cards[to_process.front()]);
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
