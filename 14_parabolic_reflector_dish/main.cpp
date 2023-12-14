#include <iostream>
#include <vector>
#include <string>
#include <fstream>

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

void roll_rocks_north(Grid& grid)
{
    for (int y = 1; y < grid.size(); y++)
        for (int x = 0; x < grid[y].size(); x++)
        {
            if (grid[y][x] != ROUND)
                continue;

            for (int curr_y = y; curr_y >= 1; curr_y--)
            {
                if (grid[curr_y-1][x] != EMPTY)
                    break;
                
                std::swap(grid[curr_y-1][x], grid[curr_y][x]);
            }
        }
}

void roll_rocks_north_fast(Grid& grid)
{
    for (int x = 0; x < grid[0].size(); x++)
    {
        int last_free = -1;

        for (int y = 0; y < grid.size(); y++)
        {
            Cell& current = grid[y][x];

            if (last_free == -1 && current == EMPTY)
                last_free = y;
            
            if (current == SQUARE)
                last_free = -1;
            
            if (last_free != -1 && current == ROUND)
            {
                current = EMPTY;
                grid[last_free][x] = ROUND;
                last_free++;
            }
        }
    }
}

void roll_rocks_south(Grid& grid)
{
    for (int y = grid.size() - 2; y >= 0; y--)
        for (int x = 0; x < grid[y].size(); x++)
        {
            if (grid[y][x] != ROUND)
                continue;

            for (int curr_y = y; curr_y <= grid.size() - 2; curr_y++)
            {
                if (grid[curr_y+1][x] != EMPTY)
                    break;
                
                std::swap(grid[curr_y+1][x], grid[curr_y][x]);
            }
        }
}

void roll_rocks_west(Grid& grid)
{
    for (int y = 0; y < grid.size(); y++)
        for (int x = 1; x < grid[y].size(); x++)
        {
            if (grid[y][x] != ROUND)
                continue;

            for (int curr_x = x; curr_x >= 1; curr_x--)
            {
                if (grid[y][curr_x-1] != EMPTY)
                    break;
                
                std::swap(grid[y][curr_x-1], grid[y][curr_x]);
            }
        }
}

void roll_rocks_east(Grid& grid)
{
    for (int y = 0; y < grid.size(); y++)
        for (int x = grid[y].size() - 2; x >= 0; x--)
        {
            if (grid[y][x] != ROUND)
                continue;

            for (int curr_x = x; curr_x <= grid[y].size() - 2; curr_x++)
            {
                if (grid[y][curr_x+1] != EMPTY)
                    break;
                
                std::swap(grid[y][curr_x+1], grid[y][curr_x]);
            }
        }
}

void spin_cycle(Grid& grid)
{
    roll_rocks_north(grid);
    roll_rocks_west(grid);
    roll_rocks_south(grid);
    roll_rocks_east(grid);
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

int main(int, char**)
{
    Grid grid = load_grid_from_file("../test.txt");

    roll_rocks_north_fast(grid);
    print_grid(grid);

    std::cout << load_on_north_beams(grid) << std::endl;

    // for (size_t i = 0; i < 1'000'000'000; i++)
    // {
    //     if (i % 10'000 == 0)
    //         std::cout << i/10'000 << "/100'000" << std::endl;

    //     // Grid before = grid;

    //     spin_cycle(grid);

    //     // if (before == grid)
    //     // {
    //     //     break;
    //     // }
    // }
    
    std::cout << load_on_north_beams(grid) << std::endl;
}
