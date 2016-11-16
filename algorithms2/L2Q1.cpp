//2009 Kursancew, Vinicius Alexandre
/*
1.) Uma fábrica precisa entregar um número de remessas de seu produto. Essa fábrica consegue produzir apenas uma remessa de seu produto por dia. E, por questões de política da fábrica, quando uma remessa começa ser produzida, a fabricação da próxima só começará quando a da outra terminar. Então, cada remessa é agendada por dia. Entretanto, cada remessa tem um prazo para ser entregue ao seu cliente, e caso seja entregue depois desse prazo, a fábrica deve pagar uma multa que não depende do número de dias de atraso.

Seu dever é programar as remessas, ou seja, estabelecer uma bijeção entre as remessas e os dias de trabalho, de modo a minimizar a multa total. Escreva um algoritmo guloso para resolver o problema.

Entrada:

A entrada é composta de diversas instâncias. A primeira linha de cada instância consiste em um inteiro n (1 ≤ n ≤ 100000), indicando o número de remessas. Nas n linhas seguintes, temos dois inteiros d e w (d ≥ 1, w ≥ 0), indicando que existe uma remessa com prazo de d dias e multa w.

Saída:

Para cada instância, deve-se imprimir uma linha com a multa total (mínima) da programação.
*/
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

template <class T, int N> class heap {
       T theHeap[N];
       int     mSize;
       bool(*compare)(T&, T&);
public:
       heap() { mSize = 0;     }
       void add(T);
       T pop();
       T top() { return theHeap[0]; }
       int     size() { return mSize; }
       void heapfy();
       void heapfy(int i);
       void set_compare(bool(*c)( T&, T&)) { compare = c; }
};

template <class T, int N> void heap<T,N>::heapfy()
{
       for(int i = mSize/2; i >= 1; --i) {
               heapfy(i);
       }
}

template <class T, int N> void heap<T,N>::heapfy(int i)
{
       while ( i <= mSize/2) {
               int maior = 2*i;
               if( i*2+1 <= mSize) {
                       if(compare(theHeap[2*i-1], theHeap[2*i]))
                               maior = 2*i + 1;
               }
               if( compare(theHeap[i-1], theHeap[maior-1])) {
                       T temp = theHeap[i-1];
                       theHeap[i-1] = theHeap[maior-1];
                       theHeap[maior-1] = temp;
                       i = maior;
               } else i = mSize;
       }
}

template <class T, int N> void heap<T,N>::add(T d)
{
       theHeap[mSize++] = d;
}

template <class T, int N> T     heap<T,N>::pop()
{
       T tmp = theHeap[0];
       --mSize;
       theHeap[0] = theHeap[mSize];
       if(mSize == 1) return tmp;
       heapfy(1);
       return tmp;
}

class pack {
public: int d, m;
};

int cmp(const void* a, const void* b) {
       return ((pack*)b)->d - ((pack*)a)->d;
}

inline bool cmpm(pack& a, pack& b) {
       return a.m < b.m;
}

int main()
{
       std::ifstream in("L2Q1.in", std::ios::in);
       std::ofstream out("L2Q1.out", std::ios::out);
       while(1) {
               int n, k = 0, multa = 0;
               in >> n;
               if(in.fail()) break;
               pack *packs = new pack[n];
               for(int i = 0; i < n; ++i) {
                       in >> packs[i].d >> packs[i].m;
               }
               qsort(packs, n, sizeof(pack), cmp);
               heap<pack, 100000> h;
               h.set_compare(cmpm);
               int final = packs[0].d;
               while(k != n) {
                       while(packs[k].d == final) {
                               h.add(packs[k++]);
                       }
                       int nextday = 0;
                       if(k != n) {
                               nextday = packs[k].d;
                       }
                       h.heapfy();
                       for(int i = 0; i < final - nextday; ++i) {
                               if(h.size()) h.pop();
                       }
                       final = nextday;
               }
               while(h.size())
                       multa += h.pop().m;

               out << multa << std::endl;
               delete [] packs;
       }
       in.close();
       out.close();
       return 0;
}
