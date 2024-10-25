#include "RhymeTxtDataBuilder.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

RhymeTxtDataBuilder::RhymeTxtDataBuilder(const std::string& inputFile) : inputFilePath(inputFile), outputFilePath("/output/unique_rhyme_parts.txt") {}

void RhymeTxtDataBuilder::setOutputPath(const std::string& outputPath) {
    outputFilePath = outputPath;
}

std::string RhymeTxtDataBuilder::getRhymePart(const std::string& pronunciation) {
    std::istringstream iss(pronunciation);
    std::string phoneme;
    std::string lastVowel;
    std::string afterLastVowel;

    while (iss >> phoneme) {

        if (std::isdigit(phoneme.back())) {
            lastVowel = phoneme;
            afterLastVowel.clear();
        } else {
            afterLastVowel += phoneme + " ";
        }
    }

    std::string rhymePart = lastVowel + " " + afterLastVowel;
    if (!rhymePart.empty() && rhymePart.back() == ' ') {
        rhymePart.pop_back();
    }

    return rhymePart;
}

void RhymeTxtDataBuilder::processWordList() {
    std::ifstream file(inputFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << inputFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find("  ");
        if (pos != std::string::npos) {
            std::string pronunciation = line.substr(pos + 2);
            std::string lastSyllable = getRhymePart(pronunciation);
            if (!lastSyllable.empty()) {
                uniqueLastSyllables.insert(lastSyllable);
            }
        }
    }
    file.close();
}

void RhymeTxtDataBuilder::writeOutputFile() {
    std::ofstream outFile(outputFilePath);
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFilePath << std::endl;
        return;
    }

    for (const auto& syllable : uniqueLastSyllables) {
        outFile << syllable << std::endl;
    }
    outFile.close();
    std::cout << "Unique last syllables written to: " << outputFilePath << std::endl;
}

void RhymeTxtDataBuilder::buildRhymeMap() {
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

void RhymeTxtDataBuilder::saveRhymeMap(const std::string& filename) {
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

void RhymeTxtDataBuilder::buildUniqueSyllables() {
    processWordList();
    writeOutputFile();
}

