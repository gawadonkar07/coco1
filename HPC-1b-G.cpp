#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

class Graph
{
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) : V(V)
    {
        adj.resize(V);
    }

    // Function to add an edge to the graph
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u); // Assuming undirected graph
    }

    // Parallel DFS traversal
    void parallelDFS(int source)
    {
        vector<bool> visited(V, false);
        stack<int> st;

        // Push the source node onto the stack
        st.push(source);

        while (!st.empty())
        {
            // Pop a node from the stack
            int u;
#pragma omp critical
            {
                u = st.top();
                st.pop();
            }

            // If the node hasn't been visited, mark it as visited and process it
            if (!visited[u])
            {
                // Print or process node u here
                cout << u << " ";

                // Mark node u as visited
                visited[u] = true;

                // Parallelize the exploration of adjacent nodes
#pragma omp parallel for
                for (size_t i = 0; i < adj[u].size(); ++i)
                {
                    int v = adj[u][i];
                    if (!visited[v])
                    {
#pragma omp critical
                        {
                            st.push(v);
                        }
                    }
                }
            }
        }
    }
};

int main()
{
    int V = 6;
    Graph g(V);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(2, 5);

    cout << "Parallel DFS starting from node 0:\n";
    g.parallelDFS(0);
    cout << endl;

    return 0;
}
