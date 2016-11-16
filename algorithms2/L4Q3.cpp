//2009 Kursancew, Vinicius Alexandre
/* BACKTRACK, BRANCH AND BOUND FOR MASTERMIND (BULLS AND COWS) C++
  Bulls and Cows é um jogo entre dois jogadores, um chamado codemaker e o outro codebreaker. O codemaker escolhe um código secreto, um número decimal de K dígitos, todos diferentes. O codebreaker, então, tenta descobrir o código chutando números de K dígitos. Para cada tentativa, o codemaker responde o número de dígitos de valor correto na posição correta (Bulls) e o número de dígitos corretos em posições incorretas (Cows). Por exemplo, se o código secreto é 1230 e o codebreaker chuta 1205, a resposta seria "2 Bulls e 1 Cow".
 
  Escreva um programa que lê uma série de chutes e respostas para uma partida do jogo e imprime o número de soluções possíveis e, entre todas soluções possíveis, a numericamente menor.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NSYM 10

int kend;
char guess[100][10]; 
char tguess = 0;
int solutions = 0;
char current[11]; //current number being checked for feasability
char first_solution[11];

bool printed;

bool solve(char *b, char *w, int test, int k)
{
      char black[30], white[30]; //constraints
      memcpy(black, b, 30);
      memcpy(white, w, 30);

      if (k == 0) {
              for (int i = 0; i < NSYM; ++i) {
                      solve(black, white, i, k + 1);
              }
              return true;
      }
      for (int i = 0; i < k - 1; ++i)
              if (current[i] == test)
                      return false;
      current[k - 1] = test;
      current[k] = 0;
      for (int j = 0; j < tguess; ++j) {
              if (test == guess[j][k - 1]) {
                      if (black[j] == 0) {
                              return false;
                      } else {
                              black[j] -= 1;
                      }
              } else {
		      int ccount = 0;
		      for (int i = 0; i < kend; ++i) {
			      if (test == guess[j][i]) {
				      ++ccount; break;
			      }
		      } if (ccount) {
                     	 if (white[j] == 0) {
                              return false;
                     	 } else {
                              white[j] -= 1;
                      	}
             	      }
	      }
      }

      if (k == kend) {
              for (int j = 0; j < tguess; ++j)
                      if (black[j] || white[j])
                              return false;
	      if(!printed) {
		      for(int d = 0; d < k; ++d) {
			      first_solution[d] = current[d] + '0';
		      }
		      printed = 1;
	      }
              ++solutions; 
	      return true;
      }
      //gerar novos filhos
      bool hassib = 0;
      for (int i = 0; i < NSYM; ++i) {
              if (solve(black, white, i, k + 1)) {
                      hassib |= 1;
              }
      }
      return hassib;
}


int main()
{
      int k, c;
      FILE *in = fopen("L4Q3.in", "r");
      FILE *out = fopen("L4Q3.out", "w");
      int inst = 1;
      while (fscanf(in, "%d %d", &k, &c) == 2) {
              fprintf(out, "Instancia %d: ", inst++);
              char black[100], white[100];
              kend = k;
              solutions = 0;
              tguess = c;
              for (int n = 0; n < c; ++n) {
                      for (int i = 0; i < k; ++i) {
                              int g;
                              fscanf(in, "%d", &g);
                              guess[n][i] = g;
                      }
                      int b, w;
                      fscanf(in, "%d %d", &b, &w);
                      black[n] = b; white[n] = w;

              }
	      printed = 0;
              solve(black, white, -1, 0);
              fprintf(out, "%s eh uma das %d possiveis solucoes.\n",
                      first_solution, solutions);
	      fflush(out);
      }
      return 0;
}
