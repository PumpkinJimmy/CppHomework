#include <iostream>
#include "date.h"

using namespace std;

int main() {
	cout << Date() << endl
		<< Date(2010, 9, 15) << endl
		<< Date(-2, 1, 1) << endl
		<< Date(2020, 13, 1) << endl
		<< Date(2020, 12, 32) << endl
		<< Date(2020, 11, 31) << endl
		<< Date(2000, 2, 30) << endl
		<< Date(2000, 2, 29) << endl
		<< Date(1900, 2, 29) << endl << endl;
	Date date;
	cout << date << endl;
	date.SetYear(2000); date.SetMonth(2); date.SetDay(29);
	cout << date << endl << date.GetYear() << ' ' << date.GetMonth() << ' ' << date.GetDay() << endl;
	return 0;
}