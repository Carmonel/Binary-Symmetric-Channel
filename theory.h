#ifndef BINARY_SYMMETRIC_CHANNEL_THEORY_H
#define BINARY_SYMMETRIC_CHANNEL_THEORY_H

#include <vector>
#include <map>
#include <cmath>

#include "Utils.h"

double theory(std::map<std::vector<bool>, std::vector<bool>> codeBook, int n, double p){
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

std::map<std::vector<bool>, std::vector<bool>> createCodeBook(const std::vector<bool>& g){
    std::map<std::vector<bool>, std::vector<bool>> map;
    std::vector<bool> m;

    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                m.emplace_back(i == 1);
                m.emplace_back(j == 1);
                m.emplace_back(k == 1);

                // c1 = m * x^degree(g)
                std::vector<bool> c1 = m;
                for (int f = 0; f < degreeVec(g); ++f) c1.insert(c1.begin(), false);
                // c = c1 mod g
                std::vector<bool> c = mod(c1, g);

                // a = c1 + c = m * x^degree(g) + ((m * x^degree(g)) mod g)
                std::vector<bool> a = c1;
                for (size_t f = 0; f < c.size(); ++f) a[f] = c[f];

                map.emplace(std::pair<std::vector<bool>, std::vector<bool>>(m, a));

                m.clear();
                c1.clear();
                c.clear();
                a.clear();
            }
        }
    }

    return map;
}

#endif //BINARY_SYMMETRIC_CHANNEL_THEORY_H
