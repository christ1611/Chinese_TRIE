# Chinese_TRIE
this program will minimize the amount of time in Chinese Language auto completion


Background: 

TRIE is an example of a digital tree. It is commonly used for the auto-prediction and auto complete features on the mobile keyboard — each node in the tree store a dynamic set or associative array of the string. In other words, the tree defines the key which it is associated in the language programming. TRIE can be seen as an efficient concept of the information retrieval concept.

Chinese is the most complicated language in the word. There is more than 50.000 Chinese characters that have been used in history. Fortunately, due to the simplification, the modern, the number of the Chinese character has been reduced, and around 20.000 Chinese characters that are using nowadays. The native Chinese speaker usually only recognizes 8.000 characters. However, understanding at least 3.000 Chinese character is enough for us to understand the newspaper. Since many words have a similar pronunciation in the Chinese language, the tone is critical since the different intonation can lead to different meaning. There is five type of the intonations in the Chinese: high and level, rises moderately, fall and rises, start high and drop and neutral.
In the programming language, the Chinese character needs to be converted to the Unicode, and the Unicode encoding result is the information that the memory save. Unicode is quite confusing for the programmer because they need to at least read the Unicode standard before executing the Chinese character. However, the high-level programming language has built some library to simplify the usage of Chinese style. For example, Python has made the Xpinyin library to execute the Chinese character.
Understanding the Chinese role is quite hard for the non-native speaker. Therefore, the pinyin translation was built to represent the Chinese in the alphabet form. The pinyin for each character represents how the character is pronounced. This system is widely used to understand how to learn Chinese. For computer programming, using pinyin also give the advantage during object-oriented programming since the word can be easy to be used.

Hanzi is the name of the Chinese character. There are more than 50.000 characters that have been existed. Nowadays, Hanzi is using widely in China, Taiwan and some area in South East Asian countries. However, the Korean and Japanese language also used a similar system (Hanzi is known as ‘Hanja’ in Korea and ‘Kanji’ in Japan). In this three system, some of the words have the similar for, while some are different. The Unicode recognize more than 70.000 Hanzi as the combination of Chinese (Simplified Chinese, Traditional Chinese, Korean and Japanese)
While Hangeul in Korean and Hiragana in Japan can help people to read and pronounce the Hanja and Kanji, there is no systematic way to represent the Hanzi based on the pronunciation way in Chinese before 1949. Therefore, the Chinese government did a project to help the outsider to learn Chinese, and so the Pinyin was made. Pinyin is only used in the Chinese language. Pinyin contains two parts, consonant and vocal. There are 23 type of consonants and 33 types of vocals. The list of the consonant and vocal is shown in Figure 2. For some cases, the intonation sign was displayed to help the reader to pronounce the character correctly. 

Represent chinese in TRIE
Same with the English TRIE, the first level of Chinese TRIE is represent the first character of the word, the second level of Chinese TRIE is represent the second character of the word and so on. Since one character in Chinese character contain two parts of pinyin (Consonant and vocal), one character represented by two layer, and the hanzis are saved in the vocal parts. 

Pre requiments:
1. The operating system must be Ubuntu 16.04 or advanced
3. The memory need to be at least 4 GB
4. The Gcc version need to be at least v 4.6.2 or advanced, other c++ compiler that can support C++ ISO 2011 (std c++11) also can be used to build this tool.

Run the program
1. Move the terminal directory into the folder which contain the tools.
2. To compile the program, type this in your console:
	 g++ -std=c++11 –otrie pinyin.cpp
3. To run the program, type this in your console
  	./trie
  
Features of Chinese TRIE

Type 0: Insert the new phrase or updating the prediction score. (input must be hanzi)

Type 1: This feature will return the list of the Hanzi based on the given pinyin.(input must be pinyin)

Type 2: This feature will return the pinyin translation based on the given hanzi. (input must be hanzi)

Type 3: This feature will return the recommendation of the next character based on the given hanzi. (input must be hanzi)

Type 4: This feature will return the words from the given sentence. (input must be hanzi, then the computer will ask whether you want to update the score or not)

Type 5: Exit and save the model.

Note:
The sample sentences was taken from the Sogou dictionary
The format of the Sample sentence should be: documentscore_filename, the document score is used for the rangking of the prediction
Inside the file, the format of the input is pinyin, then hanzi in one line. However for this program, we only use the hanzi part as the input.

In model file, each note is saved in two lines with format:
First line:(The level of the nodes) (the length of the word)
Second line: the combination of (hanzi conde) (predictictio score). the total of this combination in this line should be same (the the length of the word)
