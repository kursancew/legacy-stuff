//2009 Kursancew, Vinicius Alexandre
/* ALL PAIRS SHORTEST PATH C++ FLOYD-WARSHALL
Preocupado com a atual situação de crise no transporte aéreo, o presidente da CBF já iniciou seus preparativos para fazer as reservas das passagens aéreas para as finais mundiais em 2010. O primeiro passo foi estudar a malha aérea disponível, em que cada vôo tem um certo preço e liga duas cidades (estamos, na verdade, chamando de vôo apenas um trecho non stop de um vôo comercial). O objetivo do presidente é fazer várias consultas nesta malha de vôos.

Em geral desejamos fazer vôos sem escalas, mas estes podem ser muito caros. Para contornar este fato o presidente deseja permitir algumas escalas possíveis. Assim, ele ordenou as várias cidades da malha em sua ordem de preferência para fazer escala. Ou seja, a cidade de índice 1 é a que ele prefere fazer escala, seguida pela cidade 2, e assim por diante.

As consultas que o diretor fará são, então do seguinte tipo: são dados as cidades de partida e de chegada e um número t de cidades em que o diretor permite que sejam feitas escalas. Seu programa deverá encontrar o custo de um vôo de custo mínimo entre as cidades que faça, no máximo, t escalas. Por exemplo, se t = 1 você deverá encontrar o custo de um vôo de custo mínimo entre as duas cidades que seja, ou non stop ou que faça uma escala na primeira cidade.
Entrada:

A entrada é composta de diversas instâncias. A primeira linha de cada instância consiste em dois inteiros n (1 ≤ n ≤ 100) e m (1 ≤ m ≤ 100000), indicando o número de cidades e o número de escalas. Nas m linhas seguintes temos três inteiros u, v e w (1 ≤ u,v ≤ n e 0 ≤ w ≤ 100) indicando que existe uma escala que vai de u para v com custo w. Em seguida um inteiro c (1 ≤ c ≤ 10000) indicando o número de consultas, e nas c linhas seguintes temos três inteiros o, d e t (1 ≤ o,d ≤ n e 0 ≤ t ≤ n) onde o é a cidade de origem, d é a cidade de destino e t indica que as cidades 1,2,..,t podem ser usadas para escalas.

Saída:

Para cada instância, você deverá imprimir um identificador "Instancia k", onde k é o número da instância atual. Para cada consulta, na ordem da entrada, você deve imprimir o custo mínimo ou -1 caso não exista caminho entre as duas cidades. Após cada instância imprima uma linha em branco. 
*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
int W[100][100][100];
int P[100][100][100];
int R[100][100];
int S[100][100];
int O[100][100];
inline int min(int a, int b) { return a<b?a:b;}
int main()
{
      std::ifstream in("L2Q3.in", std::ios::in);
      std::ofstream out("L2Q3.out", std::ios::out);
      int inst = 1;
      while(1) {
              int n, m;
              in >> n >> m;
              if(in.fail()) break;
              out << "Instancia " << inst++ << std::endl;
              for(int i = 0; i < n; ++i) {
                      for(int j = 0; j < n; ++j) {
                              S[i][j] = -1;
                              if(i == j) {
                                      O[i][j] = R[i][j] = W[0][i][j] = 0;
                              } else {
                                      O[i][j] = R[i][j] = W[0][i][j] = 4000000;
                              }
                      }
              }
              for(int i = 0; i < m; ++i) {
                      int u,v,w;
                      in >> u >> v >> w;
                      if(W[0][u-1][v-1] > w) {
                              O[u-1][v-1] = R[u-1][v-1] = W[0][u-1][v-1] = w;
                              S[u-1][v-1] = u-1;
                      }
              }
              int i,j,k;
              for(i= 0 ; i <n ; i++)
              {
                      for( j = 0; j < n; j++)
                      {
                              if( i != j ) // skip over the current row
                              {
                                      for( k = 0; k < n; k++)
                                      {
                                              if( k != i ) // skip over the current column of iteration
                                              {
                                                      int t = min ( R[j][k], R[j][i] + R[i][k]);

                                                      if(R[j][k] > R[j][i] + R[i][k])
                                                              S[j][k] = S[i][k];
                                                      R[j][k] = t;
                                              }
                                      }
                              }

                      }
                      for(int a = 0; a < n; ++ a) {
                              for(int b = 0; b < n; ++b) {
                                      W[i][a][b] = R[a][b];
                                      P[i][a][b] = S[a][b];
                              }
                      }
              }

              /*for(int k = 0; k < n; ++k) {
                      for(int i = 0; i < n; ++i) {
                              for(int j = 0; j < n;++j) {
                                      int p = P[k][i][j];
                              }
                      }
              }*/
              int cons;
              in >> cons;
              for(int i = 0; i < cons; ++i) {
                      int a,b,c;
                      in >> a >> b >> c;
                      --a;--b;
                      /*if(a == b) {
                               out << 0 << std::endl;
                               continue;
                      }*/
                      //if(c >= n) c = n-1;
                      if(c == 0) {
                              if(O[a][b] < 4000000) out << O[a][b] << std::endl;
                              else out << -1 << std::endl;
                      } else {
                              if(W[c-1][a][b] < 4000000) out << W[c-1][a][b] << std::endl;
                              else out << -1 << std::endl;
                      }


              }
              out << std::endl;
      }
      in.close();
      out.close();
      return 0;
}

