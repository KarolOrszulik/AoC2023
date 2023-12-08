#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>

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

    std::string line;
    while(std::getline(file, line))
    {
        if (line.empty())
            continue;
        
        
    }
}

void wasteland(const std::string& path)
{
    

    Data data;


}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path>" << std::endl;
        std::cout << "Falling back to default" << std::endl;
        wasteland("../test.txt");
    }
    else
    {
        wasteland(argv[1]);
    }
}
