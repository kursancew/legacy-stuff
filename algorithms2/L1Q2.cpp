//2009 Kursancew, Vinicius Alexandre
//LINEAR SELECTION C++
//Ordenacao por selecao
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
bool compnota;
class candidato {
public:
      std::string nome;
      double nota;
      bool operator<=(candidato& c) {
              if(compnota) {
                      return nota <= c.nota;
              } else {
                      return nome <= c.nome;
              }
      }
};
int cmp(const void* c1, const void* c2) {
      if(((candidato*)c1)->nome < ((candidato*)c2)->nome) return -1;
      if(((candidato*)c1)->nome > ((candidato*)c2)->nome) return 1;
      return 0;
}
inline int part(candidato* A, int l, int r) {
      candidato x = A[l], t;
      int i = l;
      for(int j = l+1; j <= r; ++j) {
              if(A[j] <= x) {
                      ++i;
                      t = A[i];
                      A[i] = A[j];
                      A[j] = t;
              }
      }
      t = A[i];
      A[i] = A[l];
      A[l] = t;
      return i;
}

inline int randpart(candidato* A, int l, int r) {
      int i = l+rand()%(r-l+1);
      candidato t = A[i];
      A[i] = A[l];
      A[l] = t;
      return part(A, l, r);
}

candidato select(candidato* A, int l, int r, int i) {
      if(l == r) {
              return A[l];
      }
      int q = randpart(A, l, r);
      int k = q - l;
      if(i == k) {
              return A[q];
      } else if (i < k) {
              return select(A, l, q-1, i);
      } else {
              return select(A, q+1, r, i-k-1);
      }
}

int main()
{
      std::ifstream in("L1Q2.in", std::ios::in);
      std::ofstream out("L1Q2.out", std::ios::out);
      int inp = 1;
      while(1) {
              int n,m;
              in >> n >> m;
              if(in.fail()) break;
              out << "Instancia " << inp << std::endl;
              candidato* table = new candidato[n];
              compnota = 1;
              for(int i = 0; i < n; ++i) {
                      in >> table[i].nome >> table[i].nota;
              };
              candidato* passa = &table[n-m];
              candidato c = select(table, 0, n-1, n-m);
              qsort(passa, m, sizeof(candidato), cmp);
              for(int i = 0; i < m ; ++i) {
                              candidato c = passa[i];
                              out << c.nome << ' ' << std::fixed << std::setprecision(4)
                                      << c.nota << std::endl;
              }
              ++inp;
              delete [] table;
              out<< std::endl;
      }
      return 0;
}

