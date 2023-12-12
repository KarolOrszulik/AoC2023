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

Record unfold_record(const Record& to_unfold, size_t unfold_ratio = 5)
{
    Record result {to_unfold.springs, to_unfold.streaks};
    for (int i = 0; i < unfold_ratio - 1; i++)
    {
        result.springs.push_back('?');
        result.springs.insert(result.springs.end(), to_unfold.springs.begin(), to_unfold.springs.end());
        result.streaks.insert(result.streaks.end(), to_unfold.streaks.begin(), to_unfold.streaks.end());
    }
    return result;
}

int main()
{
    std::vector<Record> records = parse_lines("../test.txt");

    // ======= PART 1 =======
    size_t total_arrangements = 0;
    
    for (const auto& r : records)
        total_arrangements += options_for_record(r);
    
    std::cout << total_arrangements << std::endl;


    // ======= PART 2 =======
    // size_t total_arrangements_pt2 = 0;
    
    // for (const auto& r : records)
    //     total_arrangements_pt2 += options_for_record(unfold_record(r));
    
    // std::cout << total_arrangements_pt2 << std::endl;

    return 0;
}


// Below this line is code which has no chances of working, ever.
// =============================================================================================================

size_t f(const Record& rec, const bool start_with_dot, const size_t str_start, const size_t vec_start) try
{
    size_t str_adv = 0; // string advancement
    size_t vec_adv = 0; // vector advancement

    if (str_start >= rec.springs.length() && vec_start < rec.streaks.size())
        return 0; // if reached end of string, but not vector, this path was wrong and contains 0 solutions

    if (rec.springs.at(str_start) != '?')
        throw std::runtime_error("Function called incorrectly - not starting at a '?'");

    char prev = start_with_dot ? '.' : '#';

    size_t current_streak = start_with_dot ? 0 : 1;
    
    while ( !(prev == '.' && rec.springs.at(str_start+str_adv) == '?') || str_adv == 0 )
    {
        prev = rec.springs.at(str_start+str_adv);
        str_adv++;

        switch (rec.springs.at(str_start + str_adv))
        {
        case '#':
            current_streak++;
            break;
        case '.':
            if (current_streak != 0 && current_streak != rec.streaks.at(vec_start + vec_adv++))
                return 0;
            current_streak = 0;
            break;
        case '?':
            break;
        default:
            throw std::runtime_error("lol what");
        }
    }

    if (current_streak != 0 && current_streak != rec.streaks.at(vec_start + vec_adv++))
        return 0;
    
    return f(rec, true, str_start+str_adv+1, vec_start+vec_adv+1) + f(rec, false, str_start+str_adv+1, vec_start+vec_adv+1);

}
catch (std::out_of_range& e)
{
    return 0;
}

size_t options_for_record_fast(const Record& record) try
{
    size_t str_idx = 0;
    size_t vec_idx = 0;

    size_t current_length = 0;
    for (char c : record.springs)
    {
        switch (c)
        {
        case '#':
            current_length++;
            break;

        case '.':
            if (current_length != 0 && current_length != record.streaks.at(str_idx++))
                return 0;
            vec_idx++;
            current_length = 0;
            break;
        
        default:
            goto hell;
        }
    }

hell:
    if (current_length != 0 && current_length != record.streaks.at(str_idx++))
        return 0;

    return f(record, true, str_idx, vec_idx) + f(record, false, str_idx, vec_idx);
}
catch (const std::range_error& e)
{
    return 0;
}

