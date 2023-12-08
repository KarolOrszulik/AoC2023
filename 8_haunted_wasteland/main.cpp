#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

struct Node
{
    std::string left, right;
};

struct Data
{
    std::string directions;
    std::unordered_map<std::string, Node> nodes;
};

Data load_data_from_file(const std::string& path)
{
    Data data;

    std::ifstream file(path);

    if (!file)
    {
        throw std::runtime_error("File could not be opened");
    }

    file >> data.directions;

    std::string current, left, right;

    //                 this  vvvv  is to absorb the '=' sign
    while(file >> current >> left >> left >> right)
    {
        left.erase(0,1);
        left.pop_back();

        right.pop_back();

        data.nodes[current] = {left, right};
    }

    return data;
}

void wasteland_pt1(const std::string& path)
{
    const Data data = load_data_from_file(path);

    // std::cout << data.directions << std::endl;
    // for(const auto& [curr, node] : data.nodes)
    //     std::cout << curr << " -> " << node.left << " : " << node.right << std::endl;

    size_t steps = 0;

    std::string current = "AAA";

    while (current != "ZZZ")
    {
        for (char dir : data.directions)
        {
            if (dir == 'L')
                current = data.nodes.at(current).left;
            else
                current = data.nodes.at(current).right;
            steps++;
        }
    }

    std::cout << steps << std::endl;
}

void wasteland_pt2(const std::string& path)
{
    const Data data = load_data_from_file(path);

    std::unordered_map<std::string, size_t> cycle_lengths;
    for (const auto& [node, _] : data.nodes)
        if (node.back() == 'A')
            cycle_lengths[node] = 0;
    
    for (auto& [node, cycle_length] : cycle_lengths)
    {
        std::string current = node;
        size_t counter = 0;

        while (current.back() != 'Z')
        {
            char dir = data.directions[counter];

            if (dir == 'L')
                current = data.nodes.at(current).left;
            else
                current = data.nodes.at(current).right;

            cycle_length++;
            ++counter %= data.directions.length();
        }
    }

    size_t steps = 1;

    for (const auto& [node, cycle_length] : cycle_lengths)
    {
        steps = std::lcm(steps, cycle_length);
    }

    std::cout << steps << std::endl;
}

int main(int argc, char** argv)
{
    std::string path = "../input.txt";
    if (argc < 2)
        std::cout << "Usage: " << argv[0] << " <path>" << std::endl
                  << "Falling back to default: " << path << std::endl;
    else
        path = argv[1];

    wasteland_pt1(path);
    wasteland_pt2(path);
}
