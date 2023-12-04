#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

struct Card
{
    std::vector<int> winning;
    std::vector<int> numbers;
};

void scratchcards()
{
    std::ifstream file("../test.txt");
    if (!file)
    {
        throw std::runtime_error("File could not be opened");
    }

    std::vector<Card> cards;

    std::string line;
    while(std::getline(file, line))
    {
        std::string null;
        std::istringstream iss(line);
        iss >> null >> null;

        Card card;
        bool reached_numbers = false;

        std::string num_str;
        while (iss >> num_str)
        {
            if (num_str == "|")
            {
                reached_numbers = true;
                continue;
            }

            if (!reached_numbers)
                card.winning.push_back(std::stoi(num_str));
            else
                card.numbers.push_back(std::stoi(num_str));
        }

        cards.emplace_back(card);
    }

}

int main(int, char**)
{
    std::cout << "Hello, from scratchcards!\n";
}
