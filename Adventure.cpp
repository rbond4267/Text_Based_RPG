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
        std::cout << "Error: scene \"" << sceneName << "\" has " << numChoices << " choices. The limit is 3.\n";
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

void Adventure::play() {
    Scene<Moment>* currentScene = tree_.getRoot();
    List<Item> inventory;
    int Hp = startHp_;
    int loot = 0;
    int choice;
    
    // The while loop accesses the current scene's moment data, print the scene data, then gets user's choice.
    while (true) {
        Moment* currentMoment = currentScene->getData();
        
        Hp += currentMoment->getHpChange();
        loot += currentMoment->getLoot();

        std::cout << "\n----------------------------------------\n";
        std::cout << currentMoment->getText() << std::endl;

        // Add item to the list if the item has not been picked up already.
        if (currentMoment->hasItem()) {
            Item* currentItem = new Item(currentMoment->getItem());
            if (inventory.search(*currentItem)) {
                std::cout << "You already have: " << currentMoment->getItem() << "\n";
                delete currentItem;
            }
            else {
                inventory.addFront(currentItem);
                std::cout << "You picked up: " << currentMoment->getItem() << "\n";
            }
        }

        std::cout << "HP: "<< Hp << "  Loot: " << loot << "\n";

        // End the game if the player dies or an ending is reached.
        if (Hp <= 0) {
            std::cout << "Your HP hit 0. GAME OVER.\n";
            std::cout << "Final HP: " << Hp << "  Final loot: " << loot << "\n";
            std::cout << "Inventory (" << inventory.size() << " items):\n";
            inventory.print();
            return;
        }
        if (currentScene->isEnding()) {
            std::cout << "THE END.\n";
            std::cout << "Final HP: " << Hp << "  Final loot: " << loot << "\n";
            std::cout << "Inventory (" << inventory.size() << " items):\n";
            inventory.print();
            return;
        }

        // Print the available choices with corresponding labels for a given scene.
        for (int i = 0; i < currentScene->getNumChoices(); ++i) {
            Scene<Moment>* nextScene = currentScene->getChoice(i);
            Moment* nextMoment = nextScene->getData();

            std::cout << "[" << i + 1 << "] " << nextMoment->getLabel() << "\n";
        }

        choice = askChoice(currentScene->getNumChoices());
        
        if (choice == 0)
        {
            std::cout << "Final HP: " << Hp << "  Final loot: " << loot << "\n";
            std::cout << "Inventory (" << inventory.size() << " items):\n";
            inventory.print();
            return;
        }
        
        currentScene = currentScene->getChoice(choice - 1);
    }
}

// ===================== ANALYZING =====================

void Adventure::printStats() const {
    std::cout << "Scenes: " << tree_.countScenes() << "\n";
    std::cout << "Endings: " << tree_.countEndings() << "\n";
    std::cout << "Longest playthrough: " << tree_.height() << " choices\n";
}

void Adventure::printSpoilers() const {
    printEndings(tree_.getRoot(), "");
}

// TODO 16: print every ending below scene.
// path holds the choice numbers taken to reach scene, like "2-1".
// Each ending prints as:
//   Path <path>: <scene text>
// If the very first scene is an ending, print "Path (start): <scene text>".
// Endings print in order: choice 1 before choice 2 before choice 3.
void Adventure::printEndings(Scene<Moment>* scene, const std::string& path) const {
    if (scene == nullptr) {
        return;
    }
    
    if (scene->isEnding()) {
        Moment* currentMoment = scene->getData();
        
        if (path.empty()) {
            std::cout << "Path (start): " << currentMoment->getText() << "\n";
            return;
        }
        std::cout << "Path " << path << ": " << currentMoment->getText() << "\n";
        return;
    }
    
    for (int i = 0; i < scene->getNumChoices(); ++i) {
        Scene<Moment>* nextScene = scene->getChoice(i);
        printEndings(nextScene, extendPath(path, i + 1));
    }
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
