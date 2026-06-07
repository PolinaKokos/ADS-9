// Copyright 2022 NNTU-CS
#include "tree.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::ofstream;
using std::fixed;
using std::setprecision;
using std::string;
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

int generateRandomPermNumber(int maxNum, mt19937& rng) {
  uniform_int_distribution<int> dist(1, maxNum);
  return dist(rng);
}

template<typename Func>
double measureTime(Func func, int n, int permNumber) {
  vector<char> in;
  for (int i = 0; i < n; ++i) {
    in.push_back('a' + i);
  }

  PMTree tree(in);

  auto start = high_resolution_clock::now();

  if (permNumber == -1) {
    getAllPerms(tree);
  } else {
    func(tree, permNumber);
  }

  auto end = high_resolution_clock::now();
  duration<double> elapsed = end - start;

  return elapsed.count();
}

vector<char> callGetPerm1(const PMTree& tree, int num) {
  return getPerm1(tree, num);
}

vector<char> callGetPerm2(const PMTree& tree, int num) {
  return getPerm2(tree, num);
}

int main() {
  cout << "=== Demonstration ===" << endl << endl;

  vector<char> in1 = {'1', '2', '3'};
  PMTree tree1(in1);

  cout << "Input set: ";
  for (char c : in1) cout << c << " ";
  cout << endl << endl;

  cout << "All permutations (getAllPerms):" << endl;
  auto allPerms = getAllPerms(tree1);
  for (size_t i = 0; i < allPerms.size(); ++i) {
    cout << i + 1 << ": ";
    for (char c : allPerms[i]) cout << c;
    cout << endl;
  }
  cout << endl;

  cout << "Permutation #2 (getPerm1): ";
  auto perm1 = getPerm1(tree1, 2);
  for (char c : perm1) cout << c;
  cout << endl;

  cout << "Permutation #3 (getPerm2): ";
  auto perm2 = getPerm2(tree1, 3);
  for (char c : perm2) cout << c;
  cout << endl << endl;

  cout << "Permutation #10 (not exists): ";
  auto perm3 = getPerm1(tree1, 10);
  if (perm3.empty()) cout << "empty";
  cout << endl << endl;

  vector<char> in2 = {'1', '3', '5', '7'};
  PMTree tree2(in2);

  cout << "Example with set {1,3,5,7}:" << endl;
  cout << "First 5 permutations:" << endl;
  for (int i = 1; i <= 5; ++i) {
    auto perm = getPerm2(tree2, i);
    cout << i << ": ";
    for (char c : perm) cout << c;
    cout << endl;
  }
  cout << endl;

  cout << "=== PERFORMANCE EXPERIMENT ===" << endl;
  cout << "Measuring execution time..." << endl << endl;

  vector<int> sizes = {3, 4, 5, 6, 7, 8, 9, 10};
  vector<double> timesGetAllPerms, timesGetPerm1, timesGetPerm2;

  random_device rd;
  mt19937 rng(rd());

  cout << fixed << setprecision(6);
  cout << "n\tgetAllPerms\tgetPerm1\tgetPerm2" << endl;
  cout << string(60, '-') << endl;

  for (int n : sizes) {
    int totalPerms = 1;
    for (int i = 2; i <= n; ++i) totalPerms *= i;
    int randomPerm = generateRandomPermNumber(totalPerms, rng);

    double timeAll = measureTime(
        [](const PMTree& t, int) {
          getAllPerms(t);
          return vector<char>();
        }, n, -1);

    double time1 = measureTime(callGetPerm1, n, randomPerm);
    double time2 = measureTime(callGetPerm2, n, randomPerm);

    timesGetAllPerms.push_back(timeAll);
    timesGetPerm1.push_back(time1);
    timesGetPerm2.push_back(time2);

    cout << n << "\t"
         << timeAll << "\t"
         << time1 << "\t"
         << time2 << endl;
  }

  ofstream dataFile("result/data.txt");
  if (dataFile.is_open()) {
    dataFile << "# n getAllPerms getPerm1 getPerm2" << endl;
    for (size_t i = 0; i < sizes.size(); ++i) {
      dataFile << sizes[i] << " "
               << timesGetAllPerms[i] << " "
               << timesGetPerm1[i] << " "
               << timesGetPerm2[i] << endl;
    }
    dataFile.close();
    cout << "\nData saved to result/data.txt" << endl;
  }

  ofstream gnuplotScript("result/plot.gnu");
  if (gnuplotScript.is_open()) {
    gnuplotScript << "set terminal pngcairo size 800,600 enhanced "
                  << "font 'Verdana,10'\n";
    gnuplotScript << "set output 'plot.png'\n";
    gnuplotScript << "set logscale y\n";
    gnuplotScript << "set xlabel 'n (alphabet size)'\n";
    gnuplotScript << "set ylabel 'Time (seconds)'\n";
    gnuplotScript << "set title 'Performance Comparison'\n";
    gnuplotScript << "set grid\n";
    gnuplotScript << "set key left top\n";
    gnuplotScript << "plot 'data.txt' using 1:2 with linespoints "
                  << "title 'getAllPerms' lw 2, \\\n";
    gnuplotScript << "     'data.txt' using 1:3 with linespoints "
                  << "title 'getPerm1 (slow)' lw 2, \\\n";
    gnuplotScript << "     'data.txt' using 1:4 with linespoints "
                  << "title 'getPerm2 (fast)' lw 2\n";
    gnuplotScript.close();

    cout << "Gnuplot script saved to result/plot.gnu" << endl;
    cout << "To create plot run: cd result && gnuplot plot.gnu" << endl;
  }

  cout << "\n=== ANALYSIS ===" << endl;
  cout << "1. getAllPerms(): exponential complexity O(n!)" << endl;
  cout << "2. getPerm1(): linear complexity O(n!)" << endl;
  cout << "3. getPerm2(): factorial complexity O(n!) with "
       << "smaller constant" << endl;
  cout << "\nRecommendation: Use getPerm2() for individual permutations."
       << endl;

  return 0;
}
