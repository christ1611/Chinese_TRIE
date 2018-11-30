/*
#####################################################################################
#Program name  : TRIE model for chineze-Hanzi Pinyin Prediction
#Description   : Make the TRIE based on chinese Pinyin character
#                Real time training model of chinese-hanzi prediction model
#                Training The TRIE to understand the hanzi-Pinyin Relation
#                Training the TRIE to give the prediction for the next given hanzi
#                Training the TRIE to give the prediction based on the uncompleted pinyin
#                   (ex:nih -> 你好/nihao)
#                Training the TRIE to give the prediction based on given consonant only
#                   (ex:ALBB -> 阿里巴巴(Alibaba))
#                Build a adaptive TRIE Initizilation based on the Sogou dataset
#                Updating the TRIE and reccomendation score based on the user input or preferences
#                Weighting and sorting the hanzi prediction
#
#Author        : Hotama Christianus Frederick
######################################################################################
*/
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <tuple>
#include <errno.h>
#include <vector>
#include <sstream>
#include <codecvt>
#include <iconv.h>
#include <numeric>
#include <algorithm>
#include <time.h>
#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
using namespace std;

const int ALPHABET_SIZE = 35;
//search the directory
std::string GetCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

// trie node
bool sortbysec(const pair<int,int> &a, const pair<int,int> &b)
{
    return (a.second < b.second);
}
struct words
{
    string key_hanzi;
    vector <int> hanzi_code;
    double score;
};
struct prediction
{
    double score;
    string hanzi_list;
    vector<int> hanzi_code;
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
void readTrie(istream& is, vector<prediction> &vec)
{
    typename vector<prediction>::size_type size = 0;
    is.read((char*)&size, sizeof(size));
    vec.resize(size);
    is.read((char*)&vec, vec.size() * sizeof(prediction));
}
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

void insert2(struct TrieNode *root, vector<words> hanzi, vector<int> key_pinyin)
{
    //insert from the model file
	struct TrieNode *pCrawl = root;
	prediction dummy;
	int tmp,tmp2;
	vector <string> dummy_hanzi;
	std::string buffer;
	int panjang;
	prediction scores;
	double feedback;
	string key_hanzi =hanzi[0].key_hanzi;
	for (int i = 0; i < (key_hanzi.length()/3)*2; i++)
	{
	    int index=key_pinyin[i];
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
	}

	for (int i = 0; i<hanzi.size(); i++)
	{
	    scores.hanzi_code=hanzi[i].hanzi_code;
	    scores.hanzi_list=hanzi[i].key_hanzi;
	    scores.score=hanzi[i].score;

	    pCrawl->scoring.push_back(scores);
	}
	// mark last node as leaf
    return;
}

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


vector<pair<int,vector<prediction>>> cetak(struct TrieNode *root,int level)
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
                    new_buffer=cetak(pCrawl_vocal,level+1);
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
int find(std::string arr[], int len, std::string seek)
{
    //search the hanzi index
    for (int i = 0; i < len; ++i)
    {
        if (arr[i] == seek) return i;
    }
    return -1;
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

std::pair<vector<prediction>,string> search2(struct TrieNode *root, std::string key, bool chk_const)
{
    struct TrieNode *current_const;
    struct TrieNode *current_vocal;
    struct TrieNode *dummy_vocal;
    std::string con_consonant[24] = {"b","p","m","f","d","t","n","l","g","k","h","j","q","x","zh","ch","sh","r","z","c","s","w","y","?"};
	std::string con_vocal[33] ={"a","e","ai","ei","ao","ou","an","en","ang","eng","er","v","i","in","ing","ong","u","ue","uan","un","o","ia","ie","iao","iu","ian","iang","iong","uang","ua","uo","uai","ui"};
    std::pair<vector <prediction>,string> finalx,sub_finalx;
    vector <prediction> prediction_const;
    std::string constanta,vocal,dummy,dummy_key,new_const;
    std::string new_key,nk;
    vector <prediction> parent_prediction;
    int i=0;
    int poin;
    int int_constanta,int_vocal,int_dummy,int_new_const;
    constanta="/";
    for (poin=0;poin<(root->scoring.size());poin++)
    {
        parent_prediction.push_back(root->scoring[poin]);
    }
    //search constanta
    if (key[0]=='a'||key[0]=='e'||key[0]=='o')
    {
            constanta="?";
            new_key=key;
    }
    else
    {
        if ((key[0]=='z'||key[0]=='c'||key[0]=='s') && (key[1]=='h'))
        {
			constanta= key.substr(0,2);
			new_key= key.substr(2);
		}
		else
		{
			constanta= key[0] ;
            new_key= key.substr(1);
		}

    }
    int_constanta=find(con_consonant,24,constanta);
    if (root->children[int_constanta] && int_constanta!=-1 )
    {
        current_const=root->children[int_constanta];
    }
    else
    {
        constanta="/";
    }

    //search vocal
    if (constanta=="/")
    {
        new_key=key;
        finalx=std::make_pair(parent_prediction,new_key);
    }
    else
    {
        i=0;
        dummy="";
        vocal="";
        while (i<=new_key.length())
        {
            dummy=dummy+new_key[i];
            int_dummy=find(con_vocal,33,dummy);

            if (current_const->children[int_dummy] && int_dummy!=-1)
            {
                vocal=dummy;
                int_vocal=int_dummy;
            }
            i=i+1;
        }
        new_key=new_key.substr(vocal.length());
        //Making prediction start
        if (vocal!="")
        {
            //.case vocal is found
            current_vocal=current_const->children[int_vocal];

            if (new_key[0]=='-')
                new_key=new_key.substr(1);
            finalx=search2(current_vocal,new_key,true);
        }
        else
        {
            //vocal is not found
            if (new_key[0]!='a'&&new_key[0]!='u'&&new_key[0]!='i'&&new_key[0]!='e'&&new_key[0]!='o')
            {

                if (new_key[0]=='-')
                    new_key=new_key.substr(1);
                dummy_key=new_key;
                if (dummy_key[0]=='a'||dummy_key[0]=='e'||dummy_key[0]=='o')
                    new_const="?";
                else
                {
                    if (dummy_key[1]=='h' && (dummy_key[0]=='z'||dummy_key[0]=='s'||dummy_key[0]=='c'))
                        new_const=dummy_key[0]+dummy_key[1];
                    else
                        new_const=dummy_key[0];

                }

                bool exist;
                exist=false;
                dummy_key=new_key;
                vector <prediction> pred_dummy,pred_constanta_only;
                for (i=1;i<33;i++)
                {
                    //case of the constanta prediciton
                    if (current_const->children[i])
                    {

                        dummy_vocal=current_const->children[i];
                        for (int j=0;j<dummy_vocal->scoring.size();j++)
                        {

                            pred_dummy.push_back(dummy_vocal->scoring[j]);
                        }

                        int_new_const=find(con_consonant,24,new_const);
                        if ((dummy_vocal->children[int_new_const]) || (new_const=="/"))
                        {

                            exist=true;

                            sub_finalx=search2(dummy_vocal,dummy_key,true);
                            std::string tmp_key =sub_finalx.second;

                            if (new_key.length() >tmp_key.length())
                            {
                                 new_key=tmp_key;
                            }
                            if (tmp_key==new_key)
                                for (int j=0;j<sub_finalx.first.size();j++)
                                {

                                    pred_constanta_only.push_back(sub_finalx.first[j]);
                                }
                            finalx=std::make_pair(pred_constanta_only,new_key);
                        }
                    }
                }
                if (exist==false)
                {
                    //seems the prediction is not found, recommend to the main system to break the prediction (return the score of initial);
                    finalx=std::make_pair(pred_dummy,dummy_key);
                }
            }
            else
            {
                if (constanta!="?")
                    finalx=std::make_pair(parent_prediction,key);
            }
        }
    }
    //return the prediction list(left variable), and also the key that haven't been yet converted (right variable)
    return finalx;
}
vector <prediction> search(struct TrieNode *root, string key)
{
	struct TrieNode *pCrawl = root;
	std::pair<vector<prediction>,string> varcoba;
    vector <prediction> predict;
    std::string old_key;
    string new_key=key+"//";
    int i=0;
    while (new_key !="//")
    {
        old_key=new_key;
        varcoba=search2(pCrawl,new_key,false);
        new_key=varcoba.second;

        if (new_key==old_key)
        {
            new_key=new_key[0]+"-"+new_key.substr(1);
        }
        else
        {
            old_key=old_key.substr(0,(old_key.length()-new_key.length()));
            predict=varcoba.first;
            sort(predict.begin(),predict.end());
            cout<<old_key<<": ";
            for(int i=0;i<predict.size();i++)
                if (i<20)
                    cout<<predict[i].hanzi_list<<" "<<predict[i].score<<" ";
        }
        cout<<endl;
    }
	return predict;
}

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

// User defined function that returns sum of
// arr[] using accumulate() library function.
int arraySum(long a[], int n)
{
    int initial_sum  = 0;
    return accumulate(a, a+n, initial_sum);
}
// Driver
int main()
{
	// Input keys (use only 'a' through 'z'
	// and lower case)

	//initialization the list of vocal and consonant
	std::string con_consonant[24] = {"b","p","m","f","d","t","n","l","g","k","h","j","q","x","zh","ch","sh","r","z","c","s","w","y","?"};
	std::string con_vocal[34] ={"a","e","ai","ei","ao","ou","an","en","ang","eng","er","v","i","in","ing","ong","u","ue","uan","un","o","ia","ie","iao","iu","ian","iang","iong","uang","ua","uo","uai","ui","ve"};
	string dummy_vocal,dummy_const;
	fstream myReadFile;
	std::string file_name;
	std::string output,line;
	std::string starray[10000];
	vector <prediction> d;
	std::string masuk="";
	int tipe;
	vector <int> listpinyin;
	char** hanzi;
	vector <int> pinyin;
	char dump[200];
	double doc_score[10000],maxs;
	int i,j,k,a;
	double maximal,tmp;
	int char_code[10000];
	DIR *dir;
	wchar_t wc;
	int train_model;
	struct dirent *ent;
	struct TrieNode *root = getNode();
	const string mom_directory=GetCurrentWorkingDir() ;
	const string directory2= mom_directory + "/Sample/";
	const string directory = mom_directory+ "/Sample";
	char *p;
	clock_t t1,t2;
	t1=clock();
    //initialization, make the first level of hanzi
    root->pinyin_char.push_back(make_pair("0",""));
    for (i=0;i<24;i++)
    {
        root->children[i] =getNode();
        struct TrieNode *pCrawl=root->children[i];
        for (j=0;j<34;j++)
            pCrawl->children[j] =getNode();
    }

	t2=clock();
	const string file_loc =mom_directory + "/model.txt";
	i=0;
	maxs=0;

	maximal=0;
    cout<<endl;
	cout<<"type 0: train from Sample sentence, 1:train from model"<<endl;
	cin>>train_model;
	if (train_model==0)
    {
        //read and train the model from sogou file
        if ((dir = opendir (directory.c_str())) != NULL)
        {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL)
            {
                output=ent->d_name;
                starray[i]=directory2+output;
                j=0;
                while ((output[j]) != '_')
                {
                    j=j+1;
                }
                if (j<output.length())
                {
                    doc_score[i]=stod(output.substr(0,j));
                    if (doc_score[i]>maxs)
                        maxs=doc_score[i];
                    i=i+1;
                }

            }
            closedir (dir);
        }
        else
        {
  		/* could not open directory */
            perror ("Please copy the sampple file to the folder");
        }
        j=i;
        line="";
        k=0;
        maxs=maxs*141;
        for (int i=0;i<j-1; i++)
        {

            strcpy(dump,starray[i].c_str());
            myReadFile.open(dump);
            if (myReadFile.is_open() && doc_score[i]>0)
            {
                if (i%500==0)
                    cout<<"training completed for "<<i<<" files"<<endl;
                while (myReadFile >> output)
                {
                    if(output[0]>='a'&& output[0]<='z')
                    {
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
                                dummy_const= output[0] ;
                                dummy_vocal= output.substr(1);
                            }

                        }
                    pinyin.push_back(find(con_consonant, 24,dummy_const));
                    pinyin.push_back(find(con_vocal, 34,dummy_vocal));

                    }
                    else
                    {
                        int panjang=output.length()/3;
                        if (doc_score[i]>0)
                        {
                            double scores = round( doc_score[i]/maxs * 100000000.0 ) / 100000000.0;
                            if (scores==0)
                                scores=0.00001;

                            if (panjang==pinyin.size()/2)
                            {
                                insert(root, output,pinyin,scores,false,0);
                            }

                            if (tmp>maximal)
                                maximal=tmp;
                            pinyin.clear();
                        }

                    }

                }

                myReadFile.close();
            }
        }
        cout<<"training all "<<j <<"files completed"<<endl;

    }
    else
    {
        //read from the model.txt, model format should be <level of node><length of the prediction [<hanzi code><prediction score>...]
        ifstream myfile;
        string mydum_str,hanzi_dummy;
        double pred_score;
        vector <string> read_dum;
        myfile.open ("dictionary.txt",fstream::app);
        bool hanzis =false;
        string pinyins;
        while (myfile >> mydum_str)
        {
            if (hanzis==false)
            {
                pinyins=mydum_str;
                hanzis=true;
            }
            else
            {
                hanzis=false;
                root->pinyin_char.push_back(make_pair(pinyins,mydum_str));
                listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),mydum_str);
                insert(root,mydum_str,listpinyin,1,false,0);
            }
        }
        myfile.close();
        myfile.open (file_loc,fstream::app);
        while (myfile >> mydum_str)
        {
            read_dum.push_back(mydum_str);
        }
        int i=0;
        int a=0;
        int panjang_bilangan,panjang_pinyin;
        while (i<read_dum.size())
        {


            panjang_pinyin= std::stoi(read_dum[i]);
            i=i+1;
            a=a+1;
            if (a%500000==0)
                cout<<"Reading from the line "<<a<<endl;
            panjang_bilangan=std::stoi(read_dum[i]);
            i=i+1;
            vector <words> hanzi_list;
            hanzi_list.clear();
            words dummy_words;
            for (int j=0;j<panjang_bilangan;j++)
            {
                hanzi_dummy="";
                vector <int> hanzi_code;
                for (int k=0;k<panjang_pinyin;k++)
                {
                    int dummm=std::stoi(read_dum[i]);
                    hanzi_code.push_back(dummm);
                    if (dummm<root->pinyin_char.size())
                        hanzi_dummy=hanzi_dummy+root->pinyin_char[dummm].second;
                     i=i+1;
                }
                dummy_words.key_hanzi=hanzi_dummy;
                dummy_words.score=std::stod(read_dum[i]);
                dummy_words.hanzi_code=hanzi_code;
                hanzi_list.push_back(dummy_words);

                i=i+1;
            }
            if (panjang_bilangan>0)
            {
                listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),hanzi_list[0].key_hanzi);
                insert2(root,hanzi_list,listpinyin);
            }
        }
    }


    t2=clock();
    float diff2 (((float)t2-(float)t1)/CLOCKS_PER_SEC);
    cout<<"training time "<<diff2<<endl;
    //main function
	while (tipe!=5)
    {
        cout<<endl<<endl<<"Insert the input for the excecution"<<endl;
        cout<<"type 0: updating the score"<<endl;
        cout<<"type 1: Predict the hanzi based on the given input"<<endl;
        cout<<"type 2: hanzi-> pinyin convertion"<<endl;
        cout<<"type 3: predict the next hanzi"<<endl;
        cout<<"type 4: breaking the sentence"<<endl;
        cout<<"type 5: exit and save the model"<<endl;
        cin>>tipe;
        if (tipe==0)
        {
            //the score is set to automatically one
            //updating the score of existing phrase, or insert the new phrase if the phrase was not found
            cout<<"insert the training phrase (hanzi)"<<endl;
            cin>>masuk;
            listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),masuk);
            insert(root,masuk,listpinyin,1,true,0);
        }
       else if (tipe==1)
       {
           //given input pinyin, it will give the list of the correspondence hanzi;
            cout<<"enter_input"<<endl;
            cin>>masuk;
            d=search(root,masuk);
       }
       else if(tipe==2)
       {
           //simply hanzi ->pinyin converter
            pair<bool,vector<prediction>> hasil;
            cout<<"enter_input"<<endl;
            cin>>masuk;
            long k=root->pinyin_char.size();
            listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),masuk);
            for (i=0;i<listpinyin.size();i++)
            {
                if (i%2==0)
                    cout<<con_consonant[listpinyin[i]];
                else
                    cout<<con_vocal[listpinyin[i]];
            }


       }
       else if(tipe==3)
       {
            //give the prediction of the next character from the given input
            pair<bool,vector<prediction>> hasil;
            vector<prediction> predict;
            cout<<"enter_input"<<endl;
            bool mama=true;
            cin>>masuk;
            long k=root->pinyin_char.size();
            int panjang=masuk.length();
            listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),masuk);
            hasil=search_next_word(root,listpinyin,masuk);
            predict=hasil.second;
            if (hasil.first)
            {
               for (i=0;i<predict.size();i++)
                {
                    cout<<predict[i].hanzi_list.substr(panjang,3)<<" "<<predict[i].score<<" "<<endl;
                    if (i>20)
                        break;
                }
            }

       }
       else if(tipe==4)
       {
           //break the sentence into the words and asking whether the words score need to be updated/not
            int upd;
            pair<bool,vector<prediction>> hasil;
            vector<prediction> predict;
            cout<<"enter_input"<<endl;
            vector <string> keluar;
            bool mama=true;
            cin>>masuk;
            long k=root->pinyin_char.size();
            int panjang=masuk.length();
            listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),masuk);
            keluar=sentence_break(root,listpinyin,masuk);
            for (int i=0;i<=keluar.size()-1;i++)
                cout<<keluar[i]<<" ";
            cout<<"updating the score? (1:Yes/0:No)"<<endl;
            cin>>upd;
            if (upd==1)
            {

                for (int i=0;i<=keluar.size()-1;i++)
                {
                    listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),keluar[i]);
                    insert(root,keluar[i],listpinyin,1,true,0);
                }

            }
       }

    }
    cout<<"Start to save the model"<<endl;
    remove("dictionary.txt");
    t1=clock();
    //save the list of the hanzi and pinyin into file dictionary.txt
    for (i=1;i<root->pinyin_char.size();i++)
    {
        ofstream myfile;
        string mydum_str;
        myfile.open ("dictionary.txt",fstream::app);
        mydum_str=root->pinyin_char[i].first+" "+root->pinyin_char[i].second +"\n";
        myfile << mydum_str;
        myfile.close();
    }
    remove("model.txt");
    vector<pair<int,vector<prediction>>> nyetak;
    nyetak=cetak(root,1);
    //save the trie into model.txt
    for (i=0;i<nyetak.size();i++)
    {
        ofstream myfile;
        string mydum_str;
        myfile.open ("model.txt",fstream::app);

        mydum_str = std::to_string(nyetak[i].first) + " ";

        vector <prediction> mydum = nyetak[i].second;
        mydum_str=mydum_str+ std::to_string(mydum.size()) + " ";
        for (j=0;j<mydum.size();j++)
        {
            vector <int> dummyku=mydum[j].hanzi_code;
            for (k=0;k<dummyku.size();k++)
                mydum_str=mydum_str+ to_string(dummyku[k])+" ";
            mydum_str=mydum_str+std::to_string(mydum[j].score)+" ";
        }

        mydum_str= mydum_str+"\n";
        myfile << mydum_str;
        myfile.close();
    }
    t2=clock();
    diff2=(((float)t2-(float)t1)/CLOCKS_PER_SEC);
    cout<<"saving time"<<diff2<<endl;
	return 0;

}
