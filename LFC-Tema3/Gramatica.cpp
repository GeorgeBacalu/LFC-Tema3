#include "Gramatica.h"

Gramatica::Gramatica() : m_VN{ {} }, m_VT{ {} }, m_P{ {} }, m_S{ 'S' } {}

Gramatica::Gramatica(const std::set<char>& VN, const std::set<char>& VT, const std::set<Productie>& P, char S) : m_VN{ VN }, m_VT{ VT }, m_P{ P }, m_S{ S } {}

std::istream& operator>>(std::istream& in, Gramatica& gramatica)
{
	int nrNeterminale, nrTerminale, nrProductii;
	in >> nrNeterminale;
	for (int i = 0; i < nrNeterminale; i++)
	{
		char neterminal;
		in >> neterminal;
		gramatica.m_VN.insert(neterminal);
	}
	in >> nrTerminale;
	for (int i = 0; i < nrTerminale; i++)
	{
		char terminal;
		in >> terminal;
		gramatica.m_VT.insert(terminal);
	}
	in >> gramatica.m_S >> nrProductii;
	std::string stanga, dreapta;
	for (int i = 0; i < nrProductii; i++)
	{
		in >> stanga >> dreapta;
		gramatica.m_P.emplace(stanga, dreapta);
	}
	gramatica.m_VN.erase('\0'); // Remove null character from m_VN
	gramatica.m_VT.erase('\0'); // Remove null character from m_VT
	if (!gramatica.m_P.empty())
	{
		gramatica.m_P.erase(gramatica.m_P.begin());
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica)
{
	out << "G = ({ ";
	for (char ch : gramatica.m_VN)
	{
		out << ch << " ";
	}
	out << "}, { ";
	for (char ch : gramatica.m_VT)
	{
		out << ch << " ";
	}
	out << "}, " << gramatica.m_S << ", P}\n";
	for (const auto& [stanga, dreapta] : gramatica.m_P)
	{
		if (!stanga.empty())
		{
			out << stanga << " -> " << dreapta << "\n\n";
		}
	}
	return out;
}

const std::set<char>& Gramatica::GetVN() const
{
	return m_VN;
}

void Gramatica::SetVN(const std::set<char>& VN)
{
	m_VN = VN;
}

const std::set<char>& Gramatica::GetVT() const
{
	return m_VT;
}

void Gramatica::SetVT(const std::set<char>& VT)
{
	m_VT = VT;
}

const std::set<Productie>& Gramatica::GetP() const
{
	return m_P;
}

void Gramatica::SetP(const std::set<Productie>& P)
{
	m_P = P;
}

char Gramatica::GetS() const
{
	return m_S;
}

void Gramatica::SetS(char S)
{
	m_S = S;
}

std::set<std::string> Gramatica::generare(int nrCuvinte)
{
	m_cuvinteGenerate.clear();
	std::string cuvantGenerat;
	std::random_device rd;
	std::mt19937 eng(rd());
	while (m_cuvinteGenerate.size() < nrCuvinte)
	{
		cuvantGenerat = std::string{ m_S };
		while (true) {
			std::set<Productie> productiiAplicabile;
			for (const Productie& productie : m_P)
			{
				if (cuvantGenerat.find(productie.first) != std::string::npos)
				{
					productiiAplicabile.insert(productie);
				}
			}
			if (productiiAplicabile.empty())
			{
				m_cuvinteGenerate.insert(cuvantGenerat);
				break;
			}
			std::uniform_int_distribution<int> distribution(0, productiiAplicabile.size() - 1);
			int indexProductie = distribution(eng);

			// Gaseste productia selectata folosind iteratori
			auto it = productiiAplicabile.begin();
			std::advance(it, indexProductie);
			const Productie& productieSelectata = *it;
			const auto& [stanga, dreapta] = productieSelectata;

			// Alege random o pozitie in care se poate aplica o productie
			std::vector<size_t> pozitiiPosibile;
			size_t pozitie = cuvantGenerat.find(stanga);
			while (pozitie != std::string::npos)
			{
				pozitiiPosibile.push_back(pozitie);
				pozitie = cuvantGenerat.find(stanga, pozitie + 1);
			}
			std::uniform_int_distribution<int> distribution2(0, pozitiiPosibile.size() - 1);
			int pozitieInlocuire = pozitiiPosibile[distribution2(eng)];
			std::cout << cuvantGenerat << " => ";
			cuvantGenerat.replace(pozitieInlocuire, stanga.length(), dreapta == "@" ? "" : dreapta);
		}
		std::cout << cuvantGenerat << "\n\n";
	}


	std::cout << "Cuvintele generate sunt: ";
	for (const auto& cuvant : m_cuvinteGenerate)
	{
		std::cout << cuvant << " ";
	}
	std::cout << "\n\n";
	return m_cuvinteGenerate;
}

bool Gramatica::verificare() const
{
	return verificareSeturiVide() && verificareIntersectieVNVT() && verificareSInVN() && verificareProductii() && verificareCaractereProductii() && verificareProductieCuS();
}

bool Gramatica::verificareSeturiVide() const
{
	if (m_VN.empty() || m_VT.empty())
	{
		std::cout << "Gramatica invalida: VN sau VT sunt vide\n";
		return false;
	}
	return true;
}

bool Gramatica::verificareIntersectieVNVT() const
{
	for (const auto neterminal : m_VN)
	{
		if (m_VT.find(neterminal) != m_VT.end())
		{
			std::cout << "Gramatica invalida: caractere din VN se afla in VT\n";
			return false;
		}
	}
	for (const auto terminal : m_VT)
	{
		if (m_VN.find(terminal) != m_VN.end())
		{
			std::cout << "Gramatica invalida: caractere din VT se afla in VN\n";
			return false;
		}
	}
	return true;
}

bool Gramatica::verificareSInVN() const
{
	if (m_VN.find(m_S) == m_VN.end())
	{
		std::cout << "Gramatica invalida : S nu se afla in VN\n";
		return false;
	}
	return true;
}

bool Gramatica::verificareProductii() const
{
	bool areNeterminal;
	for (const auto& [stanga, _] : m_P)
	{
		areNeterminal = false;
		for (const auto neterminal : m_VN)
		{
			if (stanga.find(neterminal) != std::string::npos)
			{
				areNeterminal = true;
			}
		}
		if (areNeterminal == false)
		{
			std::cout << "Gramatica invalida: productie fara neterminal in stanga\n";
			return false;
		}
	}
	return true;
}

bool Gramatica::verificareCaractereProductii() const
{
	for (const auto& [stanga, dreapta] : m_P)
	{
		for (const char caracter : stanga + dreapta)
		{
			if (m_VN.find(caracter) == m_VN.end() && m_VT.find(caracter) == m_VT.end())
			{
				std::cout << "Gramatica invalida: caractere necunoscute in productii\n";
				return false;
			}
		}
	}
	return true;
}

bool Gramatica::verificareProductieCuS() const
{
	bool existaProductieCuS = false;
	for (const auto& [stanga, _] : m_P)
	{
		if (stanga[0] == m_S && stanga.size() == 1)
		{
			existaProductieCuS = true;
		}
	}
	if (existaProductieCuS == false)
		std::cout << "Gramatica invalida : nu exista productie cu doar S in stanga\n";
	return existaProductieCuS;
}

bool Gramatica::esteRegulata() const
{
	for (const auto& [stanga, dreapta] : m_P)
	{
		bool esteNeterminalStanga = stanga.size() == 1 && m_VN.find(stanga[0]) != m_VN.end();
		bool esteTerminalDreapta = dreapta.size() == 1 && m_VT.find(dreapta[0]) != m_VT.end();
		bool esteTerminalNeterminalDreapta = dreapta.size() == 2 && m_VT.find(dreapta[0]) != m_VT.end() && m_VN.find(dreapta[1]) != m_VN.end();
		if (!esteNeterminalStanga)
		{
			std::cout << "Gramatica neregulata: in stanga trebuie sa existe un singur neterminal\n";
			return false;
		}
		if (!(esteTerminalDreapta || esteTerminalNeterminalDreapta || dreapta != "@"))
		{
			std::cout << "Gramatica neregulata: in dreapta trebuie sa existe un terminal / un terminal urmat de neterminal / elementul vid (lambda)\n";
			return false;
		}
		if (dreapta == "@" && stanga[0] == m_S)
		{
			for (const auto& [_, dr] : m_P)
			{
				if (dr.find(m_S) != std::string::npos)
				{
					std::cout << "Gramatica neregulata: simbolul de start nu poate aparea in dreapta productiei daca exista productie S -> @\n";
					return false;
				}
			}
		}
	}
	return true;
}
//
//AutomatFinit Gramatica::transformaInAutomatFinit() const
//{
//	if (!esteRegulata())
//	{
//		std::cout << "Gramatica nu este regulata\n";
//		return AutomatFinit();
//	}
//	AutomatFinit automatFinit;
//	std::set<std::string> stari;
//	std::set<std::string> stariFinale;
//	FunctieTranzitie functieTranzitie;
//	for (const char neterminal : m_VN)
//	{
//		stari.insert({ std::string{ neterminal } });
//	}
//	stari.insert("T");
//	for (const auto& [stanga, dreapta] : m_P)
//	{
//		std::string stare1{ stanga };
//		char simbol = dreapta[0];
//		std::string stare2 = dreapta.size() == 2 ? std::string{ dreapta[1] } : "T";
//		functieTranzitie[{stare1, simbol}].push_back(stare2);
//		if (dreapta.size() == 1)
//		{
//			stariFinale.insert(stare1);
//		}
//	}
//	if (!stariFinale.empty())
//	{
//		stariFinale.insert("T");
//	}
//	automatFinit.setStari(stari);
//	automatFinit.setAlfabet(m_VT);
//	automatFinit.setStareInitiala(std::string{ m_S });
//	automatFinit.setStariFinale(stariFinale);
//	automatFinit.setFunctieTranzitie(functieTranzitie);
//	return automatFinit;
//}