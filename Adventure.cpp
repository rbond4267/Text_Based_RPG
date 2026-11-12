#include "Adventure.h"
#include <iostream>
#include <fstream>
#include <memory>

// ===================== GIVEN HELPERS (do not change) =====================

// Reads the next line that is not blank.
// Spaces at the start of the line are removed.
// Returns false when the file has no more lines.
bool Adventure::nextLine(std::ifstream& in, std::string& line) {
    std::string raw;
    while (std::getline(in, raw)) {
        int start = 0;
        while (start < (int)raw.size() && (raw[start] == ' ' || raw[start] == '\t')) {
            start++;
        }
        line = raw.substr(start);
        // Files saved on Windows can end each line with an extra '\r'.
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }
        if (!line.empty()) {
            return true;
        }
    }
    return false;
}

// Reads the next line and checks that it starts with keyword.
// Example: for the line "HP -2" and keyword "HP", value becomes "-2".
// Prints an error and returns false if the line is missing or wrong.
bool Adventure::readField(std::ifstream& in, const std::string& keyword, std::string& value) {
    std::string line;
    if (!nextLine(in, line)) {
        std::cout << "Error: expected a " << keyword << " line but the file ended." << std::endl;
        return false;
    }
    std::string prefix = keyword + " ";
    if (line.substr(0, prefix.size()) != prefix) {
        std::cout << "Error: expected a " << keyword << " line but found: " << line << std::endl;
        return false;
    }
    value = line.substr(prefix.size());
    return true;
}

// Adds one choice number to a path.
// extendPath("", 2) gives "2". extendPath("2-1", 3) gives "2-1-3".
std::string Adventure::extendPath(const std::string& path, int choiceNumber) const {
    if (path.empty()) {
        return std::to_string(choiceNumber);
    }
    return path + "-" + std::to_string(choiceNumber);
}

// ===================== LOADING =====================

bool Adventure::load(const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Error: could not open file " << fileName << std::endl;
        return false;
    }

    // Read starting HP, build the first scene, and set it to the root of the tree
    std::string startHpValue;
    if (readField(file, "Start_HP", startHpValue)) {
        startHp_ = std::stoi(startHpValue);

        Scene<Moment>* firstScene = readScene(file, "START");

        if (firstScene == nullptr) {
            return false;
        }

        tree_.setRoot(firstScene);
        return true;
    }
    return false;

}

//   Read SCENE, HP, LOOT, ITEM, and CHOICES and create new scenes based on the moment information.
Scene<Moment>* Adventure::readScene(std::ifstream& in, const std::string& label) {
    std::string sceneName, hpValue, loot, itemName, choices, nextSceneName;

    if (!readField(in, "SCENE",sceneName)) {
        return nullptr;
    }

    if (!readField(in, "HP",hpValue)) {
        return nullptr;
    }

    if (!readField(in, "LOOT", loot)) {
        return nullptr;
    }

    if (!readField(in, "ITEM",itemName)) {
        return nullptr;
    }

    if (!readField(in, "CHOICES",choices)) {
        return nullptr;
    }

    int numChoices = std::stoi(choices);
    int hpVal = std::stoi(hpValue);
    int lootVal = std::stoi(loot);

    // Print error if there are too few or too many choices
    if (numChoices < 0 || numChoices > Scene<Moment>::MAX_CHOICES) {
        std::cout << "Error: scene " << sceneName << " has " << numChoices << " choices. The limit is 3.\n";
        return nullptr;
    }

    Moment* currentMoment = new Moment(sceneName, label , hpVal, lootVal, itemName);
    Scene<Moment>* currentScene = new Scene<Moment>(currentMoment, numChoices);

    // Check if the next scene exists, if so, recursively reads and attaches the next scene
    for (int i = 0; i < numChoices; i++) {

        if (!readField(in, "LABEL", nextSceneName)) {
            delete currentScene;
            return nullptr;
        }

        Scene <Moment>* nextScene = readScene(in, nextSceneName);

        if (nextScene == nullptr) {
            delete currentScene;
            return nullptr;
        }
        currentScene->setChoice(i, nextScene);
    }
    return currentScene;
}

// ===================== PLAYING =====================

int Adventure::askChoice(int numChoices) {
    std::string choice;
    std::cout << "Your choice: ";

    while (std::getline(std::cin, choice)) {
        // Catch exception on std::stoi conversion
        try {
            std::size_t position;
            int choiceInt = std::stoi(choice, &position);



            if (choice.size() != position || (choiceInt < 1 || choiceInt > numChoices)) {
                std::cout << "Please type a number from 1 to " << numChoices << ".\n";

                std::cout << "Your choice: ";
            }
            else {
                return choiceInt;
            }
        }
        catch (const std::exception&) {
            std::cout << "Please type a number from 1 to " << numChoices << ".\n";

            std::cout << "Your choice: ";
        }
    }
    return 0;
}

void
// TODO 13: play the story once from the start.
// Every time the player ENTERS a scene, in this order:
//   1. Add its HP change to hp and its loot to loot.
//   2. Print a line of 40 dashes, then the scene text.
//   3. If the scene has an item:
//        if the bag already has it, print "You already have: <item>"
//        otherwise add it to the bag and print "You picked up: <item>"
//   4. Print "HP: <hp>  Loot: <loot>" (two spaces before Loot).
//   5. If hp is 0 or less, print "Your HP hit 0. GAME OVER." and stop.
//      If the scene is an ending, print "THE END." and stop.
//   6. Print each choice as "[1] <label>" and use askChoice.
//      If askChoice returns 0, leave play right away.
// When the game stops, print:
//   Final HP: <hp>  Final loot: <loot>
//   Inventory (<count> items):
// and then print the bag.
// The bag is a List<Item>. Start with HP = startHp_ and loot = 0.
void Adventure::play() {
}

// ===================== ANALYZING =====================

// TODO 14: print exactly three lines:
//   Scenes: <count>
//   Endings: <count>
//   Longest playthrough: <height> choices
void Adventure::printStats() const {
    std::cout << "Scenes: " << tree_.countScenes() << "\n";
    std::cout << "Endings: " << tree_.countEndings() << "\n";
    std::cout << "Longest playthrough: " << tree_.height() << " choices\n";
}

// TODO 15: print every ending in the story, one per line.
void Adventure::printSpoilers() const {
}

// TODO 16: print every ending below scene.
// path holds the choice numbers taken to reach scene, like "2-1".
// Each ending prints as:
//   Path <path>: <scene text>
// If the very first scene is an ending, print "Path (start): <scene text>".
// Endings print in order: choice 1 before choice 2 before choice 3.
void Adventure::printEndings(Scene<Moment>* scene, const std::string& path) const {
}

// TODO 17: the greedy advisor.
// Start at the first scene with startHp_ and 0 loot.
// Enter scenes the same way play does (add HP and loot).
// Stop when hp is 0 or less, or you reach an ending.
// Otherwise always move to the choice with the MOST loot right now.
// On a tie, pick the lower choice number.
// Print:
//   Greedy advisor
//     Path: <path>
//     Loot: <loot>
//     Result: died               (or "Result: reached an ending")
void Adventure::greedyRun() const {
}

// TODO 18: the best total loot you can collect from scene down to an ending,
// arriving at scene with hp health and having taken path to get here.
//   - Return -1 if every way down from here kills you.
//   - Put the path of the best run into bestPath.
//   - Must be recursive and look at every choice.
int Adventure::bestLoot(Scene<Moment>* scene, int hp, const std::string& path, std::string& bestPath) const {
    return -1;
}

// TODO 19: the optimal advisor. Use bestLoot from the first scene.
// Print:
//   Optimal advisor
//     Path: <path>
//     Loot: <loot>
//     Result: reached an ending
// If no path survives, print only:
//   Optimal advisor
//     Result: no path survives this story
void Adventure::optimalRun() const {
}
