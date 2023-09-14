#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int INF = INT_MAX; // Constante para representar infinito
const int RANDOM_WEIGHT_RANGE = 1000; // Rango de pesos aleatorios

struct Edge {
    int dest;
    int weight;
};

typedef vector<vector<Edge>> Graph;

vector<int> AStar(const Graph& graph, int start, int goal);
int heuristic(int node, int goal);
int findMinFScore(const set<int>& openSet, const vector<int>& fScore);
vector<int> reconstructPath(const map<int, int>& cameFrom, int current);

int main() {
    srand(time(0));

    int numNodes = 1000; // Número de nodos en el grafo

    Graph graph(numNodes);

    // Crear un grafo con conexiones ponderadas aleatorias
    for (int i = 0; i < numNodes; ++i) {
        for (int j = i + 1; j < numNodes; ++j) {
            int weight = rand() % RANDOM_WEIGHT_RANGE + 1; // Peso aleatorio entre 1 y 100
            graph[i].push_back({ j, weight });
            graph[j].push_back({ i, weight }); // El grafo es no dirigido
        }
    }

    int start = 0; // Nodo de inicio
    int goal = numNodes - 1; // Nodo objetivo (último nodo)

    // Medir el tiempo de ejecución
    auto startTime = high_resolution_clock::now();

    vector<int> path = AStar(graph, start, goal);

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);

    if (!path.empty()) {
        cout << "Camino más corto desde " << start << " hasta " << goal << ": ";
        for (int node : path) {
            cout << node << " ";
        }
        cout << endl;
    } else {
        cout << "No se encontró un camino desde " << start << " hasta " << goal << endl;
    }

    cout << "Tiempo de ejecución: " << duration.count() << " microsegundos" << endl;

    return 0;
}

vector<int> AStar(const Graph& graph, int start, int goal) {
    int numNodes = graph.size();
    vector<int> gScore(numNodes, INF);
    vector<int> fScore(numNodes, INF);
    set<int> openSet;
    set<int> closedSet;
    map<int, int> cameFrom;

    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);
    openSet.insert(start);

    while (!openSet.empty()) {
        int current = findMinFScore(openSet, fScore);

        if (current == goal) {
            return reconstructPath(cameFrom, current);
        }

        openSet.erase(current);
        closedSet.insert(current);

        for (const Edge& neighbor : graph[current]) {
            if (closedSet.count(neighbor.dest) > 0) {
                continue;
            }

            int tentativeGScore = gScore[current] + neighbor.weight;

            if (tentativeGScore < gScore[neighbor.dest]) {
                cameFrom[neighbor.dest] = current;
                gScore[neighbor.dest] = tentativeGScore;
                fScore[neighbor.dest] = gScore[neighbor.dest] + heuristic(neighbor.dest, goal);
                openSet.insert(neighbor.dest);
            }
        }
    }

    return vector<int>();
}

int heuristic(int node, int goal) {
    return abs(node - goal);
}

int findMinFScore(const set<int>& openSet, const vector<int>& fScore) {
    int minNode = -1;
    int minFScore = INF;

    for (int node : openSet) {
        if (fScore[node] < minFScore) {
            minNode = node;
            minFScore = fScore[node];
        }
    }

    return minNode;
}

vector<int> reconstructPath(const map<int, int>& cameFrom, int current) {
    vector<int> path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    path.push_back(current);
    reverse(path.begin(), path.end());
    return path;
}
