/*
#####################################################################################
#Program name  : Variable.h
#Description   : All the global variable that needed to be declared in pinyin.cpp
#Copyright     : Distribution, modification, reproduction, and copy are strictly prohibited to unauthorized persons.
#Author        : Hotama Christianus Frederick (christianusfrederick@gmail.com)
#Commit date   : December 2018
######################################################################################
*/
#include "pinyin.h"


const int ALPHABET_SIZE = 35;
struct words
{
    string key_hanzi;
    vector <int> hanzi_code;
    double score;
    int end_of_word;
};
struct prediction
{
    double score;
    string hanzi_list;
    vector<int> hanzi_code;
    int end_of_word;
    bool operator<(const prediction& rhs) const
    {
        return score > rhs.score;
    }
};
struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];
    vector <pair<string,string>> pinyin_char;
	// isEndOfWord is true if the node represents
	// end of a word
	bool isEndOfWord;
	vector <prediction> scoring;
	int k;

};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
	struct TrieNode *pNode = new TrieNode;
	pNode->isEndOfWord = false;
    pNode->k=0;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = NULL;

	return pNode;
}


