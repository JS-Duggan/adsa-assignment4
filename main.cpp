#include <vector>
#include <iostream>
#include <string>

struct Edge {
    int weight;
    int y;

    Edge(int y, int w) : y(y), weight(w){};
};

struct Graph {
    std::vector<std::vector<Edge>> edges;
    std::vector<std::vector<Edge>> destroy;             
    int nvertices;                                      /* number of vertices in graph */
    int nedges;                                         /* number of edges in graph */
    bool directed;                                      /* is the graph directed */
    void insertEdge(int, int);                          /* insert edge, nodes are 1 indexed */
    
    Graph(bool d) : directed(d){};                      /* constructor */
    void readGraph();                                   /* read graph from standard input */
};

int convert(char c) {
    int x = c - 'A';
    if (x < 26) return x;
    return x - 6;
}

void Graph::readGraph() {
    std::string c, b, d;
    std::cin >> c >> b >> d;
    nvertices = c.find(',');

    edges = std::vector<std::vector<Edge>>(nvertices);
    destroy = std::vector<std::vector<Edge>>(nvertices);
    int index, destroy, build;
    for (int i = 0; i < nvertices; i++) {
        for (int j = i + 1; j < nvertices; j++) {
            index = ((nvertices + 1) * i) + j;
            if (c[index] - '0') {
                destroy = convert(d[index]);
                build = convert(b[index]);
                edges[i].push_back(Edge(j, destroy));
                edges[j].push_back(Edge(i, destroy));
            }
        }
    }
}

int main() {
    Graph g(true);
    g.readGraph();

    std::vector<std::vector<Edge>> edges = g.edges;

    for (int i = 0; i < edges.size(); i++) {
        for (int j = 0; j < edges[i].size(); j++) {
            std::cout << i << " -> " << edges[i][j].y << ": " << edges[i][j].weight << std::endl;
        }
    }
}