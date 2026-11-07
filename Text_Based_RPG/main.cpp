#include <iostream>
#include <fstream>
#include <string>
#include "Adventure.h"

int main(int argc, char* argv[]) {
    // argc counts the words on the command line, including the program name.
    //   ./adventure bee_movie.txt   ->   argc is 2
    // argv holds those words.
    //   argv[0] is "./adventure"    argv[1] is "bee_movie.txt"
    if (argc != 2) {
        std::cout << "Usage: ./adventure <story_file>" << std::endl;
        return 1;
    }

    // ===================== PART 0: runs as-is =====================
    // Opens the story file named in argv[1] and prints every line.
    // Run this first so you know your file is being found.
    // When you start PART 1, comment out this whole block.

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "Error: could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();

    // ===================== PART 1: the game =====================
    // Uncomment this block once your Scene, StoryTree, and Adventure compile.
    // Do not change it.

    // Adventure game;
    // if (!game.load(argv[1])) {
    //     return 1;
    // }
    // std::cout << "Story loaded." << std::endl;
    //
    // std::string choice;
    // while (true) {
    //     std::cout << std::endl;
    //     std::cout << "==== MENU ====" << std::endl;
    //     std::cout << "1. Play" << std::endl;
    //     std::cout << "2. Story stats" << std::endl;
    //     std::cout << "3. Spoiler guide" << std::endl;
    //     std::cout << "4. Greedy vs optimal" << std::endl;
    //     std::cout << "5. Quit" << std::endl;
    //     std::cout << "Choose: ";
    //     if (!std::getline(std::cin, choice)) {
    //         break;
    //     }
    //     if (choice == "1") {
    //         game.play();
    //     } else if (choice == "2") {
    //         game.printStats();
    //     } else if (choice == "3") {
    //         game.printSpoilers();
    //     } else if (choice == "4") {
    //         game.greedyRun();
    //         game.optimalRun();
    //     } else if (choice == "5") {
    //         break;
    //     } else {
    //         std::cout << "Please type a number from 1 to 5." << std::endl;
    //     }
    // }
    // std::cout << "Goodbye!" << std::endl;

    return 0;
}
