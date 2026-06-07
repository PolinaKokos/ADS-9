// Copyright 2022 NNTU-CS
#include "../include/tree.h"

#include <algorithm>
#include <memory>
#include <vector>

PMTree::PMTree(const std::vector<char>& elements) : originalSet(elements) {
  if (elements.empty()) {
    root = nullptr;
    return;
  }

  std::vector<char> sorted = elements;
  std::sort(sorted.begin(), sorted.end());
  originalSet = sorted;

  root = std::make_shared<Node>('\0');
  buildTree(root, sorted);
}

void PMTree::buildTree(std::shared_ptr<Node> currentNode,
                       std::vector<char> remaining) {
  if (remaining.empty()) {
    return;
  }

  for (char c : remaining) {
    auto child = std::make_shared<Node>(c);
    currentNode->children.push_back(child);

    std::vector<char> newRemaining;
    for (char ch : remaining) {
      if (ch != c) {
        newRemaining.push_back(ch);
      }
    }

    buildTree(child, newRemaining);
  }
}

void PMTree::collectPermutations(std::shared_ptr<Node> node,
                                 std::vector<char>& current,
                                 std::vector<std::vector<char>>& result) const {
  if (!node || node->value == '\0') {
    for (auto& child : node->children) {
      current.push_back(child->value);
      collectPermutations(child, current, result);
      current.pop_back();
    }
    return;
  }

  if (node->children.empty()) {
    result.push_back(current);
    return;
  }

  for (auto& child : node->children) {
    current.push_back(child->value);
    collectPermutations(child, current, result);
    current.pop_back();
  }
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  std::vector<std::vector<char>> result;
  if (!tree.root) return result;

  std::vector<char> current;
  tree.collectPermutations(tree.root, current, result);

  return result;
}

int PMTree::getTotalPermutations() const {
  int total = 1;
  for (int i = 2; i <= static_cast<int>(originalSet.size()); ++i) {
    total *= i;
  }
  return total;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
  if (num < 1) return std::vector<char>();

  auto allPerms = getAllPerms(tree);

  if (num > static_cast<int>(allPerms.size())) {
    return std::vector<char>();
  }

  return allPerms[num - 1];
}

std::vector<char> PMTree::getPermByNavigation(int num) const {
  if (num < 1 || originalSet.empty()) {
    return std::vector<char>();
  }

  int totalPerms = getTotalPermutations();
  if (num > totalPerms) {
    return std::vector<char>();
  }

  std::vector<char> result;
  std::vector<char> available = originalSet;
  int remaining = num - 1;

  for (size_t i = 0; i < originalSet.size(); ++i) {
    int blockSize = 1;
    for (int j = 1; j <= static_cast<int>(available.size()) - 1; ++j) {
      blockSize *= j;
    }

    int index = remaining / blockSize;
    result.push_back(available[index]);

    available.erase(available.begin() + index);
    remaining %= blockSize;
  }

  return result;
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
  return tree.getPermByNavigation(num);
}
