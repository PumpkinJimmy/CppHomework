#include <iostream>
#include "query.h"

using namespace std;

int main()
{
	FileQuery fq("C:\\Users\\ASUS\\source\\repos\\CppHomework\\ex2-1FileQuery\\TwoCites.txt");
	fq.PrintSentences();
	cout << endl;
	fq.PrintSentencesAscend();
	cout << endl;
	fq.PrintWordCount();
	cout << endl;
	fq.PrintTopWordContext(2);
	return 0;
}