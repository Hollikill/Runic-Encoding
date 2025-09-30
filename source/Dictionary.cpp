#include <map>
#include <vector>
#include "Dictionary.h"

Dictionary::Dictionary() {
    compositeSymbolMaximum = 1;
    seed = 0;
    totalWords = 0;
    version = 0; // update each new release LAST: prealpha 9.25.2025
}

bool Dictionary::hasCompositeWords() const { return (compositeSymbolMaximum > 1); }
void Dictionary::setTotalWords(int newTotalWords) { totalWords = newTotalWords; }
//void Dictionary::setSeed(int newSeed) { seed = newSeed; }

void Dictionary::addSymbol(Symbol* newSymbol) {
    symbols.emplace(symbols.size(), newSymbol);
}

Definition* Dictionary::addNewDefinition() {
    Definition* newDefinition = new Definition(compositeSymbolMaximum);
    return newDefinition;
}

Dictionary::~Dictionary() {
    while (!symbols.empty()) {
        int lowbound = symbols.lower_bound(-1)->first;
        delete symbols.at(lowbound);
        symbols.at(lowbound) = nullptr;
        symbols.erase(lowbound);
    }
    while(!definitions.empty()) {
        delete definitions.back();
        definitions.back() = nullptr;
        definitions.pop_back();
    }
}