//2009 Kursancew, Vinicius Alexandre
//BIPARTITE GRAPH MATCHING HOPCROFT-KARP C++ IMPLEMENTATION
//implmentacao c++ do hopcroft-karp para grafos bipartidos
/*
 Hallan é um monitor de algoritmos muito inteligente, ele está freqüentemente resolvendo e criando puzzles. No seu novo desafio, ele está tentando criar algo novo com dominós. Ele irá desenhar uma figura em um grid retangular consistindo de quadrados 1x1, e irá marcar alguns desses quadrados, depois o objetivo é tentar cobrir os quadrados marcados com dominós. Como você provavelmente sabe, dominós são peças de tamanho 2x1. Por simplicidade, nós vamos assumir que os dominós podem apenas ser colocados na horizontal e na vertical e que ele tem uma quantidade ilimitada de dominós. A cobertura tem de ser perfeita, significa que os dominós devem cobrir apenas as posições marcadas e os dominós devem cobrir todas elas, e também todos os dominós devem estar estritamente dentro do grid retangular.

Agora sua tarefa é esta: Dada uma figura marcada com quadrados num grid retangular, você deve resolver o novo puzzle criado por Hallan, determinando se é possível cobrir exatamente a figura com dominós.

Entrada:

A entrada consiste de algumas instâncias. A primeira linha de cada instância consiste de dois inteiros positivos R e C (1 ≤ R,C ≤ 60), o número de linhas e colunas no grid, respectivamente. Nas R linhas seguintes, há a atual configuração de um grid de pontos '.' e shaps '#'. Cada caracter representa um quadrado no grid retangular. Um ponto '.' representa um quadrado não marcado e um '#' representa um quadrado marcado.

Saída:

Para cada instância, você deverá imprimir uma linha com um identificador "Instancia k: ", onde k é o número da instância atual, seguida de uma das seguintes mensagens: Se é impossível cobrir os quadrados marcados com dominós, imprima a palavra "Impossivel" seguida do maior número de dominós que podem ser colocados no grid. Se é possível, imprima uma simples palavra "Possivel". Veja os exemplos para mais detalhes.
*/
#include <stdio.h>
#include <queue>
#include <string.h>
#include <stdlib.h>

template < class T > class queue {
       class elmt {
             public:
               elmt(T & t) {
                       thing = t;
                       next = NULL;
               } T thing;
               elmt *next;
       };
       elmt *head;
       elmt *tail;
     public:
       bool empty() {
               return head == NULL;
       }
       T pop() {
               T ret = head->thing;
               elmt *tmp = head;
               if (head != tail) {
                       head = head->next;
               } else {
                       head = tail = NULL;
               }
               delete tmp;
               return ret;
       }
       void push(T thing) {
               elmt *tmp = new elmt(thing);
               if (head) {
                       tail->next = tmp;
                       tail = tmp;
               } else {
                       head = tail = tmp;
               }
       }
       queue() {
               head = NULL;
       }
       ~queue() {
               while (!empty())
                       pop();
       }
};
class node;
class edge {
     public:
       edge(node * n):adj(n) {
               next = NULL;
       } node *adj;
       edge *next;
};

class path {
     public:
       path() {
               n1 = n2 = 0;
       } path(node * x1, node * x2):n1(x1), n2(x2) {
       }
       node *n1;
       node *n2;
};

class node {
     public:
       node() {
               curadj = NULL;
               adj = NULL;
               nadj = 0;
               matching = NULL;
               ab = 0;
       } edge *curadj;
       edge *adj;
       int nadj;
       int id;
       int val;
       node *matching;
       int ab;
       int layer;
       int visited;
       void rew() {
               curadj = NULL;
       }
       void addadj(node * n) {
               if (!adj) {
                       adj = new edge(n);
               } else {
                       edge *a = new edge(n);
                       a->next = adj;
                       adj = a;
               }
               ++nadj;
       }

       bool nextadj() {
               if (!adj)
                       return false;
               if (!curadj) {
                       curadj = adj;
                       return curadj != NULL;
               } else {
                       curadj = curadj->next;
                       return curadj != NULL;
               }
       }
       node *getadj() {
               return curadj->adj;
       }
       ~node() {
               if (adj)
                       do {
                               edge *del = adj;
                               adj = adj->next;
                               delete del;
                       } while (adj);
       }
};


int grid[60][60];

std::queue < path > gap(node * p)
{
       if (p->visited)
               return std::queue < path > ();
       if(p->layer == 0)
               printf("asd\n");
       p->visited = 1;
       edge *e = p->adj;
       while (e) {
               node *q = e->adj;
               if (p->layer - 1 == q->layer) {
                       if (q->layer == 0 && !q->matching) {
                               std::queue < path > P;
                               P.push(path(q, p));
                               return P;
                       }
                       std::queue < path > P;
                       P = gap(q->matching);
                       if (!P.empty()) {
                               P.push(path(q, p));
                               return P;
                       }
               }
               e = e->next;
       }
       return std::queue < path > ();
}

int main()
{
       int inst = 1;
       FILE *in = fopen("L4Q2.in", "r");
       FILE *out = fopen("L4Q2.out", "w");
       while (1) {
               int r, c, sharp = 0;
               if (fscanf(in, "%d %d", &r, &c) != 2) {
                       break;
               }
               node *graph = new node[r * c];
               int k = 0;
               for (int i = 0; i < r; ++i) {
                       char col[c + 1];
                       fscanf(in, "%s", col);
                       for (int j = 0; j < c; ++j) {
                               graph[k].id = k;
                               graph[k].val = col[j];
                               if (i % 2 == j % 2)
                                       graph[k].ab = 'a';
                               else
                                       graph[k].ab = 'b';
                               if (col[j] == '#') {
                                       ++sharp;
                                       if (i != 0 && graph[(i - 1) * c + j].val == '#') {
                                               graph[k].addadj(&graph[(i - 1) * c + j]);
                                               graph[(i - 1) * c + j].addadj(&graph[k]);
                                       }
                                       if (j != 0 && graph[i * c + j - 1].val == '#') {
                                               graph[k].addadj(&graph[i * c + j - 1]);
                                               graph[i * c + j - 1].addadj(&graph[k]);
                                       }
                               } else { graph[k].ab = 'x'; }
                               ++k;
                       }
               }
               int n = r * c;
               int M = 0;
               queue < node * >Mq;
               while (1) {
                       queue < node * >F;
                       queue < node * >Q;
                       for (int i = 0; i < n; ++i) {
                               graph[i].layer = -1;
                               if (!graph[i].matching && graph[i].ab == 'b') {
                                       Q.push(&graph[i]);
                                       graph[i].layer = 0;
                               }
                       }
                       //find paths
                       bool lll = 0;
                       while (!Q.empty()) {
                               queue < node * >Ql;
                               while (!Q.empty()) {
                                       node *q = Q.pop();
                                       while (q->nextadj()) {
                                               node *p = q->getadj();
                                               if (p->layer == -1) {
                                                       p->layer = q->layer + 1;
                                                       if (!p->matching) {
                                                               F.push(p);
                                                       } else
                                                               Ql.push(p);
                                               }
                                       }
                               }
                               if (!F.empty()) {
                                       lll = 1;
                                       break;
                               }
                               while (!Ql.empty()) {
                                       node *q = Ql.pop();
                                       node *p = q->matching;
                                       p->layer = q->layer + 1;
                                       Q.push(p);
                               }

                       }
                       if (!lll) F = queue < node * >();
                       for (int i = 0; i < n; ++i) {
                         printf("(%d %d) %d", graph[i].id / c, graph[i].id % c, graph[i].layer);
                                    printf("\n");
                            }

                       if (F.empty()) {
                               M = 0;
                               for (int i = 0; i < n; ++i) {
                                       if (graph[i].ab == 'a' && graph[i].matching) {
                                               ++M;
                                               Mq.push(&graph[i]);
                                       }
                               }
                               break;
                       }
                       //augment
                       for (int i = 0; i < n; ++i) {
                               graph[i].visited = 0;
                       }
                       while (!F.empty()) {
                               node *q = F.pop();
                               std::queue < path > P = gap(q);
                               bool odd = true;
                               while (!P.empty()) {
                                       path pt = P.front();
                                       P.pop();
                                       if (odd) {
                                               pt.n1->matching = pt.n2;
                                               pt.n2->matching = pt.n1;
                                       } else if (q->matching) {
                                               //q->matching->matching = 0;
                                               //q->matching = 0;
                                       }
                                       odd = !odd;
                               }
                       }
               }
               if(M >= sharp/2) {
                       fprintf(out, "Instancia %d: Possivel\n", inst++);
               } else {
                       fprintf(out, "Instancia %d: Impossivel %d\n", inst++, M);
               }
               fflush(out);
               delete[]graph;
       }

       return 0;
}

