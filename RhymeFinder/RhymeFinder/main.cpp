#include <iostream>
#include "RhymeFinder.h"
#include <cstdlib>

void print_separator() {
    std::cout << "\n----------------------------------------\n";
}

int main() {
    std::cout << "Program starting...\n";
    print_separator();

    try {
        RhymeFinder rhymeFinder;

        if (!rhymeFinder.initialize("rhyme_map.txt")) {
            std::cerr << "Failed to initialize rhyme finder!\n";
            return EXIT_FAILURE;
        }

        std::cout << "RhymeFinder initialized successfully.\n";
        print_separator();

        std::cout << "Pattern count: " << rhymeFinder.getRhymePatternCount() << "\n";
        std::cout << "Total words: " << rhymeFinder.getTotalWords() << "\n";
        print_separator();

        std::string input;
        while (true) {
            std::cout << "\nEnter a word to find rhymes (or 'quit' to exit): ";
            if (!std::getline(std::cin, input)) {
                std::cout << "Error reading input!\n";
                return EXIT_FAILURE;
            }

            if (input == "quit") {
                std::cout << "Exiting program...\n";
                break;
            }

            if (input.empty()) {
                std::cout << "Empty input, please enter a word.\n";
                continue;
            }

            auto rhymes = rhymeFinder.findRhymes(input);
            if (rhymes.empty()) {
                std::cout << "No rhymes found.\n";
            }
            else {
                std::cout << "Found " << rhymes.size() << " rhymes:\n";
                for (size_t i = 0; i < rhymes.size(); ++i) {
                    std::cout << rhymes[i];
                    if (i < rhymes.size() - 1) std::cout << ", ";
                }
                std::cout << "\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "Program completed successfully.\n";
    return EXIT_SUCCESS;
}