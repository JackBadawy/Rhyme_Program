#ifndef RHYME_FINDER_H
#define RHYME_FINDER_H

#include <string>
#include <vector>
#include <unordered_map>

class RhymeFinder {
private:
    std::unordered_map<std::string, std::vector<std::string>> rhymeMap;
    bool loadRhymeMapFromFile(const std::string& filename);

public:
    RhymeFinder(); 
    bool initialize(const std::string& filename); 
    std::vector<std::string> findRhymes(std::string word);
    size_t getTotalWords() const;
    size_t getRhymePatternCount() const;
};

#endif 