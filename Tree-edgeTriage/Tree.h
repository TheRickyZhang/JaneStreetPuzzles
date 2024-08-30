#pragma once

#include <vector>
#include <cstdlib>
using namespace std;

struct Node {
    int id;
    bool isA;
    std::vector<Node*> children;

    Node(int _id, bool _isA)
        : id(_id), isA(_isA) {}

    void assignChildren(int numChildren, double probability) {
        for (int i = 0; i < numChildren; ++i) {
            bool label = static_cast<double>(rand()) / RAND_MAX < probability;
            children.push_back(new Node(id * numChildren + i, label));
        }
    }
};

struct Tree {
    Node* root;

    Tree(int levels, int numChildren, double probability) {
        root = new Node(0, true);
        buildTree(root, levels, numChildren, probability);
    }

    void buildTree(Node* node, int levels, int numChildren, double probability, int currentLevel = 0) {
        if (currentLevel >= levels) return;

        node->assignChildren(numChildren, probability);
        for (Node* child : node->children) {
            buildTree(child, levels, numChildren, probability, currentLevel + 1);
        }
    }
};
