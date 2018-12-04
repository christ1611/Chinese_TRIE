/*
#####################################################################################
#Program name  : TRIE model for chineze-Hanzi Pinyin Prediction
#Description   : All the library that needed in pinyin.cpp
#
#Author        : Hotama Christianus Frederick
#Commit date   : December 2018
######################################################################################
*/

#include <unistd.h>
#ifndef SRC_PINYIN_H_
#define SRC_PINYIN_H_

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



#endif
