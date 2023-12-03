// https://adventofcode.com/2023/day/3#part2

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

typedef std::vector<std::string> EngineMap;
typedef std::vector<std::vector<bool>> BitMask;

EngineMap load_engine_map(const std::string& file_path)
{
    EngineMap result;

    std::ifstream file(file_path);
    if(!file)
    {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    while(std::getline(file, line))
    {
        result.push_back(line);
    }

    return result;
}

std::vector<std::pair<int,int>> get_symbol_coords(const EngineMap& engine_map)
{
    std::vector<std::pair<int,int>> symbols;

    for (int y = 0; y < engine_map.size(); ++y)
        for (int x = 0; x < engine_map[y].length(); ++x)
        {
            const char current = engine_map[y][x];
            if (std::isdigit(current) || current == '.')
                continue;

            symbols.push_back(std::make_pair(x, y));
        }

    return symbols;
}

bool at(BitMask mask, int x, int y)
{
    if (y < 0 || y >= mask.size() || x < 0 || x >= mask[y].size())
        return false;

    return mask[y][x];
}

void gear_ratios()
{
    EngineMap engine_map = load_engine_map("../test.txt");
    BitMask mask(engine_map.size(), std::vector<bool>(engine_map[0].size(), false));

    auto symbol_coords = get_symbol_coords(engine_map);

    // stage 1: mark all digits neighbouring a 'symbol'
    for (const auto [x, y] : symbol_coords)
    {
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx)
            {
                int y_curr = y + dy;
                int x_curr = x + dx;

                if (x_curr < 0 ||
                    x_curr > engine_map[y].length() ||
                    y_curr < 0 ||
                    y_curr > engine_map.size() ||
                    dx == 0 && dy == 0)
                    continue;
                
                if (std::isdigit(engine_map[y_curr][x_curr]))
                    mask[y_curr][x_curr] = true;
            }
    }

    // stage 1.5 (for pt.2 only): find gears
    std::vector<std::pair<int,int>> gears;
    for (const auto [x, y] : symbol_coords)
    {
        if (engine_map[y][x] != '*')
            continue;

        int number_count = 0;

        for (int i = -1; i <= 1; i += 2) // i = [-1, 1]
        {
            number_count += at(mask, x + 1, y); // count number on the side

            number_count += at(mask, x - 1, y + i) ||  at(mask, x, y + i) || at(mask, x + 1, y + i); // count number on the top/bottom
            number_count += at(mask, x - 1, y + i) && !at(mask, x, y + i) && at(mask, x + 1, y + i); // 101 pattern counts as 2 numbers
        }
        
        if (number_count == 2)
            gears.push_back(std::make_pair(x, y));
    }

    // stage 2: mark all numbers of digits neighbouring a 'symbol'
    for (int y = 0; y < engine_map.size(); ++y)
        for (int x = 0; x < engine_map[y].length(); ++x)
            if (mask[y][x])
            {
                int x_curr = std::max(x - 1, 0);
                while (std::isdigit(engine_map[y][x_curr]) && x_curr >= 0)
                {
                    mask[y][x_curr] = true;
                    --x_curr;
                }
                x_curr = std::min(x + 1, static_cast<int>(engine_map[y].length()) - 1);
                while (std::isdigit(engine_map[y][x_curr]) && x_curr < engine_map[y].length())
                {
                    mask[y][x_curr] = true;
                    ++x_curr;
                }
            }

    // stage 3: erase all non-marked characters
    for (int y = 0; y < engine_map.size(); ++y)
        for (int x = 0; x < engine_map[y].length(); ++x)
            if (!mask[y][x])
                engine_map[y][x] = ' ';

    // stage 4: sum all remaining numbers
    size_t total = 0;
    for (const auto& line : engine_map)
    {
        std::istringstream iss(line);
        int number;

        while (iss >> number)
        {
            total += number;
        }
    }

    // stage 4.5 (for pt.2 only): sum all gear ratios
    size_t total_ratios = 0;
    for (const auto [x, y] : gears)
    {
        
    }

    // stage 5: print results;
    std::cout << total << std::endl;
    std::cout << total_ratios << std::endl;
}

int main()
{
    gear_ratios();
}
