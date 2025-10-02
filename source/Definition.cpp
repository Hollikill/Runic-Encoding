#include <string>
#include "Definition.h"

Definition::Definition(int newMaximumSymbols) : maximumSymbols(newMaximumSymbols) {
    word = "";
    int* newSymbolIDs = new int[newMaximumSymbols];
    length = 0;
}
Definition::Definition(int newMaximumSymbols, std::string newWord) : maximumSymbols(newMaximumSymbols), word(newWord) {
    int* newSymbolIDs = new int[newMaximumSymbols];
    length = 0;
}
Definition::Definition(int* newSymbolIDs, int newLength, int newMaximumSymbols, std::string newWord) : symbolIDs(newSymbolIDs), length(newLength), maximumSymbols(newMaximumSymbols), word(newWord) {};

void Definition::addSymbol(int id) {
    if (length != maximumSymbols) {
        symbolIDs[length] = id;
        length++;
    }
}
void Definition::setWord(std::string newWord) {
    word = newWord;
}

int Definition::getLength() const {
    return length;
}
int Definition::getMaximumSymbols() const {
    return maximumSymbols;
}
int* Definition::getSymbolIDs() const {
    return symbolIDs;
}
std::string Definition::getWord() const {
    return word;
}

Definition::~Definition() {
    delete symbolIDs;
    symbolIDs = nullptr;
}