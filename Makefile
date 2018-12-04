all: pinyin 
pinyin: pinyin.o
	g++ -std=c++11 pinyin.o -o pinyin

pinyin.o: pinyin.cpp
	g++ -std=c++11 -c pinyin.cpp
clean: 
	rm -rf *o pinyin
