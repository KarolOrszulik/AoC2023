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

std::vector<size_t> lines_above_h_reflection(const Map& map)
{
    std::vector<size_t> result;

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
            result.push_back(above);
    }

    if (result.empty())
        result.push_back(0);

    return result;
}

std::vector<size_t> lines_left_of_v_reflection(const Map& map)
{

    std::vector<size_t> result;

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
            result.push_back(to_left);
    }

    if (result.empty())
        result.push_back(0);

    return result;
}

size_t part_1(const std::vector<Map>& maps)
{
    size_t result = 0;

    for (const auto& m : maps)
        result += 100 * lines_above_h_reflection(m).front() + lines_left_of_v_reflection(m).front();

    return result;
}

size_t part_2(const std::vector<Map>& maps)
{
    size_t result = 0;

    size_t _counter = 0;
    for (const auto& m : maps)
    {
        size_t original_a = lines_above_h_reflection(m)[0];
        size_t original_l = lines_left_of_v_reflection(m)[0];

        bool found = false;
        for (size_t y = 0; y < m.size() && !found; y++)
        {
            for (size_t x = 0; x < m[y].size() && !found; x++)
            {

                Map swapped = m;
                swapped[y][x] = !swapped[y][x];

                size_t new_a = lines_above_h_reflection(swapped).front();
                if (new_a == original_a)
                    new_a = lines_above_h_reflection(swapped).back();

                size_t new_l = lines_left_of_v_reflection(swapped).front();
                if (new_l == original_l)
                    new_l = lines_left_of_v_reflection(swapped).back();

                bool found_smudge = (new_a != 0 && new_a != original_a) || (new_l != 0 && new_l != original_l);

                if (found_smudge)
                {
                    found = true;
                    if (original_a != new_a)
                        result += 100 * new_a;
                    else
                        result += new_l;
                }
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
