#!/usr/bin/python

class AVLNode:
	def __init__(self,d):
		self.dado = d
		self.balance = 0
		self.fdir = self.fesq = None

class BRet:
	def __init__(self,n,b):
		self.balancing = abs(b)
		self.gruda = n

class AVLTree:
	def __init__(self,c):
		self.root = None
		self.compare = c

	def _inserir(self, elemento, r):
		if r == None:
			tmp = AVLNode(elemento)
			return BRet(tmp, 0);
		if self.compare(elemento, r.dado) > 0:
			if r.fdir == None:
				r.fdir = AVLNode(elemento)
				r.balance += 1
				return BRet(r,r.balance)
			else:
				v = self._inserir(elemento, r.fdir)
				r.fdir = v.gruda
				if v.balancing == 0:
					return BRet(r,0)
				r.balance += v.balancing
				if (r.balance > 1) and (r.fdir.balance > 0):
					tmp = r.fdir
					r.balance = tmp.balance = 0
					r.fdir, tmp.fesq = r.fdir.fesq, r
					return BRet(tmp,0)
				elif (r.balance > 1) and (r.fdir.balance < 0):
					tmp = r.fdir
					tmp2 = r.fdir.fesq
					r.fdir = tmp2.fesq
					tmp2.fdir, tmp2.fesq = tmp, r
					if tmp2.balance == 0:
						r.balance = tmp.balance = 0
					elif tmp2.balance > 0:
						r.balance = -1
						tmp.balance = tmp2.balance = 0
					elif tmp2.balance < 0:
						tmp2.balance = r.balance = 0
						tmp.balance = 1
					return BRet(tmp2,0)
				else:
					return BRet(r,r.balance)
		elif self.compare(elemento, r.dado) < 0:
			if r.fesq == None:
				r.fesq = AVLNode(elemento)
				r.balance -= 1
				return BRet(r,r.balance)
			else:
				v = self._inserir(elemento, r.fesq)
				r.fdir = v.gruda
				if v.balancing == 0:
					return BRet(r,0)
				r.balance -= v.balancing
				if r.balance > 1 and r.esq.balance > 0:
					tmp = r.fesq
					r.balance = tmp.balance = 0
					r.fesq, tmp.fdir = r.fesq.fdir, r
					return BRet(tmp,0)
				elif r.balance > 1 and r.fesq.balance < 0:
					tmp = r.fesq
					tmp2 = r.fesq.fdir
					r.fesq = tmp2.fdir
					tmp2.fesq, tmp.fdir = tmp, r
					if tmp2.balance == 0:
						r.balance = tmp.balance = 0
					elif tmp2.balance > 0:
						r.balance = tmp2.balance = 0
						tmp.balance = -1
					elif tmp2.balance < 0:
						tmp2.balance = tmp.balance = 0
						r.balance = 1
					return BRet(tmp2,0)
				else:
					return BRet(r,r.balance)
		else:
			print 'ja existe'
			return BRet(r,0);

	def inserir(self, elemento):
		v = self._inserir(elemento, self.root)
		self.root = v.gruda
		self.find(elemento)

	def _find(self, elemento, r, n):
		if r == None:
			print 'nao encontrado'
			return
		c = self.compare(elemento,r.dado)
		if c == 0:
			print 'encontrado nivel %d' % n
		elif c == -1:
			self._find(elemento, r.fesq, n+1)
		elif c == 1:
			self._find(elemento, r.fdir, n+1)

	def find(self, elemento):
		self._find(elemento, self.root, 0)

def cmpar(a,b):
	if a.n > b.n:
		return 1
	elif a.n < b.n:
		return -1
	return 0
class professor:
	def __init__(self,n,c):
		self.n = n
		self.c = c

x = AVLTree(cmpar)
x.inserir(professor("jose",14))
x.inserir(professor("maria",22))
x.inserir(professor("ronaldo",23))
