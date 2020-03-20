#include <stdio.h> 
#include <stdbool.h> 
#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include <fstream>

#define N 9

using namespace std;

bool manhattan_heuristic = false;
int goal[N] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

typedef struct Node {
	int* state;
	int g;
	int h;
	Node* parent;
}Node;

void printState(int state[N]) {
	int row = sqrt(N);
	for (int i = 0; i < N; i++) {
		if (i%row == 0 && i > 0) {
			cout << endl;
		}
		cout << state[i] << " ";
	}
	cout << "\n";
}

int getH(int state[N]) {
	int h = 0;
	for (int i = 0; i < N; i++) {
		if (state[i] != goal[i] && state[i] != 0) {
			h++;
		}
	}
	return h;
}

int getH_manhattan(int state[N]) {
	int h = 0;
	int rows = sqrt(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (state[i] == goal[j] && state[i] != 0) {
				h += (abs(i / rows - j / rows) + abs(i%rows - j % rows));
			}
		}
	}
	return h;
}

Node* getLowestF(vector<Node*> &openlist) {
	Node* min = openlist[0];
	int fmin = min->g + min->h;
	int index = 0;
	for (int i = 1; i < openlist.size(); i++) {
		Node* node = openlist[i];
		int f = node->h + node->g;
		if (f < fmin) {
			fmin = f;
			min = node;
			index = i;
		}
	}
	openlist.erase(openlist.begin() + index);
	return min;
}

bool isSolution(int state[N]) {
	for (int i = 0; i < N; i++) {
		if (state[i] != goal[i]) {
			return false;
		}
	}
	return true;
}

bool stateEquals(int state1[N], int state2[N]) {
	for (int i = 0; i < N; i++) {
		if (state1[i] != state2[i]) {
			return false;
		}
	}
	return true;
}

int getEmptyTileIndex(int state[N]) {
	for (int i = 0; i < N; i++) {
		if (state[i] == 0) {
			return i;
		}
	}
}

Node* createSuccessor(Node* parent, int i, int j) {
	Node* node = (Node*)calloc(1, sizeof(Node));
	node->state = new int[N];
	for (int i = 0; i < N; i++) {
		node->state[i] = parent->state[i];
	}
	node->state[i] = parent->state[j];
	node->state[j] = parent->state[i];
	
	node->g = parent->g + 1;
	if (manhattan_heuristic) {
		node->h = getH_manhattan(node->state);
	}
	else {
		node->h = getH(node->state);
	}
	node->parent = parent;
	return node;
}

vector<Node*> getSuccessors(Node* parent) {
	int i = getEmptyTileIndex(parent->state);
	vector<Node*> successors;
	int rows = sqrt(N);
	if (i%rows != 0) { // nem a bal szelen van, tud balra lepni
		Node* node = createSuccessor(parent, i, i - 1);
		successors.push_back(node);
	}
	if ((i+1)%rows != 0) { //nem a jobb szelen van, tud jobbra lepni
		Node* node = createSuccessor(parent, i, i + 1);
		successors.push_back(node);
	}
	if (i < (rows-1)*rows) { //nem a legalso sor, tud lefele lepni
		Node* node = createSuccessor(parent, i, i + rows);
		successors.push_back(node);
	}
	if (i >= rows) { //nem a legfelso sor, tud felfele lepni
		Node* node = createSuccessor(parent, i, i - rows);
		successors.push_back(node);
	}
	return successors;
}

Node* getNodeIfExists(vector<Node*> &list, int state[N]) {
	for (int i = 0; i < list.size(); i++) {
		if (stateEquals(list[i]->state, state)) {
			return list[i];
		}
	}
	return NULL;
}

void deleteNode (vector<Node*> &list, Node* x) {
	for (int i = 0; i < list.size(); i++) {
		if (stateEquals(list[i]->state, x->state)) {
			list.erase(list.begin() + i);
			return;
		}
	}
}

int main(int argc, char *argv[])
{
	//int start[N] = { 1, 6, 2, 0, 4, 3, 7, 5, 8 };
	int start[N];
	bool input = false, solseq = false, nvisited = false, pcost = false;

	for (int i = 1; i < argc; ++i) {
		//cout << "\"" << argv[i] << "\"\n";
		if (strcmp(argv[i], "-input") == 0){
			input = true;
			ifstream file(argv[i+1]);
			for(int j = 0; j < N; j++){
				file >> start[j];
			}
			i++;
			continue;
		}
		if (strcmp(argv[i], "-solseq") == 0) {
			solseq = true;
			continue;
		}
		if (strcmp(argv[i], "-pcost") == 0) {
			pcost = true;
			continue;
		}
		if (strcmp(argv[i], "-nvisited") == 0) {
			nvisited = true;
			continue;
		}
		if (strcmp(argv[i], "-h") == 0) {
			if (strcmp(argv[i + 1], "1") == 0) {
				manhattan_heuristic = false;
			}
			if (strcmp(argv[i + 1], "2") == 0) {
				manhattan_heuristic = true;
			}
			continue;
		}
	}

	if (!input) {
		cout << "Start allapot:\n";
		for (int i = 0; i < N; i++) {
			cin >> start[i];
		}
		cout << endl;
	}
	

	//int start[N] = { 3, 7, 8, 2, 0, 6, 4, 5, 1 };
	//cout << "start:\n";
	//printState(start);

	vector<Node*> openlist;
	vector<Node*> closedlist;
	Node* start_node = (Node*)calloc(1, sizeof(Node));
	start_node->state = start;
	start_node->g = 0;
	if (manhattan_heuristic) {
		start_node->h = getH_manhattan(start);
	}
	else {
		start_node->h = getH(start);
	}
	start_node->parent = NULL;
	openlist.push_back(start_node);
	
	int count = 0;
	bool solutionFound = false;
	Node* node = NULL;
	while (openlist.size() > 0) {
		count++;
		if (count > 10000) {
			break;
		}
		node = getLowestF(openlist);
		closedlist.push_back(node);
		if (isSolution(node->state)) {
			solutionFound = true;
			
			break;
		}
		vector<Node*> successors = getSuccessors(node);
		//cout << "\nselected node, g: " << node->g << ", h: " << node->h << endl;
		//printState(node->state);
		for (int i = 0; i < successors.size(); i++) {
			Node* successor = successors[i];
			//cout << "successor[" << i << "], g: " << successor->g << ", h: " << successor->h << endl;;
			//printState(successor->state);
			int f = successor->g + successor->h;
			Node* x = getNodeIfExists(openlist, successor->state);
			if (x != NULL) {
				int fx = x->g + x->h;
				if (fx <= f) {
					continue;
				}
				else {
					deleteNode(openlist, x);
				}
			}
			x = getNodeIfExists(closedlist, successor->state);
			if (x != NULL) {
				int fx = x->g + x->h;
				if (fx <= f) {
					continue;
				}
				else {
					deleteNode(closedlist, x);
				}
			}
			//cout << "add to openlist\n";
			openlist.push_back(successor);
		}
	}

	if (solutionFound) {
		cout << "Van megoldas\n";
		vector<Node*> solution;
		while (node != NULL) {
			solution.push_back(node);
			node = node->parent;
		}
		if (solseq) {
			cout << "Teljes megoldasi szekvencia:\n";
			for (int i = solution.size() - 1; i >= 0; i--) {
				printState(solution[i]->state);
				cout << "\n";
			}
		}
		if (pcost) {
			cout << "Megoldas koltsege: " << solution.size() - 1 << endl;
		}
		if (nvisited) {
			cout << "Meglatogatott csomopontok szama: " << count << endl;
		}
	}
	else {
		cout << "Nem kapott megoldast 10000 csomopont meglatogatasa utan sem\n";
	}

	

	//varjon entert
	/*char c;
	c = getchar();*/
	return 0;
}