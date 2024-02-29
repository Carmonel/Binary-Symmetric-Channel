#ifndef BINARY_SYMMETRIC_CHANNEL_THEORY_H
#define BINARY_SYMMETRIC_CHANNEL_THEORY_H

#include <vector>
#include <map>
#include <cmath>

#include "Utils.h"

int combinations(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    }

    return combinations(n - 1, k - 1) + combinations(n - 1, k);
}

double theoryUp(int n, double p){
    double result = 1;
    for (int i = 0; i <= n; ++i) {
        result -= combinations(n, i) * pow(p, i) * pow(1 - p, n - i);
    }

    return result;
}

double theory(const std::map<std::vector<bool>, std::vector<bool>>& codeBook, int n, double p){
    p = 1 - p;
    double Nt = 0.0;

    for (int i = 0; i <= n; ++i) {
        int Ai = 0;
        for (auto it = codeBook.begin(); it != codeBook.end(); it++){
            if (w(it->second) == i) Ai++;
        }
        Nt += Ai * pow(p, i) * pow(1 - p, n - i);
    }

    return Nt;
}

std::map<std::vector<bool>, std::vector<bool>> createCodeBook(const std::vector<bool>& g, int size){
    std::vector<bool> mvar(size);
    std::vector<std::vector<bool>> variations;
    generateVariations(mvar, 0, variations);
    std::map<std::vector<bool>, std::vector<bool>> map;

    for (int i = 0; i < variations.size(); i++){
        // c1 = m * x^degree(g)
        std::vector<bool> c1 = variations[i];
        for (int f = 0; f < degreeVec(g); ++f) c1.insert(c1.begin(), false);
        // c = c1 mod g
        std::vector<bool> c = mod(c1, g);

        // a = c1 + c = m * x^degree(g) + ((m * x^degree(g)) mod g)
        std::vector<bool> a = c1;
        for (size_t f = 0; f < c.size(); ++f) a[f] = c[f];

        map.emplace(std::pair<std::vector<bool>, std::vector<bool>>(variations[i], a));

        c1.clear();
        c.clear();
        a.clear();
    }

    variations.clear();
    return map;
}

#endif //BINARY_SYMMETRIC_CHANNEL_THEORY_H
