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
    constexpr size_t NO_DIGITS = 10UL;
    const std::string digits[NO_DIGITS] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    size_t i_first = std::string::npos;
    size_t i_last  = std::string::npos;
    int first = -1;
    int last  = -1;

    for (int i = 0; i < NO_DIGITS; i++)
    {
        size_t first_spelled = line.find(digits[i]);
        size_t last_spelled  = line.rfind(digits[i]);

        if (first_spelled < i_first)
        {
            i_first = first_spelled;
            first = i;
        }

        if (last_spelled != std::string::npos && (last_spelled > i_last || i_last == std::string::npos))
        {
            i_last = last_spelled;
            last = i;
        }
    }

    size_t first_digit = line.find_first_of("0123456789");
    size_t last_digit  = line.find_last_of("0123456789");

    if (first_digit < i_first)
    {
        first = line[first_digit] - '0';
    }

    if (last_digit != std::string::npos && (last_digit > i_last || i_last == std::string::npos))
    {
        last = line[last_digit] - '0';
    }

    // std::cout << 10*first + last << std::endl;

    return 10*first + last;
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
        if (part == 1)
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
