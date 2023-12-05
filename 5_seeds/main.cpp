#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

struct MapRule
{
    long dest   = 0;
    long src    = 0;
    long length = 0;
};

struct Range
{
    long start  = 0;
    long length = 0;
};

long seed_location(long seed, const std::vector<std::vector<MapRule>>& mappings)
{
    for (const auto map : mappings)
        for (const auto rule : map)
            if (seed >= rule.src && seed < rule.src + rule.length)
            {
                seed += rule.dest - rule.src;
                break;
            }
    
    return seed;
}

void seeds()
{
    std::vector<std::vector<MapRule>> mappings;

    std::vector<long> seeds;
    std::vector<Range> seed_ranges;

    std::ifstream file("../input.txt");
    if (!file)
    {
        throw std::runtime_error("Could not open input file");
    }

    std::string line;
    if (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string null;
        iss >> null;

        long n1 = -1, n2 = -1;
        while (iss >> n1 >> n2)
        {
            // for pt.1: treat them as 2 separate numbers
            seeds.push_back(n1);
            seeds.push_back(n2);

            // for pt.2: treat them as a range
            seed_ranges.push_back({n1, n2});
        }
    }

    std::vector<MapRule> curr_map;
    MapRule curr_desc;
    while(std::getline(file, line))
    {
        if(line.empty())
        {
            continue;
        }
        else if(line.ends_with("map:")) // new map has started, so save the already gathered one
        {
            if (curr_desc.length != 0) // only ignore the first "map:" end, input has no 0-lengths as that would make no sense
            {
                mappings.push_back(std::move(curr_map));
                curr_map.clear();
            }
        }
        else
        {
            std::istringstream iss(line);
            iss >> curr_desc.dest >> curr_desc.src >> curr_desc.length;
            curr_map.push_back(curr_desc);
        }
    }
    mappings.push_back(std::move(curr_map)); // after file has ended, push remaining map

    for (auto& seed : seeds)
        seed = seed_location(seed, mappings);
    std::cout << *std::min_element(seeds.begin(), seeds.end()) << std::endl;
    

    long pt2_closest = __LONG_MAX__;
    long total_seeds = std::accumulate(seed_ranges.begin(), seed_ranges.end(), 0L,
        [](long total, const Range& current){ return total + current.length; });
    long counter = 0;
    for (const auto& range : seed_ranges)
    {
        std::cout << "Starting processing range of length: " << range.length << std::endl;
        for (long seed = range.start; seed < range.start + range.length; seed++)
        {
            long location = seed_location(seed, mappings);
            pt2_closest = std::min(pt2_closest, location);
        }
    }
    std::cout << pt2_closest << std::endl;
}

int main()
{
    seeds();
}
