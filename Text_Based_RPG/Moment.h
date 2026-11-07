#pragma once
#include <iostream>
#include <string>

// The payload of one scene. GIVEN. You do not need to change this file.
class Moment {
private:
    std::string text_;
    std::string label_;
    int hpChange_;
    int loot_;
    std::string item_;

public:
    Moment(const std::string& text, const std::string& label, int hpChange, int loot, const std::string& item)
        : text_(text), label_(label), hpChange_(hpChange), loot_(loot), item_(item) {}

    std::string getText() const { return text_; }
    std::string getLabel() const { return label_; }
    int getHpChange() const { return hpChange_; }
    int getLoot() const { return loot_; }
    std::string getItem() const { return item_; }

    // True if this scene gives the player an item.
    bool hasItem() const { return item_ != "none"; }

    friend std::ostream& operator<<(std::ostream& out, const Moment& moment) {
        out << moment.text_;
        return out;
    }
};
