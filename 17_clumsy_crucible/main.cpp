#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <array>

typedef std::vector<std::vector<int8_t>> Map;


enum Direction
{
    INVALID, 
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum RelDirection
{
    STRAIGHT,
    LEFT,
    RIGHT
};

struct Coord
{
    int x;
    int y;
};


Map load_map_from_file(const std::string& path)
{
    std::ifstream file(path);

    if (!file) throw std::runtime_error("Could not open file");

    Map map;

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<int8_t> row;
        for (char c : line)
        {
            row.push_back(c - '0');
        }
        map.push_back(std::move(row));
    }

    return map;
}


Direction turn(Direction dir, RelDirection how)
{
    switch (dir)
    {
        case NORTH:
            if (how == LEFT) return WEST;
            if (how == RIGHT) return EAST;
            return NORTH;
            break;
        case EAST:
            if (how == LEFT) return NORTH;
            if (how == RIGHT) return SOUTH;
            return EAST;
            break;
        case SOUTH:
            if (how == LEFT) return EAST;
            if (how == RIGHT) return WEST;
            return SOUTH;
            break;
        case WEST:
            if (how == LEFT) return SOUTH;
            if (how == RIGHT) return NORTH;
            return WEST;
            break;
        default:
            throw std::runtime_error("Invalid direction");
    }

    return INVALID;
}

Coord step(Coord coord, Direction dir)
{
    switch (dir)
    {
        case NORTH:
            return {coord.x, coord.y - 1};
            break;
        case EAST:
            return {coord.x + 1, coord.y};
            break;
        case SOUTH:
            return {coord.x, coord.y + 1};
            break;
        case WEST:
            return {coord.x - 1, coord.y};
            break;
        default:
            throw std::runtime_error("Invalid direction");
    }

    return {-1, -1};
}


size_t min_loss()
{
    return 0;
}


int main()
{
    Map map = load_map_from_file("../test_sm.txt");
    
    size_t cost = min_loss();

    std::cout << "Min cost: " << cost << std::endl;
}
