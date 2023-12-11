#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

struct Coord
{
    int x = 0;
    int y = 0;
};

struct Map
{
    Coord size;
    std::vector<Coord> coords;
};

Map load_map_from_file(const std::string& path)
{
    std::ifstream file(path);

    if(!file)
    {
        throw std::runtime_error("Could not open file");
    }

    Map result;

    std::string line;
    for (size_t y = 0; std::getline(file, line); ++y)
    {
        result.size.y = y;
        for (size_t x = 0; x < line.length(); ++x)
        {
            result.size.x = x;
            if (line[x] == '#')
                result.coords.emplace_back(x, y);
        }
    }
    ++result.size.x;
    ++result.size.y;

    return result;
}

void print_map(const Map& map)
{
    for (size_t y = 0; y < map.size.y; ++y)
    {
        for (size_t x = 0; x < map.size.x; ++x)
        {
            bool is_galaxy = false;
            for (const auto& c : map.coords)
            {
                if (c.x == x && c.y == y)
                {
                    is_galaxy = true;
                    break;
                }
            }
            std::cout << (is_galaxy ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

Map expand_map(Map map, size_t expansion_ratio)
{
    --expansion_ratio; // if set to 2, we want to double each gap, aka add 1

    std::vector<bool> empty_cols(map.size.x, true);
    std::vector<bool> empty_rows(map.size.y, true);

    for (const auto& c : map.coords)
    {
        empty_cols[c.x] = false;
        empty_rows[c.y] = false;
    }

    size_t x_expansion = 0;
    for (size_t x = 0; x < empty_cols.size(); ++x)
    {
        if(!empty_cols[x])
            continue;
        
        map.size.x += expansion_ratio;

        for (auto& c : map.coords)
            if (c.x > x + x_expansion)
                c.x += expansion_ratio;
        
        x_expansion += expansion_ratio;
    }

    size_t y_expansion = 0;
    for (size_t y = 0; y < empty_cols.size(); ++y)
    {
        if(!empty_rows[y])
            continue;
        
        map.size.y += expansion_ratio;

        for (auto& c : map.coords)
            if (c.y > y + y_expansion)
                c.y += expansion_ratio;
        
        y_expansion += expansion_ratio;
    }

    return map;
}

size_t distance(Coord a, Coord b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

size_t sum_of_distances(const Map& map)
{
    size_t total_distance = 0;
    for (size_t i = 0; i < map.coords.size() - 1; i++)
    {
        for (size_t j = i+1; j < map.coords.size(); j++)
        {
            total_distance += distance(map.coords[i], map.coords[j]);
        }
    }
    return total_distance;
}

int main()
{
    Map map = load_map_from_file("../input.txt");
    
    Map map_pt1 = expand_map(map, 2);
    std::cout << sum_of_distances(map_pt1) << std::endl;

    Map map_pt2 = expand_map(map, 1'000'000);
    std::cout << sum_of_distances(map_pt2) << std::endl;
}
