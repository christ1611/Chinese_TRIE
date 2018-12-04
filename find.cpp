/*
#####################################################################################
#Program name  : find.cpp
#Description   : File contains the function to find:
                 -The hanzi from the given pinyin (find_hanzi)
                 -The code of the hanzi to build the model (find_hanzi_code)
                 -Find pinyin from hanzi (find_pinyin)
                 - Other finding purpose (find)
#Copyright     : Distribution, modification, reproduction, and copy are strictly prohibited to unauthorized persons.
#Author        : Hotama Christianus Frederick (christianusfrederick@gmail.com)
#Commit date   : December 2018
######################################################################################
#Function input: sentence in hanzi
#Function output: list of the words
*/
#include "pinyin.h"
int find(std::string arr[], int len, std::string seek)
{
    //search the hanzi index
    for (int i = 0; i < len; ++i)
    {
        if (arr[i] == seek) return i;
    }
    return -1;
}
int find_hanzi(vector <string> arr, int len, std::string seek)
{
    for (int i = 0; i < len; ++i)
    {
        if (arr[i] == seek) return i;
    }
    return -1;
}
int find_hanzi_code(struct TrieNode *root, vector <int> current_pinyin, std::string hanzi)
{
    bool ketemu=false;
    struct TrieNode *pCrawl=root;
    std::string consonant[24] = {"b","p","m","f","d","t","n","l","g","k","h","j","q","x","zh","ch","sh","r","z","c","s","w","y","?"};
	std::string vocal[34] ={"a","e","ai","ei","ao","ou","an","en","ang","eng","er","v","i","in","ing","ong","u","ue","uan","un","o","ia","ie","iao","iu","ian","iang","iong","uang","ua","uo","uai","ui"};
    string dummy;
    pCrawl=pCrawl->children[current_pinyin[0]];
    pCrawl=pCrawl->children[current_pinyin[1]];
    for (int i=0;i<pCrawl->scoring.size();i++)
    {
        dummy = pCrawl->scoring[i].hanzi_list;

        if  (dummy==hanzi)
        {
            ketemu=true;
            return pCrawl->scoring[i].hanzi_code[0];
            break;
        }

    }
    if (ketemu==false)
    {
        for (int i=0;i<root->pinyin_char.size();i++)
        {
            dummy=root->pinyin_char[i].second;
            if (dummy==hanzi)
            {
                ketemu==true;
                return i;
            }
        }
    }
    if (ketemu==false)
        return root->pinyin_char.size();
}


vector <int> find_pinyin(vector<pair<string,string>> arr, int len, std::string seek)
{
    //function to find the correspondence pinyin based on the hanzi input
    //note the hanzi should be registered in the dictionary.txt
    //only for ubuntu because windows has different encoding system
    string output,dummy_vocal,dummy_const;
    vector <int> pinyin,hanzi_code;
    std::string con_consonant[24] = {"b","p","m","f","d","t","n","l","g","k","h","j","q","x","zh","ch","sh","r","z","c","s","w","y","?"};
	std::string con_vocal[34] ={"a","e","ai","ei","ao","ou","an","en","ang","eng","er","v","i","in","ing","ong","u","ue","uan","un","o","ia","ie","iao","iu","ian","iang","iong","uang","ua","uo","uai","ui"};
    int pointers=0;
    int mysize=seek.length();
    while (pointers<mysize)
    {
        //if we want to apply it in windows, this shoul be changed
        string mydummy=seek.substr(pointers,3);
        output="";
        for (int i = 0; i < len; ++i)
        {
            if (arr[i].second == mydummy)
            {
                output=arr[i].first;
                hanzi_code.push_back(i);
                break;
            }
        }

        if (output[0]=='a'||output[0]=='e'||output[0]=='o')
        {
            dummy_const="?";
            dummy_vocal=output;
        }
        else
        {
            if ((output[0]=='z'||output[0]=='c'||output[0]=='s') && (output[1]=='h'))
            {
                dummy_const= output.substr(0,2);
                dummy_vocal= output.substr(2);
            }
            else
            {
                dummy_const= output.substr(0,1) ;
                dummy_vocal= output.substr(1);
            }
        }
        pinyin.push_back(find(con_consonant, 24,dummy_const));
        pinyin.push_back(find(con_vocal, 33,dummy_vocal));
        pointers=pointers+3;
    }
    return pinyin;
}

pair<bool,vector<prediction>> search_next_word(struct TrieNode *root, vector <int> pinyin, string hanzi)
{
    //fucntion to search the next key
    pair<bool,vector<prediction>> results;
    vector<prediction>tmp_dummy;
    struct TrieNode *pCrawl = root;
    struct TrieNode *current_const;
    struct TrieNode *current_vocal;
    int panjang=hanzi.length();
    bool exist;
    exist=true;
    for (int i=0;i<=pinyin.size()-1;i++)
    {
        int dummy=pinyin[i];
        if (pCrawl->children[dummy])
        {
            pCrawl=pCrawl->children[dummy];
        }
        else
        {
            exist=false;
            break;
        }
    }
    for (int i=0;i<24;i++)
    {
        if (pCrawl->children[i])
        {
            current_const=pCrawl->children[i];
            for (int j=0;j<33;j++)
            {
                if (current_const->children[j])
                {
                    current_vocal=current_const->children[j];
                    for (int k=0;k<current_vocal->scoring.size();k++)
                        {
                            string dummy_string=current_vocal->scoring[k].hanzi_list;
                            dummy_string=dummy_string.substr(0,panjang);
                            if (dummy_string==hanzi)
                                tmp_dummy.push_back(current_vocal->scoring[k]);
                        }

                }
            }
        }
    }
    sort(tmp_dummy.begin(),tmp_dummy.end());
    results=make_pair(exist,tmp_dummy);

    return results;
}
