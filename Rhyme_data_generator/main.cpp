#include "RhymeTxtDataBuilder.h"
#include "RhymeCppDataBuilder.h"
#include <iostream>
#include <string>

int main() {
    RhymeCppDataBuilder cppBuilder("output/unique_rhyme_parts.txt", "output/rhyme_map.txt");
    cppBuilder.setUniqueSyllablesOutputPath("output/unique_rhyme_parts.h");
    cppBuilder.setRhymeMapOutputPath("output/rhyme_map.h");
    cppBuilder.buildCppData();

    std::cout << "Rhyme data building process completed." << std::endl;

    return 0;
}
