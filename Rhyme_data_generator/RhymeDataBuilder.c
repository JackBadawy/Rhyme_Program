#include "RhymeDataBuilder.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

class RhymeDataBuilder {
private:
    std::string inputFilePath;
    std::string outputFilePath;
    std::set<std::string> uniqueLastSyllables;
    std::unordered_map<std::string, std::vector<std::string>> rhymeMap;

    std::string getRhymePart(const std::string& pronunciation);
    void processWordList();
    void writeOutputFile();

public:
    RhymeDataBuilder(const std::string& inputFile);
    void setOutputPath(const std::string& outputPath);
    void buildUniqueSyllables();
    void buildRhymeMap();
    void saveRhymeMap(const std::string& filename);
};

RhymeDataBuilder::RhymeDataBuilder(const std::string& inputFile)
    : inputFilePath(inputFile), outputFilePath("/output/unique_syllables.txt") {}

void RhymeDataBuilder::setOutputPath(const std::string& outputPath) {
    outputFilePath = outputPath;
}

// ... (other existing methods remain the same, but rename findUniqueSyllables to buildUniqueSyllables)

void RhymeDataBuilder::buildRhymeMap() {
    std::ifstream file(inputFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << inputFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("  ");
        if (pos != std::string::npos) {
            std::string word = line.substr(0, pos);
            std::string pronunciation = line.substr(pos + 2);
            std::string rhymePart = getRhymePart(pronunciation);
            if (!rhymePart.empty()) {
                rhymeMap[rhymePart].push_back(word);
            }
        }
    }
    file.close();
}

void RhymeDataBuilder::saveRhymeMap(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << filename << std::endl;
        return;
    }

    for (const auto& entry : rhymeMap) {
        outFile << entry.first << ":";
        for (const auto& word : entry.second) {
            outFile << " " << word;
        }
        outFile << std::endl;
    }
    outFile.close();
    std::cout << "Rhyme map saved to: " << filename << std::endl;
}
