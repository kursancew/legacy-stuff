//2009 Kursancew, Vinicius Alexandre
//
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <fstream>
#include <cstdlib>
#include <string>
#include <exception>
#include <cstdio>

class eLista {
	std::string e;
public:
	eLista(std::string s):e(s){}
	std::string what() { return e; }
};

class eRuntime {
	std::string e;
public:
	eRuntime(std::string s):e(s){}
	std::string what() { return e; }
};

class eAssembler {
	std::string e;
public:
	eAssembler(std::string s):e(s){}
	std::string what() { return e; }
};

template <class T> class lista
{
	class sLista {
		public:
		sLista *next;
		T dado;
	};
public:
	class iterador {
		sLista *elm;
	public:
		iterador(sLista *e):elm(e){}
		bool next() { // retorna falso se chegou no fim da linha
			elm = elm?elm->next:0;
			if(elm) return true;
			return false;
		}
		bool isEnd() {
			return !elm;
		}
		sLista *elemento() { return elm; }
		T dado() { if(!elm) throw eLista("dereferenciamento de iterador nulo"); return elm->dado; }
		T operator*() {
			return dado();
		}
		iterador& operator++() {
			next();
			return *this;
		}
		/*iterador& operator++(int foo) {
			iterador c = iterador(*this);
			next();
			return c;
		}*/
	};

	lista();
	~lista();

	unsigned int tamanho();
	void inserir(T, unsigned int);
	void next();
	void remover(unsigned int);
	T obter(unsigned int);
	int achar(T);
	T operator[](unsigned int pos) {
		return obter(pos);
	}
	iterador inicio() {
		return iterador(mHead);
	}
private:
	sLista *mHead;
	unsigned int mCardinalidade;
};

template <class T> lista<T>::lista()
{
	mHead = 0;
	mCardinalidade = 0;
}

template <class T> lista<T>::~lista()
{
	for (unsigned int i = 0; i < tamanho(); ++i) {
		remover(0);
	}
	mHead = 0;
}

template <class T> T lista<T>::obter(unsigned int pos)
{
	iterador itr = inicio();
	if(pos >= mCardinalidade) pos = mCardinalidade-1;
	for(unsigned int i = 0; i < pos; ++i) ++itr;
	return *itr;
}

template <class T> int lista<T>::achar(T item)
{
	iterador itr = inicio();
	int i = 0;
	while (!itr.isEnd()) {
		if(*itr == item) return i;
		++itr;
		++i;
	}
	return -1;
}

template <class T> unsigned int lista<T>::tamanho()
{
	return mCardinalidade;
}

template <class T> void lista<T>::inserir(T item, unsigned int pos)
{
	if (pos > mCardinalidade) pos = mCardinalidade;
	if(!mHead || !pos) {
		sLista *tmp = new sLista;
		tmp->dado = item;
		tmp->next = mHead;
		mHead = tmp;
	} else {
		iterador itr = inicio();
		unsigned int i = 0;
		sLista *ant = 0;
		while ( i++ < pos && itr.elemento()) {
			ant = itr.elemento();
			++itr;
		}
		if(!ant) throw eLista("lista<T>::inserir: dereferencia de ponteiro nulo");
		sLista* tmp = new sLista;
		tmp->dado = item;
		tmp->next = ant->next;
		ant->next = tmp;
	}
	++mCardinalidade;
}
template <class T> void lista<T>::remover(unsigned int pos)
{
	if (pos >= mCardinalidade) pos = mCardinalidade-1;
	if(mCardinalidade--) {
		if(pos == 0) {
			sLista *tmp = mHead->next;
			delete mHead;
			mHead = tmp;
		} else {
			iterador itr = inicio();
			for(unsigned int i = 0; i < pos-1; ++i) ++itr;
			sLista *ant = itr.elemento();
			if(!ant) throw eLista("lista<T>::remover: dereferencia de ponteiro nulo");
			sLista *tmp = ant->next;
			ant->next = tmp->next;
			delete tmp;
		}
	}
}

static const int N_INSTRUCOES = 17;
std::string array_instrucoes[] = {
	std::string("add_reg"),
	std::string("add_cte"),
	std::string("sub_reg"),
	std::string("sub_cte"),
	std::string("mult_reg"),
	std::string("mult_cte"),
	std::string("div_reg"),
	std::string("div_cte"),
	std::string("carrega_reg"),
	std::string("break"),
	std::string("set_tam_pilha"),
	std::string("jump"),
	std::string("se"),
	std::string("senao"),
	std::string("se_menor"),
	std::string("funcao"),
	std::string("return")
};

class instrucao {
public:
	int inst;
	int alvo;
	int o1;
	int o2;
	int addr;
};

class instrucao_dec {
public:
	std::string inst;
	int alvo;
	int o1;
	int o2;
	std::string addr;
};

class assembler {
	int nregiters;
	lista<instrucao_dec> insts_dec;
	lista<instrucao> insts;
	
	/* correlacao linha/labels */
	lista<std::string> labels;
	lista<int> label_lines;
	int badlabels;

public:
	assembler(int register_size);
	int nregisters;
	void add_line(std::string);
	void encode();
	lista<instrucao>& getInstrucoes();
};

lista<instrucao>& assembler::getInstrucoes()
{
	return insts;
}
void assembler::add_line(std::string l)
{
	instrucao_dec tmp;
	//std::cout << l << " "; // std::endl;
	if(l.find(':') != std::string::npos) {
		l = l.substr(7);
		//l = l.substr(l.find_first_not_of(' '));
		std::string lb = l.substr(0,l.find(' '));
		l = l.substr(l.find(' ')+1);
		l = l.substr(l.find_first_not_of(' '));
		//std::cout << lb << "." << std::endl;
		if(labels.achar(lb) >= 0) badlabels = 1;
		labels.inserir(lb,0);
		label_lines.inserir(insts_dec.tamanho(),0);
	}

	tmp.inst = l.substr(0,l.find(' '));
	l = l.substr(l.find(' ')+1);

	tmp.addr = l.substr(l.rfind(' ')+1);
	//tmp.addr = tmp.addr.substr(l.find_first_not_of(' '));

	l = l.substr(0,l.rfind(' '));
	sscanf(l.c_str(), "%d %d %d", &tmp.alvo, &tmp.o1, &tmp.o2);

	//std::cout << tmp.inst << " ";
	//std::cout << tmp.alvo << " " << tmp.o1 << " " << tmp.o2 << " ";
	//std::cout << tmp.addr << std::endl;
	insts_dec.inserir(tmp,0);
}

assembler::assembler(int register_size)
{
	nregisters = register_size;
	badlabels = 0;
}

void assembler::encode ()
{
	if(badlabels) throw eAssembler("label duplicado");
	lista<instrucao_dec>::iterador itr = insts_dec.inicio();
	/*lista<std::string>::iterador x = labels.inicio();
	std::cout << "labels:" << std::endl;
	while(!x.isEnd()) {
		std::cout << *x << std::endl;
		++x;
	}
	std::cout << std::endl;
	*/
	while(!itr.isEnd()) {
		instrucao tmp;
		tmp.alvo = (*itr).alvo;
		tmp.o1 = (*itr).o1;
		tmp.o2 = (*itr).o2;
		for(int i = 0; i < N_INSTRUCOES; ++i) {
			if((*itr).inst == array_instrucoes[i]) {
				tmp.inst = i;
				break;
			}
		}
		//std::cout << "." << (*itr).addr << "." << std::endl;
		int lin = labels.achar((*itr).addr);
		tmp.addr = lin >= 0?label_lines.obter(lin):-1;
		insts.inserir(tmp,0);
		++itr;
		//std::cout << tmp.inst << " " << tmp.alvo << " " << tmp.o1 << " " << tmp.o2 << " " << tmp.addr << std::endl;
	}
}

class runtime {
	int PC;
	int* registers;
	int nregisters;
	int ninstrucoes;
	lista<int> stack;
	int max_stack;
	instrucao** program;
	void testa_reg(int);
	void testa_label(int);
	void print_regs();
	bool running;
	std::ofstream &OUT;
	
	/* instrucoes */
	void add_reg(int alvo, int o1, int o2, int addr);
	void add_cte(int alvo, int o1, int o2, int addr);
	void sub_reg(int alvo, int o1, int o2, int addr);
	void sub_cte(int alvo, int o1, int o2, int addr);
	void mult_reg(int alvo, int o1, int o2, int addr);
	void mult_cte(int alvo, int o1, int o2, int addr);
	void div_reg(int alvo, int o1, int o2, int addr);
	void div_cte(int alvo, int o1, int o2, int addr);
	void carrega_reg(int alvo, int o1, int o2, int addr);
	void break_prog(int alvo, int o1, int o2, int addr);
	void set_tam_pilha(int alvo, int o1, int o2, int addr);
	void jump(int alvo, int o1, int o2, int addr);
	void se(int alvo, int o1, int o2, int addr);
	void senao(int alvo, int o1, int o2, int addr);
	void se_menor(int alvo, int o1, int o2, int addr);
	void funcao(int alvo, int o1, int o2, int addr);
	void ret(int alvo, int o1, int o2, int addr);
	void (runtime::*instrucoes[17])(int,int,int,int);
	
public:
	runtime(int register_size, lista<instrucao> &insts, std::ofstream&);
	~runtime();
	void run();
};

runtime::~runtime()
{
	delete registers;
	for (int i = 0; i < ninstrucoes; ++i) {
		delete program[i];
	}
	delete program;
}


void runtime::run()
{
	running = 1;
	PC = 0;
	while (running) {
		(*this.*instrucoes[program[PC]->inst])(program[PC]->alvo, program[PC]->o1, program[PC]->o2, program[PC]->addr);
	}
}

void runtime::testa_reg(int r)
{
	if(r < 0 || r > nregisters-1)
		throw eRuntime("registrador invalido");
}

void runtime::testa_label(int l)
{
	if(l < 0 || l > ninstrucoes-1) {
		//std::cout << l << std::endl;
		throw eRuntime("label inexistente");
	}
}

void runtime::print_regs()
{
	OUT << array_instrucoes[program[PC]->inst] << ": ";
	for (int i = 0; i < nregisters; ++i)
	{
		OUT << "R" << i << "=" << registers[i];
		if(i+1 != nregisters) OUT << " ";
	}
	//std::cout << " PC=" << PC << " STACK=" << stack.tamanho();
	OUT << std::endl;
}

runtime::runtime(int register_size, lista<instrucao> &insts, std::ofstream &o):OUT(o)
{
	PC = 0;
	running = 0;
	registers = new int[register_size];
	for(int i = 0; i < register_size; ++i) {
		registers[i] = 0;
	}
	nregisters = register_size;
	max_stack = 5;
	instrucoes[0] = &runtime::add_reg;
	instrucoes[1] = &runtime::add_cte;
	instrucoes[2] = &runtime::sub_reg;
	instrucoes[3] = &runtime::sub_cte;
	instrucoes[4] = &runtime::mult_reg;
	instrucoes[5] = &runtime::mult_cte;
	instrucoes[6] = &runtime::div_reg;
	instrucoes[7] = &runtime::div_cte;
	instrucoes[8] = &runtime::carrega_reg;
	instrucoes[9] = &runtime::break_prog;
	instrucoes[10] = &runtime::set_tam_pilha;
	instrucoes[11] = &runtime::jump;
	instrucoes[12] = &runtime::se;
	instrucoes[13] = &runtime::senao;
	instrucoes[14] = &runtime::se_menor;
	instrucoes[15] = &runtime::funcao;
	instrucoes[16] = &runtime::ret;
	ninstrucoes = insts.tamanho();
	program = new instrucao*[insts.tamanho()];
	lista<instrucao>::iterador itr = insts.inicio();
	int i = 0;
	//std::cout << "---------" << std::endl;
	while(!itr.isEnd()) {
		program[i] = new instrucao(*itr);
		//std::cout << program[i]->inst << std::endl;
		++itr; ++i;
	}
	//std::cout << "---------" << std::endl;
	
}

void runtime::add_reg(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	testa_reg(o2);
	registers[alvo] = registers[o1] + registers[o2];
	print_regs();
	++PC;
}

void runtime::add_cte(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	registers[alvo] = registers[o1] + o2;
	print_regs();
	++PC;
}

void runtime::sub_reg(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	testa_reg(o2);
	registers[alvo] = registers[o1] - registers[o2];
	print_regs();
	++PC;
}

void runtime::sub_cte(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	registers[alvo] = registers[o1] - o2;
	print_regs();
	++PC;
}

void runtime::mult_reg(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	testa_reg(o2);
	registers[alvo] = registers[o1] * registers[o2];
	print_regs();
	++PC;
}

void runtime::mult_cte(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	registers[alvo] = registers[o1] * o2;
	print_regs();
	++PC;
}

void runtime::div_reg(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	testa_reg(o2);
	if(!registers[o2]) throw eRuntime("divisao por zero");
	registers[alvo] = registers[o1] / registers[o2];
	print_regs();
	++PC;
}

void runtime::div_cte(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	if(!o2) throw eRuntime("divisao por zero");
	registers[alvo] = registers[o1] / o2;
	print_regs();
	++PC;
}

void runtime::carrega_reg(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	registers[alvo] = o1;
	print_regs();
	++PC;
}

void runtime::break_prog(int alvo, int o1, int o2, int addr)
{
	running = 0;
	print_regs();
}

void runtime::set_tam_pilha(int alvo, int o1, int o2, int addr)
{
	if(alvo < (int)stack.tamanho()) throw eRuntime("estouro de pilha");
	max_stack = alvo;
	++PC;
}

void runtime::jump(int alvo, int o1, int o2, int addr)
{
	testa_label(addr);
	
	PC = addr;
}

void runtime::se(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	testa_label(addr);
	
	if(registers[alvo] == registers[o1]) PC = addr;
	else ++PC;
}

void runtime::senao(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	testa_label(addr);
	
	if(registers[alvo] != registers[o1]) PC = addr;
	else ++PC;
}

void runtime::se_menor(int alvo, int o1, int o2, int addr)
{
	testa_reg(alvo);
	testa_reg(o1);
	testa_label(addr);
	
	if(registers[alvo] < registers[o1]) PC = addr;
	else ++PC;
}

void runtime::funcao(int alvo, int o1, int o2, int addr)
{
	testa_label(addr);
	if((int)stack.tamanho() + 1 > max_stack)
		throw eRuntime("estouro de pilha");
	
	stack.inserir(PC+1,0);
	PC = addr;
}

void runtime::ret(int alvo, int o1, int o2, int addr)
{
	if(!stack.tamanho())
		throw eRuntime("pilha vazia");
	
	PC = stack.obter(0);
	stack.remover(0);
}



int main() {
	std::ifstream IN("L1Q2.in",std::ios::in);
	std::ofstream OUT("L1Q2.out",std::ios::out);
	try {
		int conj = 1;
		while (!IN.eof()) {
			int regs,insts;
			IN >> regs;
			IN >> insts;
			if(IN.eof()) break;
			OUT << "Conjunto #" << conj++ << std::endl;
			//std::cout << regs << " " << insts << std::endl;
			try {
				assembler a(regs);
				char l[8*1024];
				IN.getline(l,100);
				for	(int i = 0;	i <	insts; ++i)	{
					IN.getline(l, 8*1024);
					if(l[strlen(l)-1] == '\r') l[strlen(l)-1] =	0;
					//std::cout	<< l <<	std::endl;
					a.add_line(std::string(l));
				}
				a.encode();
				runtime r(regs,a.getInstrucoes(),OUT);
				r.run();
			} catch (eAssembler &e) {
				OUT << e.what() << std::endl;
			} catch (eRuntime &e) {
				OUT << e.what() << std::endl;
			}
			OUT << std::endl;
		}
	} catch (std::exception &e) {
		OUT << e.what() << std::endl;
	} 
	return 0;
}
