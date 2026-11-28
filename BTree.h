//
// Created by Trey Pierce on 11/10/26.
//

#ifndef PROJECT_3_4_BTREE_H
#define PROJECT_3_4_BTREE_H
#include <utility>
#include <vector>
#include <optional>
#include <cstdint>
#include <functional>

using KeyType = int;

template <typename T>
class BTree {
public:
    explicit BTree(uint32_t degree);
    ~BTree();
    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;

    void insert(KeyType key, T value);
    [[nodiscard]] std::optional<T> search(KeyType key) const;
    void forEach(const std::function<void(T&)>& action);

private:
    struct Node {
        struct Key {
            Key(KeyType  key, const T value) : key{std::move(key)}, value{value} {}

            KeyType key{};
            T value{};
        };

        explicit Node(const uint32_t degree) : degree{degree} {}
        Node(const uint32_t degree, Node* const parent) : degree{degree}, parent{parent} {}
        ~Node();
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        uint32_t degree{2};
        uint32_t numKeys{0};
        std::vector<Key> keys{};
        std::vector<Node*> children{};
        Node* parent{nullptr};
        bool leaf{true};

        [[nodiscard]] uint32_t lowerBound(KeyType key) const;
        [[nodiscard]] std::optional<std::pair<Node*, uint32_t>> search(KeyType key);
        void forEach(const std::function<void(T&)>& action);
        Node* split();
        uint32_t stealKey(Node* node, uint32_t index);
        std::pair<Node*, uint32_t> insertKey(KeyType key, T& value);
        void insertKeyAt(KeyType key, T& value, uint32_t index);
    };

    uint32_t degree{2};
    Node* root{new Node{degree}};
};

#endif //PROJECT_3_4_BTREE_H
