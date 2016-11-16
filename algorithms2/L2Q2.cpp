//2009 Kursancew, Vinicius Alexandre
/* DIRECTED GRAPH SHORTEST PATH (DAG) C++
 Um pequeno investidor deve decidir como investir seus fundos para os próximos meses. Uma variedade de investimentos (fundos de mercado monetário, certificados de depósito, limites de poupança, etc) estão disponíveis. Por simplicidade, suponha que os investimentos podem ser feitos e retirados apenas sobre o primeiro dia de cada mês. Então, crie um vértice correspondente ao primeiro dia de cada mês e coloque uma aresta do vértice x para o vértice y para cada investimento que pode ser feito no mês x e retirado no mês y. Note que este grafo é direcionado e acíclico. Deixando o comprimento de cada arco igual a negativa dos lucros obtidos com o investimento correspondente, o melhor plano de investimentos corresponde a um caminho mais curto, ou seja, o caminho com o comprimento total mais negativo.

Sua tarefa é calcular o melhor plano de investimento, observando que o melhor plano de investimento, que pode não começar no primeiro mês ou não terminar no último mês ou ambos.


Entrada:

A entrada é composta de diversas instâncias. A primeira linha de cada instância consiste em dois inteiros n (1 ≤ n ≤ 100) e m (1 ≤ m ≤ 100000), indicando o número de meses e o número de investimentos. Nas m linhas seguintes, temos três inteiros u, v e w (1 ≤ u < v ≤ n e 0 ≤ w ≤ 100), indicando que existe um investimento que começa no mês u e termina no mês v com lucro w.

Saída:

Para cada instância, deve-se imprimir uma linha com a seqüência de meses do melhor plano de investimento, seguida do lucro total (máximo), tudo separado por 1 espaço.

Seu programa só deve imprimir a seqüência de meses se e somente se houver apenas um melhor plano de investimentos, ou seja, houver apenas uma seqüência de meses para o lucro total máximo. Se houver mais de um melhor plano, imprima apenas o lucro total máximo.


*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
class node;
class edge {
public:
       edge(node* _adj, int _w): w(_w), adj(_adj), next(0) {}
       int w;
       node *adj;
       edge* next;
};

class node {
public:
       node() { wt = -400000000; curadj = NULL; adj = NULL; nadj = 0;coming_from = NULL; eq = 0;}
       edge* curadj;
       edge* adj;
       node* coming_from;
       int nadj,eq;
       int wt;
       int id;

       void addadj(node* n, int w) {
               if(!adj) {
                       adj = new edge(n, w);
               } else {
                       edge *a = new edge(n, w);
                       a->next = adj;
                       adj = a;
               }
               ++nadj;
       }
       void printpath(std::ofstream &out) {
               int x[200], p=1, r=eq;
               x[0] = id;
               node* n = coming_from;
               while(n) {
		       x[p++] = n->id; r |= n->eq;
                       n = n->coming_from;
               }
	       for(int i = p-1; i >= 0 && !r; --i) {
                       out << x[i] << ' ';
               }
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
       node* getadj() { return curadj->adj; }
       int getadjw() { return curadj->w; }
       ~node() {
               if(adj)
               do {
                       edge* del = adj;
                       adj = adj->next;
                       delete del;
               } while(adj);
       }
};

int main()
{
       std::ifstream in("L2Q2.in", std::ios::in);
       std::ofstream out("L2Q2.out", std::ios::out);
       while(1) {
               int n, m;
               in >> n >> m;
               if(in.fail()) break;
               node *graph = new node[n];
               bool startpoints[n];
               int scanorder[n];
               for(int i = 0; i<n;++i) {
                       graph[i].id = i+1;
                       startpoints[i] = true;
               }

               for(int i = 0; i < m; ++i) {
                       int u,v,w;
                       in >> u >> v >> w;
                       startpoints[v-1] = false;
                       graph[u-1].addadj(&graph[v-1],w);
               }
               int k = 0;
               for(int i = 0; i < n; ++i) {
                       if(startpoints[i]) {
                               scanorder[k++] = i;
                               graph[i].wt = 0;
                       }
               }
               for(int i = 0; i < n; ++i) {
                       if(!startpoints[i]) {
                               scanorder[k++] = i;
                       }
               }


               ////////////////
/*              for(int s = 0; s < n; ++s) {
                       if(!startpoints[s]) continue;
                       std::cout << s << std::endl;
                       for(int i = s; i < n; ++i)
                               graph[i].reset();
                       graph[s].wt = 0;*/
               int wi=0;
               int ww=0;
               int haseq = 0;
               for(int j = 0; j < n; ++j) {
                       int i = scanorder[j];
                       while(graph[i].nextadj()) {
                               node* adj = graph[i].getadj();
                               int w = graph[i].getadjw();
                               //relax(graph[i], adj, graph[i].getadjw())
                               //if(adj->wt == graph[i].wt + w) haseq = 1;
                               if(adj->wt < graph[i].wt + w) {
                                       adj->wt = graph[i].wt + w;
				       adj->coming_from = &graph[i]; adj->eq = 0;
                               } else if (adj->wt == graph[i].wt + w) adj->eq = 1;
                       }
                       //std::cout << graph[i].wt << ' ';
                       if(ww < graph[i].wt) {
                               ww = graph[i].wt;
			       wi = i; haseq = 0;
                       } else if(ww == graph[i].wt) haseq = 1;
               }
               if(!haseq)
                       graph[wi].printpath(out);
               out << graph[wi].wt << std::endl;
               delete [] graph;
       }
       in.close();
       out.close();
       return 0;
}

