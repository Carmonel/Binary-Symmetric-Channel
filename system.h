#ifndef BINARY_SYMMETRIC_CHANNEL_SYSTEM_H
#define BINARY_SYMMETRIC_CHANNEL_SYSTEM_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "Utils.h"

std::vector<bool> coder(const std::vector<bool>& m, const std::vector<bool>& g, bool output){
    // c1 = m * x^degree(g)
    std::vector<bool> c1 = m;
    for (int i = 0; i < degreeVec(g); ++i) c1.insert(c1.begin(), false);
    if (output) std::cout << "m * x^degree(g) = " << c1 << std::endl;
    // c = c1 mod g
    std::vector<bool> c = mod(c1, g);
    if (output) std::cout << "m * x^degree(g) mod g = " << c << std::endl;

    // a = c1 + c = m * x^degree(g) + ((m * x^degree(g)) mod g)
    std::vector<bool> a = c1;
    for (size_t i = 0; i < c.size(); ++i) a[i] = c[i];
    if (output) std::cout << "a = " << a << std::endl;

    c.clear();
    c1.clear();

    return a;
}

std::vector<bool> channel(const std::vector<bool>& a, const std::vector<bool>& e, bool output){
    // b = a ^ e (применение ошибок в канале)
    if (output) std::cout << "e = " << e << std::endl;
    std::vector<bool> b = a;
    for (size_t i = 0; i < b.size(); ++i) b[i] = b[i] ^ e[i];
    if (output) std::cout << "b = " << b << std::endl;

    return b;
}

std::vector<bool> decoder(const std::vector<bool>& b, const std::vector<bool>& g, bool output){
    // s = b mod g (если s != {0}, то произошли ошибки в канале)
    std::vector<bool> s = mod(b, g);
    if (output) std::cout << "s = " << s << std::endl;
    return s;
}

std::vector<bool> secondChannel(const std::vector<bool>& a, const std::vector<bool>& e, bool output){
    // b = a ^ e (применение ошибок в канале)
    if (output) std::cout << "e = " << e << std::endl;
    std::vector<bool> b = a;
    for (size_t i = 0; i < b.size(); ++i) if (b[i]) b[i] = b[i] ^ e[i];
    if (output) std::cout << "b = " << b << std::endl;

    return b;
}

bool secondDecoder(const std::vector<bool>& b, const std::vector<bool>& g, bool output){
    // s = b mod g (если s != {0}, то произошли ошибки в канале)
    std::vector<bool> m;
    for (int i = b.size() - 1; i >= b.size() - 3; i--) m.push_back(b[i]);
    std::reverse(m.begin(), m.end());
    if (output) std::cout << "m_new = " << m << std::endl;
    std::vector<bool> a = coder(m, g, false); // снова считаем a
    if (output) std::cout << "a_new = " << a << std::endl;
    for (int i = 3; i >= 0; i++){
        if (a[i] != b[i]) return false;
    }
    return true;
}

void checkStatement3(const std::map<std::vector<bool>, std::vector<bool>>& codeBook, const std::vector<bool>& g){
    // Store errors count and errors finded
    std::vector<std::pair<int, int>> eCount(codeBook.begin()->second.size()+1, std::pair<int, int>(0, 0));
    std::vector<bool> etmp(codeBook.begin()->second.size());
    std::vector<std::vector<bool>> eVar;
    generateVariations(etmp, 0, eVar);

    for (const auto& a: codeBook){
        for (const auto& e: eVar){
            auto b = channel(a.second, e, false);
            int wcount = w(decoder(b, g, false));
            int ecount = w(e);

            // s == 0 -> ошибок не было
            if (wcount == 0){
                eCount[ecount].first++;
            }
            // s != 0 -> ошибки обнаружены
            if (wcount != 0){
                eCount[ecount].first++;
                eCount[ecount].second++;
            }
            b.clear();
        }
    }
    etmp.clear();
    eVar.clear();
    for (int i = 0; i < eCount.size(); i++){
        std::cout << "Errors: " << i << "; Count: " << eCount[i].first << "; Found errors: " << eCount[i].second << std::endl;
    }
    std::cout << "checkStatement3() finished." << std::endl;
    eCount.clear();
}

#endif //BINARY_SYMMETRIC_CHANNEL_SYSTEM_H
