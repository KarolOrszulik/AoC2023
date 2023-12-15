#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <unordered_map>
#include <algorithm>

enum Cell : int8_t
{
    INVALID = -1,
    EMPTY,
    ROUND,
    SQUARE
};

typedef std::vector<std::vector<Cell>> Grid;

Grid load_grid_from_file(const std::string& path)
{
    std::ifstream file(path);

    if (!file)
        throw std::runtime_error("Could not open file");

    Grid result;
    
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<Cell> row;
        for (const char c : line)
        {
            Cell cell = INVALID;
            switch (c)
            {
                case '.': cell = EMPTY; break;
                case 'O': cell = ROUND; break;
                case '#': cell = SQUARE; break;
                default: throw std::runtime_error(std::string("Unsupported char: " + c));
            }
            row.push_back(cell);
        }
        result.push_back(std::move(row));
    }

    return result;
}

void roll_rocks_north_fast(Grid& grid)
{
    for (int x = 0; x < grid[0].size(); x++)
    {
        int last_free = -1;

        for (int y = 0; y < grid.size(); y++)
        {
            Cell& current = grid[y][x];

            switch (current)
            {
            case EMPTY:
                if (last_free == -1)
                    last_free = y;
                break;

            case SQUARE:
                last_free = -1;
                break;

            case ROUND:
                if (last_free != -1)
                {
                    current = EMPTY;
                    grid[last_free][x] = ROUND;
                    last_free++;
                }
                break;
            
            default: throw std::runtime_error("Invalid cell type");
            }
        }
    }
}

void roll_rocks_south_fast(Grid& grid)
{
    for (int x = 0; x < grid[0].size(); x++)
    {
        int last_free = -1;

        for (int y = grid.size()-1; y >= 0; y--)
        {
            Cell& current = grid[y][x];

            switch (current)
            {
            case EMPTY:
                if (last_free == -1)
                    last_free = y;
                break;

            case SQUARE:
                last_free = -1;
                break;

            case ROUND:
                if (last_free != -1)
                {
                    current = EMPTY;
                    grid[last_free][x] = ROUND;
                    last_free--;
                }
                break;
            
            default: throw std::runtime_error("Invalid cell type");
            }
        }
    }
}

void roll_rocks_west_fast(Grid& grid)
{
    for (int y = 0; y < grid.size(); y++)
    {
        int last_free = -1;

        for (int x = 0; x < grid[y].size(); x++)
        {
            Cell& current = grid[y][x];

            switch (current)
            {
            case EMPTY:
                if (last_free == -1)
                    last_free = x;
                break;

            case SQUARE:
                last_free = -1;
                break;

            case ROUND:
                if (last_free != -1)
                {
                    current = EMPTY;
                    grid[y][last_free] = ROUND;
                    last_free++;
                }
                break;
            
            default: throw std::runtime_error("Invalid cell type");
            }
        }
    }
}

void roll_rocks_east_fast(Grid& grid)
{
    for (int y = 0; y < grid.size(); y++)
    {
        int last_free = -1;

        for (int x = grid[y].size()-1; x >= 0; x--)
        {
            Cell& current = grid[y][x];

            switch (current)
            {
            case EMPTY:
                if (last_free == -1)
                    last_free = x;
                break;

            case SQUARE:
                last_free = -1;
                break;

            case ROUND:
                if (last_free != -1)
                {
                    current = EMPTY;
                    grid[y][last_free] = ROUND;
                    last_free--;
                }
                break;
            
            default: throw std::runtime_error("Invalid cell type");
            }
        }
    }
}

void print_grid(const Grid& grid)
{
    for (const auto& row : grid)
    {
        for (const auto& cell : row)
        {
            char ch = 0;
            switch (cell)
            {
                case EMPTY: ch = '.'; break;
                case ROUND: ch = 'O'; break;
                case SQUARE: ch = '#'; break;
                default: throw std::runtime_error("Invalid cell type to print");
            }
            std::cout << ch;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void spin_cycle_fast(Grid& grid)
{
    roll_rocks_north_fast(grid);
    // print_grid(grid);

    roll_rocks_west_fast(grid);
    // print_grid(grid);

    roll_rocks_south_fast(grid);
    // print_grid(grid);

    roll_rocks_east_fast(grid);
    // print_grid(grid);
}

size_t load_on_north_beams(const Grid& grid)
{
    size_t total_load = 0;

    for (size_t y = 0; y < grid.size(); y++)
    {
        for (const auto& c : grid[y])
        {
            if (c == ROUND)
                total_load += grid.size() - y;
        }
    }

    return total_load;
}

int main()
{
    Grid grid = load_grid_from_file("../input.txt");

    roll_rocks_north_fast(grid);

    std::cout << load_on_north_beams(grid) << std::endl;

    std::unordered_map<size_t, std::vector<size_t>> visited; // keyed by load_on_north so not very good but works
    std::unordered_map<size_t, size_t> potential_cycle_lengths;

    constexpr size_t ITERATIONS = 1'000'000'000;

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        spin_cycle_fast(grid);

        size_t load = load_on_north_beams(grid);

        visited[load].push_back(i);

        size_t repeats = visited[load].size();

        if (repeats >= 10) // wait until there has the same load for 10 different iterations
        {
            size_t cycle_length = visited[load][repeats-1] - visited[load][repeats-2];
            potential_cycle_lengths[cycle_length]++;
        }

        if (repeats >= 100)
        {
            size_t definite_cycle_length = std::max_element(potential_cycle_lengths.begin(), potential_cycle_lengths.end(), [](const auto& a, const auto& b) { return a.second < b.second; })->first;

            size_t delta = (ITERATIONS - 1 - i) / definite_cycle_length;
            i += delta * definite_cycle_length;
        }
    }
    
    std::cout << load_on_north_beams(grid) << std::endl;
    // I don't think this is guaranteed to be the correct answer, but it works for my input
}