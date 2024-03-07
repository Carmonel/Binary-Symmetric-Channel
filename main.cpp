#include <iostream>
#include <vector>
#include <fstream>
#include <thread>

#include "system.h"
#include "theory.h"

void task(int arg){
    /*std::vector<bool> g = {1, 1, 1, 0, 1};
    int l = arg;
    auto codeBook = createCodeBook(g, l);


    std::vector<bool> mtmp(arg);
    std::vector<std::vector<bool>> mVar;
    generateVariations(mtmp, 0, mVar);

    for (auto m: codeBook){
        auto a = m.second;
        std::vector<bool> etmp(a.size());
        std::vector<std::vector<bool>> eVar;
        generateVariations(etmp, 0, eVar);
        for (auto e: eVar){
            if (w(e) >= d) continue;
            auto b = channel(a, e, false);
            int scount = w(decoder(b, g, false));
            int ecount = w(e);
            //std::cout << scount << " <-> " << ecount << std::endl;
            if ((scount == 0) && (ecount != 0)){
                std::cout << "m = " << m.first << std::endl;
                std::cout << "a = " << a << std::endl;
                std::cout << "e = " << e << std::endl;
                std::cout << "b = " << b << std::endl;

                b.clear();
                goto exit;
            }
            b.clear();
        }
        etmp.clear();
        eVar.clear();
        a.clear();
    }
    exit:

    codeBook.clear();
    mtmp.clear();
    mVar.clear();*/
}

int main() {
    int l = 3; // длина сообщения
    std::vector<bool> g = {1, 1, 1, 0, 1}; // x^4 + x^2 + x + 1

    /// График теоретической ошибки
    std::vector<double> theoryGraph;
    auto codeBook = createCodeBook(g, l);
    double delta = 0.01;
    for (double p = 0.0; p <= 1.001; p += delta){
        double theoryValue = theory(codeBook, l + degreeVec(g), p);
        theoryGraph.emplace_back(theoryValue);
    }

    /// Проверка функции mod
    std::cout << "Mod function checker:" << std::endl;
    modChecker();

    /// Вывод кодовой книги
    std::cout << "Code book:" << std::endl;
    for (auto i: codeBook){
        std::cout << i.second << " = " << w(i.second) << std::endl;
    }

    /// Вывод графика
    std::ofstream file("C:\\2\\Graph.txt");
    if (!file.is_open()){
        std::cerr << "File hasn't opened!" << std::endl;
        exit(-1);
    }

    /// Проверка утв. 2
    checkStatement2(codeBook);

    /// Мин. раст. кода
    int d = 10000;
    for (auto c: codeBook){
        for (auto b: codeBook){
            if (c == b) continue;
            int dtmp = 0;
            for (int h = 0; h < c.second.size(); h++){
                if (c.second[h] != b.second[h]) dtmp++;
            }
            if (dtmp < d) d = dtmp;
        }
    }
    std::cout << "d = " << d << " - min. code length" << std::endl;

    /// Проверка утв. 3
    checkStatement3(codeBook, g); // May throw exception, use debugger

    /// График экспериментальной ошибки
    double eps = 0.1;
    int it = 0;
    for (double p = 0.0; p <= 1.001; p += delta){
        if ((p > 0.9) & (p < 1.0)){
            std::cout << "1";
        }
        int Nerr = 0;
        double iterations = 9;
        iterations *= (theoryGraph[it] - theoryGraph[it] * theoryGraph[it]);
        iterations /= eps * eps;
        for (int i = 0; i < iterations; i++){
            auto m = genRandomMessage(l);
            auto a = coder(m, g, false);
            auto e = genRandomErrors(a.size(), p);
            auto b = channel(a, e, false);
            int scount = w(decoder(b, g, false));
            int ecount = w(e);
            if ((scount != 0) && (ecount != 0)) Nerr++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
        }
        file << p << "=" << theoryGraph[it] << "=" << Nerr/iterations << "=" << iterations << std::endl;
        it++;
    }
    file.close();
    std::cout << "C:\\2\\Graph.txt created" << std::endl;

    theoryGraph.clear();
}
