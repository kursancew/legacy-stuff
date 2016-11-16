//2009 Kursancew, Vinicius Alexandre
/*
Você, um programador de uma importante software house, foi despedido porque você não conseguiu resolver um problema importante que lhe foi atribuído. Você está muito furioso e quer se vingar do seu chefe, interrompendo a comunicação entre seu computador e o servidor central.

O computador do seu chefe e o servidor central estão na mesma rede, que é composta de muitas máquinas (computadores) e cabos ligando pares dessas máquinas. Existe, no máximo, um cabo entre qualquer par de máquinas e pode haver pares de máquinas sem cabo entre elas.

Para atingir seu objetivo, você pode destruir máquinas e cortar cabos, mas você não pode destruir nem o computador do seu chefe nem o servidor central porque essas máquinas são monitoradas por câmeras de segurança. Assim sendo, dois computadores A  e B podem se comunicar se existe uma seqüência de máquinas não destruídas x1 ,...,xn tal que x1 = A, xn = B e xi está ligado com xi+1 por um cabo não cortado (para cada 1 ≤ i ≤ n-1).

Você tem o custo estimado de destruir cada máquina e o custo de cortar cada cabo na rede. Seu dever é determinar o custo mínimo de interrupção da comunicação entre o computador do seu patrão e o servidor central.

Entrada:

A entrada é composta de várias instâncias. A primeira linha de cada instância contém uma linha que consiste em dois inteiros M e W (2 ≤ M ≤ 100 e 0 ≤ W ≤ 1000), representando respectivamente o número de máquinas e o número de cabos em uma rede. Nas M-2 linhas seguintes, uma por máquina (exceto a máquina do chefe e o servidor central), temos dois inteiros i e c (2 ≤ i ≤ M-1 e 0 ≤ c ≤ 100000), representando o identificador e o custo de destruição da máquina. As W linhas seguintes, uma por cabo, contêm três inteiros j, k e d (1 ≤ j,k ≤ M, 0 ≤ d ≤ 100000), indicando que as máquinas de identificadores j e k estão ligadas por um cabo (bidirecional) cujo custo para cortá-lo é d.

Assuma que a máquina do chefe tem id 1 e o servidor central tem id M. Assuma também que as máquinas têm ids diferentes.

Saída:

Para cada instância, imprima uma linha com um identificador "Instancia k: ", onde k é o número da instância atual, seguida do mínimo custo de interrupção da comunicação entre o computador do seu chefe e o servidor central.
*/
//implementation has mistakes
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

int capacity[200][200];
int flow[200][200];

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
       std::ifstream in("L3Q1.in", std::ios::in);
       std::ofstream out("L3Q1.out", std::ios::out);
       int inst = 1;
       while(1) {
               int m, w;
               in >> m >> w;
               if(in.fail()) break;
               node* graph = new node[m*2-2];
               for(int i = 0; i < 200; ++i) {
                       for(int j = 0; j < 200; ++j) {
                               capacity[i][j] = 0;
                               flow[i][j] = 0;
                       }
               }
               for(int i = 0; i < m-2; ++i) {
                       int id, c;
                       in >> id >> c;
                       graph[(id-1)*2-1].id = (id-1)*2-1;
                       graph[(id-1)*2].id = (id-1)*2;
                       graph[(id-1)*2-1].addadj(&graph[(id-1)*2]);
                       capacity[(id-1)*2-1][(id-1)*2] = c;
               }
               for(int i = 0; i < w; ++i) {
                       int j, k , d;
                       in >> j >> k >> d;
                       graph[(j-1)*2].addadj(&graph[(k-1)*2-1]);
                       capacity[(j-1)*2][(k-1)*2-1] = d;
               }
               graph[0].id = 0;
               graph[(m-1)*2-1].id = (m-1)*2-1;
               out << "Instancia " << inst++ << ": " << max_flow(graph, m*2-2) << std::endl;
               delete [] graph;
       }
       return 0;
}
