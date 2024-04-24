#include <iostream>
#include <vector>
#include <queue>
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

    // Parallel BFS traversal
    void parallelBFS(int source)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        // Mark the source node as visited and enqueue it
        visited[source] = true;
        q.push(source);

        while (!q.empty())
        {
            // Get the current level size
            int level_size = q.size();

// Parallelize the exploration of the current level
#pragma omp parallel
            {
#pragma omp for
                for (int i = 0; i < level_size; ++i)
                {
                    int u;
// Use critical section to access and pop from the queue safely
#pragma omp critical
                    {
                        u = q.front();
                        q.pop();
                    }

                    // Print or process node u here
                    cout << u << " ";

                    // Enqueue all adjacent nodes of u that have not been visited
                    for (vector<int>::iterator it = adj[u].begin(); it != adj[u].end(); ++it)
                    {
                        int v = *it;
                        if (!visited[v])
                        {
// Use critical section to access and modify visited and queue safely
#pragma omp critical
                            {
                                visited[v] = true;
                                q.push(v);
                            }
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

    cout << "Parallel BFS starting from node 0:\n";
    g.parallelBFS(0);
    cout << endl;

    return 0;
}
