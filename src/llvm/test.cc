#include <iostream>
extern "C" int condition(int i, int j);
extern "C" int schleife(int i);

int main()
{
    std::cout << "**** TEST 1 ****" << std::endl;
    std::cout << "CONDITION" << std::endl;

    int result = condition(1, 3);
    std::cout << "...." << std::endl;
    std::cout << "OUTPUT: " << result << std::endl
              << std::endl;

    std::cout << "**** TEST 2 ****" << std::endl;
    std::cout << "SCHLEIFE" << std::endl;

    result = schleife(100);
    std::cout << "...." << std::endl;
    std::cout << "OUTPUT: " << result << std::endl
              << std::endl;

    return 0;
}