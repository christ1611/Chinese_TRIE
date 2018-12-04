/*
#####################################################################################
#Program name  : insert.cpp
#Description   : File contains the function to:
                 -Train the TRIE from the sample snetence/model
                 -Added the new word
                 -Updating the prediction score of the existing words
#Copyright     : Distribution, modification, reproduction, and copy are strictly prohibited to unauthorized persons.
#Author        : Hotama Christianus Frederick (christianusfrederick@gmail.com)
#Commit date   : December 2018
######################################################################################
#Function input: word in hanzi
#Function output: Notification whether the word is added/updated
*/

#include "pinyin.h"


void insert(struct TrieNode *root, string key_hanzi, vector<int> key_pinyin, double score, bool updating, int dictionary)
{
    //function to insert/updating the score
    // If not present, inserts key into trie
    // If the key is prefix of trie node, just
    std::string con_consonant[24] = {"b","p","m","f","d","t","n","l","g","k","h","j","q","x","zh","ch","sh","r","z","c","s","w","y","?"};
	std::string con_vocal[34] ={"a","e","ai","ei","ao","ou","an","en","ang","eng","er","v","i","in","ing","ong","u","ue","uan","un","o","ia","ie","iao","iu","ian","iang","iong","uang","ua","uo","uai","ui","ve"};
	struct TrieNode *pCrawl = root;
	prediction dummy;
	int tmp,tmp2;
	vector <string> dummy_hanzi;
	std::string buffer;
	int panjang;
	panjang=0;
	for (int i = 0; i < (key_hanzi.length()/3)*2; i++)
	{
	    int index=key_pinyin[i];
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();
        pCrawl = pCrawl->children[index];
        if ((i % 2)==1)
        {
            panjang=panjang+3;
            int ketemu;
            dummy.score=score;
            dummy.hanzi_list=key_hanzi.substr(0,panjang);
            if (dummy.hanzi_list==key_hanzi)
                dummy.end_of_word=1;
            else
                dummy.end_of_word=0;
            dummy_hanzi.clear();
            for (int j=0; j<pCrawl->scoring.size();j++)
                dummy_hanzi.push_back(pCrawl->scoring[j].hanzi_list);
            ketemu=find_hanzi(dummy_hanzi,dummy_hanzi.size(),dummy.hanzi_list);
            if (ketemu==-1)
            {

                vector <int> current_pinyin_vec;
                current_pinyin_vec.push_back(key_pinyin[i-1]);
                current_pinyin_vec.push_back(key_pinyin[i]);
                int codes=find_hanzi_code(root,current_pinyin_vec,key_hanzi.substr(panjang-3,3));
                dummy.hanzi_code.push_back(codes);
                if (codes==root->pinyin_char.size())
                {
                    std::string pinyin_dum;
                    pinyin_dum=con_consonant[key_pinyin[i-1]]+con_vocal[key_pinyin[i]];
                    root->pinyin_char.push_back(make_pair(pinyin_dum,key_hanzi.substr(panjang-3,3)));
                }


                pCrawl->scoring.push_back(dummy);
                pCrawl->isEndOfWord = true;
                if (updating)
                    cout<<"the phrase "<<key_hanzi.substr(0,panjang)<<" is added "<<endl;
            }
            else
            {
                if (updating)
                    cout<<"the score of "<<key_hanzi.substr(0,panjang)<<" change from "<<pCrawl->scoring[ketemu].score;
                pCrawl->scoring[ketemu].score=pCrawl->scoring[ketemu].score+score;
                if (updating)
                    cout<<" to "<<pCrawl->scoring[ketemu].score<<endl;
                dummy.hanzi_code=pCrawl->scoring[ketemu].hanzi_code;
            }
            sort(pCrawl->scoring.begin(),pCrawl->scoring.end());
            //normalization into range 0..10
            if (pCrawl->scoring[0].score>10)
            {
                for (int k=0; k<pCrawl->scoring.size();k++)
                    pCrawl->scoring[k].score=pCrawl->scoring[k].score/pCrawl->scoring[0].score*10;
            }

        }

	}
	// mark last node as leaf
    return;
}
