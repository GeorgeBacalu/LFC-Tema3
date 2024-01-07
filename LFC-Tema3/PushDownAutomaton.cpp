#include "PushDownAutomaton.h"
#include <stack>

PushDownAutomaton::PushDownAutomaton()
    : stari{ {} }, alfabetIntrare{ {} }, alfabetMemorie{ {} },
    stareInitiala{ "" }, simbolStartMemorie{ '\0' }, stariFinale{ {} },
    functieTranzitiePD{ {} } {}

PushDownAutomaton::PushDownAutomaton(const std::set<std::string>& stari, const std::set<char>& alfabetIntrare,
    const std::set<char>& alfabetMemorie, const std::string& stareInitiala,
    char simbolStartMemorie, const std::set<std::string>& stariFinale,
    const FunctieTranzitiePD& functieTranzitiePD)
    : stari{ stari }, alfabetIntrare{ alfabetIntrare }, alfabetMemorie{ alfabetMemorie },
    stareInitiala{ stareInitiala }, simbolStartMemorie{ simbolStartMemorie },
    stariFinale{ stariFinale }, functieTranzitiePD{ functieTranzitiePD } {}

std::set<std::string> PushDownAutomaton::getStari() const
{
    return stari;
}

const std::set<char>& PushDownAutomaton::getAlfabetIntrare() const
{
    return alfabetIntrare;
}

const std::set<char>& PushDownAutomaton::getAlfabetMemorie() const
{
    return alfabetMemorie;
}

const std::string& PushDownAutomaton::getStareInitiala() const
{
    return stareInitiala;
}

char PushDownAutomaton::getSimbolStartMemorie() const
{
    return simbolStartMemorie;
}

std::set<std::string> PushDownAutomaton::getStariFinale() const
{
    return stariFinale;
}

FunctieTranzitiePD PushDownAutomaton::getFunctieTranzitiePD() const
{
    return functieTranzitiePD;
}

void PushDownAutomaton::setStari(const std::set<std::string>& stari)
{
    this->stari = stari;
}

void PushDownAutomaton::setAlfabetIntrare(const std::set<char>& alfabetIntrare)
{
    this->alfabetIntrare = alfabetIntrare;
}

void PushDownAutomaton::setAlfabetMemorie(const std::set<char>& alfabetMemorie)
{
    this->alfabetMemorie = alfabetMemorie;
}

void PushDownAutomaton::setStareInitiala(const std::string& stareInitiala)
{
    this->stareInitiala = stareInitiala;
}

void PushDownAutomaton::setSimbolStartMemorie(char simbolStartMemorie)
{
    this->simbolStartMemorie = simbolStartMemorie;
}

void PushDownAutomaton::setStariFinale(const std::set<std::string>& stariFinale)
{
    this->stariFinale = stariFinale;
}

void PushDownAutomaton::setFunctieTranzitiePD(const FunctieTranzitiePD& functieTranzitiePD)
{
    this->functieTranzitiePD = functieTranzitiePD;
}

bool PushDownAutomaton::verificareAutomat()
{
    if (multimeStariNevida() && alfabetIntrareNevid() && alfabetMemorieNevid() &&
        stareInitialaInMultimeaStari() && stariFinaleInMultimeaStari() &&
        functieTranzitiePDCorecta())
    {
        std::cout << "Automat valid\n\n";
        return true;
    }
    return false;
}

bool PushDownAutomaton::multimeStariNevida()
{
    if (stari.size() == 0)
    {
        std::cout << "Automat nevalid : Multimea de stari este vida\n";
        return false;
    }
    return true;
}

bool PushDownAutomaton::alfabetIntrareNevid()
{
    if (alfabetIntrare.size() == 0)
    {
        std::cout << "Automat nevalid : Alfabetul de intrare este vid\n";
        return false;
    }
    return true;
}

bool PushDownAutomaton::alfabetMemorieNevid()
{
    if (alfabetMemorie.size() == 0)
    {
        std::cout << "Automat nevalid : Alfabetul memoriei push-down este vid\n";
        return false;
    }
    return true;
}

bool PushDownAutomaton::stareInitialaInMultimeaStari()
{
    if (stareInitiala.size() == 0)
    {
        std::cout << "Automat nevalid : Starea initiala este vida\n";
        return false;
    }

    if (stari.find(stareInitiala) == stari.end())
    {
        std::cout << "Automat nevalid : Starea initiala nu se afla in multimea de stari\n";
        return false;
    }
    return true;
}

bool PushDownAutomaton::stariFinaleInMultimeaStari()
{
    if (stariFinale.size() == 0)
    {
        std::cout << "Automat nevalid : Multimea de stari finale este vida\n";
        return false;
    }

    for (const auto& stareFinala : stariFinale)
        if (stari.find(stareFinala) == stari.end())
        {
            std::cout << "Automat nevalid : Starea finala nu se afla in multimea de stari\n";
            return false;
        }
    return true;
}

bool PushDownAutomaton::functieTranzitiePDCorecta()
{
    if (functieTranzitiePD.size() == 0)
    {
        std::cout << "Automat nevalid : Functia de tranzitie push-down este vida\n";
        return false;
    }

    for (const auto& tranzitie : functieTranzitiePD)
    {
        const auto& tuplu = tranzitie.first;
        const auto& stariUrmatoare = tranzitie.second;
        if (stari.find(std::get<0>(tuplu)) == stari.end())
        {
            std::cout << "Automat nevalid : Starea " << std::get<0>(tuplu) << " din functia de tranzitie nu se afla in multimea de stari\n";
            return false;
        }
        if (alfabetIntrare.find(std::get<1>(tuplu)) == alfabetIntrare.end())
        {
            std::cout << "Automat nevalid : Simbolul " << std::get<1>(tuplu) << " din functia de tranzitie nu se afla in alfabetul de intrare\n";
            return false;
        }
        if (alfabetMemorie.find(std::get<2>(tuplu)) == alfabetMemorie.end())
        {
            std::cout << "Automat nevalid : Simbolul " << std::get<2>(tuplu) << " din functia de tranzitie nu se afla in alfabetul memoriei push-down\n";
            return false;
        }
        for (const auto& stare : stariUrmatoare)
            if (stari.find(stare) == stari.end())
            {
                std::cout << "Automat nevalid : Starea " << stare << " din functia de tranzitie nu se afla in multimea de stari\n";
                return false;
            }
    }
    return true;
}

void PushDownAutomaton::printAutomaton() const
{
    std::cout << "PushDownAutomaton = ({ ";
    for (const auto& stare : stari)
        std::cout << stare << " ";
    std::cout << "}, { ";
    for (const auto& simbol : alfabetIntrare)
        std::cout << simbol << " ";
    std::cout << "}, { ";
    for (const auto& simbol : alfabetMemorie)
        std::cout << simbol << " ";
    std::cout << "}, " << stareInitiala << ", " << simbolStartMemorie << ", { ";
    for (const auto& stareFinala : stariFinale)
        std::cout << stareFinala << " ";
    std::cout << "})\n\n";

    std::cout << "functieTranzitiePD :\n";
    for (const auto& tranzitie : functieTranzitiePD)
    {
        const auto& tuplu = tranzitie.first;
        const auto& stariUrmatoare = tranzitie.second;
        for (const auto& stareUrmatoare : stariUrmatoare)
            std::cout << "  (" << std::get<0>(tuplu) << " , " << std::get<1>(tuplu) << " , " << std::get<2>(tuplu) << ") -> " << stareUrmatoare << "\n";
    }
    std::cout << "\n";
}

bool PushDownAutomaton::checkWord(const std::string& cuvant)
{
    std::stack<char> stack;  
    stack.push(simbolStartMemorie); 

    std::string currentState = stareInitiala;
    char currentSymbol;

    for (size_t i = 0; i < cuvant.size(); ++i)
    {
        currentSymbol = cuvant[i];

        auto it = functieTranzitiePD.find({ currentState, currentSymbol, stack.top() });

        if (it != functieTranzitiePD.end())
        {
            
            stack.pop();

            // Push the symbols from the transition onto the stack
            for (const auto& nextState : it->second)
                stack.push(nextState[0]);

            // Update the current state
            currentState = it->second[0];
        }
        else
        {
            // No valid transition found, reject the word
            return false;
        }
    }

    // Check if the final state is one of the accepting states
    return stariFinale.find(currentState) != stariFinale.end() && stack.top() == '\0';
}

bool PushDownAutomaton::isDeterministic() const
{
    for (const auto& state : stari)
    {
        for (const auto& symbol : alfabetIntrare)
        {
            for (const auto& memSymbol : alfabetMemorie)
            {
                // Check if there is more than one transition for the same input
                if (functieTranzitiePD.count({ state, symbol, memSymbol }) > 1)
                    return false;
            }
        }
    }

    return true;
}
std::istream& operator>>(std::istream& in, PushDownAutomaton& pda)
{
    int nrStari, nrElementeAlfabetIntrare, nrElementeAlfabetMemorie, nrStariFinale, nrTranzitii;
    in >> nrStari;

    for (size_t i = 0; i < nrStari; i++)
    {
        std::string stare;
        in >> stare;
        pda.stari.insert(stare);
    }

    in >> nrElementeAlfabetIntrare;
    for (size_t i = 0; i < nrElementeAlfabetIntrare; i++)
    {
        char simbol;
        in >> simbol;
        pda.alfabetIntrare.insert(simbol);
    }

    in >> nrElementeAlfabetMemorie;
    for (size_t i = 0; i < nrElementeAlfabetMemorie; i++)
    {
        char simbol;
        in >> simbol;
        pda.alfabetMemorie.insert(simbol);
    }

    in >> pda.stareInitiala >> pda.simbolStartMemorie >> nrStariFinale;
    for (size_t i = 0; i < nrStariFinale; i++)
    {
        std::string stareFinala;
        in >> stareFinala;
        pda.stariFinale.insert(stareFinala);
    }

    in >> nrTranzitii;
    for (size_t i = 0; i < nrTranzitii; i++)
    {
        std::string stare1, stare2;
        char simbolIntrare, simbolMemorie;
        in >> stare1 >> simbolIntrare >> simbolMemorie >> stare2;
        pda.functieTranzitiePD[{stare1, simbolIntrare, simbolMemorie}].push_back(stare2);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const PushDownAutomaton& pda)
{
    out << "PushDownAutomaton = ({ ";
    for (const auto& stare : pda.stari)
        out << stare << " ";
    out << "}, { ";
    for (const auto& simbol : pda.alfabetIntrare)
        out << simbol << " ";
    out << "}, { ";
    for (const auto& simbol : pda.alfabetMemorie)
        out << simbol << " ";
    out << "}, " << pda.stareInitiala << ", " << pda.simbolStartMemorie << ", { ";
    for (const auto& stareFinala : pda.stariFinale)
        out << stareFinala << " ";
    out << "})\n\n";

    out << "functieTranzitiePD :\n";
    for (const auto& tranzitie : pda.functieTranzitiePD)
    {
        const auto& tuplu = tranzitie.first;
        const auto& stariUrmatoare = tranzitie.second;
        for (const auto& stareUrmatoare : stariUrmatoare)
            out << "  (" << std::get<0>(tuplu) << " , " << std::get<1>(tuplu) << " , " << std::get<2>(tuplu) << ") -> " << stareUrmatoare << "\n";
    }
    out << "\n";

    return out;
}