#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <climits>

struct Edge {
    int weight;
    int x;
    int y;

    Edge(int x, int y, int w) : x(x), y(y), weight(w){};
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

struct EdgeComp { /* comparator for pq to compare edge weights */
    bool operator()(Edge const &a, Edge const &b) {
        return a.weight < b.weight;
    }
};

void Graph::readGraph() {
    std::string c, b, d;
    std::cin >> c >> b >> d;
    nvertices = c.find(',');

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

int primModified(std::vector<bool> &inTree, Graph &g, int start) {
    std::priority_queue<Edge, std::vector<Edge>, EdgeComp> pq;
    std::vector<int> distance(g.nvertices, INT_MAX);
    for (auto edge : g.edges[start]) {
        if (!inTree[edge.y] && distance[edge.y] > edge.weight) {
            pq.push(edge);
            distance[edge.y] = edge.weight;
        }
    }

    int weight = 0;
    inTree[start] = true;
    distance[start] = 0;
    Edge vertex;

    while (!pq.empty()) {
        vertex = pq.top();
        pq.pop();

        if (!inTree[vertex.y]) {
            std::cout << "Edge (" << vertex.x << "," << vertex.y << ")" << std::endl;
            weight += vertex.weight;
            inTree[vertex.y] = true;

            for (auto edge : g.edges[start]) {
                if (!inTree[edge.y] && distance[edge.y] > edge.weight) {
                    pq.push(edge);
                    distance[edge.y] = edge.weight;
                }
            }
        }
    }
    return weight;
}

int main() {
    Graph g(true);
    g.readGraph();
    std::vector<bool> inTree(g.nvertices, false);
    int comp = 0;
    for (int i = 0; i < g.nvertices; i++) {
        if (!inTree[i]) {
            comp++;
            std::cout << comp << std::endl;
            primModified(inTree, g, i);
        }
    }
}