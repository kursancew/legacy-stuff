//2009 Kursancew, Vinicius Alexandre
/*
 O ano é 2010. O espetacular resultado de um projeto ultra-secreto, iniciado três anos antes por um grupo de pesquisadores da SBC (Soluções Brasileiras em Cabeamento) está prestes a ser divulgado: a SBC conseguiu a proeza de transportar matéria através de cabos de fibra ótica! A pesquisa contraria a famosa e polêmica frase do ex-senador e atual presidente dos EUA, que na época do início da pesquisa, há três anos, afirmara que "a internet não é como um caminhão de carga, em que você despeja o que quiser; a internet na verdade é uma série de tubos". Com isso, a SBC, que atualmente aluga a sua rede de cabos para uma operadora de TV paga, pensa em mudar de negócio e iniciar-se na atividade de transporte de carga. Apesar de a tecnologia desenvolvida servir também para o transporte de seres vivos, há dificuldades políticas na homologação desse meio de transporte para seres humanos.

A rede de fibra ótica da SBC cobre todas as capitais do país. A rede é composta por ramos de fibra ótica e concentradores. Há um concentrador em cada capital, e um ramo de fibra ótica conecta diretamente um par de concentradores. Nem todo concentrador está conectado diretamente por um ramo de fibra a todos os outros concentradores, mas a rede é conexa. Ou seja, a partir de um dado concentrador existe uma seqüência de ramos e concentradores que permite que uma informação gerada em qualquer um dos concentradores pode ser enviada a qualquer outro concentrador da rede.

Para comunicação de dados, é normal que um ramo de fibra ótica possa ser utilizado para enviar mensagens nos dois sentidos. A tecnologia desenvolvida, no entanto, tem uma peculiaridade: depois que um ramo de fibra ótica é utilizado para transportar matéria em uma direção, a fibra ótica guarda uma memória desse fato, e a partir de então esse ramo somente pode ser utilizado para transportar matéria naquela direção. Concentradores não são afetados por essa memória de direção.

O grupo de pesquisa da SBC é muito bom em física, mas muito fraco em computação. Por isso, você foi contratado para determinar se a rede de fibra ótica existente poderá ser utilizada pela SBC para transportar carga entre qualquer par de capitais, mesmo considerando a restrição de memória de sentido dos ramos de fibra ótica.

Existe, porém, um segundo tipo de fibra ótica descoberta depois da construção da atual rede. Essa fibra não guarda memória da direção do transporte de matéria. Caso a rede existente não possa ser utilizada pela SBC para transportar carga entre qualquer par de capitais, seu dever é indicar os ramos que deverão ser substituídos pela fibra ótica mais nova para que a rede existente possa ser utilizada para esse fim.

Entrada:

A entrada é composta de diversas instâncias. A primeira linha de cada instância contém dois inteiros N e M separados por um espaço em branco, que representam, respectivamente, a quantidade de capitais (2 ≤ N ≤ 1000) e a quantidade de ramos de fibra ótica existentes (1 ≤ M ≤ 50000). As capitais são numeradas de 1 a N. Cada uma das M linhas seguintes contém dois inteiros A e B (1 ≤ A,B ≤ N, A ≠ B) separados por um espaço em branco, indicando que existe um ramo de fibra ligando a capital A à capital B. Note que para comunicação de dados o ramo descrito pode ser utilizado para enviar mensagens tanto de A para B quanto de B para A, mas para transferência de matéria ele poderá ser utilizado em apenas uma direção. Há no máximo um único ramo de fibra ligando um par de capitais.

Saída:

Para cada instância, você deverá imprimir um identificador "Instancia k", onde k é o número da instância atual. Seu programa deve imprimir uma linha, contendo a letra 'S' caso seja possível utilizar a rede existente conforme especificado ou a letra 'N' caso contrário. Se imprimir a letra 'N', seu programa deve imprimir os ramos que devem ser substituídos por fibras óticas sem memória para que seja possível utilizar a rede. Cada ramo deve ser impresso em uma linha no formato "A B" (A < B) na ordem que foram apresentados na entrada. Após cada instância imprima uma linha em branco.
FIX-ME: there is a bug that causes the algorithm to fail in some special cases
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
class node;
class edge {
public:
       edge(node* _adj): vert(_adj), next(0) {}
       node *vert;
       edge* next;
};

int bingo[50000][2];
int nbingo;

inline int min(int a, int b) { return a<b?a:b;}
class node {
public:
       node() { curadj = NULL; d = 0; adj = NULL; vis = 0; pred = NULL; childs = 0;}
       edge* curadj;
       edge* adj;
       int id;
       int vis;
       void addadj(node* n) {
               if(!adj) {
                       adj = new edge(n);
               } else {
                       edge *a = new edge(n);
                       a->next = adj;
                       adj = a;
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
       int low,d;
       node* pred;
       int childs;

       void visit() { vis = 1; }

       void bridges(int& timeg) {
               visit();
               low = d = ++timeg;
               while(nextadj()) {
                       node* v = getadj();
                       if(!v->visited()) {
                               v->pred = this;
                               v->bridges(timeg);
                               low = min(low, v->low);
                               if(pred == NULL) {
                                       ++childs;
                               } else if(v->low >= d) {
                                       bingo[nbingo][0] = id;
                                       bingo[nbingo++][1] = v->id;
                               }
                       } else if (v != pred) {
                               low = min(low, v->d);
                       }
               }
       }
       bool visited() { return vis; }
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

int main()
{
       std::ifstream in("L2Q4.in", std::ios::in);
       std::ofstream out("L2Q4.out", std::ios::out);
       int inst = 1;
       while(1) {
               int n, m;
               in >> n >> m;
               if(in.fail()) break;
               out << "Instancia " << inst++ << std::endl;
               node *graph = new node[n];
               for(int i = 0; i < n; ++i)
                       graph[i].id = i+1;
               for(int i = 0; i < m; ++i) {
                       int u,v;
                       in >> u >> v;
                       graph[u-1].addadj(&graph[v-1]);
                       graph[v-1].addadj(&graph[u-1]);
               }
               int timeg = 0;
               nbingo = 0;
               for(int i = 0; i < n; ++i) {
                       if(!graph[i].visited()) {
                               graph[i].bridges(timeg);
                       }
               }
               if(!nbingo) {
                       out << 'S' << std::endl;
               } else {
                       out << 'N' << std::endl;
                       if(graph[0].childs == 1)
                                               out << graph[0].id << ' ' << graph[0].adj->vert->id << std::endl;
                       for(int i = nbingo-1; i >= 0; --i) {
                               out << bingo[i][0] << ' ' << bingo[i][1] << std::endl;
                       }
               }
               delete [] graph;
               out << std::endl;
       }
       in.close();
       out.close();
       return 0;
}
