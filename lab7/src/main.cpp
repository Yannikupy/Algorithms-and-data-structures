#include <iostream>
#include <vector>
#include <stack>
#include <chrono>

using namespace std;

struct Node{
    int x;
    int Height;
    Node(int x, int Height) : x(x), Height(Height) {};
};

int findMaxArea(const int array[], int n) {
    stack<Node> stack;
    int res = 0;
    stack.push(Node(0,-1));
    int h;
    int hPrev;
    int area;
    for(int i = 1; i <= n + 1; ++i) {
        if(i <= n) {
            h = array[i - 1];
        }
        else {
            h = 0;
        }
        int x = i;
        while(h <= stack.top().Height) {
            x = stack.top().x;
            hPrev = stack.top().Height;
            stack.pop();
            area = 1LL * hPrev * (i - x);
            if(area > res) {
                res = area;
            }
        }
        stack.push(Node(x, h));
    }
    return res;
}

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;
    auto begin = std::chrono::steady_clock::now();
    int matrix[n][m];
    std::fill(matrix[0], matrix[0] + m * n, 0);
    std::string inputLine;
    for(int i = 0; i < n; ++i) {
        std::cin >> inputLine;
        for(int j = 0; j < m; ++j) {
            matrix[i][j] = inputLine[j] - 48;
        }
    }
    int array[m];
    int area;
    int newArea;
    for (int j = 0; j < m; ++j) {
        if (matrix[0][j] == 0) {
            array[j] = 1;
        } else {
            array[j] = 0;
        }
    }
    area = findMaxArea(array, m);
    for(int i = 1; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if(matrix[i][j] == 0) {
                ++array[j];
            }
            else {
                array[j] = 0;
            }
        }
        newArea = findMaxArea(array, m);
        if(area < newArea) {
            area = newArea;
        }
    }
    std::cout << area << '\n';
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << (double)elapsed_ms.count() / 1000.0 << "s";
    return 0;
}
