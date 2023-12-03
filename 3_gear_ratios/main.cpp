// https://adventofcode.com/2023/day/3#part2

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

typedef std::vector<std::string> EngineMap;
typedef std::vector<std::vector<bool>> BitMask;
typedef std::pair<int,int> Coord;

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

bool at(BitMask mask, int x, int y)
{
    if (y < 0 || y >= mask.size() || x < 0 || x >= mask[y].size())
        return false;

    return mask[y][x];
}

std::vector<Coord> get_symbol_coords(const EngineMap& engine_map)
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

BitMask mask_of_size(const EngineMap& map)
{
    return BitMask(map.size(), std::vector<bool>(map[0].size(), false));
}

void mask_numbers_touching_coords(BitMask& mask, const EngineMap& map, const std::vector<Coord> coords)
{
    // stage 1: mark all digits neighbouring a 'symbol'
    for (const auto [x, y] : coords)
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx)
            {
                int y_curr = y + dy;
                int x_curr = x + dx;

                if (y_curr < 0 ||
                    y_curr > map.size() ||
                    x_curr < 0 ||
                    x_curr > map[y].length() ||
                    dx == 0 && dy == 0)
                    continue;
                
                if (std::isdigit(map[y_curr][x_curr]))
                    mask[y_curr][x_curr] = true;
            }

    // stage 2: propagate the mask to all numbers neighbouring a 'symbol'
    for (int y = 0; y < map.size(); ++y)
        for (int x = 0; x < map[y].length(); ++x)
            if (mask[y][x])
            {
                int x_curr = std::max(x - 1, 0);
                while (std::isdigit(map[y][x_curr]) && x_curr >= 0)
                {
                    mask[y][x_curr] = true;
                    --x_curr;
                }
                x_curr = std::min(x + 1, static_cast<int>(map[y].length()) - 1);
                while (std::isdigit(map[y][x_curr]) && x_curr < map[y].length())
                {
                    mask[y][x_curr] = true;
                    ++x_curr;
                }
            }
}

void erase_unmasked(EngineMap& map, const BitMask& mask)
{
    for (int y = 0; y < map.size(); ++y)
        for (int x = 0; x < map[y].length(); ++x)
            if (!mask[y][x])
                map[y][x] = ' ';
}

size_t sum_of_numbers(const EngineMap& map)
{
    size_t total = 0;
    for (const auto& line : map)
    {
        std::istringstream iss(line);
        int number;

        while (iss >> number)
        {
            total += number;
        }
    }

    return total;
}

size_t sum_of_gear_ratios(const EngineMap& map, const BitMask& mask, const std::vector<Coord>& all_symbols)
{
    std::vector<Coord> gears;
    size_t total = 0;

    for (const auto [x, y] : all_symbols)
    {
        if (map[y][x] != '*')
            continue;

        int number_count = 0;

        for (int i = -1; i <= 1; i += 2) // i = [-1, 1]
        {
            number_count += at(mask, x + i, y); // count number on the side

            number_count += at(mask, x - 1, y + i) ||  at(mask, x, y + i) || at(mask, x + 1, y + i); // count number on the top/bottom
            number_count += at(mask, x - 1, y + i) && !at(mask, x, y + i) && at(mask, x + 1, y + i); // 101 pattern counts as 2 numbers
        }
        
        if (number_count == 2)
            gears.push_back(std::make_pair(x, y));
    }

    // print gears
    for (const auto [x, y] : gears)
        std::cout << x << " " << y << std::endl;

    for (const auto [x, y] : gears)
    {
        std::vector<Coord> potential_starts;

        // find all masked coords
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx)
            {
                int y_curr = y + dy;
                int x_curr = x + dx;

                if (y_curr < 0 ||
                    y_curr > map.size() ||
                    x_curr < 0 ||
                    x_curr > map[y].length() ||
                    dx == 0 && dy == 0)
                    continue;
                
                if (mask[y_curr][x_curr])
                    potential_starts.push_back(std::make_pair(x_curr, y_curr));
            }
        
        // advance each coord's x until we hit an unmasked coord
        for (auto& [x_p, y_p] : potential_starts)
        {
            while (at(mask, x_p - 1, y_p))
                --x_p;
        }

        Coord first = potential_starts[0];
        Coord second = potential_starts[1];
        for (int i = 2; i < potential_starts.size() && first == second; ++i)
        {
            second = potential_starts[i];
        }

        int gear_1, gear_2;

        std::string first_str = map[first.second].substr(first.first);
        std::istringstream first_iss(first_str);
        first_iss >> gear_1;

        std::string second_str = map[second.second].substr(second.first);
        std::istringstream second_iss(second_str);
        second_iss >> gear_2;

        std::cout << gear_1 << " " << gear_2 << std::endl;

        total += gear_1 * gear_2;
    }

    return total;
}

void gear_ratios()
{
    EngineMap engine_map = load_engine_map("../input.txt");
    BitMask mask = mask_of_size(engine_map);

    // stage 1: find all symbols
    auto symbol_coords = get_symbol_coords(engine_map);

    // stage 2: mark all numbers touching a 'symbol'
    mask_numbers_touching_coords(mask, engine_map, symbol_coords);

    // stage 3: erase all unmasked characters
    size_t total_ratios = sum_of_gear_ratios(engine_map, mask, symbol_coords);

    // stage 4 (for pt.2 only): calculate total of gear ratios
    erase_unmasked(engine_map, mask);

    // stage 5: calculate total of all remaining numbers
    size_t total = sum_of_numbers(engine_map);    

    // stage 6: print results
    std::cout << "results: " << total << " " << total_ratios << std::endl;
}

int main()
{
    gear_ratios();
}
