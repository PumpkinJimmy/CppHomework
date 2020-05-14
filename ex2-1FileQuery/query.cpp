#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "query.h"
using namespace std;
void FileQuery::insertWord(const string& word, int& wloc)
{
	words.push_back(word); 
	if (!wordcnt.count(word)) {
		wordcnt[word] = 0;
	}
	wordcnt[word]++;
	wordlocs[word].push_back(wloc);
	wloc++;
}
FileQuery::FileQuery(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "File Not Exist" << endl;
		exit(-1);
	}
	char c = fin.get();
	vector<char> buf;
	vector<char> wbuf;
	string word;
	int wloc = 0;
	while (fin) {
		switch (c)
		{
		case ' ':
			if (!wbuf.empty()) {
				word = string(wbuf.begin(), wbuf.end());
				insertWord(word, wloc);
				wbuf.clear();
			}
			buf.push_back(c);
			break;
		case '.': case ',':
			if (!wbuf.empty())
			{
				word = string(wbuf.begin(), wbuf.end());
				insertWord(word, wloc);
				wbuf.clear();
			}
			buf.push_back(c);
			sentences_.push_back(string(buf.begin(), buf.end()));
			buf.clear();
			break;
		case '\n': case '\r':
			break;
		default:
			buf.push_back(c);
			wbuf.push_back(c);
		}
		c = fin.get();
	}
	if (!buf.empty()) {
		sentences_.push_back(string(buf.begin(), buf.end()));
	}
	if (!wbuf.empty()) {
		insertWord(string(wbuf.begin(), wbuf.end()), wloc);
	}
	sorted_words = vector<pair<string, int> >(wordcnt.begin(), wordcnt.end());
	sort(sorted_words.begin(), sorted_words.end(),
		[](const pair<string, int>& p1, const pair<string, int>& p2) {
			return p1.second > p2.second;
		});
	sorted_sentences = sentences_;
	sort(sorted_sentences.begin(), sorted_sentences.end());
}

void FileQuery::PrintSentences() const {
	for (auto& s : sentences_) {
		cout << s << endl;
	}
}
void FileQuery::PrintSentencesAscend() const {
	for (auto& s : sorted_sentences) {
		cout << s << endl;
	}
}
void FileQuery::PrintWordCount() const {
	for (auto& p : sorted_words) {
		cout << p.first << ':' << p.second << endl;
	}
}
void FileQuery::PrintTopWordContext(int n) {
	for (int i = 0; i < n; i++) {
		if (i < sorted_words.size()) {
			string word = sorted_words[i].first;
			cout << word << ":\n";
			for (auto p: wordlocs[word]) {
				if (p > 0) {
					cout << words[p - 1] << ' ';
				}
				cout << '<' << words[p] << '>';
				if (p < words.size() - 1) {
					cout << ' ' << words[p + 1];
				}
				cout << endl;
			}
			cout << endl;
		}
	}
}