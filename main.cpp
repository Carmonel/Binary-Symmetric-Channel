#include <iostream>
#include <vector>
#include <random>
#include <map>

std::ostream& operator<<(std::ostream& os, const std::vector<bool>& a) {
    for (int i = a.size() - 1; i >= 0; --i) {
        os << (a[i] ? '1' : '0');
    }
    return os;
}

int degreeVec(const std::vector<bool>& a) {
    if (a.empty()) return -1; // Обработка пустого вектора
    for (size_t i = a.size() - 1; i != static_cast<size_t>(-1); --i) {
        if (a[i])
            return static_cast<int>(i);
    }
    return -1;
}

void clearZeros(std::vector<bool>& a) {
    while (!a.empty() && !a.back())
        a.pop_back();
}

std::vector<bool> mod(const std::vector<bool>& a, const std::vector<bool>& b) {
    std::vector<bool> remainder = a;

    while (!remainder.empty() && degreeVec(remainder) >= degreeVec(b)) {
        int shift = degreeVec(remainder) - degreeVec(b);

        for (size_t i = 0; i < b.size(); ++i) {
            if (i + shift < remainder.size())
                remainder[i + shift] = remainder[i + shift] ^ b[i];
        }
    }
    clearZeros(remainder);

    return remainder;
}

bool func(const std::vector<bool>& m, const std::vector<bool>& g, const std::vector<bool>& e) {
    // c1 = m * x^degree(g)
    std::vector<bool> c1 = m;
    for (int i = 0; i < degreeVec(g); ++i) c1.insert(c1.begin(), false);
    // c = c1 mod g
    std::vector<bool> c = mod(c1, g);

    // a = c1 + c = m * x^degree(g) + ((m * x^degree(g)) mod g)
    std::vector<bool> a = c1;
    for (size_t i = 0; i < c.size(); ++i)
        a[i] = c[i];
    //std::cout << "a = " << a << std::endl;

    // b = a ^ e (применение ошибок в канале)
    std::vector<bool> b = a;
    for (size_t i = 0; i < b.size(); ++i)
        b[i] = b[i] ^ e[i];
    //std::cout << "b = " << b << std::endl;

    // s = b mod g (если s != {0}, то произошли ошибки в канале)
    std::vector<bool> s = mod(b, g);
    //std::cout << "s = " << s << std::endl;

    c1.clear();
    c.clear();
    a.clear();
    b.clear();

    return s.empty();
}

int w(const std::vector<bool>& a){
    int count = 0;
    for (int i = 0; i < a.size(); i++) if (a[i]) count++;
    return count;
}

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

int main() {
    std::vector<bool> g = {1, 1, 1, 0, 1}; // порождающий многочлен
    //std::cout << "g = " << g << std::endl;

    // Генератор для m - генератор случайного сообщения
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<int> dis1(0, 1);

    // Генератор для e - генератор случайного вектора ошибок
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    int N = 10000; // количество итераций
    std::map<std::vector<bool>, std::vector<bool>> codeBook = createCodeBook(g);

    for (double p = 0.0; p <= 1.0; p += 0.01){
        int Ne = 0; // количество ошибок
        for (int j = 0; j < N; j++){
            //if (j % 1000 == 0) std::cout << j / 100 << "%" << std::endl;
            // Формирование случайного сообщения
            std::vector<bool> m;
            for (int i = 0; i < 3; i++){
                m.push_back(dis1(gen1) == 1);
            }

            // Формирование случайного вектора ошибок
            std::vector<bool> e;
            for (int i = 0; i < 7; i++) e.push_back(dis(gen) < p);

            // Применение кодирования и применение ошибки в канале
            bool check = func(m, g, e);
            // Если вектор ошибок не нулевой и алгоритм не увидел ошибку
            if ((degreeVec(e) != -1) & (check)) Ne++;

            m.clear();
            e.clear();
        }
        std::cout << p << "=" << (double)(Ne)/(double)(N) << "=" << theory(codeBook, 7, p) << std::endl;
    }
}
