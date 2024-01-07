#include <fstream>
#include "Grammar.h"
#include "PushDownAutomaton.h"

int main() {
	Grammar grammar;
	PushDownAutomaton pda;
	std::ifstream finG{ "inputG.txt" };
	if (!finG.is_open()) {
		std::cerr << "Could not open file!\n";
		return 1;
	}
	std::ifstream finPDA{ "inputPDA.txt" };
	if (!finPDA.is_open()) {
		std::cerr << "Could not open file!\n";
		return 1;
	}
	int option;
	std::set<std::string> generatedWordSet;
	std::string generatedWord;
	std::string word;
	do {
		std::cout << "Command menu:\n";
		std::cout << "0. Exit\n";
		std::cout << "1. Display grammar\n";
		std::cout << "2. Generate word in grammar\n";
		std::cout << "3. Display simplified grammar\n";
		std::cout << "4. Display grammar in FNG\n";
		std::cout << "5. Generate words in grammar and check if it's accepted by APD\n";
		std::cout << "6. Check if word read from keyboard is accepted by APD\n";
		std::cout << "Choose an option: ";
		std::cin >> option;
		switch (option) {
		case 0: break;
		case 1:
			std::cout << "Grammar:\n\n" << grammar << "\n";
			break;
		case 2:
			grammar.GenerateWords(1);
			break;
		case 3:
			if (grammar.IsValid()) {
				std::cout << "Grammar is valid\n";
				if (grammar.IsIDC()) {
					std::cout << "Grammar is IDC\n\n";
					std::cout << "Simplified grammar:\n\n" << grammar.SimplifyIDC() << "\n";
				}
			}
			break;
		case 4:
			std::cout << "Grammar in FNG:\n\n" << grammar.ConvertFNCtoFNG() << "\n";
			break;
		case 5:
			generatedWordSet = grammar.GenerateWords(1);
			generatedWord = *generatedWordSet.begin();
			std::cout << "Generated word: " << generatedWord << "\n";
			std::cout << "Word: " << generatedWord << " generated from grammar is" << (pda.checkWord(generatedWord) ? " " : " not ") << "accepted by automaton\n";
			break;
		case 6:
			std::cout << "Word: ";
			std::cin >> word;
			std::cout << "Word: " << generatedWord << " generated from grammar is" << (pda.checkWord(generatedWord) ? " " : " not ") << "accepted by automaton\n";
			break;
		}
	} while (option);
}