#include "Definition.h"

Definition::Definition(int newMaximumSymbols) {
    Definition(newMaximumSymbols, "");
}
Definition::Definition(int newMaximumSymbols, std::string newWord) {
    int* newSymbolIDs = new int[newMaximumSymbols];
    Definition(newSymbolIDs, 0, newMaximumSymbols, newWord);
}
Definition::Definition(int* newSymbolIDs, int newLength, int newMaximumSymbols, std::string newWord) : symbolIDs(symbolIDs), length(length), maximumSymbols(maximumSymbols), word(word) {};

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