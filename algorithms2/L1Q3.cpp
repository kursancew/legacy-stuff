//2009 Kursancew, Vinicius Alexandre
//RABIN-KARP STRING MATCH C++
//Two dimentional rabin-karp implementation
#include <fstream>
#include <iostream>
#include <string>
long long hashrem;

inline long long comp(int **str, int **pat, int oi, int oj, int s) {
      for(int i = 0; i < s; ++i) {
              for(int j = 0; j < s;++j) {
                      if(str[i+oi][j+oj] != pat[i][j])
                              return 0;
              }
      }
      return 1;
}

inline long long prehash(int **pat, int s) {
      long long h = 0;
      hashrem = 0;
      for(int i = 0; i < s;++i) {
              for(int j = 0; j < s; ++j) {
                      h <<= 1;
                      hashrem <<= 1;
                      if(i == 0) {
                              hashrem |= 1;
                      }
                      h |= pat[i][j];
              }
      }
      hashrem = ~hashrem;
      return h;
}

inline long long prehash2(int **str, int oi, int oj, int s) {
      long long h = 0;
      for(int i = oi; i < s+oi;++i) {
              for(int j = oj; j < s+oj; ++j) {
                      h <<= 1;
                      h |= str[i][j];
              }
      }
      return h;
}

inline long long hash(int **str, int oi, int oj, int s, long long h) {
      h &= hashrem;
      for(int j = oj; j < s+oj; ++j) {
              h <<= 1;
              h |= str[oi+s-1][j];
      }
      return h;
}

int main()
{
      std::ifstream in("L1Q3.in", std::ios::in);
      std::ofstream out("L1Q3.out", std::ios::out);
      int conj = 1;
      while(1) {
              int n,m,oc=0;
              in >> n >> m;
              if(in.fail()) break;
              out << "Instancia " << conj++ << std::endl;
              int **str = new int*[n];
              for (int i = 0; i < n; ++i) {
                      str[i] = new int[n];
                      std::string s;
                      in >> s;
                      for (int j = 0; j < n; ++j) {
                              str[i][j] = s[j] =='|'?1:0;
                      }
              }
              int **pat = new int*[m];
              for (int i = 0; i < m; ++i) {
                      pat[i] = new int[m];
                      std::string s;
                      in >> s;
                      for (int j = 0; j < m; ++j) {
                              pat[i][j] = (s[j] =='|')?1:0;
                      }
              }

              long long phash = prehash(pat, m);
              for(int j = 0; j < n-m+1; ++j) {
                      long long nh = prehash2(str, 0, j,m );
                      if(nh == phash && comp(str,pat,0,j,m)) {
                              //std::cout << "match " << i << " " << j << std::endl;
                              out << 0 << ' ' << j << std::endl;
                      }
                      for(int i = 1; i < n-m+1; ++i) {
                              nh = hash(str, i, j, m, nh);
                              //std::cout << nh << std::endl;
                              if(nh == phash && comp(str,pat,i,j,m)) {
                                      //std::cout << "match " << i << " " << j << std::endl;
                                      out << i << ' ' << j << std::endl;oc=1;
                              }
                      }
              }
              oc?out << std::endl : out << "nenhuma ocorrencia\n" << std::endl;
      }
      return 0;
}

