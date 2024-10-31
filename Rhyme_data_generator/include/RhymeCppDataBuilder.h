#ifndef RHYME_CPP_DATA_BUILDER_H
#define RHYME_CPP_DATA_BUILDER_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class RhymeCppDataBuilder {
public:
    RhymeCppDataBuilder(const std::string& uniqueSyllablesFile, const std::string& rhymeMapFile);

    void setUniqueSyllablesOutputPath(const std::string& path);
    void setRhymeMapOutputPath(const std::string& path);

    void loadUniqueSyllables();
    void loadRhymeMap();
    void writeUniqueSyllablesCpp();
    void writeRhymeMapCpp();
    void buildCppData();

private:
    std::string uniqueSyllablesInputPath;
    std::string rhymeMapInputPath;
    std::string uniqueSyllablesOutputPath;
    std::string rhymeMapOutputPath;

    std::unordered_set<std::string> uniqueSyllables;
    std::unordered_map<std::string, std::vector<std::string>> rhymeMap;
};

#endif
