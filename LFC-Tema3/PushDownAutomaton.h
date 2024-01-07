#pragma once

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using FunctieTranzitiePD = std::map<std::tuple<std::string, char, char>, std::vector<std::string>>;

class PushDownAutomaton
{
private:
    std::set<std::string> stari;
    std::set<char> alfabetIntrare;
    std::set<char> alfabetMemorie;
    std::string stareInitiala;
    char simbolStartMemorie;
    std::set<std::string> stariFinale;
    FunctieTranzitiePD functieTranzitiePD;

public:
    PushDownAutomaton();
    PushDownAutomaton(const std::set<std::string>& stari, const std::set<char>& alfabetIntrare,
        const std::set<char>& alfabetMemorie, const std::string& stareInitiala,
        char simbolStartMemorie, const std::set<std::string>& stariFinale,
        const FunctieTranzitiePD& functieTranzitiePD);
    ~PushDownAutomaton() = default;

    friend std::istream& operator>>(std::istream& in, PushDownAutomaton& pda);
    friend std::ostream& operator<<(std::ostream& out, const PushDownAutomaton& pda);

    std::set<std::string> getStari() const;
    const std::set<char>& getAlfabetIntrare() const;
    const std::set<char>& getAlfabetMemorie() const;
    const std::string& getStareInitiala() const;
    char getSimbolStartMemorie() const;
    std::set<std::string> getStariFinale() const;
    FunctieTranzitiePD getFunctieTranzitiePD() const;

    void setStari(const std::set<std::string>& stari);
    void setAlfabetIntrare(const std::set<char>& alfabetIntrare);
    void setAlfabetMemorie(const std::set<char>& alfabetMemorie);
    void setStareInitiala(const std::string& stareInitiala);
    void setSimbolStartMemorie(char simbolStartMemorie);
    void setStariFinale(const std::set<std::string>& stariFinale);
    void setFunctieTranzitiePD(const FunctieTranzitiePD& functieTranzitiePD);

    bool verificareAutomat();
    void printAutomaton() const;
    bool checkWord(const std::string& cuvant);
    bool isDeterministic() const;

private:
    bool multimeStariNevida();
    bool alfabetIntrareNevid();
    bool alfabetMemorieNevid();
    bool stareInitialaInMultimeaStari();
    bool stariFinaleInMultimeaStari();
    bool functieTranzitiePDCorecta();
};

