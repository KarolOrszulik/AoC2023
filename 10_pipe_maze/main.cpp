#include <iostream>
#include <vector>
#include <fstream>
#include <array>

enum Direction
{
    INVALID = 0,
    NORTH = 0b0001,
    EAST  = 0b0010,
    SOUTH = 0b0100,
    WEST  = 0b1000
};

struct Cell
{
    int8_t mask = 0;
    bool   visited = false;

    bool has_north() const { return mask & NORTH; }
    bool has_east()  const { return mask & EAST; }
    bool has_south() const { return mask & SOUTH; }
    bool has_west()  const { return mask & WEST; }

    int connection_count() const
    {
        return has_north() + has_east() + has_south() + has_west();
    }

    char as_char() const
    {
        if (has_north() && has_south())return '|';
        if (has_east()  && has_west()) return '-';
        if (has_north() && has_east()) return 'L';
        if (has_north() && has_west()) return 'J';
        if (has_south() && has_west()) return '7';
        if (has_south() && has_east()) return 'F';
        return '.';
    }

    explicit Cell(const char c, bool v = false) : visited(v)
    {
        switch (c)
        {
        case '|': mask = NORTH | SOUTH; break;
        case '-': mask = EAST  | WEST; break;
        case 'L': mask = NORTH | EAST; break;
        case 'J': mask = NORTH | WEST; break;
        case 'F': mask = SOUTH | EAST; break;
        case '7': mask = SOUTH | WEST; break;

        case '.':
        case 'S': mask = 0; break;

        default:
            throw std::runtime_error("Invalid character: " + std::to_string(c));
        }
    }
};

Cell find_cell_type(Cell n, Cell e, Cell s, Cell w)
{
    Cell cell('S');
    cell.mask |= NORTH * n.has_south();
    cell.mask |= EAST  * e.has_west();
    cell.mask |= SOUTH * s.has_north();
    cell.mask |= WEST  * w.has_east();

    if (cell.connection_count() != 2)
    {
        throw std::runtime_error("Invalid S type");
    }

    return cell;
}

struct Maze
{
    std::vector<std::vector<Cell>> grid;
    size_t start_x = 0;
    size_t start_y = 0;

    Cell at(int x, int y) const
    {
        if (x >= grid.size() || y >= grid[x].size())
            return Cell('.');

        return grid[y][x];
    }

    void set(Cell to_set, size_t x, size_t y)
    {
        if (x >= grid.size() || y >= grid[x].size())
            return;

        grid[y][x] = to_set;
    }
};

Maze load_maze_from_file(const std::string& path)
{
    std::ifstream file(path);

    if (!file)
    {
        throw std::runtime_error("Could not open file");
    }

    Maze maze;

    std::string line;
    for (size_t y = 0; std::getline(file, line); ++y)
    {
        std::vector<Cell> row;
        for (size_t x = 0; x < line.length(); ++x)
        {
            row.push_back(Cell(line[x]));
            if (line[x] == 'S')
            {
                maze.start_x = x;
                maze.start_y = y;
            }
        }
        maze.grid.push_back(std::move(row));
    }

    return maze;
}

Direction opposite(Direction dir)
{
    switch (dir)
    {
        case NORTH: return SOUTH;
        case EAST:  return WEST;
        case SOUTH: return NORTH;
        case WEST:  return EAST;
    }
    return INVALID;
}

Direction change_direction(Direction from, Cell at)
{
    for (int i = 1; i < 1<<4; i<<=1)
    {
        if (i != from && at.mask & i)
            return (Direction)i;
    }
    
    throw std::runtime_error("Could not change direction");
}

size_t mark_loop(Maze& maze)
{
    size_t x = maze.start_x;
    size_t y = maze.start_y;

    Cell start = find_cell_type(
        maze.at(x,     y - 1),
        maze.at(x + 1, y    ),
        maze.at(x,     y + 1),
        maze.at(x - 1, y    )
    );
    maze.set(start, x, y);

    Direction dir = change_direction(NORTH, maze.at(x, y));

    size_t steps = 0;

    while (x != maze.start_x || y != maze.start_y || steps == 0)
    {
        ++steps;
        maze.grid[y][x].visited = true;
        switch (dir)
        {
            case NORTH: --y; break;
            case EAST:  ++x; break;
            case SOUTH: ++y; break;
            case WEST:  --x; break;
            default: throw std::runtime_error("Oops!");
        }
        dir = change_direction(opposite(dir), maze.at(x, y));
    }

    return steps;
}

size_t count_enclosed(const Maze& maze)
{
    size_t inside_count = 0;

    for (int y = 0; y < maze.grid.size(); ++y)
    {
        for (int x = 0; x < maze.grid[y].size(); ++x)
        {
            if (maze.at(x, y).visited)
                continue; // if a cell is a part of the path, it is not enclosed by the path

            size_t crossings = 0;
            bool slope = false;
            for (int curr_y = y; curr_y >= 0; --curr_y)
            {
                if (!maze.at(x, curr_y).visited)
                    continue; // if a cell is not part of the path, it can't contribute to the crossings count

                char current = maze.at(x, curr_y).as_char();
                
                if (current == '-' ||
                    current == 'F' && slope ||
                    current == '7' && !slope)
                    ++crossings;

                if (current == 'J')
                    slope = true;

                if (current == 'L')
                    slope = false;
            }
            if (crossings % 2 == 1)
                ++inside_count;
        }
    }

    return inside_count;
}

int main()
{
    Maze maze = load_maze_from_file("../input.txt");

    size_t farthest = mark_loop(maze) / 2UL;
    std::cout << farthest << std::endl;

    size_t enclosed = count_enclosed(maze);
    std::cout << enclosed << std::endl;
}
