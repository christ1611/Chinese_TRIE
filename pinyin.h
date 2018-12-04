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
#Project       : SLM - 논리 고도화 (redmine 10054,10095)
#Version       : v2.0
#Update        : The TRIE Scoring system
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
