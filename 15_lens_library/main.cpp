#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <array>
#include <algorithm>

std::vector<std::string> load_seqs_from_file(const std::string& path)
{
    std::ifstream file(path);

    if (!file) throw std::runtime_error("Could not open file");

    std::vector<std::string> seqs;

    std::string seq;
    while (std::getline(file, seq, ','))
    {
        seqs.push_back(std::move(seq));
    }

    return seqs;
}

#include <stdint.h>
uint8_t HASH(const std::string& str)
{
    uint16_t hash = 0;
    for (const char ch : str)
    {
        hash += static_cast<uint16_t>(ch);
        hash *= 17;
        hash %= 256;
    }
    return static_cast<uint8_t>(hash);
}

struct Lens
{
    std::string label;
    uint8_t focal = 0;
};

bool operator==(const Lens& a, const Lens& b)
{
    return a.label == b.label;
}

struct Step
{
    Lens lens;
    bool op_add = false;
};

std::vector<Step> parse_steps(const std::vector<std::string>& seqs)
{
    std::vector<Step> steps;

    for (const auto seq : seqs)
    {
        size_t op_idx = seq.find_first_of("-=");

        if (op_idx == std::string::npos) throw std::runtime_error("Operation not specified correctly");

        bool op_add = seq[op_idx] == '=';

        Lens lens {seq.substr(0, op_idx), static_cast<uint8_t>(op_add ? (seq[op_idx + 1] - '0') : 0)};
        steps.emplace_back(lens, op_add);
    }

    return steps;
}

int main()
{
    std::vector<std::string> seqs = load_seqs_from_file("../input.txt");

    // part 1
    size_t sum_of_hashes = 0;
    for (const auto s : seqs)
        sum_of_hashes += HASH(s);
    
    std::cout << sum_of_hashes << std::endl;


    // part 2
    std::vector<Step> steps = parse_steps(seqs);

    std::array<std::list<Lens>, 256> HASHMAP;
    for (const auto step : steps)
    {
        size_t box_idx = HASH(step.lens.label);

        auto it = std::find(HASHMAP[box_idx].begin(), HASHMAP[box_idx].end(), step.lens);

        if (step.op_add) // operation: add
        {
            if (it != HASHMAP[box_idx].end())
                *it = step.lens;
            else
                HASHMAP[box_idx].push_back(step.lens);
        }
        else // operation: delete
        {
            if (it != HASHMAP[box_idx].end())
                HASHMAP[box_idx].erase(it);
        }
    }

    size_t total_focusing_power = 0;

    for (size_t i = 0; i < HASHMAP.size(); i++)
    {
        size_t j = 1;
        for (const auto lens : HASHMAP[i])
        {
            total_focusing_power += (i+1) * (j) * (lens.focal);
            j++;
        }
    }

    std::cout << total_focusing_power << std::endl;
}
