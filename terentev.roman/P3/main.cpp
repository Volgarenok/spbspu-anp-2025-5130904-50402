#include <iostream>
#include <fstream>
#include <string>

bool anum(char * a);

int main(int argc, char ** argv)
{
    std::ifstream input(argv[2]);
    if (argc < 4)
    {
        std::cerr << "Not enough arguments\n";
        return 1;
    }
    if (argc > 4)
    {
        std::cerr << "Too many arguments\n";
        return 1;
    }
    if (!anum (argv[1])) {
        std::cerr << "First parameter is not a number\n";
        return 1;
    }
    if (std::stoi(std::string(argv[1]))>2 || std::stoi(std::string(argv[1])) < 1) {
        std::cerr << "First parameter is out of range\n";
    }
    std::ofstream output(argv[3]);
    std::cout << "\n";
}

bool anum(char * a) {

}
