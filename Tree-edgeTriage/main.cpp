#include <iostream>
#include <bits/stdc++.h>
#include "Tree.h"

bool checkTree(Node* root) {
    if (!root) return true;

    vector<Node*> children;
    bool allAChildren = true;
    for (Node* child : root->children) {
        if(!child) continue;
        if(!child->isA) {
            allAChildren = false;
        } else {
            children.push_back(child);
        }
    }
    if(!allAChildren) return false;
    if(children.empty()) return true;

    for (Node* child : children) {
        bool hasBInGrandchildren = false;
        for (Node* grandchild : child->children) {
            if (grandchild && !grandchild->isA) {
                hasBInGrandchildren = true;
                break;
            }
        }
        if (hasBInGrandchildren) continue;

        for (Node* grandchild : child->children) {
            if (checkTree(grandchild)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    int trials = 100;
    int curr = 0;
    for(int i = 0; i < trials; ++i) {
        Tree tree(16, 2, 0.90);
        if(checkTree(tree.root)) curr++;
    }
    cout << static_cast<double>(curr)/trials << " Success rate" << endl;
}
