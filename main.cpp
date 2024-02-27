#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <fstream>

#include "system.h"
#include "theory.h"

int main() {
    std::string outputPath = "E:\\2\\";
    std::vector<bool> g = {1, 1, 1, 0, 1}; // порождающий многочлен
    std::cout << "g = " << g << std::endl;

    // Генератор для m - генератор случайного сообщения
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<int> dis1(0, 1);

    // Генератор для e - генератор случайного вектора ошибок
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    /// Проверка работоспособности
    std::vector<bool> mtest, etest;
    for (int i = 0; i < 3; i++) mtest.push_back(dis1(gen1) == 1);
    std::cout << "m = " << mtest << std::endl;
    std::vector<bool> atest = coder(mtest, g, true);
    for (int i = 0; i < 7; i++) etest.push_back(dis(gen) < 0.3);
    std::vector<bool> btest = channel(atest, etest, true);
    std::cout << (decoder(btest, g, true) ? "Haven't errors" : "There is errors!") << std::endl;
    std::cout << std::endl;

    mtest.clear();
    atest.clear();
    etest.clear();
    btest.clear();

    /// Альтернативный декодер
    // Формирование случайного сообщения
    std::vector<bool> mtest1;
    for (int i = 0; i < 3; i++) mtest1.push_back(dis1(gen1) == 1);
    std::cout << "m = " << mtest1 << std::endl;
    std::vector<bool> atest1 = coder(mtest1, g, true);

    // Формирование случайного вектора ошибок
    std::vector<bool> etest1;
    for (int i = 0; i < 7; i++) etest1.push_back(dis(gen) < 0.3);

    // Применение ошибки в канале
    std::vector<bool> btest1 = channel(atest1, etest1, true);

    std::cout << (secondDecoder(btest1, g, true) ? "Haven't errors" : "There is errors!") << std::endl;

    mtest1.clear();
    atest1.clear();
    etest1.clear();
    btest1.clear();

    /// График зависимости оценки ошибки от вер-ти ошибки
    int N = 10000; // количество итераций
    std::map<std::vector<bool>, std::vector<bool>> codeBook = createCodeBook(g);
    std::ofstream file(outputPath + "Pe_p.txt");
    if (!file.is_open()){
        std::cerr << "Unable open file!";
        exit(-1);
    }
    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        for (int j = 0; j < N; j++){
            // Формирование случайного сообщения
            std::vector<bool> m;
            for (int i = 0; i < 3; i++) m.push_back(dis1(gen1) == 1);
            std::vector<bool> a = coder(m, g, false);

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение ошибки в канале
            std::vector<bool> b = channel(a, e, false);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (decoder(b, g, false))) Ne++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
        }
        file << p << "=" << (double)(Ne)/(double)(N) << "=" << theory(codeBook, 7, p) << std::endl;
    }
    file.close();
    std::cout << outputPath + "Pe_p.txt created" << std::endl;

    /// Фиксированные значения
    // m = 000
    file = std::ofstream(outputPath + "Fixed_0.txt");
    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        for (int j = 0; j < N; j++){
            // Формирование случайного сообщения
            std::vector<bool> m = {0, 0, 0};
            std::vector<bool> a = coder(m, g, false);

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение ошибки в канале
            std::vector<bool> b = channel(a, e, false);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (decoder(b, g, false))) Ne++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
        }
        file << p << "=" << (double)(Ne)/(double)(N) << std::endl;
    }
    file.close();
    std::cout << outputPath + "Fixed_0.txt created" << std::endl;

    // m = 001
    file = std::ofstream(outputPath + "Fixed_1.txt");
    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        for (int j = 0; j < N; j++){
            // Формирование случайного сообщения
            std::vector<bool> m = {0, 0, 1};
            std::vector<bool> a = coder(m, g, false);

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение ошибки в канале
            std::vector<bool> b = channel(a, e, false);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (decoder(b, g, false))) Ne++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
        }
        file << p << "=" << (double)(Ne)/(double)(N) << std::endl;
    }
    file.close();
    std::cout << outputPath + "Fixed_1.txt created" << std::endl;

    // m = 010
    file = std::ofstream(outputPath + "Fixed_2.txt");
    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        for (int j = 0; j < N; j++){
            // Формирование случайного сообщения
            std::vector<bool> m = {0, 1, 0};
            std::vector<bool> a = coder(m, g, false);

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение ошибки в канале
            std::vector<bool> b = channel(a, e, false);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (decoder(b, g, false))) Ne++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
        }
        file << p << "=" << (double)(Ne)/(double)(N) << std::endl;
    }
    file.close();
    std::cout << outputPath + "Fixed_2.txt created" << std::endl;

    // m = 100
    file = std::ofstream(outputPath + "Fixed_3.txt");
    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        for (int j = 0; j < N; j++){
            // Формирование случайного сообщения
            std::vector<bool> m = {1, 0, 0};
            std::vector<bool> a = coder(m, g, false);

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение ошибки в канале
            std::vector<bool> b = channel(a, e, false);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (decoder(b, g, false))) Ne++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
        }
        file << p << "=" << (double)(Ne)/(double)(N) << std::endl;
    }
    file.close();
    std::cout << outputPath + "Fixed_3.txt created" << std::endl;

    /// Альтернативный канал
    file = std::ofstream(outputPath + "secondChannel.txt");
    if (!file.is_open()){
        std::cerr << "Unable open file!";
        exit(-1);
    }
    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        int Ne_s = 0; // количество ошибок во втором варианте
        for (int j = 0; j < N; j++){
            // Формирование случайного сообщения
            std::vector<bool> m;
            for (int i = 0; i < 3; i++) m.push_back(dis1(gen1) == 1);
            std::vector<bool> a = coder(m, g, false);

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение ошибки в канале
            std::vector<bool> b = channel(a, e, false);
            std::vector<bool> b_s = secondChannel(a, e, false);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (decoder(b, g, false))) Ne++;
            if ((degreeVec(e) != -1) & (decoder(b_s, g, false))) Ne_s++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
            b_s.clear();
        }
        file << p << "=" << (double)(Ne)/(double)(N) << "=" << (double)(Ne_s)/(double)(N) << std::endl;
    }
    file.close();
    std::cout << outputPath + "secondChannel.txt created" << std::endl;

    /// Альтернативный декодер (исследование)
    file = std::ofstream(outputPath + "secondDecoder.txt");
    if (!file.is_open()){
        std::cerr << "Unable open file!";
        exit(-1);
    }
    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        int Ne_s = 0; // количество ошибок во втором варианте
        for (int j = 0; j < N; j++){
            // Формирование случайного сообщения
            std::vector<bool> m;
            for (int i = 0; i < 3; i++) m.push_back(dis1(gen1) == 1);
            std::vector<bool> a = coder(m, g, false);

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение ошибки в канале
            std::vector<bool> b = channel(a, e, false);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (decoder(b, g, false))) Ne++;
            if ((degreeVec(e) != -1) & (secondDecoder(b, g, false))) Ne_s++;

            m.clear();
            a.clear();
            e.clear();
            b.clear();
        }
        file << p << "=" << (double)(Ne)/(double)(N) << "=" << (double)(Ne_s)/(double)(N) << std::endl;
    }
    file.close();
    std::cout << outputPath + "secondDecoder.txt created" << std::endl;

    /// Теоретические оценки ошибок
    file = std::ofstream(outputPath + "theory.txt");
    for (double p = 0.0; p <= 1.0; p += 0.01){
        file << p << "=" << theory(codeBook, 7, p) << "=" << theoryUp(7, p) << std::endl;
    }
    file.close();
    std::cout << outputPath + "theory.txt created" << std::endl;

    codeBook.clear();
}
