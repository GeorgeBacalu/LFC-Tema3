#include "Grammar.h"
#include <vector>
#include <random>

Grammar::Grammar() : m_VN{}, m_VT{}, m_P{}, m_S{ 'S' } {}

Grammar::Grammar(const std::set<char>& VN, const std::set<char>& VT, const std::set<Production>& P, char S) : m_VN{ VN }, m_VT{ VT }, m_P{ P }, m_S{ S } {}

const std::set<char>& Grammar::GetVN() const { return m_VN; }

const std::set<char>& Grammar::GetVT() const { return m_VT; }

const std::set<Production>& Grammar::GetP() const { return m_P; }

char Grammar::GetS() const { return m_S; }

void Grammar::SetVN(const std::set<char>& VN) { m_VN = VN; }

void Grammar::SetVT(const std::set<char>& VT) { m_VT = VT; }

void Grammar::SetP(const std::set<Production>& P) { m_P = P; }

void Grammar::SetS(char S) { m_S = S; }

std::istream& operator>>(std::istream& in, Grammar& grammar) {
	int nrNonTerminals, nrTerminals, nrProductions;
	in >> nrNonTerminals;
	for (int i = 0; i < nrNonTerminals; ++i) {
		char nonTerminal;
		in >> nonTerminal;
		grammar.m_VN.insert(nonTerminal);
	}
	in >> nrTerminals;
	for (int i = 0; i < nrTerminals; ++i) {
		char terminal;
		in >> terminal;
		grammar.m_VT.insert(terminal);
	}
	in >> grammar.m_S >> nrProductions;
	std::string left, right;
	for (int i = 0; i < nrProductions; ++i) {
		in >> left >> right;
		grammar.m_P.emplace(left, right);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Grammar& grammar) {
	out << "G = ({ ";
	for (char ch : grammar.m_VN)
		out << ch << " ";
	out << "}, { ";
	for (char ch : grammar.m_VT)
		out << ch << " ";
	out << "}, " << grammar.m_S << ", P})\n";
	for (const auto& [left, right] : grammar.m_P)
		if (!left.empty())
			out << left << " -> " << right << "\n";
	return out << "\n";
}

const std::set<std::string>& Grammar::GenerateWords(int nrWords) {
	std::string generatedWord;
	std::random_device rd;
	std::mt19937 eng(rd());
	m_generatedWords.clear();
	while (m_generatedWords.size() < nrWords) {
		generatedWord = std::string{ m_S };
		while (true) {
			std::set<Production> applicableProductions;
			for (const auto& production : m_P)
				if (generatedWord.find(production.first) != std::string::npos)
					applicableProductions.insert(production);
			if (applicableProductions.empty()) {
				m_generatedWords.insert(generatedWord);
				break;
			}
			std::uniform_int_distribution<int> distribution(0, applicableProductions.size() - 1);
			size_t productionIndex = distribution(eng);
			auto it = applicableProductions.begin();
			std::advance(it, productionIndex);
			const auto& [left, right] = *it; // selected production
			// choose random position to apply production
			std::vector<size_t> possiblePositions;
			size_t position = generatedWord.find(left);
			while (position != std::string::npos) {
				possiblePositions.push_back(position);
				position = generatedWord.find(left, position + 1);
			}
			std::uniform_int_distribution<int> distribution2(0, possiblePositions.size() - 1);
			size_t replacePosition = possiblePositions[distribution2(eng)];
			std::cout << generatedWord << " => ";
			generatedWord.replace(replacePosition, left.length(), right == "@" ? "" : right);
		}
		std::cout << generatedWord << "\n\n";
	}
	return m_generatedWords;
}

bool Grammar::IsValid() const {
	return CheckNonEmptySets() && CheckDisjunctVNVT() && CheckSInVN() && CheckProductionsLeftNonTerminal() && CheckProductionValidCharacters() && CheckProductionWithS();
}

bool Grammar::CheckNonEmptySets() const {
	if (m_VN.empty() || m_VT.empty()) {
		std::cout << "Invalid grammar: VN or VT is empty!\n";
		return false;
	}
	return true;
}

bool Grammar::CheckDisjunctVNVT() const {
	for (char nonTerminal : m_VN)
		if (m_VT.find(nonTerminal) != m_VT.end()) {
			std::cout << "Invalid grammar: VN characters are in VT!\n";
			return false;
		}
	for (char terminal : m_VT)
		if (m_VN.find(terminal) != m_VN.end()) {
			std::cout << "Invalid grammar: VT characters are in VN!\n";
			return false;
		}
	return true;
}

bool Grammar::CheckSInVN() const {
	if (m_VN.find(m_S) == m_VN.end()) {
		std::cout << "Invalid grammar: S is not in VN!\n";
		return false;
	}
	return true;
}

bool Grammar::CheckProductionsLeftNonTerminal() const {
	bool hasNonTerminal;
	for (const auto& [left, _] : m_P) {
		hasNonTerminal = false;
		for (char nonTerminal : m_VN) {
			if (left.find(nonTerminal) != std::string::npos) {
				hasNonTerminal = true;
				break;
			}
		}
		if (!hasNonTerminal) {
			std::cout << "Invalid grammar: production without left side non-terminal!\n";
			return false;
		}
	}
	return true;
}

bool Grammar::CheckProductionValidCharacters() const {
	for (const auto& [left, right] : m_P)
		for (char symbol : left + right)
			if (m_VN.find(symbol) == m_VN.end() && m_VT.find(symbol) == m_VT.end()) {
				std::cout << "Invalid grammar: production with invalid characters!\n";
				return false;
			}
	return true;
}

bool Grammar::CheckProductionWithS() const {
	bool existsProductionWithS = false;
	for (const auto& [left, _] : m_P)
		if (left[0] == m_S && left.size() == 1)
			existsProductionWithS = true;
	if (!existsProductionWithS)
		std::cout << "Invalid grammar: no production with left side S!\n";
	return existsProductionWithS;
}

bool Grammar::IsIDC() const {
	if (!IsValid()) {
		std::cout << "Invalid grammar!\n";
		return false;
	}
	for (const auto& [left, _] : m_P)
		if (left.size() != 1 || m_VN.find(left[0]) == m_VN.end()) {
			std::cout << "Invalid IDC grammar: production " << left << " does not have a single non-terminal on the left.\n";
			return false;
		}
	return true;
}

Grammar Grammar::SimplifyIDC() const
{
	Grammar grammar1 = RemoveLambdaProductions();
	Grammar grammar2 = grammar1.RemoveRenamings();
	Grammar grammar3 = grammar2.RemoveSymbolsNotGeneratingWordsFromVT();
	return grammar3.RemoveInaccessibleSymbols();
}

Grammar Grammar::RemoveLambdaProductions() const {
	// Step 1: Identify non-terminals producing lambda
	std::set<char> nullable;
	for (const auto& prod : m_P)
		if (prod.second == "@" || prod.second.empty())
			nullable.insert(prod.first[0]);
	bool changes;
	do {
		changes = false;
		for (const auto& prod : m_P) {
			bool allNullable = true;
			for (const char& symbol : prod.second)
				if (nullable.find(symbol) == nullable.end()) {
					allNullable = false;
					break;
				}
			if (allNullable && prod.second.size() > 0 && nullable.insert(prod.first[0]).second)
				changes = true;
		}
	} while (changes);
	// Step 2: Adjust productions
	std::set<Production> newProductions;
	for (const auto& prod : m_P) {
		std::set<std::string> combinations{ "" };
		for (char symbol : prod.second) {
			std::set<std::string> newCombinations;
			for (const std::string& str : combinations) {
				newCombinations.insert(str + symbol);
				if (nullable.find(symbol) != nullable.end())
					newCombinations.insert(str);
			}
			combinations = std::move(newCombinations);
		}
		for (const std::string& str : combinations)
			if (!str.empty())
				newProductions.insert({ prod.first, str });
	}
	// Step 3: Add New Start Symbol
	char newStartSymbol = m_S;
	if (nullable.find(m_S) != nullable.end()) {
		newStartSymbol = 'S';
		while (m_VN.find(newStartSymbol) != m_VN.end()) {
			newStartSymbol++;
		}
		newProductions.insert({ std::string(1, newStartSymbol), "@" });
		newProductions.insert({ std::string(1, newStartSymbol), std::string(1, m_S) });
	}
	return { m_VN, m_VT, newProductions, newStartSymbol };
}

Grammar Grammar::RemoveRenamings() const {
	std::set<Production> Pi;
	std::set<Production> Pi_1{ m_P.begin(), m_P.end() };
	bool changed;
	for (const auto& prod : m_P)
		if (prod.second.size() != 1 || m_VT.find(prod.second[0]) != m_VT.end())
			Pi.insert(prod);
	do {
		changed = false;
		std::set<Production> newProductions{ Pi.begin(), Pi.end() };
		for (const auto& [left, right] : Pi_1)
			if (right.size() == 1 && m_VN.find(right[0]) != m_VN.end())
				for (const auto& prod : Pi_1)
					if (prod.first[0] == right[0]) {
						Production newProd = { left, prod.second };
						if (Pi.insert(newProd).second) {
							newProductions.insert(newProd);
							changed = true;
						}
					}
		Pi_1 = newProductions;
	} while (changed);
	std::set<char> VN1;
	for (const auto& prod : Pi)
		VN1.insert(prod.first[0]);
	return { VN1, m_VT, Pi, m_S };
}

Grammar Grammar::RemoveSymbolsNotGeneratingWordsFromVT() const {
	std::set<char> VN1;
	std::set<Production> P1;
	bool changed;
	do {
		changed = false;
		for (const auto& [left, right] : m_P) {
			bool canProduce = true;
			for (const char& symbol : right)
				if (m_VN.find(symbol) != m_VN.end() && VN1.find(symbol) == VN1.end()) {
					canProduce = false;
					break;
				}
			if (canProduce && VN1.insert(left[0]).second)
				changed = true;
		}
	} while (changed);
	for (const auto& [left, right] : m_P)
		if (VN1.find(left[0]) != VN1.end()) {
			bool isValid = true;
			for (const char& symbol : right)
				if (m_VN.find(symbol) != m_VN.end() && VN1.find(symbol) == VN1.end()) {
					isValid = false;
					break;
				}
			if (isValid)
				P1.insert({ left, right });
		}
	return { VN1, m_VT, P1, m_S };
}

Grammar Grammar::RemoveInaccessibleSymbols() const {
	std::set<char> accessible;
	std::set<char> newAccessible = { m_S };
	std::set<Production> newProductions;
	while (newAccessible != accessible) {
		accessible = newAccessible;
		for (const auto& [left, right] : m_P)
			if (accessible.find(left[0]) != accessible.end()) {
				newProductions.insert({ left, right });
				for (const char& symbol : right)
					if (m_VN.find(symbol) != m_VN.end())
						newAccessible.insert(symbol);
			}
	}
	return { newAccessible, m_VT, newProductions, m_S };
}

Grammar Grammar::ConvertIDCtoFNC() const {
	if(!IsIDC()) {
		std::cout << "Invalid IDC grammar!\n";
		return *this;
	}
	std::set<Production> newProductions;
	std::map<char, char> terminalToNonTerminal;
	char nextNonTerminal = 'K';

	// Step 1: Remove renamings
	Grammar simplifiedGrammar = RemoveRenamings();

	// Step 2: Replace terminals with new non-terminals where necessary and add new productions
	for (const auto& [left, right] : simplifiedGrammar.m_P) {
		std::string newRight;
		for (char symbol : right) {
			if (simplifiedGrammar.m_VT.find(symbol) != simplifiedGrammar.m_VT.end()) {
				if (terminalToNonTerminal.find(symbol) == terminalToNonTerminal.end()) {
					while (simplifiedGrammar.m_VN.find(nextNonTerminal) != simplifiedGrammar.m_VN.end())
						nextNonTerminal++;
					terminalToNonTerminal[symbol] = nextNonTerminal;
					newProductions.insert({ std::string(1, nextNonTerminal), std::string(1, symbol) });
					simplifiedGrammar.m_VN.insert(nextNonTerminal);
				}
				newRight.push_back(terminalToNonTerminal[symbol]);
			}
			else newRight.push_back(symbol);
		}
		newProductions.insert({ left, newRight });
	}

	// Step 3: Ensure all productions are binary
	for (auto it = newProductions.begin(); it != newProductions.end(); ) {
		const auto& [left, right] = *it;
		if (right.size() > 2) {
			// Break down productions with more than two symbols on the right-hand side
			std::string remaining = right.substr(1);
			char newSymbol;
			do {
				newSymbol = nextNonTerminal;
				while (simplifiedGrammar.m_VN.find(nextNonTerminal) != simplifiedGrammar.m_VN.end())
					nextNonTerminal++;
				std::string newRight = remaining.substr(0, 1) + std::string(1, newSymbol);
				newProductions.insert({ std::string(1, newSymbol), remaining.substr(1) });
				remaining = newRight;
			} while (remaining.size() > 2);
			it = newProductions.erase(it); // Remove old production
			newProductions.insert({ left, remaining }); // Insert the new production with a binary right-hand side
		}
		else ++it;
	}
	return { simplifiedGrammar.m_VN, simplifiedGrammar.m_VT, newProductions, simplifiedGrammar.m_S };
}

Grammar Grammar::ConvertFNCtoFNG() const {
	if (!IsIDC()) {
		std::cout << "Invalid IDC grammar!\n";
		return *this;
	}
	Grammar simplifiedGrammar = ConvertIDCtoFNC();
	std::set<Production> newProductions;
	char newNonTerminal = 'Z';

	// Step 1: Ensure that all rules with a non-terminal on the right-hand side start with a terminal
	for (const auto& [left, right] : simplifiedGrammar.m_P) {
		if (right.size() > 1 && simplifiedGrammar.m_VT.find(right[0]) == simplifiedGrammar.m_VT.end()) {
			// If the production starts with a non-terminal, apply Lemma 1
			// Find all productions of the non-terminal that starts the production
			char nonTerminal = right[0];
			for (const auto& [subLeft, subRight] : simplifiedGrammar.m_P)
				if (subLeft[0] == nonTerminal && simplifiedGrammar.m_VT.find(subRight[0]) != simplifiedGrammar.m_VT.end()) {
					// For each such production, replace the non-terminal with its productions
					std::string newRight = subRight + right.substr(1);
					newProductions.insert({ left, newRight });
				}
		}
		else newProductions.insert({left, right}); // If the production already starts with a terminal or is a single non-terminal, keep it
	}

	// Step 2: Eliminate left recursion using Lemma 2
	for (const auto& [left, right] : newProductions)
		if (left[0] == right[0]) {
			// We have a Left recursion (A -> Aα) => Introduce a new non-terminal 'Z' and replace rules according to Lemma 2
			while (simplifiedGrammar.m_VN.find(newNonTerminal) != simplifiedGrammar.m_VN.end())
				newNonTerminal++; // Ensure the new symbol is not already in use
			std::string newNonTerminalStr(1, newNonTerminal);
			simplifiedGrammar.m_VN.insert(newNonTerminal);
			// Replace the recursive rule with: A -> βZ
			newProductions.erase({left, right}); // Remove the recursive production
			newProductions.insert({ left, right.substr(1) + newNonTerminalStr });
			// Add a rule for the new non-terminal: Z -> αZ | λ
			newProductions.insert({ newNonTerminalStr, right.substr(1) + newNonTerminalStr });
			newProductions.insert({ newNonTerminalStr, "" }); // This represents the empty string λ
		}
	return { m_VN, m_VT, newProductions, m_S };
}

PushDownAutomaton Grammar::toPushDownAutomaton() const
{
	PushDownAutomaton pda;

	
	std::set<std::string> stari;
	for (char neterminal : m_VN) {
		stari.insert({ std::string{ neterminal } });
	}
	stari.insert("T");

	
	pda.setStari(stari);
	pda.setAlfabetIntrare(m_VT);
	pda.setAlfabetMemorie(m_VN);  
	pda.setStareInitiala(std::string{ m_S });
	pda.setSimbolStartMemorie('@'); 

	
	FunctieTranzitiePD functieTranzitie;
	for (const auto& [stanga, dreapta] : m_P) {
		std::string stare1{ stanga };
		char simbolIntrare = dreapta[0];
		char simbolMemorie = dreapta.size() == 2 ? dreapta[1] : '@';  // Assuming '@' represents the empty string
		std::string stare2 = dreapta.size() == 2 ? std::string{ dreapta[1] } : "T";
		functieTranzitie[{stare1, simbolIntrare, simbolMemorie}].push_back(stare2);
	}


	pda.setFunctieTranzitiePD(functieTranzitie);

	
	std::set<std::string> stariFinale;
	for (const auto& [left, _] : m_P) {
		if (left.size() == 1 && m_VN.find(left[0]) != m_VN.end()) {
			stariFinale.insert(left);
		}
	}
	if (!stariFinale.empty()) {
		stariFinale.insert("T");
	}
	pda.setStariFinale(stariFinale);

	return pda;
}