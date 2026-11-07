#pragma once
#include <fstream>
#include <string>
#include "Moment.h"
#include "Item.h"
#include "List.h"
#include "StoryTree.h"

// The game. It loads a story, plays it, and analyzes it.
class Adventure {
private:
    StoryTree<Moment> tree_;
    int startHp_;

    // GIVEN helpers
    bool nextLine(std::ifstream& in, std::string& line);
    bool readField(std::ifstream& in, const std::string& keyword, std::string& value);
    std::string extendPath(const std::string& path, int choiceNumber) const;

    // You write these
    Scene<Moment>* readScene(std::ifstream& in, const std::string& label);
    int askChoice(int numChoices);
    void printEndings(Scene<Moment>* scene, const std::string& path) const;
    int bestLoot(Scene<Moment>* scene, int hp, const std::string& path, std::string& bestPath) const;

public:
    Adventure() : startHp_(0) {}

    bool load(const std::string& fileName);
    void play();
    void printStats() const;
    void printSpoilers() const;
    void greedyRun() const;
    void optimalRun() const;
};
