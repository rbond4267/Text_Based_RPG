#pragma once
#include <iostream>
#include "Node.h"

// A simple linked list. GIVEN. You do not need to change this file.
// The list OWNS every T* you give it. It deletes them when they leave.
template <typename T>
class List {
private:
    Node<T>* head_;
    int size_;

public:
    List() : head_(nullptr), size_(0) {}

    ~List() {
        while (!isEmpty()) {
            deleteFront();
        }
    }

    // Puts a new value at the front. The list now owns value.
    void addFront(T* value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head_;
        head_ = newNode;
        size_++;
    }

    // Removes the front value and deletes it.
    void deleteFront() {
        if (head_ == nullptr) {
            return;
        }
        Node<T>* doomed = head_;
        head_ = head_->next;
        delete doomed->data;
        delete doomed;
        size_--;
    }

    // Returns true if a value equal to target is in the list.
    bool search(const T& target) const {
        Node<T>* current = head_;
        while (current != nullptr) {
            if (*(current->data) == target) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool isEmpty() const {
        return head_ == nullptr;
    }

    int size() const {
        return size_;
    }

    // Prints one value per line.
    void print() const {
        if (head_ == nullptr) {
            std::cout << "  (empty)" << std::endl;
            return;
        }
        Node<T>* current = head_;
        while (current != nullptr) {
            std::cout << "  - " << *(current->data) << std::endl;
            current = current->next;
        }
    }
};
