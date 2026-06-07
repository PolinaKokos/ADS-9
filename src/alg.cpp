// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include  "tree.h"

PMTree::PMTree(const std::vector<char>& src) : base(src) {
    top = new Node(0);

    std::vector<char> sorted_src = src;
    std::sort(sorted_src.begin(), sorted_src.end());

    for (char ch : sorted_src) {
        std::vector<char> leftover = sorted_src;
        leftover.erase(std::find(leftover.begin(), leftover.end(), ch));
        Node* child = generate(leftover);
        child->sym = ch;
        top->links.push_back(child);
    }
}

PMTree::~PMTree() {
    destroy(top);
}

PMTree::Node* PMTree::generate(const std::vector<char>& rest) {
    if (rest.empty()) return new Node(0);
    Node* cur = new Node(0);
    std::vector<char> sorted_rest = rest;
    std::sort(sorted_rest.begin(), sorted_rest.end());
    for (char ch : sorted_rest) {
        std::vector<char> leftover = sorted_rest;
        leftover.erase(std::find(leftover.begin(), leftover.end(), ch));
        Node* child = generate(leftover);
        child->sym = ch;
        cur->links.push_back(child);
    }
    return cur;
}

void PMTree::destroy(Node* ptr) {
    if (!ptr) return;
    for (auto child : ptr->links) {
        destroy(child);
    }
    delete ptr;
}

void collectPaths(PMTree::Node* start, std::vector<char>& buffer,
    std::vector<std::vector<char>>& result, int currentDepth, int maxDepth) {
    if (currentDepth == maxDepth) {
        result.push_back(buffer);
        return;
    }
    for (auto next : start->links) {
        buffer.push_back(next->sym);
        collectPaths(next, buffer, result, currentDepth + 1, maxDepth);
        buffer.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(PMTree& obj) {
    std::vector<std::vector<char>> result;
    std::vector<char> buffer;
    int total = obj.base.size();
    for (auto first : obj.top->links) {
        buffer.push_back(first->sym);
        collectPaths(first, buffer, result, 1, total);
        buffer.pop_back();
    }
    return result;
}

std::vector<char> getPerm1(PMTree& obj, int pos) {
    auto full = getAllPerms(obj);
    if (pos <= 0 || pos > static_cast<int>(full.size())) return {};
    return full[pos - 1];
}

size_t fact(int n) {
    size_t res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

std::vector<char> getPerm2(PMTree& obj, int pos) {
    int total = obj.base.size();
    if (pos <= 0 || static_cast<size_t>(pos) > fact(total)) return {};

    std::vector<char> result;
    int remainder = pos - 1;
    PMTree::Node* current = obj.top;

    for (int step = 0; step < total; ++step) {
        size_t block = fact(total - step - 1);
        int idx = remainder / block;
        remainder %= block;
        if (idx >= static_cast<int>(current->links.size())) {
            return {};
        }
        current = current->links[idx];
        result.push_back(current->sym);
    }
    return result;
}
