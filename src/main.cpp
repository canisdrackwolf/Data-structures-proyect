#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;

#define ROJO_T      "\x1b[31m"
#define VERDE_T     "\x1b[32m"
#define MAGENTA_T   "\x1b[35m"
#define BLANCO_T	"\x1b[37m"


//PARTE DE ARBOLES ------------------------------------------------------------------------------------------------------------------------------
enum Color { RED, BLACK };

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

struct RBNode {
    int val;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    RBNode(int x) : val(x), color(RED), left(NULL), right(NULL), parent(NULL) {}
};

// Funciones auxiliares para AVL
int height(TreeNode* node) {
    if (node == NULL) return 0;
    return 1 + max(height(node->left), height(node->right));
}

bool isAVL(TreeNode* root) {
    if (root == NULL) return true;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    if (abs(leftHeight - rightHeight) > 1) return false;
    return isAVL(root->left) && isAVL(root->right);
}

TreeNode* rotateRight(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}

TreeNode* rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}

int getBalance(TreeNode* node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

TreeNode* insertAVL(TreeNode* node, int key) {
    if (node == NULL)
        return new TreeNode(key);

    if (key < node->val)
        node->left = insertAVL(node->left, key);
    else if (key > node->val)
        node->right = insertAVL(node->right, key);
    else
        return node;

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->val)
        return rotateRight(node);

    if (balance < -1 && key > node->right->val)
        return rotateLeft(node);

    if (balance > 1 && key > node->left->val) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && key < node->right->val) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

TreeNode* insertBST(TreeNode* root, int key) {
    if (root == NULL)
        return new TreeNode(key);

    if (key < root->val)
        root->left = insertBST(root->left, key);
    else if (key > root->val)
        root->right = insertBST(root->right, key);

    return root;
}

TreeNode* convertToAVL(TreeNode* root) {
    if (root == NULL) return NULL;
    vector<int> nodes;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        nodes.push_back(node->val);
        if (node->left != NULL) q.push(node->left);
        if (node->right != NULL) q.push(node->right);
    }
    TreeNode* avlRoot = NULL;
    for (int val : nodes) {
        avlRoot = insertAVL(avlRoot, val);
    }
    return avlRoot;
}


// Funciones auxiliares para Rojo-Negro
void leftRotate(RBNode*& root, RBNode*& pt) {
    RBNode* pt_right = pt->right;
    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_right;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void rightRotate(RBNode*& root, RBNode*& pt) {
    RBNode* pt_left = pt->left;
    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_left;
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

void balanceInsert(RBNode*& root, RBNode*& pt) {
    RBNode* parent_pt = NULL;
    RBNode* grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (parent_pt == grand_parent_pt->left) {
            RBNode* uncle_pt = grand_parent_pt->right;

            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->right) {
                    leftRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rightRotate(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        } else {
            RBNode* uncle_pt = grand_parent_pt->left;

            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->left) {
                    rightRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                leftRotate(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    root->color = BLACK;
}

RBNode* insertRB(RBNode* root, int val) {
    RBNode* pt = new RBNode(val);

    if (root == NULL) {
        pt->color = BLACK;
        return pt;
    }

    RBNode* current = root;
    RBNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (pt->val < current->val)
            current = current->left;
        else if (pt->val > current->val)
            current = current->right;
        else
            return root; // Duplicates are not allowed
    }

    pt->parent = parent;
    if (pt->val < parent->val)
        parent->left = pt;
    else
        parent->right = pt;

    balanceInsert(root, pt);

    return root;
}

RBNode* convertToRB(TreeNode* root) {
    if (root == NULL) return NULL;
    vector<int> nodes;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        nodes.push_back(node->val);
        if (node->left != NULL) q.push(node->left);
        if (node->right != NULL) q.push(node->right);
    }
    RBNode* rbRoot = NULL;
    for (int val : nodes) {
        rbRoot = insertRB(rbRoot, val);
    }
    return rbRoot;
}

void printRBTree(RBNode* root) {
    if (root == NULL) return;
    queue<RBNode*> q;
    q.push(root);
    while (!q.empty()) {
        RBNode* node = q.front();
        q.pop();
        cout << node->val << (node->color == RED ? "R " : "B ");
        if (node->left != NULL) q.push(node->left);
        if (node->right != NULL) q.push(node->right);
    }
    cout << endl;
}

void printTree(TreeNode* root) {
    if (root == NULL) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        cout << node->val << " ";
        if (node->left != NULL) q.push(node->left);
        if (node->right != NULL) q.push(node->right);
    }
    cout << endl;
}

void generateDot(TreeNode* root, ofstream& file) {
    if (root == NULL) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        if (node->left != NULL) {
            file << "    " << node->val << " -> " << node->left->val << ";\n";
            q.push(node->left);
        }
        if (node->right != NULL) {
            file << "    " << node->val << " -> " << node->right->val << ";\n";
            q.push(node->right);
        }
    }
}

void generateDot(RBNode* root, ofstream& file) {
    if (root == NULL) return;
    queue<RBNode*> q;
    q.push(root);
    while (!q.empty()) {
        RBNode* node = q.front();
        q.pop();
        file << "    " << node->val << " [style=filled, fillcolor="
             << (node->color == RED ? "red" : "gray") << "];\n";
        if (node->left != NULL) {
            file << "    " << node->val << " -> " << node->left->val << ";\n";
            q.push(node->left);
        }
        if (node->right != NULL) {
            file << "    " << node->val << " -> " << node->right->val << ";\n";
            q.push(node->right);
        }
    }
}

void printBSTTreeDot(TreeNode* root, const string& filename) {
    ofstream file(filename);
    file << "digraph BST {\n";
    generateDot(root, file);
    file << "}\n";
    file.close();
}

void printAVLTreeDot(TreeNode* root, const string& filename) {
    ofstream file(filename);
    file << "digraph AVL {\n";
    generateDot(root, file);
    file << "}\n";
    file.close();
}

void printRBTreeDot(RBNode* root, const string& filename) {
    ofstream file(filename);
    file << "digraph RBTree {\n";
    generateDot(root, file);
    file << "}\n";
    file.close();
}

// PARTE DE GRAFOS -------------------------------------------------------------------------------------------------------------------------

// Estructura para representar un grafo
struct Graph {
    vector<vector<int>> adjacencyMatrix;
};

// Estructura para representar una arista
struct Edge {
    int src, dest, weight;
};

// Función para imprimir la matriz de adyacencia
void printAdjacencyMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

// Función para leer la matriz de adyacencia desde un archivo de texto
vector<vector<int>> readAdjacencyMatrixFromFile(const string& filename) {
    vector<vector<int>> matrix;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            vector<int> row;
            int value;
            while (ss >> value) {
                row.push_back(value);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            matrix.push_back(row);
        }
        file.close();
    } else {
        cerr << "Error: No se pudo abrir el archivo." << endl;
    }
    return matrix;
}

// Función para leer el grafo desde un archivo de texto
Graph readGraphFromFile(const string& filename) {
    Graph graph;
    graph.adjacencyMatrix = readAdjacencyMatrixFromFile(filename);
    return graph;
}

// Función para generar y guardar un archivo DOT para un grafo
void generateDotFile(const Graph& graph, const string& filename) {
    ofstream dotFile(filename);
    if (dotFile.is_open()) {
        dotFile << "graph G {" << endl;
        for (int i = 0; i < graph.adjacencyMatrix.size(); ++i) {
            for (int j = i + 1; j < graph.adjacencyMatrix.size(); ++j) {
                if (graph.adjacencyMatrix[i][j] != 0) {
                    dotFile << i << " -- " << j << " [label=\"" << graph.adjacencyMatrix[i][j] << "\"];" << endl;
                }
            }
        }
        dotFile << "}" << endl;
        dotFile.close();
        cout << "Archivo DOT generado: " << filename << endl;
    } else {
        cerr << "Error: No se pudo abrir el archivo DOT para escribir." << endl;
    }
}

// Algoritmo de Floyd-Warshall para encontrar todas las distancias más cortas entre todos los pares de vértices
void floydWarshall(const Graph& graph) {
    int V = graph.adjacencyMatrix.size();
    vector<vector<int>> dist(V, vector<int>(V, numeric_limits<int>::max()));

    // Inicializar la matriz de distancias con los pesos de las aristas
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            dist[i][j] = graph.adjacencyMatrix[i][j];
        }
    }

    // Actualizar las distancias mínimas entre todos los pares de vértices
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][k] != numeric_limits<int>::max() && dist[k][j] != numeric_limits<int>::max() &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Imprimir las distancias mínimas entre todos los pares de vértices
    cout << "Distancias mínimas entre todos los pares de vértices (Floyd-Warshall):" << endl;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (dist[i][j] == numeric_limits<int>::max())
                cout << "INF ";
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }

    // Generar archivo DOT para visualizar el resultado de Floyd-Warshall
    ofstream dotFile("floyd_warshall.dot");
    if (dotFile.is_open()) {
        dotFile << "graph G {" << endl;
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][j] != numeric_limits<int>::max() && i != j) {
                    dotFile << i << " -- " << j << " [label=\"" << dist[i][j] << "\"];" << endl;
                }
            }
        }
        dotFile << "}" << endl;
        dotFile.close();
        cout << "Archivo DOT generado para Floyd-Warshall: floyd_warshall.dot" << endl;

        // Verificar si el archivo DOT se ha generado correctamente
        ifstream checkFile("floyd_warshall.dot");
        if (checkFile.is_open()) {
            checkFile.close();
            // Generar la imagen PNG a partir del archivo DOT
            if (system("dot -Tpng floyd_warshall.dot -o floyd_warshall.png") == 0) {
                cout << "Imagen de Floyd-Warshall generada como 'floyd_warshall.png'." << endl;
            } else {
                cerr << "Error: No se pudo generar la imagen de Floyd-Warshall." << endl;
            }
        } else {
            cerr << "Error: El archivo DOT de Floyd-Warshall no se generó correctamente." << endl;
        }
    } else {
        cerr << "Error: No se pudo abrir el archivo DOT para escribir." << endl;
    }
}

// Función para el algoritmo de Dijkstra
void dijkstra(const Graph& graph, int startNode) {
    int numNodes = graph.adjacencyMatrix.size();
    vector<int> dist(numNodes, numeric_limits<int>::max());
    vector<bool> visited(numNodes, false);

    dist[startNode] = 0;

    for (int count = 0; count < numNodes - 1; ++count) {
        int minDist = numeric_limits<int>::max();
        int minIndex = -1;

        // Encontrar el nodo no visitado con la distancia mínima
        for (int i = 0; i < numNodes; ++i) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIndex = i;
            }
        }

        if (minIndex == -1)
            break;

        visited[minIndex] = true;

        // Actualizar las distancias de los nodos adyacentes al nodo seleccionado
        for (int i = 0; i < numNodes; ++i) {
            if (!visited[i] && graph.adjacencyMatrix[minIndex][i] && 
                dist[minIndex] != numeric_limits<int>::max() &&
                dist[minIndex] + graph.adjacencyMatrix[minIndex][i] < dist[i]) {
                dist[i] = dist[minIndex] + graph.adjacencyMatrix[minIndex][i];
            }
        }
    }

    // Imprimir las distancias mínimas desde el nodo inicial
    cout << "Distancias mínimas desde el nodo " << startNode << " (Dijkstra):" << endl;
    for (int i = 0; i < numNodes; ++i) {
        cout << "Nodo " << i << ": " << dist[i] << endl;
    }

    // Generar archivo DOT para visualizar el resultado de Dijkstra
    ofstream dotFile("dijkstra.dot");
    if (dotFile.is_open()) {
        dotFile << "graph G {" << endl;
        for (int i = 0; i < numNodes; ++i) {
            if (dist[i] != numeric_limits<int>::max() && i != startNode) {
                dotFile << startNode << " -- " << i << " [label=\"" << dist[i] << "\"];" << endl;
            }
        }
        dotFile << "}" << endl;
        dotFile.close();
        cout << "Archivo DOT generado para Dijkstra: dijkstra.dot" << endl;

        // Verificar si el archivo DOT se ha generado correctamente
        ifstream checkFile("dijkstra.dot");
        if (checkFile.is_open()) {
            checkFile.close();
            // Generar la imagen PNG a partir del archivo DOT
            if (system("dot -Tpng dijkstra.dot -o dijkstra.png") == 0) {
                cout << "Imagen de Dijkstra generada como 'dijkstra.png'." << endl;
            } else {
                cerr << "Error: No se pudo generar la imagen de Dijkstra." << endl;
            }
        } else {
            cerr << "Error: El archivo DOT de Dijkstra no se generó correctamente." << endl;
        }
    } else {
        cerr << "Error: No se pudo abrir el archivo DOT para escribir." << endl;
    }
}

// Función para encontrar la raíz del conjunto al que pertenece un nodo
int find(vector<int>& parent, int i) {
    if (parent[i] == -1)
        return i;
    return find(parent, parent[i]);
}

// Función para unir dos conjuntos en un solo conjunto
void unionSets(vector<int>& parent, int x, int y) {
    int xSet = find(parent, x);
    int ySet = find(parent, y);
    parent[xSet] = ySet;
}

// Función para comparar el peso de dos aristas
bool compareEdges(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

// Algoritmo de Kruskal para encontrar un árbol de expansión mínima
void kruskal(const Graph& graph) {
    int V = graph.adjacencyMatrix.size();
    vector<Edge> result(V - 1); // Árbol de expansión mínima tendrá (V - 1) aristas
    vector<int> parent(V, -1);  // Vector de padres para representar los conjuntos

    vector<Edge> edges; // Lista de aristas ordenadas por peso
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            if (graph.adjacencyMatrix[i][j] != 0) {
                edges.push_back({i, j, graph.adjacencyMatrix[i][j]});
            }
        }
    }
    // Ordenar las aristas por peso
    sort(edges.begin(), edges.end(), compareEdges);

    int edgeCount = 0;
    int i = 0;
    while (edgeCount < V - 1 && i < edges.size()) {
        Edge nextEdge = edges[i++];
        int x = find(parent, nextEdge.src);
        int y = find(parent, nextEdge.dest);
        if (x != y) {
            result[edgeCount++] = nextEdge;
            unionSets(parent, x, y);
        }
    }

    // Imprimir el árbol de expansión mínima
    cout << "Arbol de expansion minima (Kruskal):" << endl;
    for (int i = 0; i < V - 1; ++i) {
        cout << result[i].src << " -- " << result[i].dest << " [peso=" << result[i].weight << "]" << endl;
    }

    // Generar archivo DOT para visualizar el resultado de Kruskal
    ofstream dotFile("kruskal.dot");
    if (dotFile.is_open()) {
        dotFile << "graph G {" << endl;
        for (int i = 0; i < V - 1; ++i) {
            dotFile << result[i].src << " -- " << result[i].dest << " [label=\"" << result[i].weight << "\"];" << endl;
        }
        dotFile << "}" << endl;
        dotFile.close();
        cout << "Archivo DOT generado para Kruskal: kruskal.dot" << endl;

        // Verificar si el archivo DOT se ha generado correctamente
        ifstream checkFile("kruskal.dot");
        if (checkFile.is_open()) {
            checkFile.close();
            // Generar la imagen PNG a partir del archivo DOT
            if (system("dot -Tpng kruskal.dot -o kruskal.png") == 0) {
                cout << "Imagen de Kruskal generada como 'kruskal.png'." << endl;
            } else {
                cerr << "Error: No se pudo generar la imagen de Kruskal." << endl;
            }
        } else {
            cerr << "Error: El archivo DOT de Kruskal no se generó correctamente." << endl;
        }
    } else {
        cerr << "Error: No se pudo abrir el archivo DOT para escribir." << endl;
    }
}

// Algoritmo de Prim para encontrar un árbol de expansión mínima
void prim(const Graph& graph) {
    int V = graph.adjacencyMatrix.size();
    vector<int> key(V, numeric_limits<int>::max());
    vector<bool> inMST(V, false);
    vector<int> parent(V, -1);
    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; ++count) {
        int minKey = numeric_limits<int>::max();
        int u = -1;

        for (int v = 0; v < V; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        inMST[u] = true;

        for (int v = 0; v < V; ++v) {
            if (graph.adjacencyMatrix[u][v] && !inMST[v] && graph.adjacencyMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph.adjacencyMatrix[u][v];
            }
        }
    }

    // Imprimir el árbol de expansión mínima
    cout << "Arbol de expansion minima (Prim):" << endl;
    for (int i = 1; i < V; ++i) {
        cout << parent[i] << " -- " << i << " [peso=" << graph.adjacencyMatrix[i][parent[i]] << "]" << endl;
    }

    // Generar archivo DOT para visualizar el resultado de Prim
    ofstream dotFile("prim.dot");
    if (dotFile.is_open()) {
        dotFile << "graph G {" << endl;
        for (int i = 1; i < V; ++i) {
            dotFile << parent[i] << " -- " << i << " [label=\"" << graph.adjacencyMatrix[i][parent[i]] << "\"];" << endl;
        }
        dotFile << "}" << endl;
        dotFile.close();
        cout << "Archivo DOT generado para Prim: prim.dot" << endl;

        // Verificar si el archivo DOT se ha generado correctamente
        ifstream checkFile("prim.dot");
        if (checkFile.is_open()) {
            checkFile.close();
            // Generar la imagen PNG a partir del archivo DOT
            if (system("dot -Tpng prim.dot -o prim.png") == 0) {
                cout << "Imagen de Prim generada como 'prim.png'." << endl;
            } else {
                cerr << "Error: No se pudo generar la imagen de Prim." << endl;
            }
        } else {
            cerr << "Error: El archivo DOT de Prim no se generó correctamente." << endl;
        }
    } else {
        cerr << "Error: No se pudo abrir el archivo DOT para escribir." << endl;
    }
}

// int main
int main() {
    int opcion;
    do {
        cout << ROJO_T"\nMenu Principal\n" << endl;
        cout << BLANCO_T"1. Presentar ABB Autobalanceados" << endl;
        cout << "2. Presentar Grafos" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
		
		system("cls");
		
        switch (opcion) {
            case 1: {
                // Submenú para Árboles
                TreeNode* root = NULL;
                char subopcion;
                do {
                    cout << ROJO_T"\nSubmenu para Arboles Binarios de Busqueda Autobalanceados\n";
                    cout << BLANCO_T"a. Ingrese la lista de nodos y el recorrido para construir el arbol\n";
                    cout << "b. Mostrar de forma visual el arbol inicial (indica si el arbol es AVL o no)\n";
                    cout << "c. Convertir en AVL el arbol inicial y mostrar la visualizacion del arbol\n";
                    cout << "d. Convertir en arbol Rojo y Negro el arbol inicial y mostrar la visualizacion del arbol\n";
                    cout << "e. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> subopcion;
					
					system("cls");
		
                    switch (subopcion) {
                        case 'a': {
                            int n, val;
                            cout << "Ingrese el numero de nodos: ";
                            cin >> n;
                            cout << "Ingrese los valores de los nodos: ";
                            for (int i = 0; i < n; ++i) {
                                cin >> val;
                                root = insertBST(root, val);
                            }
                            break;
                        }
                        case 'b': {
                            cout << "Arbol inicial (nivel por nivel):\n";
                            printTree(root);
                            printBSTTreeDot(root, "images/bst_tree.dot");
                            cout << "El arbol " << (isAVL(root) ? "es" : "no es") << " un AVL.\n";
                            cout << "Archivo DOT para el arbol inicial generado como 'bst_tree.dot'.\n";
                            system("dot -Tpng images/bst_tree.dot -o images/bst_tree.png");
                            cout << "Imagen del arbol inicial generada como 'images/bst_tree.png'.\n";
                            break;
                        }
                        case 'c': {
                            TreeNode* avlRoot = convertToAVL(root);
                            cout << "Arbol convertido a AVL (nivel por nivel):\n";
                            printTree(avlRoot);
                            printAVLTreeDot(avlRoot, "images/avl_tree.dot");
                            cout << "Archivo DOT para AVL generado como 'images/avl_tree.dot'.\n";
                            system("dot -Tpng images/avl_tree.dot -o images/avl_tree.png");
                            cout << "Imagen del arbol AVL generada como 'images/avl_tree.png'.\n";
                            break;
                        }
                        case 'd': {
                            RBNode* rbRoot = convertToRB(root);
                            cout << "Arbol convertido a Rojo-Negro (nivel por nivel):\n";
                            printRBTree(rbRoot);
                            printRBTreeDot(rbRoot, "images/rb_tree.dot");
                            cout << "Archivo DOT para Rojo-Negro generado como 'images/rb_tree.dot'.\n";
                            system("dot -Tpng images/rb_tree.dot -o images/rb_tree.png");
                            cout << "Imagen del arbol Rojo-Negro generada como 'images/rb_tree.png'.\n";
                            break;
                        }
                        case 'e': {
                            cout << VERDE_T"Volviendo al menu principal...\n";
                            break;
                        }
                        default: {
                            cout << MAGENTA_T"Opcion no valida. Por favor, ingrese una opcion valida.\n";
                            break;
                        }
                    }
                } while (subopcion != 'e');
                break;
            }
            case 2: {
                // Submenú para Grafos
                Graph graph;
                char subopcion;
                do {
                    cout << ROJO_T"\nSubmenu para Grafos\n";
                    cout << BLANCO_T"a. Ingrese la matriz de adyacencias\n";
                    cout << "b. Mostrar la visualizacion del grafo inicial\n";
                    cout << "c. Elija un algoritmo para encontrar el camino mas corto\n";
                    cout << "d. Elija un algoritmo para encontrar arboles de peso minimo\n";
                    cout << "e. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> subopcion;
					
					system("cls");
		
                    switch (subopcion) {
                        case 'a': {
                            string filepath;
                            cout << MAGENTA_T"Current repository path: adjancency_Matrix/matriz_de_adyacencias_30x30.txt\n";
                            cout << BLANCO_T"Ingrese la ruta absoluta del archivo de texto: ";                            
                            cin >> filepath;
                            graph.adjacencyMatrix = readAdjacencyMatrixFromFile(filepath);
                            break;
                        }
                        case 'b': {
                            generateDotFile(graph, "graph.dot");

                            ifstream checkFile("graph.dot");
                            if (checkFile.is_open()) {
                                checkFile.close();
                                if (system("dot -Tpng graph.dot -o graph.png") == 0) {
                                    cout << "Imagen del grafo generada como 'graph.png'." << endl;
                                } else {
                                    cerr << ROJO_T"Error: No se pudo generar la imagen del grafo." << endl;
                                }
                            } else {
                                cerr << ROJO_T"Error: El archivo DOT no se genero correctamente." << endl;
                            }
                            break;
                        }
                        case 'c': {
                            char subChoice;
                            do {
                                cout << ROJO_T"\nSubmenu para algoritmos de camino mas corto:\n" << endl;
                                cout << BLANCO_T"a. Dijkstra" << endl;
                                cout << "b. Floyd-Warshall" << endl;
                                cout << "c. Volver al menu principal" << endl;
                                cout << "Ingrese su eleccion: ";
                                cin >> subChoice;
                                switch (subChoice) {
                                    case 'a': {
                                        int startNode;
                                        cout << "Ingrese el nodo inicial para Dijkstra: ";
                                        cin >> startNode;
                                        dijkstra(graph, startNode);
                                        break;
                                    }
                                    case 'b': {
                                        floydWarshall(graph);
                                        break;
                                    }
                                    case 'c': {
                                        cout << VERDE_T"Volviendo al menu principal." << endl;
                                        break;
                                    }
                                    default: {
                                        cout << ROJO_T"Opcion no valida. Por favor, ingrese una opcion valida." << endl;
                                        break;
                                    }
                                }
                            } while (subChoice != 'c');
                            break;
                        }
                        case 'd': {
                            char subChoice;
                            do {
                                cout << ROJO_T"\nSubmenu para algoritmos de arboles de peso minimo:\n" << endl;
                                cout << BLANCO_T"a. Kruskal" << endl;
                                cout << "b. Prim" << endl;
                                cout << "c. Volver al menu principal" << endl;
                                cout << "Ingrese su eleccion: ";
                                cin >> subChoice;
                                
                                system("cls");
		
                                switch (subChoice) {
                                    case 'a': {
                                        kruskal(graph);
                                        break;
                                    }
                                    case 'b': {
                                        prim(graph);
                                        break;
                                    }
                                    case 'c': {
                                        cout << VERDE_T"\nVolviendo al menu principal." << endl;
                                        break;
                                    }
                                    default: {
                                        cout << ROJO_T"\nOpcion no valida. Por favor, ingrese una opcion valida." << endl;
                                        break;
                                    }
                                }
                            } while (subChoice != 'c');
                            break;
                        }
                        case 'e': {
                            cout << VERDE_T"\nVolviendo al menu principal." << endl;
                            break;
                        }
                        default: {
                            cout << ROJO_T"\nOpcion no valida. Por favor, ingrese una opcion valida." << endl;
                            break;
                        }
                    }
                } while (subopcion != 'e');
                break;
            }
            case 3: {
                cout << VERDE_T"\nSaliendo..." << endl;
                break;
            }
            default: {
                cout << ROJO_T"\nOpcion no valida. Por favor, seleccione una opcion valida." << endl;
                break;
            }
        }
    } while (opcion != 3);

    return 0;
}
