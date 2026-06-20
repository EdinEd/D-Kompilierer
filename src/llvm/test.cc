#include <iostream>

// Sagt dem C++ Compiler: Die Funktion 'haupt' existiert in einer anderen Datei
// und sie verwendet kein C++ Name-Mangling (daher "C").
extern "C" int add(int i, int j);

int main()
{
    std::cout << "Starte generiertes Programm..." << std::endl;

    // Hier rufen wir DEINEN generierten Code auf!
    int result = add(1, 2);

    std::cout << "Dein Compiler hat folgendes berechnet: " << result << std::endl;

    return 0;
}