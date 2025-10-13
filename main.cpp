#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <climits>
#include <numeric>

struct Edge {
    int x;
    int y;
    int weight;

    Edge(int x, int y, int w) : x(x), y(y), weight(w){};
    Edge(){};
};

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

int find(std::vector<int> &parent, int index) {
    if (parent[index] == index) return index;
    return find(parent, parent[index]);
}

bool sameComponent(std::vector<int> &parent, int index1, int index2) {
    return find(parent, index1) == find(parent, index2);
}

void unionSets(std::vector<int> &parent, int index1, int index2) {
    int root1 = find(parent, index1); /* find root of sets */
    int root2 = find(parent, index2);

    if (root1 == root2) return; /* already in same set */

    parent[root2] = root1;
}

struct Graph {
    std::vector<std::vector<Edge>> edges;                           /* adjacency list for Prim's */
    std::priority_queue<Edge, std::vector<Edge>, CompBuild> cost;   /* edge list for Kruskal's */
    int nvertices;                                                  /* number of vertices in graph */
    int nedges;                                                     /* number of edges in graph */
    bool directed;                                                  /* is the graph directed */
    void insertEdge(int, int);                                      /* insert edge, nodes are 1 indexed */
    
    Graph(bool d) : directed(d){};                                  /* constructor */
    void readGraph();                                               /* read graph from standard input */
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
    std::string c, b, d;
    std::cin >> c >> b >> d;
    nvertices = c.find(',');
    if (nvertices < 0) {
        return;
    }
    edges = std::vector<std::vector<Edge>>(nvertices); 
    int index, destroy, build;
    for (int i = 0; i < nvertices; i++) {
        for (int j = i + 1; j < nvertices; j++) {
            index = ((nvertices + 1) * i) + j;
            if (c[index] - '0') {
                destroy = convert(d[index]);
                edges[i].push_back(Edge(i, j, destroy));
                edges[j].push_back(Edge(j, i, destroy));
            }
            build = convert(b[index]);
            cost.push(Edge(i, j, build));
        }
    }
}

int primModified(std::vector<int> &parent, std::vector<bool> &inTree, Graph &g, int start) {
    std::priority_queue<Edge, std::vector<Edge>, CompDestroy> pq;

    for (auto edge : g.edges[start]) {
        if (!inTree[edge.y]) {
            pq.push(edge);
        }
    }

    inTree[start] = true;
    int weight = 0;
    Edge vertex;

    while (!pq.empty()) {
        vertex = pq.top();
        pq.pop();
        if (!inTree[vertex.y]) {
            inTree[vertex.y] = true;
            parent[vertex.y] = vertex.x;
            for (auto edge : g.edges[vertex.y]) {
                if (!inTree[edge.y]) {
                    pq.push(edge);
                }
            }
        } else {
            weight += vertex.weight; /* cost of removing back edge */
        }
    }

    return weight;
}

int main() {
    Graph g(true);
    g.readGraph();
    if (g.nvertices < 0) {
        std::cout << 0 << std::endl;
        return 0;
    }
    std::vector<bool> inTree(g.nvertices, false);
    std::vector<int> parent(g.nvertices);
    std::iota(parent.begin(), parent.end(), 0);
    int cost = 0;

    for (int i = 0; i < g.nvertices; i++) {
        if (!inTree[i]) {
            cost += primModified(parent, inTree, g, i);
        }
    }
    for (int i = 0; i < parent.size(); i++) {
        parent[i] = find(parent, i);
    }
    
    Edge edge;
    while (!g.cost.empty()) {
        edge = g.cost.top();
        g.cost.pop();
        if (!sameComponent(parent, edge.x, edge.y)) {
            cost += edge.weight;
            unionSets(parent, edge.x, edge.y);
        }
    }
    
    std::cout << cost << std::endl;
}