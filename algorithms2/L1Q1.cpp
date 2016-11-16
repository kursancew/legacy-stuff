//2009 Kursancew, Vinicius Alexandre
//RADIX SORT C++
#include <cstdio>

class brinquedo {
public:
     char nome[31], cat[31];
     char data[8];
     char dd[3],mm[3],aaaa[5];
};

int main()
{
     FILE* in = fopen("L1Q1.in", "r");
     FILE* out = fopen("L1Q1.out", "w");
     int inp = 1;
     while(1) {
             int n;
             if(fscanf(in, "%d",&n) != 1) break;

             fprintf(out, "Instancia %d\n", inp++);
             brinquedo *table = new brinquedo[n];
             brinquedo *table2 = new brinquedo[n];
             for(int i = 0; i < n; ++i) {
                     fscanf(in, "%s %s %s %s %s", table[i].nome, table[i].cat, table[i].dd, table[i].mm, table[i].aaaa);

                     table[i].data[0] = table[i].dd[1]-'0';
                     table[i].data[1] = table[i].dd[0]-'0';
                     table[i].data[2] = table[i].mm[1]-'0';
                     table[i].data[3] = table[i].mm[0]-'0';
                     table[i].data[4] = table[i].aaaa[3]-'0';
                     table[i].data[5] = table[i].aaaa[2]-'0';
                     table[i].data[6] = table[i].aaaa[1]-'0';
                     table[i].data[7] = table[i].aaaa[0]-'0';
             }
             for(int j = 0; j < 8; ++j) {
                     int count[10] = {0,0,0,0,0,0,0,0,0,0};
                     for(int i = 0; i < n; ++i) {
                             char r = table[i].data[j];
                             ++count[(int)r];
                     }
                     for(int i = 1; i < 10; ++i) {
                             count[i] = count[i-1]+count[i];
                     }
                     for(int i = n-1; i >= 0; --i) {
                             int newi = count[(int)table[i].data[j]];
                             table2[newi-1] = table[i];
                             --count[(int)table[i].data[j]];
                     }
                     brinquedo* tmptab;
                     tmptab = table;
                     table = table2;
                     table2 = tmptab;
             }
             for(int i = 0; i < n; ++i) {
                     fprintf(out, "%s %s %s %s %s\n", table[i].nome, table[i].cat, table[i].dd, table[i].mm, table[i].aaaa);
             }
             fprintf(out, "\n");
//              flush(out);
             delete [] table;
             delete [] table2;
     }
     fclose(out);
}

