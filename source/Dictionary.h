#ifndef DICTIONARY_
#define DICTIONARY_

#include <map>
#include <vector>
#include "Definition.h"
#include "Symbol.h"

class Dictionary
{
public:
    // dictonary facts
    int compositeSymbolMaximum;
    int totalWords;

    // generation facts, only useful for display
    int version;
    int seed;

    // data structures for words and symbols
    std::map<int, Symbol*> symbols;
    std::vector<Definition*> definitions;

    Dictionary();
    //Dictionary(std::string filename);

    bool hasCompositeWords() const;

    void setTotalWords(int);
    //void setSeed(int);

    void addSymbol(Symbol* newSymbol);
    //bool symbolInDictionary(Symbol* symbol) const;

    Definition* addNewDefinition();

    // 'search' functions
    //bool wordDefined(std::string word) const;
    //bool symbolCombinationDefined(int* symbolIDs, int length) const;

    //Definition* getDefinitionByWord(std::string word) const;
    //Definition* getDefinitionBySymbolCombination(int* symbolIDs, int length);

    // this is gonna take a while...
    //void serializeDictionary(std::string filename);

    ~Dictionary();
};

#include "Dictionary.cpp"

#endif