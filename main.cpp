#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <fstream>

#include "system.h"
#include "theory.h"

int main() {
    int l = 3; // длина сообщения
    std::vector<bool> g = {1, 1, 1, 0, 1}; // x^4 + x^2 + x + 1
    double p = 0.3; // вероятность ошибки на бит
    bool graphs = true;

    /// Проверка на работоспособность стандратного декодера
    std::cout << "Standard decoder:" << std::endl;
    std::vector<bool> m = {0, 1, 1};
    std::cout << "m = " << m << std::endl;
    std::vector<bool> a = coder(m, g, true);
    std::vector<bool> e = genErrors(a.size(), true);
    std::vector<bool> b = channel(a, e, true);
    std::cout << (decoder(b, g, true)? "Haven't errors" : "There is errors!") << std::endl;

    m.clear();
    a.clear();
    e.clear();
    b.clear();

    if (!graphs) return 0;

    /// График теоретической ошибки
    std::map<double, double> theoryGraph;
    auto codeBook = createCodeBook(g, l);
    double delta = 0.01;
    for (p = 0.0; p <= 1.001; p += delta){
        double theoryValue = theory(codeBook, 4 + 3, p);
        theoryGraph.emplace(std::pair<double, double>(p, theoryValue));
    }


    /// График экспериментальной ошибки
    std::vector<std::vector<double>> expGraph;
    double eps = 0.01;
    int maxIterations = 5000;
    auto it = theoryGraph.begin();
    for (p = 0.0; p <= 1.001; p += delta){
        double Nerr = 0; // количество ошибок
        int iterations = 0; // количество итераций
        double currentTheoryValue = it->second; // теоретическая ошибка
        double currentExpValue = 0; // эксперементальная ошибка
        double difference = currentTheoryValue - currentExpValue; // разница теор. и эксп.
        if (difference < 0) difference *= -1;

        while (iterations < maxIterations){
            iterations++;

            m = genRandomMessage(l);
            a = coder(m, g, false);
            e = genRandomErrors(a.size(), p);
            b = channel(a, e, false);
            bool check = decoder(b, g, false);
            int wcount = w(e);
            if (check && (wcount != 0)) Nerr++;

            currentExpValue = (double)(Nerr)/(double)(iterations); // вычисление новой теор. ошибки
            difference = currentTheoryValue - currentExpValue; // вычисление новой разницы теор. и эксп.
            if (difference < 0) difference *= -1;
            if ((difference < eps)) break;
        }
        expGraph.emplace_back(std::vector<double>({p, currentExpValue, (double)iterations / (double)maxIterations}));
        it++;
    }

    /// Вывод графика
    std::ofstream file("C:\\2\\Graph.txt");
    if (!file.is_open()){
        std::cerr << "File hasn't opened!" << std::endl;
        exit(-1);
    }
    auto theoryIt = theoryGraph.begin();
    int expGraphI = 0;
    while (theoryIt != theoryGraph.end()){
        file << theoryIt->first << "=" << theoryIt->second << "=" << expGraph[expGraphI][1] << "=" << expGraph[expGraphI][2] << std::endl;
        theoryIt++;
        expGraphI++;
    }
    file.close();
    std::cout << "C:\\2\\Graph.txt created" << std::endl;


    /// График для альтернативного канала
    std::vector<std::vector<double>> altChannelGraph;
    for (p = 0.0; p <= 1.001; p += delta){
        double Nerr = 0; // количество ошибок

        for (int i = 0; i < 5000; i++){
            m = genRandomMessage(l);
            a = coder(m, g, false);
            e = genRandomErrors(a.size(), p);
            b = secondChannel(a, e, false);
            bool check = decoder(b, g, false);
            int wcount = w(e);
            if (check && (wcount != 0)) Nerr++;
        }
        altChannelGraph.push_back(std::vector<double>({p, Nerr/5000, (double)it->second / (double)maxIterations}));
        it++;
    }

    /// Вывод графика для альтернативного канала
    file = std::ofstream("C:\\2\\altGraph.txt");
    if (!file.is_open()){
        std::cerr << "File hasn't opened!" << std::endl;
        exit(-1);
    }
    theoryIt = theoryGraph.begin();
    expGraphI = 0;
    while (theoryIt != theoryGraph.end()){
        file << theoryIt->first << "=" << theoryIt->second << "=" << altChannelGraph[expGraphI][1] << "=" << altChannelGraph[expGraphI][2] << std::endl;
        theoryIt++;
        expGraphI++;
    }
    file.close();
    std::cout << "C:\\2\\altGraph.txt created" << std::endl;

    theoryGraph.clear();
    expGraph.clear();
    altChannelGraph.clear();
}
