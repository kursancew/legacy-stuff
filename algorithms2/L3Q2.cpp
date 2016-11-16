//2009 Kursancew, Vinicius Alexandre
/* 
GRAPH PERFECT MATCHING EDMONDS-KARP FLOW ADAPTATION C++
  A Polícia Montada Real Canadense (Royal Canadian Mounted Police) é uma instituição muito famosa, cujas origens remontam ao século XIX. Sua tarefa é levar a lei aos locais mais longínquos do país continental. Hoje a polícia montada tem um efetivo de 25000 homens e cerca de 5000 cavalos.

Cada sede da RCMP tem um haras em que os animais são muito bem cuidados, e designados aos policiais com quem têm mais afinidade. Esta afinidade é inferida em observações dos oficiais com vários anos de experiência, observando os policiais montando os animais disponíveis. No Fairmont Banff Springs Haras, onde ficam os cavalos montados pelos policiais da região de Banff Springs, é necessário resolver o problema de decidir quais soldados montarão quais cavalos. Note que um cavalo pode ser montado por vários policiais, mas um policial só monta um determinado cavalo. Cada cavalo tem um limite de policiais que podem montá-lo. Ou seja, de posse da afinidade dos vários policiais com os animais que montou nos últimos tempos, deseja-se encontrar uma atribuição dos cavalos aos vários policiais, de tal forma que o maior número possível de policiais tenham um cavalo para montar. 

Entrada:

A entrada é composta de diversas instâncias. A primeira linha de cada instância consiste em três inteiros n (1 ≤ n ≤ 100), m (1 ≤ m ≤ 100) e k (1 ≤ k ≤ 1000) indicando o número de cavalos, o número de soldados e o número de afinidades. A linha seguinte contêm n inteiros c1,c2,..,cn indicando que no i-ésimo cavalo pode montar ci (1 ≤ ci ≤ 100) soldados. Nas k linhas seguintes, temos dois inteiros u (1 ≤ u ≤ n) e v (1 ≤ v ≤ m) indicando que existe afinidade entre o cavalo u e o soldado v.

Saída:

Para cada instância, você deverá imprimir uma linha com um identificador "Instancia k: ", onde k é o número da instância atual, seguida do número máximo de policiais que podem ter um cavalo para montar em uma atribuição.
*/
#include <fstream>
#include <string>
#include <iostream>
inline int min(int a, int b) { return a <b?a:b;}
template <class T> class queue {
       class elmt {
       public:
               elmt(T& t) { thing = t; next = NULL;}
               T thing;
               elmt* next;
       };
       elmt* head;
       elmt* tail;
public:
       bool empty() { return head == NULL; }
       T pop() {
               T ret = head->thing;
               elmt* tmp = head;
               if(head != tail) {
                       head = head->next;
               } else {
                       head = tail = NULL;
               }
               delete tmp;
               return ret;
       }
       void push(T thing) {
               elmt* tmp = new elmt(thing);
               if(head) {
                       tail->next = tmp;
                       tail = tmp;
               } else {
                       head = tail = tmp;
               }
       }
       queue() { head = NULL; }
};

int capacity[300][300];

class node;
class edge {
public:
       edge(node* _adj): vert(_adj), next(0) {}
       node* vert;
       edge* next;
};

class node {
public:
       node() { nadj = 0; curadj = NULL; adj = NULL;}
       edge* curadj;
       edge* adj;
       int nadj;
       int id;
       void print() {
               std::cout << id << ": ";
               while(nextadj()) {
                       std::cout << getadj()->id << " ";
               }
               std::cout << std::endl;
       }
       void rew() { curadj=NULL; }
       void addadj(node* n) {
               if(!adj) {
                       adj = new edge(n);
               } else {
                       edge *a = new edge(n);
                       a->next = adj;
                       adj = a;
               }
               ++nadj;
       }
       bool nextadj() {
               if(!adj) return false;
               if(!curadj) {
                       curadj = adj;
                       return curadj != NULL;
               } else {
                       curadj = curadj->next;
                       return curadj != NULL;
               }
       }

       node* getadj() { return curadj->vert; }

       ~node() {
               if(adj)
                       do {
                               edge* del = adj;
                               adj = adj->next;
                               delete del;
                       } while(adj);
       }
};

int bfs(node* graph, int m)
{
       bool visit[m];
       node* from[m];
       for(int i = 0; i < m; ++i) {
               visit[i] = false;
               from[i] = NULL;
               graph[i].rew();
       }
       queue<int> q;
       q.push(0);
       visit[0] = true;
       bool sink_reached = false;
       while(!q.empty() && !sink_reached) {
               node* where = &graph[q.pop()];

               while(where->nextadj() && !sink_reached) { //for each vertex n adjacent to where
                       node* n = where->getadj();
                       if(!visit[n->id] && capacity[where->id][n->id] > 0) {
                               q.push(n->id);
                               visit[n->id] = true;
                               from[n->id] = where;
                               if(n == &graph[m-1])
                                       sink_reached = true;
                       }
               }
       }
       node* where = &graph[m-1];
       int path_cap = 100000000;
       while (from[where->id]) {
               node* prev = from[where->id];
               path_cap = min(path_cap, capacity[prev->id][where->id]);
               where = prev;
       }
       where = &graph[m-1];
       while(from[where->id]) {
               node* prev = from[where->id];
               capacity[prev->id][where->id] -= path_cap;
               capacity[where->id][prev->id] += path_cap;
               where = prev;
       }

       if(path_cap == 100000000) {
               return 0;
       } else {
               return path_cap;
       }
}

int max_flow(node* graph, int m) {
       int res = 0;
       while(1) {
               int path_cap = bfs(graph, m);
               if(!path_cap)break;
               res += path_cap;
       }
       return res;
}

int main()
{
       std::ifstream in("L3Q2.in", std::ios::in);
       std::ofstream out("L3Q2.out", std::ios::out);
       int inst = 1;
       while(1) {
               int n, m, k;
               in >> n >> m >> k;
               if(in.fail()) break;
               node* graph = new node[n+m+2];
               graph[0].id = 0;
               graph[n+m+2-1].id = m+n+2-1;
               for(int i = 0; i < 300; ++i) {
                       for(int j = 0; j < 300; ++j) {
                               capacity[i][j] = 0;
                       }
               }
               for(int i = 0; i < n; ++i) {
                       int mon;
                       in >> mon;
                       graph[1+i].id = 1+i;

                       //cria aresta pro sink
                       graph[1+i].addadj(&graph[1+m+n]);
                       capacity[1+i][m+n+1] = mon;
               }
               for(int i = 0; i < m; ++i) {
                       graph[1+n+i].id = 1+n+i;

                       //aresta do source para os guardas
                       graph[0].addadj(&graph[1+n+i]);
                       capacity[0][1+n+i] = 1;
               }
               for(int i = 0; i < k; ++i) {
                       int u, v;
                       in >> u >> v;
                       --v;--u;
                       graph[1+n+v].addadj(&graph[1+u]);
                       capacity[1+n+v][1+u] = 100;
               }
               out << "Instancia " << inst++ << ": " << max_flow(graph, 1+m+n+1) << std::endl;
               delete [] graph;
       }
       return 0;
}
