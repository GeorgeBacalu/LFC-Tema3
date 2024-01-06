#pragma once

//#include "AutomatFinit.h"
#include <random>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using Productie = std::pair<std::string, std::string>;

class Gramatica {
	std::set<char> m_VN;
	std::set<char> m_VT;
	std::set<Productie> m_P;
	char m_S;
	std::set<std::string> m_cuvinteGenerate;

	bool verificareSeturiVide() const;
	bool verificareIntersectieVNVT() const;
	bool verificareSInVN() const;
	bool verificareProductii() const;
	bool verificareCaractereProductii() const;
	bool verificareProductieCuS() const;
public:
	Gramatica();
	Gramatica(const std::set<char>& VN, const std::set<char>& VT, const std::set<Productie>& P, char S);
	~Gramatica() = default;
	friend std::istream& operator>>(std::istream& in, Gramatica& gramatica);
	friend std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica);

	const std::set<char>& GetVN() const;
	void SetVN(const std::set<char>& VN);
	const std::set<char>& GetVT() const;
	void SetVT(const std::set<char>& VP);
	const std::set<Productie>& GetP() const;
	void SetP(const std::set<Productie>& P);
	char GetS() const;
	void SetS(char S);

	std::set<std::string> generare(int nrCuvinte);
	bool verificare() const;
	bool esteRegulata() const;
	//AutomatFinit transformaInAutomatFinit() const;
};