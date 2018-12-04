/*
#####################################################################################
#Program name  : main.cpp
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
#Copyright     : Distribution, modification, reproduction, and copy are strictly prohibited to unauthorized persons.
#Author        : Hotama Christianus Frederick (christianusfrederick@gmail.com)
#Commit date   : December 2018
######################################################################################
*/

#include "pinyin.h"


std::string GetCurrentWorkingDir( void ) {
  //function to get the position of the directory
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
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
            perror ("Please copy the sample file to the folder");
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
            if (a%500000==0)
                cout<<"Reading from the line "<<a<<endl;
            a=a+1;

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
                if (j==0)
                {
                    listpinyin=find_pinyin(root->pinyin_char,root->pinyin_char.size(),hanzi_dummy);
                //insert2(root,hanzi_list,listpinyin);
                }
                insert(root,hanzi_dummy,listpinyin,dummy_words.score,false,0);
                i=i+1;
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
    nyetak=node_print(root,1);
    //save the trie into model.txt
    for (i=0;i<nyetak.size();i++)
    {
        ofstream myfile;
        string mydum_str,mydum_str2;


        mydum_str = std::to_string(nyetak[i].first) + " ";

        vector <prediction> mydum = nyetak[i].second;
        //mydum_str=mydum_str+ std::to_string(mydum.size()) + "\n";
        bool dec_print=false;
        int array_sizes=0;
        for (j=0;j<mydum.size();j++)
        {

            if (mydum[j].end_of_word==1)
            {
                array_sizes=array_sizes+1;
                dec_print=true;
                vector <int> dummyku=mydum[j].hanzi_code;
                for (k=0;k<dummyku.size();k++)
                    mydum_str2=mydum_str2+ to_string(dummyku[k])+" ";
                mydum_str2=mydum_str2+std::to_string(mydum[j].score)+" ";
            }

        }
        mydum_str=mydum_str+ to_string(array_sizes) + "\n";
        if (dec_print)
        {
             myfile.open ("model.txt",fstream::app);

             mydum_str2= mydum_str2+"\n";
             myfile << mydum_str;
             myfile << mydum_str2;
            myfile.close();
        }


    }
    t2=clock();
    diff2=(((float)t2-(float)t1)/CLOCKS_PER_SEC);
    cout<<"saving time "<<diff2<<endl;
	return 0;

}
