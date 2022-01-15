//
// Created by Ryan.Zurrin001 on 1/14/2022.
//

#ifndef FLOWNETWORK_BAG_H
#define FLOWNETWORK_BAG_H

#include <bits/stdc++.h>
using namespace std;

template<typename ITEM>
class Node {
public:
    ITEM item;
    Node<ITEM> *next;

    Node(ITEM item_) {
        this->item = item_;
        this->next = nullptr;
    }
};


template <typename ITEM>
class Bag {
    Node<ITEM> *first_; // first_ node in the bag
    int n; // number of items in the bag
public:
    //using ValueType = ITEM;
    Bag() {
        first_ = nullptr;
        n = 0;
    }
    ~Bag() {
        Node<ITEM> *tmp;
        while (first_ != nullptr) {
            tmp = first_;
            first_ = first_->next;
            delete tmp;
        }
    }
    bool isEmpty() const {
        return first_ == nullptr;
    }
    int size() const {
        return n;
    }
    void add(ITEM &item) {
        Node<ITEM> *oldfirst_ = first_;
        first_ = new Node<ITEM>(item);
        first_->item = item;
        first_->next = oldfirst_;
        n++;
    }
    bool contains(const ITEM &item) const {
        Node<ITEM> *current = first_;
        while (current != nullptr) {
            if (current->item == item) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    // make a getFirst() method to get address of first item
    Node<ITEM> first() const {
        return *first_;
    }

    void remove(const ITEM &item) {
        Node<ITEM> *current = first_;
        Node<ITEM> *previous = nullptr;
        while (current != nullptr) {
            if (current->item == item) {
                if (previous == nullptr) {
                    first_ = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                n--;
                return;
            }
            previous = current;
            current = current->next;
        }
    }
    void clear() {
        Node<ITEM> *tmp;
        while (first_ != nullptr) {
            tmp = first_;
            first_ = first_->next;
            delete tmp;
        }
        n = 0;
    }
    void print() const {
        Node<ITEM> *current = first_;
        while (current != nullptr) {
            std::cout << current->item << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // add an iterator to the bag
    class Iterator {
    public:
        Iterator(Node<ITEM> *current) {
            this->current = current;
        }
        Iterator(const Iterator &other) {
            this->current = other.current;
        }
        Iterator &operator=(const Iterator &other) {
            this->current = other.current;
            return *this;
        }
        Iterator &operator++() {
            this->current = this->current->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        bool operator==(const Iterator &other) const {
            return this->current == other.current;
        }
        bool operator!=(const Iterator &other) const {
            return this->current != other.current;
        }
        ITEM &operator*() {
            return this->current->item;
        }
        ITEM *operator->() {
            return &(this->current->item);
        }

        bool hasNext() {
            return this->current->next != nullptr;
        }

        FlowEdge *next() {
            FlowEdge *edge = this->current->item;
            this->current = this->current->next;
            return edge;
        }

    private:
        Node<ITEM> *current;
    };

    Iterator begin() {
        return Iterator(first_);
    }
    Iterator end() {
        return Iterator(nullptr);
    }
    // overload the [] operator
    ITEM &operator[](int index) {
        Node<ITEM> *current = first_;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        // make sure the index is valid
        if (current == nullptr) {
            throw std::out_of_range("Index out of range");
        }
        return current->item;
    }
    // overload the () operator
    ITEM &operator()(int index) {
        Node<ITEM> *current = first_;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        // make sure the index is valid
        if (current == nullptr) {
            throw std::out_of_range("Index out of range");
        }
        return current->item;
    }

    // over load the operator <<
    friend ostream &operator<<(ostream &os, Bag<ITEM> &bag) {
        for (auto it = bag.begin(); it != bag.end(); ++it) {
            os << *it << " ";
        }
        return os;
    }
    // over load the relational operators
    bool operator==(const Bag<ITEM> &other) const {
        if (this->n != other.n) {
            return false;
        }
        Node<ITEM> *current = this->first_;
        Node<ITEM> *otherCurrent = other.first_;
        while (current != nullptr) {
            if (current->item != otherCurrent->item) {
                return false;
            }
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
        return true;
    }
    bool operator!=(const Bag<ITEM> &other) const {
        return !(*this == other);
    }
    bool operator<(const Bag<ITEM> &other) const {
        if (this->n < other.n) {
            return true;
        }
        if (this->n > other.n) {
            return false;
        }
        Node<ITEM> *current = this->first_;
        Node<ITEM> *otherCurrent = other.first_;
        while (current != nullptr) {
            if (current->item < otherCurrent->item) {
                return true;
            }
            if (current->item > otherCurrent->item) {
                return false;
            }
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
        return false;
    }
    bool operator>(const Bag<ITEM> &other) const {
        return other < *this;
    }
    bool operator<=(const Bag<ITEM> &other) const {
        return !(other < *this);
    }
    bool operator>=(const Bag<ITEM> &other) const {
        return !(*this < other);
    }

    ITEM get(int i) {
        Node<ITEM> *current = first_;
        for (int j = 0; j < i; j++) {
            current = current->next;
        }
        return current->item;
    }
    ITEM pickRandom() {
        if (n == 0) {
            throw std::out_of_range("Empty bag");
        }
        srand(time(NULL));
        int randomIndex = rand() % n;
        Node<ITEM> *current = first_;
        for (int i = 0; i < randomIndex; i++) {
            current = current->next;
        }
        return current->item;
    }
    ITEM removeRandom() {
        if (n == 0) {
            throw std::out_of_range("Empty bag");
        }
        srand(time(NULL));
        int randomIndex = rand() % n;
        Node<ITEM> *current = first_;
        for (int i = 0; i < randomIndex; i++) {
            current = current->next;
        }
        ITEM item = current->item;
        if (current == first_) {
            first_ = first_->next;
        } else {
            Node<ITEM> *previous = first_;
            while (previous->next != current) {
                previous = previous->next;
            }
            previous->next = current->next;
        }
        delete current;
        n--;
        return item;
    }
};


#endif //FLOWNETWORK_BAG_H
