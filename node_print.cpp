#include "pinyin.h"

vector<pair<int,vector<prediction>>> node_print(struct TrieNode *root,int level)
{
    //function to record the trie into the string-int variable>
    vector<pair<int,vector<prediction>>> savefile,new_buffer;
    std::string consonant[24] = {"b","p","m","f","d","t","n","l","g","k","h","j","q","x","zh","ch","sh","r","z","c","s","w","y","?"};
	std::string vocal[34] ={"a","e","ai","ei","ao","ou","an","en","ang","eng","er","v","i","in","ing","ong","u","ue","uan","un","o","ia","ie","iao","iu","ian","iang","iong","uang","ua","uo","uai","ui"};
    struct TrieNode *pCrawl = root;
    struct TrieNode *pCrawl_const;
    struct TrieNode *pCrawl_vocal;
	std::string buffer;
	for (int i=0;i<=23;i++)
    {
        if (pCrawl->children[i])
        {
            pCrawl_const=pCrawl->children[i];
            for (int j=0;j<=32;j++)
            {
                if (pCrawl_const->children[j])
                {
                    pCrawl_vocal=pCrawl_const->children[j];
                    buffer=consonant[i]+vocal[j];
                    savefile.push_back(make_pair(level,pCrawl_vocal->scoring));
                    new_buffer=node_print(pCrawl_vocal,level+1);
                    for (int i=0;i<new_buffer.size();i++)
                    {
                        savefile.push_back(new_buffer[i]);
                    }
                }

            }
        }

    }
    return savefile;

}

