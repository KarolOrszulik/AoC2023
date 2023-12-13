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
    
    std::string line;
    while(std::getline(file, line))
    {

    }
}

int main()
{
    std::cout << "Hello, from point!\n";

}
