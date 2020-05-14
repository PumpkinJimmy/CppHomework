#pragma once
#ifndef _QUERY_H
#define _QUERY_H

#include <string>
#include <vector>
#include <map>
using namespace std;
class FileQuery
{
public:
	FileQuery(const char* filename);
	void PrintSentences() const;
	void PrintSentencesAscend() const;
	void PrintWordCount() const;
	void PrintTopWordContext(int n);
private:
	void insertWord(const string& word, int& wloc);
	vector<string> sentences_;
	vector<string> words;
	map<string, int> wordcnt;
	map<string, vector<int> > wordlocs;
	vector<pair<string, int> > sorted_words;
	vector<string> sorted_sentences;
};

#endif