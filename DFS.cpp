#include "Profiler.h"
#include <iostream>
#include <crtdbg.h>
#include <stack>
#include <utility>
#include <vector>
#include <algorithm>
#define _CRTDBG_MAP_ALLOC

Profiler p("dfs");

using std::cout;
using std::cin;
using std::stack;
using std::vector;
using std::pair;

 enum {
    WHITE = 0,
    GRAY,
    BLACK
};

 struct NodeDFS {
    int val;
    int start_time;
    int finish_time;
    int adjSize;
    int adjUsedCapacity;
    int color;
    struct NodeDFS** adj;
    struct NodeDFS* parent;

    ~NodeDFS() {
        delete[] adj;
    }

};

 struct NodeTarjan{

    int val;
    int index;
    int low_link;
    int comp;
    bool on_stack;
    int adjSize;
    int adjUsedCapacity;
    struct NodeTarjan** adj;

    ~NodeTarjan() {
        delete[] adj;
    }
};

 struct Graph {
    int nrNodes;
    NodeDFS** v;

    ~Graph() {
        delete[] v;
    }

};

 struct GraphTarjan {
     int nrNodes;
     NodeTarjan** v;

     ~GraphTarjan() {
         delete[] v;
     }

 };

 template <typename T_Graph,typename T_Node>
 void addNode(T_Graph*graph,int A,int B) {

     if (A > graph->nrNodes - 1 || B > graph->nrNodes - 1) {
         cout << "Node value too high\n";
         return;
     }

     T_Node*a = graph->v[A];
     T_Node*b = graph->v[B];

     if (a->adjSize == 0) /// first node added into the list
     {
         a->adjSize = 2;
         a->adj = new T_Node*[2];
         a->adj[a->adjUsedCapacity++] = b;
     }
     else {
         if (a->adjUsedCapacity == a->adjSize) {
             a->adjSize *= 2; /// double the size everytime
             T_Node** aux = new T_Node*[a->adjSize]; /// resize
             std::copy(a->adj, a->adj + a->adjUsedCapacity, aux);
             delete[] a->adj;
             a->adj = aux;
         }
         a->adj[a->adjUsedCapacity++] = b; /// add node into adj list
     }
 }

 template <typename T_Graph,typename T_Node> /// accepts both Tarjan field and normal dfs nodes
 void createGraph(int nodes, T_Graph*& graph) {

     graph = new T_Graph();
     graph->nrNodes = nodes;
     graph->v = new T_Node *[nodes];

     for (int i = 0; i < nodes; ++i) {
         graph->v[i] = new T_Node();  /// create node in graph
         memset(graph->v[i], 0, sizeof(T_Node)); /// set with 0 all the fields
         graph->v[i]->val = i;
     }
 }

 void generate_random_edges(Graph* graph, int v, int e) {

     if (e < v - 1) {
         cout << "not enough edges to be connected";
         exit(1);
     }
     if (e > (v * (v - 1)) / 2) {
         cout << "to many edges";
         exit(2);
     }

     vector<pair<int, int>> arr;

     for (int j = 0; j < e; ++j) {

         int A, B;
         A = rand() % v; /// randomly generate ends
         B = rand() % v;

         /// check if there aren`t multiple same way edges between A and B
         while (std::find(arr.begin(), arr.end(), std::make_pair(A, B)) != arr.end()) {
             A = rand() % v;
             B = rand() % v;
         }

         addNode<Graph, NodeDFS>(graph, A, B);

         /// color,parent and times will be done inside dfs
         arr.emplace_back(A, B);
     }

     arr.clear();
 }

 template <typename T_Graph>
 void destroyGraph(T_Graph*& graph) {

     if (graph == nullptr)
         return;

     for (int i = 0; i < graph->nrNodes; ++i) {
         
            delete graph->v[i];  /// free individual node , delete fires the destructor which will free the adj list of pointers block
            graph->v[i] = nullptr;
         
     }

     delete graph; /// free graph , here also the destructor will free its pointer of nodes array
     graph = nullptr;
 }

 //////////////////////////////////////////////////// DFS

 int timeDFS = 0;
 void DFS_VISIT(NodeDFS*, Operation* op);

 void DFS( Graph& graph,Operation*op=nullptr) {
     for (int i = 0; i < graph.nrNodes; ++i){
         graph.v[i]->color = WHITE;
         graph.v[i]->parent = nullptr;
         if (op != NULL) op->count(2);
     }
         
     for (int i = 0; i < graph.nrNodes; ++i) {
         if (op != NULL) op->count();
         if (graph.v[i]->color == WHITE) {
             DFS_VISIT(graph.v[i], op);
             if (!op) cout << "\n";
         }
     }
 }

 void DFS_VISIT (NodeDFS* node, Operation* op = nullptr) {

     if (!op) cout << node->val << " ";

     timeDFS++;
     node->start_time = timeDFS;
     node->color = GRAY;

     if (op != NULL) op->count(3);

     for (int i = 0; i < node->adjUsedCapacity; ++i) {
         if (op != NULL) op->count();
         if (node->adj[i]->color == WHITE) {
             node->adj[i]->parent = node;
             if (op != NULL) op->count();
             DFS_VISIT(node->adj[i], op);
         }
     }

     if (op != NULL) op->count(3);
     node->color = BLACK;
     timeDFS++;
     node->finish_time = timeDFS;

 }

 //////////////////////////////////////////////////// PP

 template <typename T_Graph>
 void print_adjlists(T_Graph* graph)
 {
     for (int i = 0; i < graph->nrNodes; ++i) {
         cout << "Node " << graph->v[i]->val << " List: ";
         if (graph->v[i]->adj == nullptr) {
             cout << " ";
         }
         else {
         for (int j = 0; j < graph->v[i]->adjUsedCapacity; ++j) {
             cout << graph->v[i]->adj[j]->val<< " ";
         }
     }
       cout << "\n";
     }
 }

 void PP1(int* a, NodeDFS** repr, int length, int current_root, int white_space) {
     for (int i = 0; i < white_space; ++i) { // display spaces from margin, each level will increase by 3 white spaces
         std::cout << " ";
     }
     std::cout << "(" << repr[current_root]->val << ")\n";

     for (int i = 0; i < length; ++i) {
         if (a[i] == current_root) { // searches again the array and finds child of current root
             PP1(a, repr, length, i, white_space + 3); // will try to go deep to find the 1st most left child of the passed root
         }
     }
     // when it doesn't find children, it will return the next found children of current root and search for it instead again
 }

 void print_bfs_tree(Graph* graph) {
     int n = 0; // the number of nodes
     int* p = nullptr; // the parent array
     NodeDFS** repr = nullptr; // the representation for each element in p

     // some of the nodes in graph->v may not have been reached by DFS
     int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
     for (int i = 0; i < graph->nrNodes; ++i) {
         if (graph->v[i]->color == BLACK) {
             transf[i] = n;
             ++n;
         }
         else {
             transf[i] = -1;
         }
     }
     if (n == 0) {
         // no BFS tree
         free(transf);
         return;
     }

     int err = 0;
     p = (int*)malloc(n * sizeof(int));
     repr = (NodeDFS**)malloc(n * sizeof(NodeDFS*));
     for (int i = 0; i < graph->nrNodes && !err; ++i) {
         if (graph->v[i]->color == BLACK) {
             if (transf[i] < 0 || transf[i] >= n) {
                 err = 1;
             }
             else {
                 repr[transf[i]] = graph->v[i];
                 if (graph->v[i]->parent == nullptr) {
                     p[transf[i]] = -1;
                 }
                 else {
                     err = 1;
                     for (int j = 0; j < graph->nrNodes; ++j) {
                         if (graph->v[i]->parent == graph->v[j]) {
                             if (transf[j] >= 0 && transf[j] < n) {
                                 p[transf[i]] = transf[j];
                                 err = 0;
                             }
                             break;
                         }
                     }
                 }
             }
         }
     }
     free(transf);
     transf = nullptr;

     int cnt = 0;
     if (!err) {
         for (int i = 0; i < n; ++i) { // search root
             if (p[i] == -1) {
                 cout << "\ntree" << (cnt++) + 1 <<"\n";
                 PP1(p, repr, n, i, 0);
             }
         }
     }

     if (p != nullptr) {
         free(p);
         p = nullptr;
     }
     if (repr != nullptr) {
         free(repr);
         repr = nullptr;
     }
 }

 ////////////////////////////////////////////////// TOPO

 int timeTopo = 0;
 void TopoSortHelper(stack<NodeDFS*>& , NodeDFS*,bool&);

 void TopoSort(Graph & graph, stack<NodeDFS*>& stack,bool &has_cycle) {

     for (int i = 0; i < graph.nrNodes; ++i) {
         graph.v[i]->color = WHITE;
         graph.v[i]->parent = nullptr;
     }

     for (int i = 0; i < graph.nrNodes; ++i)
         if (graph.v[i]->color == WHITE) {
             TopoSortHelper(stack, graph.v[i], has_cycle);
             if (has_cycle) return;
         }
 }

 void TopoSortHelper(stack<NodeDFS*>& stack,NodeDFS*node,bool& has_cycle) {
     
     timeTopo++;
     node->start_time = timeTopo;
     node->color = GRAY;

     for (int i = 0; i < node->adjUsedCapacity; ++i) {
         if (node->adj[i]->color == WHITE) {
             node->adj[i]->parent = node;
             TopoSortHelper(stack, node->adj[i], has_cycle);
             if (has_cycle) return;
         }
         else if (node->adj[i]->color == GRAY) {
             has_cycle = true;
             cout << "Graph is non - DAG ! \n";
             return;
         }
     }
    
    node->color = BLACK;
    timeTopo++;
    node->finish_time = timeTopo;
    stack.push(node);
     
 }

 ////////////////////////////////////////////////// TARJAN

 int TarjanIndex = 0, NrComp = 0;
 void Strong_Component(stack<NodeTarjan*>& , NodeTarjan* );

 void Tarjan(GraphTarjan& graph, stack<NodeTarjan*>& stack) {
     for (int i = 0; i < graph.nrNodes; ++i) {
         graph.v[i]->index = graph.v[i]->low_link = -1;
         graph.v[i]->on_stack = false;
         graph.v[i]->comp = 0;
     }

     for (int i = 0; i < graph.nrNodes; ++i)
         if (graph.v[i]->index == -1) {
             Strong_Component(stack, graph.v[i]);
         }
 }

 void Strong_Component(stack<NodeTarjan*>& stack, NodeTarjan*node) {
     stack.push(node);
     node->index = node->low_link = TarjanIndex++;
     node->on_stack = true;

     for (int i = 0; i < node->adjUsedCapacity; ++i) {
         if (node->adj[i]->index == -1)  /// unexplored vertex
             Strong_Component(stack, node->adj[i]);
         if (node->adj[i]->on_stack) /// if where we came from,on the call back, is on the same stack => we can update low link (there must be a path between the 2)
             node->low_link = min(node->low_link, node->adj[i]->low_link); /// alows low_link values to propagate through a cycle
     }
     /// after having visited all node`s neighb. if it`s the start of a SCC pop all the associated nodes from the stack => component
     if (node->index == node->low_link) { 
         NrComp++;
         NodeTarjan* top;
         cout << NrComp << ": ";
         do {
             top = stack.top();
             cout << top->val << " ";
             stack.pop();
             top->on_stack = false;
             top->comp = NrComp;
         } while (top != node);
         cout << "\n";
     }
 }

 ////////////////////////////////////////////////// DEMO/PERF

 void demo() {

     ////////////////////////////////////////////////////// RANDOM

     Graph* graph = nullptr;
     createGraph<Graph,NodeDFS>(10, graph);
     generate_random_edges(graph, 10, 15);

     cout << "RANDOM GRAPH:\n";
     print_adjlists(graph);

     destroyGraph(graph);

     ////////////////////////////////////////////////////// DFS
     cout << "\n";
     createGraph<Graph, NodeDFS>(6, graph);

     addNode<Graph, NodeDFS>(graph, 0, 1);
     addNode<Graph, NodeDFS>(graph, 1, 2);
     addNode<Graph, NodeDFS>(graph, 2, 3);
     addNode<Graph, NodeDFS>(graph, 3, 1);
     addNode<Graph, NodeDFS>(graph, 0, 3);
     addNode<Graph, NodeDFS>(graph, 4, 3);
     addNode<Graph, NodeDFS>(graph, 4, 5);
     addNode<Graph, NodeDFS>(graph, 5, 5);

     cout << "\nDFS DEMO GRAPH:\n";
     print_adjlists(graph);

     cout << "\n";
     cout << "DFS TREES:\n";
     DFS(*graph);
     
     cout << "\nDFS_FOREST:\n";
     print_bfs_tree(graph);

     destroyGraph(graph);

     ////////////////////////////////////////////////////// TOPO
     cout << "\n";
     bool has_cycle = false;
     stack<NodeDFS*> stack1;
     createGraph<Graph, NodeDFS>(6, graph);

     //addNode<Graph,NodeDFS>(graph, 0, 1);
     //addNode<Graph,NodeDFS>(graph, 1, 2);
     //addNode<Graph,NodeDFS>(graph, 2, 0);

     addNode<Graph, NodeDFS>(graph, 0, 2);
     addNode<Graph, NodeDFS>(graph, 1, 2);
     addNode<Graph, NodeDFS>(graph, 1, 3);
     addNode<Graph, NodeDFS>(graph, 2, 4);
     addNode<Graph, NodeDFS>(graph, 3, 5);
     addNode<Graph, NodeDFS>(graph, 4, 5);
     
     cout << "\nTOPOLOGICAL DEMO GRAPH:\n";
     print_adjlists(graph);

     cout << "\nTOPOLOGICAL DEMO RESULT LIST:\n";
     TopoSort(*graph, stack1, has_cycle);

     /// the resulted stack is non-empty upon succesfully ordering the nodes by their discovery time , if all its descendands have been visited , or
     /// dont exists ,we add the node. If there is a cycle we dont add any elements and we return , because it`s impossible to create a directed dependency between nodes in cycle
     /// we dont know who comes before who
    
     if (stack1.empty()) {
         cout << "empty stack!\n";
     }
     while (!stack1.empty() && !has_cycle) {
         cout << stack1.top()->val << "  ";
         stack1.pop();
     }
     destroyGraph(graph);
     ///////////////////////////////////////////////////// TARJAN
     cout << "\n";
     stack<NodeTarjan*> stack2;
     GraphTarjan* graphtarjan = nullptr;
     createGraph<GraphTarjan, NodeTarjan>(8, graphtarjan);

     
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 1,0);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 2,3);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 0,2);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 1,3);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 2,1);

     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 2,4);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 4,6);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 4,5);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 6,7);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 5,7);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 3,5);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 5,3);
     addNode<GraphTarjan, NodeTarjan>(graphtarjan, 6,4);
     

     ///addNode<GraphTarjan, NodeTarjan>(graphtarjan, 0, 1); /// one comp
     ///addNode<GraphTarjan, NodeTarjan>(graphtarjan, 1, 0);
     ///addNode<GraphTarjan, NodeTarjan>(graphtarjan, 1, 2);
     ///addNode<GraphTarjan, NodeTarjan>(graphtarjan, 2, 1);

     cout << "\nTARJAN DEMO ADJ LISTS\n";
     print_adjlists(graphtarjan);
     cout << "\nSCC :\n";
     Tarjan(*graphtarjan, stack2);
     cout << "Index "<<TarjanIndex<<"\n";
     destroyGraph(graphtarjan);
 }

 void perf() {
     int n, i;
     

     // vary the number of edges
     for (n = 1000; n <= 4500; n += 100) {

         Operation op = p.createOperation("dfs-edges", n);

         Graph* graph;
         createGraph<Graph, NodeDFS>(100, graph);// create graph and initialize the nodes of the graph 

         // TODO: generate n random edges
         generate_random_edges(graph, 100, n);

         DFS(*graph, &op);
         destroyGraph(graph);
     }


     // vary the number of vertices
     for (n = 100; n <= 200; n += 10) {
         Operation op = p.createOperation("dfs-vertices", n);

         Graph* graph;
         createGraph<Graph, NodeDFS>(n, graph);// create graph and initialize the nodes of the graph 

         // TODO: generate n random edges
         generate_random_edges(graph, n, 4500);

         DFS(*graph, &op);
         destroyGraph(graph);
     }

     p.showReport();
 }
 
int main() {

    srand(time(NULL));
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    demo();
    //perf();

return 0;
}