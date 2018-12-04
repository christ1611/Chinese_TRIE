/*
#####################################################################################
#Program name  : search.cpp
#Description   : File contains the function to identify the correspondence hanzi based on the given input (type 1)
#Copyright     : Distribution, modification, reproduction, and copy are strictly prohibited to unauthorized persons.
#Author        : Hotama Christianus Frederick (christianusfrederick@gmail.com)
#Commit date   : December 2018
######################################################################################
#Function input: pinyin
#Function output: list of correspondence hanzi with the order of the reccomendation score
*/

#include "pinyin.h"

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

