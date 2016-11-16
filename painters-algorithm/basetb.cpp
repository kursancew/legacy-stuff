#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include "vetor.h"
#include "ponto.h"
#include "matriz.h"
#include "objeto3d.h"
#include "camera.h"
#include "cena.h"
#include "primitives.h"
#include "parametros.h"

void do_help()
{
	Primitives::write(5,10, "Ajuda");
	Primitives::write(15,25, "1 - pos. camera");
	Primitives::write(15,35, "2 - pos. luz");
	Primitives::write(15,45, "3 - I_l (cor da luz)");
	Primitives::write(15,55, "4 - K_d (constante difusa - tripla 0~1)");
	Primitives::write(15,65, "5 - O_d (tripla 0~1)");
	Primitives::write(15,75, "6 - K_a (constante ambiental 0~1)");
	Primitives::write(15,85, "7 - K_s (constante especular 0~1)");
	Primitives::write(15,95, "8 - ETA ( >= 0)");
	Primitives::write(15,105, "9 - N");
	Primitives::write(15,115, "0 - V");
	Primitives::write(15,125, "Menos - d. camera-plano");
	Primitives::write(15,135, "Igual - I_a");
	Primitives::write(15,145, "i - H_x");
	Primitives::write(15,155, "o - H_y");
	Primitives::write(15,165, "Z,X,C - ajusta o incremento");
	Primitives::write(15,175, "A/W - incrementa/decrementa indice 0 do parametro");
	Primitives::write(15,185, "W/S - incrementa/decrementa indice 1 do parametro");
	Primitives::write(15,195, "E/D - incrementa/decrementa indice 2 do parametro");
	Primitives::write(15,205, "R/T - rotaciona x");
	Primitives::write(15,215, "Y/U - rotaciona y");
	Primitives::write(15,225, "F/G - rotaciona z");
	Primitives::write(15,235, "P - proximo objeto");
	Primitives::write(15,245, "J - fecha ajuda");
	Primitives::flip();
}

int main()
{
	std::cout << "-=-=-=-= vetores =-=-=-=-" << std::endl;
	Vetor v1(1, -3, 0);
	Vetor v2(3, 1, 4);
	Vetor v3(-2, 0, 1);
	std::cout << "v1 = ";
	v1.print();
	std::cout << "v2 = ";
	v2.print();
	std::cout << "v3 = ";
	v3.print();

	std::cout << "<v1, v2> = " << v1.pi(v2) << std::endl;
	std::cout << "v1 X v2 = ";
	v1.pv(v2).print();
	
	std::cout << "v3 _|_ v2 _|_ v1: " << std::endl;
	v1.orto(v2, v3);
	v1.print();
	v2.print();
	v3.print();
	std::cout << "v1 Modulo: " << v2.modulo() << " <v2,v3>: " << v2.pi(v3) << std::endl;
	std::cout << "v2 Modulo: " << v2.modulo() << " <v1,v2>: " << v2.pi(v1) << std::endl;
	std::cout << "v3 Modulo: " << v3.modulo() << " <v1,v3>: " << v3.pi(v1) << std::endl;
	
	std::cout << "v1 Normalizado: ";
	v1.normalizar();
	std::cout << v1.modulo() << " ";
	v1.print();
	std::cout << "v2 Normalizado: ";
	v2.normalizar();
	std::cout << v2.modulo() << " ";
	v2.print();
	std::cout << "v3 Normalizado: ";
	v3.normalizar();
	std::cout << v3.modulo() << " ";
	v3.print();
	
	std::cout << "Como matriz: " << std::endl << v2.asMatriz().asString() << std::endl;
	
	std::cout << "-=-=-=-= pontos =-=-=-=-" << std::endl;
	Ponto p1(10, 10, 5);
	Ponto p2(0, 0 , 3);
	Ponto p3(0, 10, 0);
	p1.print();
	p2.print();
	p3.print();
	std::cout << "soma baricentrica a=b=c=1/3: ";
	Ponto b = p1.somabar(p2, p3, 1.0/3, 1.0/3, 1.0/3);
	b.print();
	
	std::cout << "-=-=-=-= matriz =-=-=-=-" << std::endl;
	Matriz m(2, 2);
	m.setAt(0,0, 5);
	m.setAt(0,1, 2);
	m.setAt(1,0, 4);
	m.setAt(1,1, 3);
	
	std::cout << m.asString() << std::endl;
	std::cout << (m*m).asString() << std::endl;

	std::cout << "-=-=-=-= cena =-=-=-=-" << std::endl;
	Primitives prim;
	SDL_Event e;
	Parametros parm;
	void (Parametros::*fset)(float*);
	float zeroi[3] = { 0, 0, 0 };
	fset = &Parametros::setPos;
	(parm.*fset)(zeroi);
	float ix=1, iy=1, iz=1;
	bool show_help = false;
	
	DIR* d = opendir(".");
	std::vector<std::string> obj_list;
	unsigned int obj_list_pos = 0;
	while(struct dirent *de = readdir(d)) {
		if(strstr(de->d_name, "byu")) {
			obj_list.push_back(de->d_name);
			std::cout << de->d_name << std::endl;
		}
	}
	
	while (1) {
		Primitives::clean();
		Camera cam = Camera(&parm);
		if(show_help) {
			do_help();
		} else {
			char byf[64];
			sprintf(byf, "%s - Incremento: %03f -  Parametro Atual (%s) = %.3f, %.3f, %.3f",
				 obj_list[obj_list_pos].c_str(),ix, parm.nomepar,
				 parm.atual[0], parm.atual[1], parm.atual[2]);
			Objeto3d ob = Objeto3d(obj_list[obj_list_pos].c_str(), &parm);
			
			Cena cena = Cena(&ob, &cam, &parm);
			cena.draw();
			Primitives::write(0,0, byf);
			Primitives::flip();
		}
		bool done = false;
		while(SDL_PollEvent(&e) || !done) {
			if(e.type == SDL_KEYDOWN) {
				done = true;
				if(e.key.keysym.sym == SDLK_q) {
					float incr[3] = { ix, 0, 0 };
					(parm.*fset)(incr);
				} else if(e.key.keysym.sym == SDLK_a) {
					float incr[3] = { -ix, 0, 0 };
					(parm.*fset)(incr);
				} else if(e.key.keysym.sym == SDLK_w) {
					float incr[3] = { 0, iy, 0 };
					(parm.*fset)(incr);
				} else if(e.key.keysym.sym == SDLK_s) {
					float incr[3] = { 0, -iy, 0 };
					(parm.*fset)(incr);
				} else if(e.key.keysym.sym == SDLK_e) {
					float incr[3] = { 0, 0, iz };
					(parm.*fset)(incr);
				} else if(e.key.keysym.sym == SDLK_d) {
					float incr[3] = { 0, 0, -iz };
					(parm.*fset)(incr);
				} else if(e.key.keysym.sym == SDLK_1) {
					fset = &Parametros::setPos;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_i) {
					fset = &Parametros::setHx;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_o) {
					fset = &Parametros::setHy;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_2) {
					fset = &Parametros::setLuz;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_3) {
					fset = &Parametros::setIl;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_4) {
					fset = &Parametros::setKd;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_5) {
					fset = &Parametros::setOd;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_6) {
					fset = &Parametros::setKa;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_7) {
					fset = &Parametros::setKs;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_8) {
					fset = &Parametros::setEta;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_9) {
					fset = &Parametros::setN;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_0) {
					fset = &Parametros::setV;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_MINUS) {
					fset = &Parametros::setD;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_EQUALS) {
					fset = &Parametros::setIa;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_z) {
					ix = iy = iz = 0.1;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_x) {
					ix = iy = iz = 1;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_c) {
					ix = iy = iz = 10;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_h) {
					show_help = true;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_j) {
					show_help = false;
					(parm.*fset)(zeroi);
				} else if(e.key.keysym.sym == SDLK_r) {
					cam.rotx(0.1);
				} else if(e.key.keysym.sym == SDLK_t) {
					cam.rotx(-0.1);
				} else if(e.key.keysym.sym == SDLK_y) {
					cam.roty(0.1);
				} else if(e.key.keysym.sym == SDLK_u) {
					cam.roty(-0.1);
				} else if(e.key.keysym.sym == SDLK_f) {
					cam.rotz(0.1);
				} else if(e.key.keysym.sym == SDLK_g) {
					cam.rotz(-0.1);
				} else if(e.key.keysym.sym == SDLK_p) {
					++obj_list_pos;
					if(obj_list_pos >= obj_list.size()) {
						obj_list_pos = 0;
					}
				} else if(e.key.keysym.sym == SDLK_ESCAPE) {
					exit(0);
				}
			}
		}
	}
	
	return 0;
}
