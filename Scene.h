#pragma once

// One scene of the story and the doors leading out of it.
//
// RULES (do not change):
//   - Keep every member variable, name, and signature exactly as written.
//   - choices_ must stay a double pointer: Scene<T>** choices_.
//   - No std::vector, no std::array, no fixed-size array like choices_[3].
//   - The scene OWNS data_ and every scene its slots point to.
template<typename T>
class Scene {
private:
    T *data_;
    Scene<T> **choices_;
    int numChoices_;

public:
    static const int MAX_CHOICES = 3;

    Scene(T *data, int numChoices) : data_(data), choices_(nullptr), numChoices_(numChoices) {
        // Allocate and initialize the choice slots for a non-ending scene
        if (!isEnding()) {
            choices_ = new Scene<T> *[numChoices_];

            for (int i = 0; i < numChoices_; ++i) {
                choices_[i] = nullptr;
            }
        }
    }

    ~Scene() {
        // Destroy all child scenes and release the memory owned by this scene
        for (int i = 0; i < numChoices_; ++i)
            delete choices_[i];

        delete[] choices_;

        delete data_;
    }

    T *getData() const {
        return data_;
    }

    int getNumChoices() const {
        return numChoices_;
    }

    bool isEnding() const {
        if (numChoices_ == 0)
            return true;

        return false;
    }

    Scene<T> *getChoice(int index) const {
        if (index < 0 || index >= numChoices_)
            return nullptr;

        return choices_[index];
    }

    void setChoice(int index, Scene<T> *next) {
        if (index < 0 || index >= numChoices_)
            return;

        // Replace the child at this index, deleting the previously owned child first
        if (next != choices_[index]) {
            delete choices_[index];
            choices_[index] = next;
        }
    }
};
