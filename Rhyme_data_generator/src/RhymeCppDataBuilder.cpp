#include "RhymeCppDataBuilder.h"
#include <fstream>
#include <iostream>
#include <sstream>

RhymeCppDataBuilder::RhymeCppDataBuilder(const std::string& uniqueSyllablesFile, const std::string& rhymeMapFile)
    : uniqueSyllablesInputPath(uniqueSyllablesFile),
      rhymeMapInputPath(rhymeMapFile),
      uniqueSyllablesOutputPath("unique_rhyme_parts.h"),
      rhymeMapOutputPath("rhyme_map.h") {}

void RhymeCppDataBuilder::setUniqueSyllablesOutputPath(const std::string& path) {
    uniqueSyllablesOutputPath = path;
}

void RhymeCppDataBuilder::setRhymeMapOutputPath(const std::string& path) {
    rhymeMapOutputPath = path;
}

void RhymeCppDataBuilder::loadUniqueSyllables() {
    std::ifstream file(uniqueSyllablesInputPath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << uniqueSyllablesInputPath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        uniqueSyllables.insert(line);
    }
    file.close();
}

void RhymeCppDataBuilder::loadRhymeMap() {
    std::ifstream file(rhymeMapInputPath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << rhymeMapInputPath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string rhymePart;
        std::getline(iss, rhymePart, ':');

        std::string words;
        std::getline(iss, words);
        std::istringstream wordStream(words);
        std::string word;
        while (wordStream >> word) {
            rhymeMap[rhymePart].push_back(word);
        }
    }
    file.close();
}

void RhymeCppDataBuilder::writeUniqueSyllablesCpp() {
    std::ofstream outFile(uniqueSyllablesOutputPath);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << uniqueSyllablesOutputPath << std::endl;
        return;
    }

    outFile << "#ifndef UNIQUE_RHYME_PARTS_H\n";
    outFile << "#define UNIQUE_RHYME_PARTS_H\n\n";
    outFile << "#include <unordered_set>\n";
    outFile << "#include <string>\n\n";
    outFile << "const std::unordered_set<std::string> uniqueRhymeParts = {\n";

    for (auto it = uniqueSyllables.begin(); it != uniqueSyllables.end(); ++it) {
        outFile << "    \"" << *it << "\"";
        if (std::next(it) != uniqueSyllables.end()) {
            outFile << ",";
        }
        outFile << "\n";
    }

    outFile << "};\n\n";
    outFile << "#endif // UNIQUE_RHYME_PARTS_H\n";

    outFile.close();
    std::cout << "Unique last syllables C++ code written to: " << uniqueSyllablesOutputPath << std::endl;
}

void RhymeCppDataBuilder::writeRhymeMapCpp() {
    std::ofstream outFile(rhymeMapOutputPath);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << rhymeMapOutputPath << std::endl;
        return;
    }

    outFile << "#ifndef RHYME_MAP_H\n";
    outFile << "#define RHYME_MAP_H\n\n";
    outFile << "#include <unordered_map>\n";
    outFile << "#include <vector>\n";
    outFile << "#include <string>\n\n";
    outFile << "const std::unordered_map<std::string, std::vector<std::string>> rhymeMap = {\n";

    for (auto it = rhymeMap.begin(); it != rhymeMap.end(); ++it) {
        outFile << "    {\"" << it->first << "\", {";
        for (auto wordIt = it->second.begin(); wordIt != it->second.end(); ++wordIt) {
            outFile << "\"" << *wordIt << "\"";
            if (std::next(wordIt) != it->second.end()) {
                outFile << ", ";
            }
        }
        outFile << "}}";
        if (std::next(it) != rhymeMap.end()) {
            outFile << ",";
        }
        outFile << "\n";
    }

    outFile << "};\n\n";
    outFile << "#endif // RHYME_MAP_H\n";

    outFile.close();
    std::cout << "Rhyme map C++ code written to: " << rhymeMapOutputPath << std::endl;
}

void RhymeCppDataBuilder::buildCppData() {
    loadUniqueSyllables();
    loadRhymeMap();
    writeUniqueSyllablesCpp();
    writeRhymeMapCpp();
}
