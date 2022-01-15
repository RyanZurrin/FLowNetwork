//
// Created by Ryan.Zurrin001 on 1/15/2022.
//

#ifndef FLOWNETWORK_ULTRABAG_H
#define FLOWNETWORK_ULTRABAG_H

//
// Created by Ryan.Zurrin001 on 1/15/2022.
//


#include <bits/stdc++.h>
using namespace std;

template<typename ITEM>
class Node {
public:
    ITEM item;
    Node<ITEM> *next;
    Node<ITEM> *prev;

    explicit Node(ITEM item_) {
        this->item = item_;
        this->next = nullptr;
        this->prev = nullptr;
    }
};


template <typename ITEM>
class Bag {
    Node<ITEM> *first; // first node in the bag
    int n; // number of items in the bag
public:
    //using ValueType = ITEM;
    Bag() {
        first = nullptr;
        n = 0;
    }
    // copy constructor
    Bag(const Bag<ITEM> &other) {
        first = nullptr;
        n = 0;
        for (Node<ITEM> *p = other.first; p != nullptr; p = p->next) {
            add(p->item);
        }
    }
    // move constructor
    Bag(Bag<ITEM> &&other) {
        first = other.first;
        n = other.n;
        other.first = nullptr;
        other.n = 0;
    }
    // copy assignment
    Bag<ITEM> &operator=(const Bag<ITEM> &other) {
        if (this != &other) {
            clear();
            for (Node<ITEM> *p = other.first; p != nullptr; p = p->next) {
                add(p->item);
            }
        }
        return *this;
    }
    // move assignment
    Bag<ITEM> &operator=(Bag<ITEM> &&other) {
        if (this != &other) {
            clear();
            first = other.first;
            n = other.n;
            other.first = nullptr;
            other.n = 0;
        }
        return *this;
    }
    ~Bag() {
        Node<ITEM> *p = first;
        while (p != nullptr) {
            Node<ITEM> *q = p;
            p = p->next;
            delete q;
        }
    }
    [[nodiscard]] bool isEmpty() const {
        return first == nullptr;
    }
    [[nodiscard]] int size() const {
        return n;
    }
    void add(ITEM &item) {
        Node<ITEM> *tmp = new Node<ITEM>(item);
        tmp->next = first;
        if (first != nullptr) {
            first->prev = tmp;
        }
        first = tmp;
        n++;
    }
    bool contains(const ITEM &item) const {
        Node<ITEM> *current = first;
        while (current != nullptr) {
            if (current->item == item) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    void remove(const ITEM &item) {
        Node<ITEM> *current = first;
        while (current != nullptr) {
            if (current->item == item) {
                if (current->prev != nullptr) {
                    current->prev->next = current->next;
                }
                if (current->next != nullptr) {
                    current->next->prev = current->prev;
                }
                delete current;
                n--;
                return;
            }
            current = current->next;
        }
    }
    ITEM get(int i) {
        Node<ITEM> *current = first;
        for (int j = 0; j < i; j++) {
            current = current->next;
        }
        return current->item;
    }
    ITEM pickRandom() {
        if (n == 0) {
            throw std::out_of_range("Empty bag");
        }
        // generate using twister
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, n - 1);
        int index = dis(gen);
        Node<ITEM> *current = first;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->item;
    }
    ITEM removeRandom() {
        if (n == 0) {
            throw std::out_of_range("Empty bag");
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, n - 1);
        int index = dis(gen);
        Node<ITEM> *current = first;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        ITEM item = current->item;
        if (current == first) {
            first = first->next;
        } else {
            Node<ITEM> *previous = first;
            while (previous->next != current) {
                previous = previous->next;
            }
            previous->next = current->next;
        }
        delete current;
        n--;
    }
    void clear() {
        Node<ITEM> *tmp;
        while (first != nullptr) {
            tmp = first;
            first = first->next;
            delete tmp;
        }
        n = 0;
    }
    Node<ITEM> *getFirst() const {
        return first;
    }
    // sort the bag
    void sort();
    void shuffle();
    void print() const {
        Node<ITEM> *current = first;
        while (current != nullptr) {
            std::cout << current->item << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    class Iterator {
    public:
        Iterator() {
            current = nullptr;
        }

        explicit Iterator(Node<ITEM> *current) {
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
        Iterator operator--() {
            this->current = this->current->prev;
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp(*this);
            --(*this);
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
    private:
        Node<ITEM> *current;
    };
    class ReverseIterator {
    public:
        explicit ReverseIterator(Node<ITEM> *current) {
            this->current = current;
        }
        ReverseIterator(const ReverseIterator &other) {
            this->current = other.current;
        }
        ReverseIterator &operator=(const ReverseIterator &other) {
            this->current = other.current;
            return *this;
        }
        ReverseIterator &operator++() {
            this->current = this->current->prev;
            return *this;
        }
        ReverseIterator operator++(int) {
            ReverseIterator tmp(*this);
            ++(*this);
            return tmp;
        }
        ReverseIterator operator--() {
            this->current = this->current->next;
            return *this;
        }
        ReverseIterator operator--(int) {
            ReverseIterator tmp(*this);
            --(*this);
            return tmp;
        }

        bool operator==(const ReverseIterator &other) const {
            return this->current == other.current;
        }
        bool operator!=(const ReverseIterator &other) const {
            return this->current != other.current;
        }
        ITEM &operator*() {
            return this->current->item;
        }
        ITEM *operator->() {
            return &(this->current->item);
        }
    private:
        Node<ITEM> *current;
    };
    ReverseIterator rbegin() {
        // return an iterator to the last element by loop through the list
        // and return the last element
        Node<ITEM> *current = first;
        while (current->next != nullptr) {
            current = current->next;
        }
        return Bag::ReverseIterator(current);
    }
    ReverseIterator rend() {
        return Bag::ReverseIterator(nullptr);
    }
    Iterator begin() {
        return Iterator(first);
    }
    Iterator end() {
        return Iterator(nullptr);
    }

    ITEM &operator[](int index) {
        Node<ITEM> *current = first;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        // make sure the index is valid
        if (current == nullptr) {
            throw std::out_of_range("Index out of range");
        }
        return current->item;
    }
    ITEM &operator()(int index) {
        Node<ITEM> *current = first;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        // make sure the index is valid
        if (current == nullptr) {
            throw std::out_of_range("Index out of range");
        }
        return current->item;
    }

    friend ostream &operator<<(ostream &os, Bag<ITEM> &bag) {
        for (auto it = bag.begin(); it != bag.end(); ++it) {
            os << *it << " ";
        }
        return os;
    }
    bool operator==(const Bag<ITEM> &other) const {
        if (this->n != other.n) {
            return false;
        }
        // create two temp bags to store the two bags and then sort
        Bag<ITEM> tmp1 = *this;
        Bag<ITEM> tmp2 = other;
        tmp1.sort();
        tmp2.sort();
        // compare the two bags
        for (int i = 0; i < this->n; i++) {
            if (tmp1[i] != tmp2[i]) {
                return false;
            }
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
        Node<ITEM> *current = this->first;
        Node<ITEM> *otherCurrent = other.first;
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

    // overload the + operator to add two bags together
    Bag<ITEM> operator+(const Bag<ITEM> &other) const {
        Bag<ITEM> result;
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            result.add(current->item);
            current = current->next;
        }
        current = other.first;
        while (current != nullptr) {
            result.add(current->item);
            current = current->next;
        }
        return result;
    }
    // overload the += operator to add a bag to another bag
    Bag<ITEM> &operator+=(const Bag<ITEM> &other) {
        Node<ITEM> *current = other.first;
        while (current != nullptr) {
            this->add(current->item);
            current = current->next;
        }
        return *this;
    }
    // overload the - operator to remove an item from a bag
    Bag<ITEM> operator-(const ITEM &item) const {
        Bag<ITEM> result;
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            if (current->item != item) {
                result.add(current->item);
            }
            current = current->next;
        }
        return result;
    }
    // overload the -= operator to remove an item from a bag
    Bag<ITEM> &operator-=(const ITEM &item) {
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            if (current->item == item) {
                this->remove(current->item);
            }
            current = current->next;
        }
        return *this;
    }
    // overload the - to remove similar items from one bag out of another
    Bag<ITEM> operator-(const Bag<ITEM> &other) const {
        Bag<ITEM> result;
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            if (!other.contains(current->item)) {
                result.add(current->item);
            }
            current = current->next;
        }
        return result;
    }
    // overload the % to find the intersection of two bags
    Bag<ITEM> operator%(const Bag<ITEM> &other) const {
        Bag<ITEM> result;
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            if (other.contains(current->item)) {
                result.add(current->item);
            }
            current = current->next;
        }
        return result;
    }
    // overload the * to find the union of two bags
    Bag<ITEM> operator*(const Bag<ITEM> &other) const {
        Bag<ITEM> result;
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            result.add(current->item);
            current = current->next;
        }
        current = other.first;
        while (current != nullptr) {
            result.add(current->item);
            current = current->next;
        }
        return result;
    }
    // overload the *= to find the union of two bags
    Bag<ITEM> &operator*=(const Bag<ITEM> &other) {
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            this->add(current->item);
            current = current->next;
        }
        current = other.first;
        while (current != nullptr) {
            this->add(current->item);
            current = current->next;
        }
        return *this;
    }
    // overload the / to find the difference of both bags
    Bag<ITEM> operator/(const Bag<ITEM> &other) const {
        // go through both bags and any items that are not in both bags add to
        // the result so the resuly will contain elements from both bags
        Bag<ITEM> result;
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            if (!other.contains(current->item)) {
                result.add(current->item);
            }
            current = current->next;
        }
        current = other.first;
        while (current != nullptr) {
            if (!this->contains(current->item)) {
                result.add(current->item);
            }
            current = current->next;
        }
        return result;
    }
    // overload the /= to find the difference of both bags
    Bag<ITEM> &operator/=(const Bag<ITEM> &other) {
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            if (!other.contains(current->item)) {
                this->remove(current->item);
            }
            current = current->next;
        }
        current = other.first;
        while (current != nullptr) {
            if (!this->contains(current->item)) {
                this->remove(current->item);
            }
            current = current->next;
        }
        return *this;
    }
    // overload the ^ to find the complement of one bag
    Bag<ITEM> operator^(const Bag<ITEM> &other) const {
        Bag<ITEM> result;
        Node<ITEM> *current = this->first;
        while (current != nullptr) {
            if (!other.contains(current->item)) {
                result.add(current->item);
            }
            current = current->next;
        }
        return result;
    }



    // overload the << operator
    friend ostream &operator<<(ostream &out, const Bag<ITEM> &bag) {
        Node<ITEM> *current = bag.first;
        while (current != nullptr) {
            out << current->item << " ";
            current = current->next;
        }
        return out;
    }


};

template<typename ITEM>
void Bag<ITEM>::sort() {
    // sort the bag using insertion sort
    Node<ITEM> *current = this->first;
    while (current != nullptr) {
        Node<ITEM> *current2 = current->next;
        while (current2 != nullptr) {
            if (current->item > current2->item) {
                ITEM temp = current->item;
                current->item = current2->item;
                current2->item = temp;
            }
            current2 = current2->next;
        }
        current = current->next;
    }
}

template<typename ITEM>
void Bag<ITEM>::shuffle() {
    // shuffle the bag using the Fisher-Yates shuffle
    Node<ITEM> *current = this->first;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, this->size() - 1);
    while (current != nullptr) {
        Node<ITEM> *current2 = current->next;
        while (current2 != nullptr) {
            int index = dis(gen);
            if (index % 2 == 0) {
                ITEM temp = current->item;
                current->item = current2->item;
                current2->item = temp;
            }
            current2 = current2->next;
        }
        current = current->next;
    }
}


#endif //FLOWNETWORK_ULTRABAG_H
