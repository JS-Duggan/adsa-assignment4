#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <climits>
#include <numeric>

struct Edge {
    int destroy;
    int x;
    int y;

    Edge(int x, int y, int d) : x(x), y(y), destroy(d){};
    Edge(){};
};

struct Graph {
    std::vector<std::vector<Edge>> edges;
    std::vector<std::vector<Edge>> cost;             
    int nvertices;                                      /* number of vertices in graph */
    int nedges;                                         /* number of edges in graph */
    bool directed;                                      /* is the graph directed */
    void insertEdge(int, int);                          /* insert edge, nodes are 1 indexed */
    
    Graph(bool d) : directed(d){};                      /* constructor */
    void readGraph();                                   /* read graph from standard input */
};

int convert(char c) { /* helper function to convert char value to edge weight */
    int x = c - 'A';
    if (x < 26) return x;
    return x - 6;
}

struct CompDestroy { /* comparator for pq to compare edge weights */
    bool operator()(Edge const &a, Edge const &b) {
        return a.destroy < b.destroy;
    }
};

void Graph::readGraph() {
    std::string c, b, d;
    std::cin >> c >> b >> d;
    nvertices = c.find(',');
    if (nvertices < 0) {
        return;
    }
    edges = std::vector<std::vector<Edge>>(nvertices);
    cost = std::vector<std::vector<Edge>>(nvertices);
    int index, destroy, build;
    for (int i = 0; i < nvertices; i++) {
        for (int j = i + 1; j < nvertices; j++) {
            index = ((nvertices + 1) * i) + j;
            if (c[index] - '0') {
                destroy = convert(d[index]);
                build = convert(b[index]);
                edges[i].push_back(Edge(i, j, destroy));
                edges[j].push_back(Edge(j, i, destroy));
                cost[i].push_back(Edge(i, j, build));
                cost[j].push_back(Edge(j, i, build));
            }
        }
    }
}

int primModified(std::vector<bool> &inTree, std::vector<int> &parent, Graph &g, int start) {
    std::priority_queue<Edge, std::vector<Edge>, CompDestroy> pq;
    std::vector<int> distance(g.nvertices, INT_MAX);

    for (auto edge : g.edges[start]) {
        if (!inTree[edge.y] && distance[edge.y] > edge.destroy) {
            pq.push(edge);
            distance[edge.y] = edge.destroy;
        }
    }

    inTree[start] = true;
    parent[start] = start;
    distance[start] = 0;
    int weight = 0;
    Edge vertex;

    while (!pq.empty()) {
        vertex = pq.top();
        pq.pop();
        if (!inTree[vertex.y]) {
            inTree[vertex.y] = true;
            parent[vertex.y] = vertex.x;
            for (auto edge : g.edges[vertex.y]) {
                if (!inTree[edge.y] && distance[edge.y] < edge.destroy) {
                    pq.push(edge);
                    distance[edge.y] = edge.destroy;
                }
            }
        } else {
            weight += vertex.destroy; /* cost of removing back edge */
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
    int comp = 1;
    int cost = 0;

    for (int i = 0; i < g.nvertices; i++) {
        if (!inTree[i]) {
            std::cout << "Component " << comp << ":" << std::endl;
            comp++;
            cost += primModified(inTree, parent, g, i);
        }
        std::cout << "  " << i << std::endl;
    }
    
    
    std::cout << "cost: " << cost << std::endl;
}