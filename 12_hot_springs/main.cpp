#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <algorithm>
#include <numeric>

struct Record
{
    std::string springs;
    std::vector<size_t> streaks;
};

// std::vector<size_t> count_streaks(const std::string& to_check)
// {
//     std::vector<size_t> result;

//     size_t current_length = 0;
//     for (char c : to_check)
//     {
//         switch (c)
//         {
//         case '#':
//             current_length++;
//             break;

//         case '.':
//             if (current_length > 0)
//                 result.push_back(current_length);
//             current_length = 0;
//             break;
        
//         default:
//             throw std::runtime_error(std::string("Unsupported char: ") + c);
//         }
//     }

//     if (current_length > 0)
//         result.push_back(current_length);
    
//     return result;
// }

bool compare_streaks(const std::string& to_check, const std::vector<size_t>& reference) try
{
    size_t ref_idx = 0;

    size_t current_length = 0;
    for (char c : to_check)
    {
        switch (c)
        {
        case '#':
            current_length++;
            break;

        case '.':
            if (current_length != 0 && current_length != reference.at(ref_idx++))
                return false;

            current_length = 0;
            break;
        
        default:
            throw std::runtime_error(std::string("Unsupported char: ") + c);
        }
    }

    if (current_length != 0 && current_length != reference.at(ref_idx++)
        || ref_idx != reference.size())
        return false;
    
    return true;
}
catch (const std::out_of_range& e)
{
    return false;
}

size_t options_for_record(const Record& record)
{
    const size_t qms = std::ranges::count(record.springs, '?');

    size_t result = 0;

    for (size_t bitmask = 0; bitmask < 1<<qms; bitmask++)
    {
        std::string swapped = record.springs;
        size_t mask_idx = 0;
        for (size_t i = 0; i < swapped.length(); i++)
        {
            if (swapped[i] == '?')
                swapped[i] = (1<<mask_idx++ & bitmask) ? '.' : '#';
        }

        // std::cout << std::endl << swapped;

        result += compare_streaks(swapped, record.streaks);
    }

    return result;
}

std::vector<Record> parse_lines(const std::string& path)
{
    std::ifstream file(path);

    if (!file)
        throw std::runtime_error("Could not open file");
    
    std::vector<Record> result;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        Record rec;
        iss >> rec.springs;

        size_t streak = 0;
        while (iss >> streak)
        {
            rec.streaks.push_back(streak);
            char comma = 0;
            iss >> comma; // will fail on the last streak but that's not a problem
        }

        result.push_back(std::move(rec));
    }

    return result;
}

int main()
{
    std::vector<Record> records = parse_lines("../input.txt");

    size_t total_arrangements = 0;
    
    for (const auto& r : records)
        total_arrangements += options_for_record(r);

    std::cout << total_arrangements << std::endl;

    return 0;
}
