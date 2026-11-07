#pragma once
#include <iostream>
#include <string>

// One thing the player carries. GIVEN. You do not need to change this file.
class Item {
private:
    std::string name_;

public:
    Item(const std::string& name) : name_(name) {}

    std::string getName() const { return name_; }

    bool operator==(const Item& other) const {
        return name_ == other.name_;
    }

    friend std::ostream& operator<<(std::ostream& out, const Item& item) {
        out << item.name_;
        return out;
    }
};
