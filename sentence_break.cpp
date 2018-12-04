/*
#####################################################################################
#Program name  : sentence_break.cpp
#Description   : File contains the function to break the chinese sentence into words
#Copyright     : Distribution, modification, reproduction, and copy are strictly prohibited to unauthorized persons.
#Author        : Hotama Christianus Frederick (christianusfrederick@gmail.com)
#Commit date   : December 2018
######################################################################################
#Function input: sentence in hanzi
#Function output: list of the words
*/

#include "pinyin.h"
vector <string> sentence_break(struct TrieNode *root, vector <int> key_pinyin, string hanzi)
{
    //breaking the sentence into small words
    struct TrieNode *pCrawl;
    int pointer=0;
    string dummy=hanzi.substr(pointer,3);
    string prediction;
    int i=0;
    int index;
    bool ketemu=false;

    vector <string> final_pred;
    pCrawl=root;
    while (i<key_pinyin.size())
    {
        index=key_pinyin[i];
        if (pCrawl->children[index])
        {
            pCrawl=pCrawl->children[index];
            i=i+1;
            index=key_pinyin[i];
            if (pCrawl->children[index])
            {

                pCrawl=pCrawl->children[index];
                i=i+1;
                ketemu=false;

                for (int j=0;j<pCrawl->scoring.size();j++)
                {
                    if (dummy==pCrawl->scoring[j].hanzi_list)
                    {
                        prediction=dummy;
                        pointer=pointer+3;
                        dummy=dummy+hanzi.substr(pointer,3);
                        ketemu=true;
                        break;
                    }
                }

                if(ketemu==false)
                {
                    final_pred.push_back(prediction);
                    dummy=hanzi.substr(pointer,3);
                    i=i-2;
                    pCrawl=root;

                }
            }
            else
            {
                pCrawl=root;
                i=i-1;
            }
        }
        else
        {

            pCrawl=root;
        }

    }
    final_pred.push_back(dummy);
    return final_pred;
}

