#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

struct node {
    int data;
    vector<node *> childrens;

    //enforce di and bi edges
    explicit node(int d) {
        data = d;
    }

    void addDiEdge(node *n) {
        if (find(childrens.begin(), childrens.end(), n) == childrens.end())
            childrens.push_back(n);
    }

    void addBiEdge(node *n) {
        if (find(childrens.begin(), childrens.end(), n) == childrens.end())
            childrens.push_back(n);
        if (find(n->childrens.begin(), n->childrens.end(), this) == n->childrens.end())
            n->childrens.push_back(this);
    }

    void clearEdges() {
        childrens.clear();
    }


};

struct graph {
    vector<node *> nodes;

    explicit graph(vector<node *> n) {
        nodes = std::move(n);
    }

    void print() {
        cout << endl << "Printing the graph:" << endl;
        for (auto node:nodes) {
            cout << node->data << ": ";
            for (auto i:node->childrens) {
                cout << i->data << " ";
            }
            cout << endl;
        }
    }

    void dfs() {
        map<node *, bool> visited;
        cout << "dfs: ";
        for (auto node:nodes) {
            if (!visited.count(node)) {
                dfsUtil(node, visited);
            }
        }
        cout << endl;
    }


    void bfs() {
        map<node *, bool> visited;
        queue<node *> q;
        cout << "bfs: ";
        node *temp;
        for (auto node:nodes) {
            if (!visited[node]) {
                visited[node] = true;
                q.push(node);
                while (!q.empty()) {
                    int sz = q.size();
                    for (int i = 0; i < sz; i++) {
                        temp = q.front();
                        q.pop();
                        cout << temp->data << " ";
                        for (auto cur:temp->childrens) {
                            if (!visited[cur]) {
                                visited[cur] = true;
                                q.push(cur);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    bool hasCycle() {
        map<node *, bool> visited;
        map<node *, bool> recVisited;
        for (auto node:nodes) {
            if (hasCycleUtils(node, visited, recVisited))
                return true;
        }
        return false;
    }

    //Pointing to itself
    bool hasLoop() {
        map<node *, bool> visited;
        for (auto node:nodes) {
            if (find(node->childrens.begin(), node->childrens.end(), node) != node->childrens.end()) {
                return true;
            }
        }
        return false;
    }

    bool isDirected() {
        for (auto node : nodes) {
            for (auto cur:node->childrens) {
                if (find(cur->childrens.begin(), cur->childrens.end(), node) == cur->childrens.end()) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isGraphConnected() {
        map<node *, bool> visited;
        int c = 0;
        for (auto node:nodes) {
            if (!visited.count(node)) {
                if (c > 0)
                    return false;
                c++;
                traverseNode(node, visited);
            }
        }
        return true;
    }

    int connectedComponents() {
        map<node *, bool> visited;
        int c = 0;
        for (auto node:nodes) {
            if (!visited.count(node)) {
                c++;
                traverseNode(node, visited);
            }
        }
        return c;
    }

    void getConnectedComponents() {
        map<node *, bool> visited;
        cout << "Connected components are:" << endl;
        for (auto node:nodes) {
            if (!visited.count(node)) {
                getConnectedComponentsUtil(node, visited);
                cout << endl;
            }
        }
    }

    void degreeOfVertexUnDirected() {
        cout << endl << "degreeOfVertexUnDirected: " << endl;
        for (auto node:nodes) {
            int count = 0;
            for (auto n:node->childrens) {
                if (n == node) {
                    count++;
                }
                count++;
            }
            cout << node->data << " : " << count << endl;

        }
    }

    void outDegreeDirected() {
        cout << endl << "outDegreeDirected: " << endl;
        for (auto node:nodes) {
            cout << node->data << " : " << node->childrens.size() << endl;

        }
    }

    void inDegreeDirected() {
        cout << endl << "inDegreeDirected: " << endl;
        int sz = nodes.size();
        vector<int> in(sz, 0);
        for (int i = 0; i < sz; i++) {
            for (auto &children : nodes[i]->childrens) {
                in[children->data]++;
            }
        }

        for (int i = 0; i < sz; i++) {
            cout << nodes[i]->data << " : " << in[i] << endl;
        }
    }

    int shortestPath(node *a, node *b) {
        int count = 0;
        map<node *, int> visited;
        queue<node *> q;
        visited[a] = true;
        q.push(a);
        node *temp;
        while (!q.empty()) {
            int sz = q.size();
            count++;
            for (int i = 0; i < sz; i++) {
                temp = q.front();
                q.pop();
                for (auto curr:temp->childrens) {
                    if (!visited[curr]) {
                        if (curr == b) {
                            return count;
                        }
                        visited[curr] = true;
                        q.push(curr);
                    }
                }
            }
        }
        return -1;
    }

    void topologicalSort() {
        map<node *, bool> visited;
        stack<node *> s;
        for (auto node:nodes) {
            if (!visited[node]) {
                topologicalSortUtils(node, visited, s);
            }
        }
        cout << "Topological Sort: ";
        while (!s.empty()) {
            cout << s.top()->data << " ";
            s.pop();
        }
        cout << endl;
    }

    void topologicalSortUtils(node *n, map<node *, bool> &visited, stack<node *> &s) {
        visited[n] = true;

        for (auto cur:n->childrens) {
            if (!visited[cur]) {
                topologicalSortUtils(cur, visited, s);
            }
        }
        s.push(n);
    }

private:
    void dfsUtil(node *n, map<node *, bool> &visited) {
        visited[n] = true;
        for (auto cur:n->childrens) {
            if (!visited.count(cur)) {
                dfsUtil(cur, visited);
            }
        }
        cout << n->data << " ";
    }

    bool hasCycleUtils(node *n, map<node *, bool> visited, map<node *, bool> &recVisited) {
        if (!visited[n]) {
            visited[n] = true;
            recVisited[n] = true;
            for (auto cur:n->childrens) {
                if ((!visited[cur] && hasCycleUtils(cur, visited, recVisited)) || recVisited[cur])
                    return true;
            }
        }
        recVisited[n] = false;
        return false;
    }

    void traverseNode(node *n, map<node *, bool> &visited) {
        visited[n] = true;
        for (auto cur:n->childrens) {
            if (!visited.count(cur)) {
                traverseNode(cur, visited);
            }
        }
    }

    void getConnectedComponentsUtil(node *n, map<node *, bool> &visited) {
        visited[n] = true;
        cout << n->data << " ";
        for (auto cur:n->childrens) {
            if (!visited.count(cur)) {
                dfsUtil(cur, visited);
            }
        }
    }
//    paths - 2d array -- all the paths, path from one node to another
//    areAdjacentVertices() bool
//    hasBridge() bool --   G - n - G  -- remove N ---> disconnected
//    reverse -- reverse
//    reverseNew -- into a new graph
//    getConnectedComponents -> DI -- raju
};


////Has weak and strong connected? no connected and disconnected? connected components in directed graph?
void directed() {

    cout << "Directed Graph" << endl;

    node *zero = new node(0);
    node *one = new node(1);
    node *two = new node(2);
    node *three = new node(3);
    node *four = new node(4);
    node *five = new node(5);

    zero->addDiEdge(one);
    one->addDiEdge(two);
    two->addDiEdge(zero);
    two->addBiEdge(three);
    four->addDiEdge(five);
    two->addDiEdge(two);

    //DAG - Directed Acyclic Graph
//    zero->addDiEdge(one);
//    zero->addDiEdge(two);
//    two->addDiEdge(three);
//    one->addDiEdge(four);
//    one->addDiEdge(three);
//    four->addDiEdge(five);
//    three->addDiEdge(five);

    auto g = graph({zero, one, two, three, four, five});
    g.print();
    g.dfs();
    g.bfs();

    cout << "isDirected : " << ((g.isDirected() == 1) ? "true" : "false") << endl;

    cout << "hasCycle: " << ((g.hasCycle() == 1) ? "true" : "false") << endl;

    cout << "hasLoop: " << ((g.hasLoop() == 1) ? "true" : "false") << endl;

    g.outDegreeDirected();
    g.inDegreeDirected();
    cout << "Shortest path: " << g.shortestPath(zero, two) << endl;
    cout << "Shortest path: " << g.shortestPath(two, zero) << endl;

    if (g.isDirected() && !g.hasCycle())
        g.topologicalSort();

//    zero->clearEdges();
//    g.print();
}

void unDirected() {

    cout << "UnDirected Graph" << endl;
    node *zero = new node(0);
    node *one = new node(1);
    node *two = new node(2);
    node *three = new node(3);
    node *four = new node(4);
    node *five = new node(5);

    zero->addDiEdge(one);
    one->addDiEdge(two);
    one->addDiEdge(zero);
    one->addBiEdge(one);
    two->addDiEdge(one);
    two->addBiEdge(three);
    four->addBiEdge(five);
//    three->addBiEdge(five);

    auto g = graph({zero, one, two, three, four, five});
    g.print();
    g.dfs();
    g.bfs();

    cout << "isDirected : " << ((g.isDirected() == 1) ? "true" : "false") << endl;

    cout << "hasLoop: " << ((g.hasLoop() == 1) ? "true" : "false") << endl;

    cout << "isGraphConnected : " << ((g.isGraphConnected() == 1) ? "true" : "false") << endl;

    cout << "Number of connected components: " << g.connectedComponents() << endl;
    g.getConnectedComponents();

    g.degreeOfVertexUnDirected();

    cout << "Shortest path: " << g.shortestPath(zero, two) << endl;
    cout << "Shortest path: " << g.shortestPath(two, zero) << endl;

    zero->clearEdges();
    g.print();

}

int main() {

    directed();
    cout << endl << endl;
//    unDirected();
    return 0;
}
