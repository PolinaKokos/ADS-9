// Copyright 2022 NNTU-CS
#ifndef PMTREE_H
#define PMTREE_H

#include <vector>
#include <memory>
#include <algorithm>

class PMTree {
private:
    struct Node {
        char value;
        std::vector<std::shared_ptr<Node>> children;
        
        Node(char val) : value(val) {}
    };
    
    std::shared_ptr<Node> root;
    std::vector<char> originalSet;
    
    void buildTree(std::shared_ptr<Node> currentNode, std::vector<char> remaining);
    void collectPermutations(std::shared_ptr<Node> node, std::vector<char>& current, 
                            std::vector<std::vector<char>>& result) const;
    std::vector<char> getPermByNavigation(int num) const;
    
public:
    PMTree(const std::vector<char>& elements);
    ~PMTree() = default;
    
    friend std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
    friend std::vector<char> getPerm1(const PMTree& tree, int num);
    friend std::vector<char> getPerm2(const PMTree& tree, int num);
    
    const std::vector<char>& getOriginalSet() const { return originalSet; }
    std::shared_ptr<Node> getRoot() const { return root; }
    
    int getTotalPermutations() const;
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif
