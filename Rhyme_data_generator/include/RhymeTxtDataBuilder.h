#ifndef RHYME_DATA_BUILDER_H
#define RHYME_DATA_BUILDER_H

#include <string>
#include <set>
#include <unordered_map>
#include <vector>

class RhymeTxtDataBuilder {
public:
    RhymeTxtDataBuilder(const std::string& inputFile);

    void setOutputPath(const std::string& outputPath);
    void buildUniqueSyllables();
    void buildRhymeMap();
    void saveRhymeMap(const std::string& filename);

private:
    std::string inputFilePath;
    std::string outputFilePath;
    std::set<std::string> uniqueLastSyllables;
    std::unordered_map<std::string, std::vector<std::string>> rhymeMap;

    std::string getRhymePart(const std::string& pronunciation);
    void processWordList();
    void writeOutputFile();
};

#endif
