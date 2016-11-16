#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <cstdlib>
#include <ctime>
#include <string>

int main()
{
	srandom(time(NULL));
	for(int i = 0; i < 10; ++i) {
		std::string t = random()%2?"codigo":"nome";
		std::cout << t << std::endl;
		std::cout << 10 << std::endl;
		for (int j = 0; j < 10; ++j) {
			char s[4];
			s[0] = (char)(random()%26+'a');
			s[1] = (char)(random()%26+'a');
			s[2] = (char)(random()%26+'a');
			s[2] = 0;
			std::cout << "I " << s << " " << random()%100 << std::endl;
		}
		std::cout << std::endl;
	}
	return 0;
}
