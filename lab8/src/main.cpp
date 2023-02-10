#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "cmath"
#include <chrono>
double area(int a, int b, int c) {
    double p = double (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

int main() {
    int N;
    std::cin >> N;

    if(N < 3) {
        std::cout << 0;
        return 0;
    }

    std::vector<int> lengths;
    for(int i = 0; i < N; ++i) {
        int temp = 0;
        std::cin >> temp;
        lengths.push_back(temp);
    }
    auto begin = std::chrono::steady_clock::now();
    std::sort(lengths.begin(), lengths.end());

    double maxTriangleArea = 0;
    int A = 0, B = 0, C = 0;

    for(int i = 0; i < lengths.size() - 2; ++i) {
        for(int j = i + 1; j < lengths.size() - 1; ++j) {
            int a = lengths[i];
            int b = lengths[j];
            int c = lengths[j + 1];
            if (a < b + c && b < a + c && c < a + b) {
                if (area(a, b, c) > maxTriangleArea) {
                    maxTriangleArea = area(a, b, c);
                    A = a;
                    B = b;
                    C = c;
                }
            }
        }
    }
    std::vector<int> sortedLengths;
    sortedLengths.push_back(A);
    sortedLengths.push_back(B);
    sortedLengths.push_back(C);
    std::sort(sortedLengths.begin(), sortedLengths.end());
    if(A != 0){
        std::cout << std::fixed << std::setprecision(3) << maxTriangleArea << std::endl;
        std::cout << sortedLengths[0] << " " << sortedLengths[1] << " " << sortedLengths[2] << std::endl;
    }
    else {
        std::cout << 0;
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << (double)elapsed_ms.count() / 1000.0 << "s";
    return 0;
}

