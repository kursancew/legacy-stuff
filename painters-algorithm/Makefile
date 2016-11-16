CXX=g++
CFLAGS=-g -O0 -Wall
LDADD=-lsdl -lm

BASETBOBJS=basetb.o vetor.o ponto.o matriz.o camera.o objeto3d.o triangulo.o cor.o cena.o primitives.o parametros.o
OBJS=main.o vetor.o ponto.o matriz.o camera.o objeto3d.o triangulo.o cor.o primitives.o cena.o

all: pg

parametros.o: parametros.cpp parametros.h
	$(CXX) $(CFLAGS) -c -oparametros.o parametros.cpp

basetb.o: basetb.cpp
	$(CXX) $(CFLAGS) -c -obasetb.o basetb.cpp
vetor.o: vetor.cpp vetor.h
	$(CXX) $(CFLAGS) -c -ovetor.o vetor.cpp
ponto.o: ponto.cpp ponto.h
	$(CXX) $(CFLAGS) -c -oponto.o ponto.cpp
matriz.o: matriz.cpp matriz.h
	$(CXX) $(CFLAGS) -c -omatriz.o matriz.cpp
triangulo.o: triangulo.cpp triangulo.h
	$(CXX) $(CFLAGS) -c -otriangulo.o triangulo.cpp
camera.o: camera.cpp camera.h
	$(CXX) $(CFLAGS) -c -ocamera.o camera.cpp
objeto3d.o: objeto3d.cpp objeto3d.h
	$(CXX) $(CFLAGS) -c -oobjeto3d.o objeto3d.cpp
cor.o: cor.cpp cor.h
	$(CXX) $(CFLAGS) -c -ocor.o cor.cpp
cena.o: cena.cpp cena.h
	$(CXX) $(CFLAGS) -c -ocena.o cena.cpp
main.o: main.cpp
	$(CXX) $(CFLAGS) -c -omain.o main.cpp

basetb: $(BASETBOBJS)
	$(CXX) -obasetb $(BASETBOBJS) -lSDL_gfx -lSDL -lX11 -lm
main: $(OBJS)
	$(CXX) -O0 -g -omain $(OBJS) -lSDL_gfx -lSDL -lX11 -lm

primitives.o: primitives.cpp primitives.h
	$(CXX) $(CFLAGS) -c -oprimitives.o primitives.cpp


clean:
	rm -Rf *.o basetb
