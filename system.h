#ifndef BINARY_SYMMETRIC_CHANNEL_SYSTEM_H
#define BINARY_SYMMETRIC_CHANNEL_SYSTEM_H

#include <vector>
#include <iostream>

#include "Utils.h"

std::vector<bool> coder(const std::vector<bool>& m, const std::vector<bool>& g, bool output){
    // c1 = m * x^degree(g)
    std::vector<bool> c1 = m;
    for (int i = 0; i < degreeVec(g); ++i) c1.insert(c1.begin(), false);
    // c = c1 mod g
    std::vector<bool> c = mod(c1, g);

    // a = c1 + c = m * x^degree(g) + ((m * x^degree(g)) mod g)
    std::vector<bool> a = c1;
    for (size_t i = 0; i < c.size(); ++i)
        a[i] = c[i];
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

std::vector<bool> secondChannel(const std::vector<bool>& a, const std::vector<bool>& e, bool output){
    // b = a ^ e (применение ошибок в канале)
    if (output) std::cout << "e = " << e << std::endl;
    std::vector<bool> b = a;
    for (size_t i = 0; i < b.size(); ++i) if (b[i]) b[i] = b[i] ^ e[i];
    if (output) std::cout << "b = " << b << std::endl;

    return b;
}

bool decoder(const std::vector<bool>& b, const std::vector<bool>& g, bool output){
    // s = b mod g (если s != {0}, то произошли ошибки в канале)
    std::vector<bool> s = mod(b, g);
    if (output) std::cout << "s = " << s << std::endl;
    return s.empty();
}

bool secondDecoder(const std::vector<bool>& b, const std::vector<bool>& g, bool output){
    // s = b mod g (если s != {0}, то произошли ошибки в канале)
    std::vector<bool> s = mod(b, g);
    if (output) std::cout << "s = " << s << std::endl;
    return s.empty();
}

#endif //BINARY_SYMMETRIC_CHANNEL_SYSTEM_H
