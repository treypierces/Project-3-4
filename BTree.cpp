//
// Created by Trey Pierce on 11/10/26.
//

#include "BTree.h"

#include <algorithm>
#include <optional>
#include <utility>
#include <cstdint>

template <typename T>
BTree<T>::BTree(const uint32_t degree) : degree(std::max(degree, static_cast<uint32_t>(2))) {
}

template <typename T>
BTree<T>::~BTree() {
    delete root;
}

template <typename T>
BTree<T>::Node::~Node() {
    for (const auto& child : children) {
        delete child;
    }
}

template <typename T>
uint32_t BTree<T>::Node::lowerBound(KeyType key) const {
    return std::ranges::lower_bound(keys, key, {}, &Key::key) - keys.begin();
}

template <typename T>
uint32_t BTree<T>::Node::stealKey(Node* node, const uint32_t index) {
    const Key key{node->keys[index]};

    node->keys.erase(node->keys.begin() + index);

    --node->numKeys;
    ++numKeys;

    return keys.insert(keys.begin() + lowerBound(key.key), key) - keys.begin();
}

template <typename T>
BTree<T>::Node* BTree<T>::Node::split() {
    Node* newParent{parent}; // the new parent of this and the other node that this splits into

    if (parent == nullptr) {
        newParent = new Node{degree};
    }

    Node* newNode{new Node{degree, newParent}}; // takes top half of this keys
    const uint32_t middle{numKeys / 2};

    newNode->leaf = leaf;
    newNode->keys.assign(keys.begin() + middle + 1, keys.end()); // steal the keys
    newNode->numKeys = numKeys - middle - 1;

    keys.erase(keys.begin() + middle + 1, keys.end()); // remove stolen keys

    numKeys -= newNode->numKeys;

    if (!leaf) { // steal the children
        newNode->children.assign(children.begin() + middle + 1, children.end());

        children.erase(children.begin() + middle + 1, children.end());
    }

    for (const auto& child : newNode->children) {
        child->parent = newNode;
    }

    const uint32_t newIndex{newParent->stealKey(this, middle)}; // take this median key

    if (parent != newParent) {
        parent = newParent; // make this child of the new parent

        parent->children.push_back(this); // children are empty, pushing this then newNode should have correct ordering
        parent->children.push_back(newNode);
    } else {
        parent->children.insert(parent->children.begin() + newIndex + 1, newNode);
    }

    newParent->leaf = false;

    if (newParent-> numKeys > (2 * newParent->degree) - 1) {
        newParent->split();
    }

    return newNode;
}

template <typename T>
void BTree<T>::Node::insertKeyAt(KeyType key, T& value, const uint32_t index) {
    keys.insert(keys.begin() + index, Key{key, value});

    ++numKeys;
}

template <typename T>
std::pair<typename BTree<T>::Node*, uint32_t> BTree<T>::Node::insertKey(KeyType key, T& value) {
    if (!leaf) {
        Node* curNode{this};

        while (!curNode->leaf) {
            uint32_t pos{curNode->lowerBound(key)};

            if (pos < curNode->numKeys && curNode->keys[pos].key == key) {
                curNode->keys[pos].value = value;

                return std::pair{curNode, pos};
            }

            curNode = curNode->children[pos];
        }

        return curNode->insertKey(key, value);
    }

    if (numKeys == (2 * degree) - 1) {
        if (const uint32_t pos{lowerBound(key)}; pos < numKeys && keys[pos].key == key) {
            keys[pos].value = value;

            return std::pair{this, pos};
        }

        const KeyType oldKey{keys[numKeys / 2].key};
        Node* newNode{split()};

        if (oldKey < key) {
            return newNode->insertKey(key, value);
        }

        return insertKey(key, value);
    }

    uint32_t pos{lowerBound(key)};

    if (pos < numKeys && keys[pos].key == key) {
        keys[pos].value = value;
    } else {
        insertKeyAt(key, value, pos);
    }

    return std::pair{this, pos};
}

template <typename T>
void BTree<T>::insert(KeyType key, const T value) {
    root->insertKey(key, value);

    if (root->parent != nullptr) {
        root = root->parent;
    }
}

template <typename T>
std::optional<std::pair<typename BTree<T>::Node*, uint32_t>> BTree<T>::Node::search(KeyType key) {
    uint32_t child{lowerBound(key)};

    if (child < numKeys && keys[child].key == key) {
        return std::pair{this, child};
    }

    if (!leaf) {
        return children[child]->search(key);
    }

    return std::nullopt; // value does not exist in tree, let caller know
}

template <typename T>
std::optional<T> BTree<T>::search(KeyType key) const {
    if (const std::optional res{root->search(key)}; res.has_value()) {
        return res->first->keys[res->second].value;
    }

    return std::nullopt;
}

template <typename T>
std::optional<T> BTree<T>::search(const std::string& last, const std::string& first, const std::string& date) {
    return search({last, first, date});
}