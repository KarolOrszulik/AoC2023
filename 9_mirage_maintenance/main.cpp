#include <iostream>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

std::vector<std::vector<int>> load_from_file(const std::string& path)
{
    std::ifstream file(path);

    if(!file)
        throw std::runtime_error("Input file could not be opened");
    
    std::vector<std::vector<int>> result;

    std::string line;
    while(std::getline(file, line))
    {
        std::istringstream iss(line);

        std::vector<int> current;
        int num;
        while(iss >> num)
        {
            current.push_back(num);
        }
        result.push_back(std::move(current));
    }

    return result;
}

void mirage_maintenance(const std::string& path, int part)
{
    auto data = load_from_file(path);

    size_t total = 0;

    for (const auto& record : data)
    {
        std::deque<int> diff(record.begin(), record.end());
        std::vector<int> edge_vals;

        edge_vals.push_back(part == 1 ? diff.back() : diff.front());
        
        while (std::any_of(diff.begin(), diff.end(), [](int x){ return x != 0; }))
        {
            // for (int n : diff)
            //     std::cout << n << " ";
            // std::cout << std::endl;

            std::adjacent_difference(diff.begin(), diff.end(), diff.begin());
            diff.pop_front(); // because adj_diff() leaves the first value in place

            edge_vals.push_back(part == 1 ? diff.back() : diff.front());
        }

        int sign = part == 1 ? 1 : -1; // add previous edge_val for pt.1, subtract for pt.2
        for (int i = edge_vals.size() - 1; i >= 1; --i)
        {
            edge_vals[i-1] += edge_vals[i] * sign;
        }

        // std::cout << last_vals.front() << std::endl;
        total += edge_vals.front();
    }

    std::cout << total << std::endl;
}

int main(int argc, char** argv)
{
    std::string path = "../input.txt";

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path>" << std::endl;
        std::cout << "Falling back to default: " << path << std::endl;
    }
    else
    {
        path = argv[1];
    }

    mirage_maintenance(path, 1);
    mirage_maintenance(path, 2);
}
