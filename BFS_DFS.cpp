#include<iostream>
#include<vector>
#include <algorithm>
#include <cstdlib>  
#include <ctime>    
#include <queue>
#include <stack>
#include <unordered_map>
using namespace std;


class Node {
public:
	int id;
	bool enable = true;
	vector<int> conexiones;
	Node *R= nullptr, *L = nullptr,*U = nullptr,*D = nullptr;
	
	Node(int a) {
		id = a;
	}

	void add(Node *a, char P) {
		if (P == 'R')
			R = a;
		if (P == 'L')
			L = a;
		if (P == 'U')
			U = a;
		if (P == 'D')
			D = a;
		conexiones.push_back(a->id);
	}

	void show_connections() {
		cout << "R= " << R->id << ";   L= " << L->id << ";   U= " << U->id << ";   D= " << D->id;
	}
};


class gridgraph {
public:
	int n_rows, n_columns;
	vector<vector<Node*>*> grafo;
	


	gridgraph(int n, int m) {
		int cont = 1;
		n_columns = n;
		n_rows = m;

		for (int i=0; i < n_rows; i++) {
			vector<Node*> *FILA = new vector<Node*>;

			for (int j = 0; j < n_columns; j++) {
				Node *a = new Node(cont);
				FILA->push_back(a);
				cont++;
			}
			grafo.push_back(FILA);
		}

		for (int i = 0; i < n_rows; i++) {
			
			for (int j = 0; j < n_columns; j++) {
				if (j > 0) {
					grafo[i]->at(j - 1)->add(grafo[i]->at(j), 'R');
					grafo[i]->at(j)->add(grafo[i]->at(j - 1),'L');
					
				}
				if (i > 0) {
					grafo[i]->at(j)->add(grafo[i - 1]->at(j), ('U'));
					grafo[i-1]->at(j)->add(grafo[i]->at(j), ('D'));
				}
			}
			
		}
	}



	

	Node* at(int a) {
		int cont = 1;
		for (int i = 0; i < n_rows; i++) {
			for (int j = 0; j < n_columns; j++) {
				
				if (a == cont) {
					return grafo[i]->at(j);
				}
				cont++;
			}
		}
	}

	void print() {
		for (int i = 0; i < n_rows; i++) {
			for (int j =0 ; j < n_columns; j++) {
			
				cout << grafo[i]->at(j)->id << "\t-\t";
			}
			cout << endl <<'|' << endl;
		}
	}



	~gridgraph() { 
		for (int i = 0; i < n_rows; i++) {
			for (int j = 0; j < n_columns; j++) {
				delete grafo[i]->at(j); 
			}
			delete grafo[i];
		}
	}

};



bool exist(const vector<int>& nums, int a) {
	for (int num : nums) {
		if (num == a) {
			return true;
		}
	}
	return false;
}



void delete_percentage(int x, int n_rows,int n_columns,gridgraph &grafo) {
	int cant = (n_rows * n_columns) * x / 100;
	vector<int> a_disable;
	int r;


	while (cant != 0) {
		r = (rand() % (n_rows * n_columns)) + 1;
		if (exist(a_disable, r)) {

		}
		else {
			grafo.at(r)->enable = false;
			a_disable.push_back(r);
			cant--;
		}
	}

}


void probar(int a,gridgraph & grafo) {
	Node* actual = grafo.at(a);

	//actual->show_connections();
	cout << endl << endl << actual->conexiones.size();
}



int menor(const std::vector<int>& nums) {
	int minVal = std::numeric_limits<int>::max(); 

	for (int num : nums) {
		if (num != -1 && num < minVal) {
			minVal = num;
		}
	}

	return minVal;
}


int randoms(int n) {


	int randomNumber = (rand() % (n * n)) + 1;
	return randomNumber;
}


vector<Node*> BFS(gridgraph& grafo,int begin, int end) {
	vector<Node*> path;

	Node* ini = grafo.at(begin);
	Node* fin= grafo.at(end);

	if (!ini || !fin) {
		cout << "no existe los nodos" << endl;
		return path; 
	}

	
	queue<Node*> q;

	
	vector<vector<bool>> visitados(grafo.n_rows, vector<bool>(grafo.n_columns, false));

	unordered_map<Node*, Node*> padre;

	
	q.push(ini);
	visitados[ini->id / grafo.n_columns][ini->id % grafo.n_columns] = true;

	while (!q.empty()) {
		Node* actual = q.front();
		q.pop();

		if (actual == fin) {
			
			Node* node = fin;
			while (node) {
				path.insert(path.begin(), node); 
				node = padre[node];
			}
			return path;
		}


		for (char dire : { 'R', 'L', 'U', 'D' }) {
			Node* next = nullptr;
			if (dire == 'R' && actual->R && actual->R->enable) {
				next = actual->R;
			}
			else if (dire == 'L' && actual->L && actual->L->enable) {
				next = actual->L;
			}
			else if (dire == 'U' && actual->U && actual->U->enable) {
				next = actual->U;
			}
			else if (dire == 'D' && actual->D && actual->D->enable) {
				next = actual->D;
			}


			if (next && !visitados[next->id / grafo.n_columns][next->id % grafo.n_columns]) {
				q.push(next);
				visitados[next->id / grafo.n_columns][next->id % grafo.n_columns] = true;
				padre[next] = actual;
			}
		}
	}

	return path;
}



vector<Node*> DFS(gridgraph& grafo,int begin, int end) {

	int coord_x, coord_y;
	Node* ini = grafo.at(begin);
	Node* fin = grafo.at(end);

	vector<vector<bool>> visitados(grafo.n_rows, vector<bool>(grafo.n_columns, false));
	stack<Node*> cola;
	unordered_map<Node*, Node*> padre; 

	cola.push(ini);

	while (!cola.empty()) {
		Node* actual = cola.top();
		cola.pop();
		coord_x = actual->id / grafo.n_columns;
		coord_y = actual->id % grafo.n_columns;

		if (!visitados[coord_x][coord_y] && actual->enable) {
			visitados[coord_x][coord_y] = true;

			
			if (actual == fin) {
//para el camino
				vector<Node*> path;
				Node* node = fin;
				while (node != nullptr) {
					path.push_back(node);
					node =  padre[node];
				}
				reverse(path.begin(), path.end());
				return path;
			}

			if (actual->R != nullptr && !visitados[actual->R->id / grafo.n_columns][actual->R->id % grafo.n_columns] && actual->R->enable) {
				cola.push(actual->R);
				padre[actual->R] = actual; 
			}

			if (actual->L != nullptr && !visitados[actual->L->id / grafo.n_columns][actual->L->id % grafo.n_columns] && actual->L->enable) {
				cola.push(actual->L);
				padre[actual->L] = actual;
			}

			if (actual->U != nullptr && !visitados[actual->U->id / grafo.n_columns][actual->U->id % grafo.n_columns] && actual->U->enable) {
				cola.push(actual->U);
				padre[actual->U] = actual;
			}


			if (actual->D != nullptr && !visitados[actual->D->id / grafo.n_columns][actual->D->id % grafo.n_columns] && actual->D->enable) {
				cola.push(actual->D);
				padre[actual->D] = actual;
			}

		}


	}


	return vector<Node*>(); // RETORNA vector vacio si no hay camino
}



int main() {

	srand(time(nullptr));
	int n=10, begin =randoms(n), end = randoms(n);

	cout << "Inicio: " << begin << "\t Final:" << end << endl;
	
	/*
	cout << "Dimension (nxn)?";
	cin >> n;

	while (begin<0 || begin >(n * n)) {
		cout << endl << "Nodo de inicio?";
		cin >> begin;
	}

	while (end<0 || end >(n * n) || end == begin) {
		cout << endl << "Nodo de fin?";
		cin >> end;
	}
	*/



	gridgraph grafo(n,n);


	/*
	cout << endl << endl << "DFS" << endl;

	vector<Node*> path1 = DFS(grafo, begin, end);

	
	if (!path1.empty()) {
		cout << "Path:" << endl;
		for (Node* node : path1) {
			cout << node->id << " ";
		}
		cout << endl;
	}
	else {
		cout << "No hay path" << endl;
	}
	
	*/

	

	
	cout << endl << endl<<"BFS"<< endl;

	vector<Node*> path2 = BFS(grafo, begin, end);

	if (!path2.empty()) {
		cout << "Path:" << endl;
		for (Node* node : path2) {
			cout << node->id << " ";
		}
		cout << endl;
	}
	else {
		cout << "No hay path" << endl;
	}
	
	
	
}