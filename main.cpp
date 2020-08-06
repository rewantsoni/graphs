#include <iostream>
#include <utility>
#include <vector>
#include <queue>
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
        childrens.push_back(n);
    }

    void addBiEdge(node *n) {
        childrens.push_back(n);
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
        for (auto node:nodes) {
            if (!visited.count(node)) {
                if (hasCycleUtils(node, visited))
                    return true;
            }
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

    bool hasCycleUtils(node *n, map<node *, bool> &visited) {
        visited[n] = true;
        for (auto cur:n->childrens) {
            if (visited.count(cur)) {
                return true;
            }
            return hasCycleUtils(cur, visited);
        }
        return false;
    }

//    shortest path -- number b/w two nodes
//    paths - 2d array -- all the paths, path from one node to another
//    areAdjacentVertices() bool
//    degreeOfVertex() int
//    connectedComponents() int
//    hasBridge() bool --   G - n - G  -- remove N ---> disconnected
//    isGraphConnected -->BI
//    getConnectedComponents -> BI
//    Topological Sort
//    reverse -- reverse
//    reverseNew -- into a new graph
//    getConnectedComponents -> DI -- raju
};

int main() {
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
//    two->addDiEdge(two);

    //BiDirected --->
//    zero->addDiEdge(one);
//    one->addDiEdge(two);
//    one->addDiEdge(zero);
//    two->addDiEdge(one);
//    two->addBiEdge(three);
//    four->addBiEdge(five);

    auto g = graph({zero, one, two, three, four, five});
    g.print();
    g.dfs();
    g.bfs();
    cout << "hasCycle: " << ((g.hasCycle() == 1) ? "true" : "false") << endl;
    cout << "hasLoop: " << ((g.hasLoop() == 1) ? "true" : "false") << endl;
    cout << "isDirected : " << ((g.isDirected() == 1) ? "true" : "false") << endl;
    zero->clearEdges();
    g.print();
    return 0;
}
