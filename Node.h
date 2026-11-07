#pragma once

// One link in a List.
// It holds a pointer to the data and a pointer to the next link.
template <typename T>
class Node {
public:
    T* data;
    Node<T>* next;

    Node(T* value) : data(value), next(nullptr) {}
};
