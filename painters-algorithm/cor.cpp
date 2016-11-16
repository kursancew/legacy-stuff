#include "cor.h"

Cor::Cor(int r, int g, int b)
{
}

Cor::Cor(unsigned int c)
{
}

Cor::Cor()
{
}

unsigned int Cor::asInt()
{
	return mr << 16 | mg << 8 | mb;
}
