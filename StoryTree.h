#pragma once
#include "Scene.h"
#include <algorithm>

// The whole story. It knows the first scene and nothing else.
//
// RULES (do not change):
//   - Keep every name and signature exactly as written.
//   - The private helpers must be recursive. No loops that walk down the tree.
//   - This class must work for ANY T. Never mention Moment in here.
template <typename T>
class StoryTree {
private:
    Scene<T>* root_;

    int countScenes(Scene<T>* scene) const {
        if (scene == nullptr)
            return 0;

        int count {1};

        // Traverse all branches, summing each count while unwinding
        count += countScenes(scene->getChoice(0));
        count += countScenes(scene->getChoice(1));
        count += countScenes(scene->getChoice(2));

        return count;
    }

    int countEndings(Scene<T>* scene) const {
        if (scene == nullptr)
            return 0;

        if (scene->isEnding())
            return 1;

        int count {0};

        // Traverse all branches to find the endings, then recursively sum them
        count += countEndings(scene->getChoice(0));
        count += countEndings(scene->getChoice(1));
        count += countEndings(scene->getChoice(2));

        return count;
    }

    int height(Scene<T>* scene) const {
        if (scene == nullptr)
            return -1;

        if (scene->isEnding())
            return 0;

        // Traverse each subtree and store the height, then find the max height
        int heightSubtree1 { height(scene->getChoice(0)) };
        int heightSubtree2 { height(scene->getChoice(1)) };
        int heightSubtree3 { height(scene->getChoice(2)) };

        int max1 = std::max(heightSubtree1, heightSubtree2);
        int max2 = std::max(heightSubtree3, max1);
        return max2 + 1;
    }

public:
    StoryTree() : root_(nullptr) {}

    ~StoryTree() {
        clear();
    }

    void setRoot(Scene<T>* root) {
        clear();
        root_ = root;
    }

    Scene<T>* getRoot() const {
        return root_;
    }

    bool isEmpty() const {
        return root_ == nullptr;
    }

    void clear() {
        if (isEmpty())
            return;

        delete root_;
        root_ = nullptr;
    }

    int countScenes() const {
        return countScenes(root_);
    }

    int countEndings() const {
        return countEndings(root_);
    }

    int height() const {
        return height(root_);
    }
};
