#include <iostream>
#include <vector>
#include <string>
#include <fstream>

enum Direction : int8_t
{
  INVALID = 0b0000,
    NORTH = 0b0001,
    EAST  = 0b0010,
    SOUTH = 0b0100,
    WEST  = 0b1000
};

typedef std::vector<std::string> Grid;
typedef std::vector<std::vector<int8_t>> VisitMask;

Grid load_grid_from_file(const std::string& path)
{
    std::ifstream file(path);

    if (!file) throw std::runtime_error("Could not open file");

    Grid result;

    std::string line;
    while (std::getline(file, line))
    {
        result.push_back(std::move(line));
    }

    return result;
}

VisitMask make_mask_of_size(const Grid& grid)
{
    return std::vector(grid.size(), std::vector<int8_t>(grid[0].size(), 0));
}

Direction reflect(char tile, Direction dir)
{
    if (tile != '\\' && tile != '/')
        throw std::runtime_error("Invalid tile to reflect off of");
    
    if (tile == '\\')
    {
        switch (dir)
        {
            case NORTH: return WEST;
            case SOUTH: return EAST;
            case WEST:  return NORTH;
            case EAST:  return SOUTH;
        }
    }
    else // tile == '/'
    {
        switch (dir)
        {
            case NORTH: return EAST;
            case SOUTH: return WEST;
            case WEST:  return SOUTH;
            case EAST:  return NORTH;
        }
    }

    return INVALID;
}

void traverse_grid(const Grid& grid, int x, int y, Direction dir, VisitMask& mask)
{
    if (mask[y][x] & dir) // if we've already traversed this cell in this direction, return
        return;

    bool hit_fork = false;
    while(!hit_fork)
    {
        mask[y][x] |= dir;
        
        switch (grid[y][x])
        {
        case '.':
            break;
        case '/':
        case '\\':
            dir = reflect(grid[y][x], dir);
            break;
        case '-':
            if (dir == EAST || dir == WEST) break;
            else { hit_fork = true; break; }
        case '|':
            if (dir == NORTH || dir == SOUTH) break;
            else { hit_fork = true; break; }
        }

        if (hit_fork)
            break;

        switch (dir)
        {
            case NORTH: y--; break;
            case SOUTH: y++; break;
            case EAST: x++; break;
            case WEST: x--; break;
        }

        if (y < 0 || y >= grid.size() || x < 0 || x >= grid[y].size())
            return;
    }

    if (grid[y][x] == '|')
    {
        traverse_grid(grid, x, y, NORTH, mask);
        traverse_grid(grid, x, y, SOUTH, mask);
    }
    else // grid[y][x] == '-'
    {
        traverse_grid(grid, x, y, EAST, mask);
        traverse_grid(grid, x, y, WEST, mask);
    }

}

size_t count_visited(const VisitMask& mask)
{
    size_t result = 0;

    for (const auto& row : mask)
        for (const auto& m : row)
            result += m != 0;

    return result;
}

void print_mask(const VisitMask& mask)
{
    for (const auto& row : mask)
    {
        for (const auto& m : row)
        {
            std::cout << (m == 0 ? '.' : '#');
        }
        std::cout << std::endl;
    }
}

int main()
{
    const Grid grid = load_grid_from_file("../input.txt");
    VisitMask mask = make_mask_of_size(grid);

    // part 1
    traverse_grid(grid, 0, 0, EAST, mask);
    std::cout << count_visited(mask) << std::endl;

    // part 2
    size_t most_visited = 0;

    for (int y = 0; y < grid.size(); y++)
    {
        mask = make_mask_of_size(grid);
        traverse_grid(grid, 0, y, EAST, mask);
        most_visited = std::max(most_visited, count_visited(mask));

        mask = make_mask_of_size(grid);
        traverse_grid(grid, grid[y].size() - 1, y, WEST, mask);
        most_visited = std::max(most_visited, count_visited(mask));
    }
    for (int x = 0; x < grid[0].size(); x++)
    {
        mask = make_mask_of_size(grid);
        traverse_grid(grid, x, 0, SOUTH, mask);
        most_visited = std::max(most_visited, count_visited(mask));

        mask = make_mask_of_size(grid);
        traverse_grid(grid, x, grid.size() - 1, WEST, mask);
        most_visited = std::max(most_visited, count_visited(mask));
    }

    std::cout << most_visited << std::endl;
}
