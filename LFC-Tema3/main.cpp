#include <fstream>
#include "Grammar.h"

int main() {
	Grammar grammar;
	std::ifstream finG{ "inputG.txt" };
	if (!finG.is_open()) {
		std::cerr << "Could not open file!\n";
		return 1;
	}
	finG >> grammar;
	std::cout << grammar;
	if (grammar.IsValid()) {
		std::cout << "Grammar is valid\n";
		if (grammar.IsIDC()) {
			std::cout << "Grammar is IDC\n\n";
			grammar.GenerateWords(3);
		}
	}
}