// https://adventofcode.com/2023/day/1

#include <iostream>
#include <fstream>

int line_value(const std::string& line)
{
    int first = -1, last = 0;

    for (char c : line)
    {
        if (c >= '0' && c <= '9')
        {
            int digit = c - '0';

            last = digit;

            if (first == -1)
                first = digit;
        }
    }

    return 10*first + last;
}

int line_value_spelled(const std::string line)
{
    return 0; // TODO
}

void trebuchet(int part)
{
    std::ifstream file("../input.txt");
    if (!file)
    {
        std::cerr << "Input file could not be opened" << std::endl;
        return;
    }

    unsigned long result = 0L;

    std::string line;
    while(std::getline(file, line))
    {
        if (part = 1)
            result += line_value(line);
        else
            result += line_value_spelled(line);
    }

    std::cout << result << std::endl;
}

int main()
{   
    trebuchet(1);
    trebuchet(2);
}
