#include <iostream>
#include <vector>
#include <climits>
#include <chrono>

const long INF = LONG_MAX;

struct Edge{
    int start, end;
    long long weight;
    Edge(int start, int end, long long weight);
};

Edge::Edge(int start, int end, long long weight) {
    this->start = start, this->end = end, this->weight = weight;
}

struct Graph{
    std::vector<Edge> edges;
    int vertices;
    Graph(int N);
};

Graph::Graph(int N) {
    this->vertices = N + 1;
}


std::pair<bool,long long*> BellmanFord(Graph* graph) {
    auto* dist = new long long[graph->vertices];
    dist[graph->vertices - 1] = 0;
    for(int i = 0; i < graph->vertices - 1; ++i) {
        dist[i] = INF;
    }
    for(int i = 0; i < graph->vertices - 1; ++i) {
        for (auto & edge : graph->edges) {
            if (dist[edge.start] < INF) {
                dist[edge.end] = std::min(dist[edge.start] + edge.weight, dist[edge.end]);
            }
        }
    }
    for (auto & edge : graph->edges) {
        if (dist[edge.start] < INF) {
            if (dist[edge.start] + edge.weight < dist[edge.end]) {
                return std::pair(false, nullptr);
            }
        }
    }
    for (auto & edge : graph->edges) {
        edge.weight = edge.weight + dist[edge.start] - dist[edge.end];
    }
    return std::pair(true, dist);
}

void Dijkstra(Graph* graph, long long* dist1) {
    for(int vertice = 0; vertice < graph->vertices - 1; ++vertice) {
        long long dist[graph->vertices - 1];
        dist[vertice] = 0;
        for (int i = 0; i < graph->vertices - 1; ++i) {
            if (i != vertice) {
                dist[i] = INF;
            }
        }
        bool visited[graph->vertices - 1];
        for (int i = 0; i < graph->vertices - 1; ++i) {
            visited[i] = false;
        }
        for (int i = 0; i < graph->vertices - 1; ++i) {
            int start = -1;
            for (int j = 0; j < graph->vertices - 1; ++j) {
                if ((start == -1 || dist[j] < dist[start]) && !visited[j]) {
                    start = j;
                }
            }
            if(dist[start] == INF)
                break;
            visited[start] = true;
            for (auto &edge: graph->edges) {
                if (edge.start == start) {
                    dist[edge.end] = std::min(dist[edge.end],
                                              dist[edge.start] + edge.weight);
                }
            }
        }
        for(int i = 0; i < graph->vertices - 1; ++i) {
            if(dist[i] == INF) {
                std::cout << "inf" << ' ';
            }
            else {
                std::cout << dist[i] - dist1[vertice] + dist1[i]<< ' ';
            }
        }
        std::cout << '\n';
    }
}

int main() {
    int N, M;
    std::cin >> N >> M;
    auto* graph = new Graph(N);
    int vertice1, vertice2, weigth;
    for(int i = 0; i < M; ++i) {
        std::cin >> vertice1 >> vertice2 >> weigth;
        graph->edges.emplace_back(vertice1 - 1, vertice2 - 1, weigth);
    }
    for(int i = 0; i < N; ++i) {
        graph->edges.emplace_back(N, i, 0);
    }
    auto begin = std::chrono::steady_clock::now();
    std::pair<bool, long long*> bf = BellmanFord(graph);
    if(!bf.first) {
        std::cout << "Negative cycle";
    }
    else {
       Dijkstra(graph, bf.second);
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << (double)elapsed_ms.count() / 1000.0 << "s";
    return 0;
}
