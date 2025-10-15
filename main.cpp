#include <vector>
#include <iostream>
#include <queue>
#include <numeric>

struct Edge {
    int x;
    int y;
    int weight;

    Edge(int x, int y, int w) : x(x), y(y), weight(w){};
    Edge(){};
};

int find(std::vector<int> &parent, int index) {
    if (parent[index] == index) return index;
    return find(parent, parent[index]);
}

bool sameComponent(std::vector<int> &parent, int index1, int index2) {
    return find(parent, index1) == find(parent, index2);
}

void unionSets(std::vector<int> &parent, std::vector<int> &size, int index1, int index2) {
    int root1 = find(parent, index1); /* find root of sets */
    int root2 = find(parent, index2);

    if (root1 == root2) return; /* already in same set */

    if (size[root1] >= size[root2]) {
        size[root1] += size[root2];
        parent[root2] = root1;
    } else {
        size[root2] += size[root1];
        parent[root1] = root2;
    }
}

struct CompDestroy { /* comparator for pq to compare edge weights */
    bool operator()(Edge const &a, Edge const &b) {
        return a.weight < b.weight;
    }
};

struct CompBuild { /* comparator for pq to compare edge weights */
    bool operator()(Edge const &a, Edge const &b) {
        return a.weight > b.weight;
    }
};

struct Graph {
    std::priority_queue<Edge, std::vector<Edge>, CompDestroy> current;  /* edge list for existing edges */
    std::priority_queue<Edge, std::vector<Edge>, CompBuild> cost;       /* edge list for cost to build edges */
    std::vector<int> size;                                              /* size of trees */
    std::vector<int> parent;                                            /* parent node */
    int nvertices;                                                      /* number of vertices in graph */

    void readGraph();                                                   /* read graph from standard input */
    int buildTree();                                                    /* find minimum cost to build spanning tree */
};

int convert(char c) { /* helper function to convert char value to edge weight */
    int x = c - 'A';
    if (x < 26) return x;
    return x - 6;
}

void Graph::readGraph() {
    /* expected input format is twoo strings representing edge matrix
    i.e.: 011,101,110 ABB,BAB,BBA ABB,BAB,BBA is existing edges, cost \
    to destroy, cost to build*/

    /* read values */
    std::string c, b, d;
    std::cin >> c >> b >> d;
    nvertices = c.find(',');
    if (nvertices < 0) {
        return;
    }

    /* init vectors */
    size = std::vector<int>(nvertices, 1);
    parent = std::vector<int>(nvertices, 0);
    std::iota(parent.begin(), parent.end(), 0);

    /* insert edges into queue */
    int index, destroy, build;
    for (int i = 0; i < nvertices; i++) {
        for (int j = i + 1; j < nvertices; j++) {
            index = ((nvertices + 1) * i) + j;
            if (c[index] - '0') {               /* if exists */
                destroy = convert(d[index]);
                current.push(Edge(i, j, destroy));
            } else {                            /* if doesn't exist*/
                build = convert(b[index]);
                cost.push(Edge(i, j, build));
            }
        }
    }
}

int Graph::buildTree() {
    int total = 0;
    Edge edge;
    while (!current.empty()) {
        edge = current.top();
        current.pop();
        if (!sameComponent(parent, edge.x, edge.y)) {
            unionSets(parent, size, edge.x, edge.y);
        } else {
            total += edge.weight;
        }
    }

    while (!cost.empty()) {
        edge = cost.top();
        cost.pop();
        if (!sameComponent(parent, edge.x, edge.y)) {
            total += edge.weight;
            unionSets(parent, size, edge.x, edge.y);
        }
    }

    return total;
}

int main() {
    Graph g;
    g.readGraph();
    int cost = g.buildTree();
    std::cout << cost << std::endl;
}