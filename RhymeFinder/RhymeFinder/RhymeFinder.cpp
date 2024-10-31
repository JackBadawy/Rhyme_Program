#include "RhymeFinder.h"
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace {
    std::string normalizeWord(const std::string& word) {
        std::string result = word;
        size_t parenPos = result.find('(');
        if (parenPos != std::string::npos) {
            result = result.substr(0, parenPos);
        }
        return result;
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
}

std::string RhymeFinder::cleanAndFormatWord(const std::string& word) const {

    std::string cleaned = normalizeWord(word);
    cleaned = trim(cleaned);

    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::tolower);

    if (!cleaned.empty()) {
        cleaned[0] = std::toupper(cleaned[0]);
    }

    return cleaned;
}

RhymeFinder::RhymeFinder() {
    std::cout << "RhymeFinder constructed.\n";
}

bool RhymeFinder::initialize(const std::string& filename) {
    std::cout << "Loading rhyme map from: " << filename << "\n";
    return loadRhymeMapFromFile(filename);
}

bool RhymeFinder::loadRhymeMapFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string pattern = line.substr(0, colonPos);
        pattern = trim(pattern);

        std::string wordsStr = line.substr(colonPos + 1);

        std::vector<std::string> words;
        std::istringstream wordStream(wordsStr);
        std::string word;
        while (wordStream >> word) {
            words.push_back(word);
        }

        if (!pattern.empty() && !words.empty()) {
            rhymeMap[pattern] = words;
        }
    }

    std::cout << "Loaded " << rhymeMap.size() << " rhyme patterns\n";
    return !rhymeMap.empty();
}

std::vector<std::string> RhymeFinder::findRhymes(std::string word) {
    try {
        std::cout << "Finding rhymes for: " << word << "\n";
        std::string upperWord = word;
        std::transform(upperWord.begin(), upperWord.end(), upperWord.begin(), ::toupper);
        std::string normalizedInput = normalizeWord(word);

        for (const auto& entry : rhymeMap) {
            const auto& words = entry.second;
            if (std::find(words.begin(), words.end(), upperWord) != words.end()) {
                std::vector<std::string> rhymes;
                for (const auto& rhyme : words) {
                    if (normalizeWord(rhyme) != normalizedInput) {
                        rhymes.push_back(cleanAndFormatWord(rhyme));
                    }
                }
                return rhymes;
            }
        }
        return std::vector<std::string>();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in findRhymes: " << e.what() << "\n";
        throw;
    }
}

size_t RhymeFinder::getTotalWords() const {
    std::unordered_set<std::string> uniqueWords;
    for (const auto& entry : rhymeMap) {
        for (const auto& word : entry.second) {
            uniqueWords.insert(normalizeWord(word));
        }
    }
    return uniqueWords.size();
}

size_t RhymeFinder::getRhymePatternCount() const {
    return rhymeMap.size();
}