#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// Función para generar una solución aleatoria inicial
vector<int> randomSolution(int numCities) {
    vector<int> cities(numCities);
    for (int i = 0; i < numCities; i++) {
        cities[i] = i;
    }

    random_shuffle(cities.begin(), cities.end());

    return cities;
}

// Función para calcular la longitud de una ruta
int routeLength(const vector<vector<int>>& tsp, const vector<int>& solution) {
    int length = 0;
    int numCities = tsp.size();
    for (int i = 0; i < numCities; i++) {
        int from = solution[i];
        int to = solution[(i + 1) % numCities];
        length += tsp[from][to];
    }
    return length;
}

// Función para obtener vecinos intercambiando dos ciudades
vector<int> getNeighbour(const vector<int>& solution, int i, int j) {
    vector<int> neighbour = solution;
    swap(neighbour[i], neighbour[j]);
    return neighbour;
}

// Función para encontrar el mejor vecino
pair<vector<int>, int> getBestNeighbour(const vector<vector<int>>& tsp, const vector<int>& solution) {
    int numCities = tsp.size();
    int bestLength = routeLength(tsp, solution);
    vector<int> bestNeighbour = solution;

    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            vector<int> neighbour = getNeighbour(solution, i, j);
            int neighbourLength = routeLength(tsp, neighbour);

            if (neighbourLength < bestLength) {
                bestLength = neighbourLength;
                bestNeighbour = neighbour;
            }
        }
    }

    return make_pair(bestNeighbour, bestLength);
}

// Función principal para el algoritmo de Hill Climbing
pair<vector<int>, int> hillClimbing(const vector<vector<int>>& tsp) {
    int numCities = tsp.size();
    vector<int> currentSolution = randomSolution(numCities);
    int currentLength = routeLength(tsp, currentSolution);

    while (true) {
        pair<vector<int>, int> bestNeighbour = getBestNeighbour(tsp, currentSolution);

        if (bestNeighbour.second < currentLength) {
            currentSolution = bestNeighbour.first;
            currentLength = bestNeighbour.second;
        }
        else {
            break;
        }
    }

    return make_pair(currentSolution, currentLength);
}

int main() {
    srand(time(0));

    int numCities = 100; // Cambiar a 100 para 100 ciudades

    vector<vector<int>> tsp(numCities, vector<int>(numCities));

    // Generar distancias aleatorias (ajustar según sea necesario)
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            if (i != j) {
                tsp[i][j] = rand() % 100 + 1; // Distancias aleatorias entre 1 y 100
            }
            else {
                tsp[i][j] = 0;
            }
        }
    }

    pair<vector<int>, int> result = hillClimbing(tsp);

    cout << "Mejor ruta encontrada: ";
    for (int city : result.first) {
        cout << city << " ";
    }
    cout << endl;
    cout << "Longitud de la ruta: " << result.second << endl;

    return 0;
}
