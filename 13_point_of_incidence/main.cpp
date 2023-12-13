#include <iostream>
#include <vector>
#include <string>
#include <fstream>

typedef std::vector<std::vector<bool>> Map;

std::vector<Map> load_maps_from_file(const std::string& path)
{
    std::ifstream file(path);

    if (!file)
        throw std::runtime_error("Could not open file");
    
    std::vector<Map> result;

    Map current;
    std::string line;

    while(std::getline(file, line))
    {
        if (line.empty())
        {
            if (!current.empty())
            {
                result.push_back(std::move(current));
                current.clear();
            }
            continue;
        }

        std::vector<bool> row;

        for (const char c : line)
            row.push_back(c == '#');
        
        current.push_back(std::move(row));
    }

    if (!current.empty())
        result.push_back(std::move(current));

    return result;
}

size_t lines_above_h_reflection(const Map& map)
{
    for (size_t above = map.size() - 1; above > 0; above--)
    {
        size_t rows_to_check = std::min(above, map.size() - above);

        bool match = true;
        for (size_t i = 0; i < rows_to_check && match; i++)
        {
            size_t ya = above + i;
            size_t yb = above - i - 1;
           
            for (size_t x = 0; x < map[ya].size(); x++)
                if (map[ya][x] != map[yb][x])
                {
                    match = false;
                    break;
                }
        }
        if (match)
            return above;
    }

    return 0;
}

size_t lines_left_of_v_reflection(const Map& map)
{
    for (size_t to_left = map[0].size() - 1; to_left > 0; to_left--)
    {
        size_t cols_to_check = std::min(to_left, map[0].size() - to_left);

        bool match = true;
        for (size_t i = 0; i < cols_to_check && match; i++)
        {
            size_t xa = to_left + i;
            size_t xb = to_left - i - 1;
           
            for (size_t y = 0; y < map.size(); y++)
                if (map[y][xa] != map[y][xb])
                {
                    match = false;
                    break;
                }
        }
        if (match)
            return to_left;
    }

    return 0;
}

size_t part_1(const std::vector<Map>& maps)
{
    size_t result = 0;

    for (const auto& m : maps)
        result += 100 * lines_above_h_reflection(m) + lines_left_of_v_reflection(m);

    return result;
}

// the result of this function is not correct
// partly because I don't think I understand what
// exactly to do
size_t part_2(const std::vector<Map>& maps)
{
    size_t result = 0;

    size_t _counter = 0;
    for (const auto& m : maps)
    {
        _counter++;
        size_t original_a = lines_above_h_reflection(m);
        size_t original_l = lines_left_of_v_reflection(m);

        Map swapped = m;

        bool found = false;
        for (size_t y = 0; y < swapped.size() && !found; y++)
        {
            for (size_t x = 0; x < swapped[y].size(); x++)
            {
                swapped[y][x] = !swapped[y][x];

                size_t new_a = lines_above_h_reflection(swapped);
                size_t new_l = lines_left_of_v_reflection(swapped);

                if (new_a != 0 && new_a != original_a || new_l != 0 && new_l != original_l)
                {
                    if (original_a != new_a)
                        result += 100 * new_a;
                    else
                        result += new_l;

                    found = true;
                    break;
                }
                
                swapped[y][x] = !swapped[y][x];
            }
        }
    }

    return result;
}

int main()
{
    std::vector<Map> maps = load_maps_from_file("../input.txt");

    std::cout << part_1(maps) << std::endl;
    std::cout << part_2(maps) << std::endl;
}
