#ifndef DEFINITION_
#define DEFINITION_

#include <string>

class Definition {
private:
    int* symbolIDs;
    int length;
    int maximumSymbols;
    std::string word;
public:
    Definition(int newMaximumSymbols);
    Definition(int newMaximumSymbols, std::string newWord);
    Definition(int* symbolIDs, int length, int maximumSymbols, std::string word);

    void addSymbol(int id);
    void setWord(std::string word);

    int getLength() const;
    int getMaximumSymbols() const;
    int* getSymbolIDs() const;
    std::string getWord() const;

    ~Definition();
};

#include "Definition.cpp"

#endif