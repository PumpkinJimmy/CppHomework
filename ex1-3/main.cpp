#include <iostream>
#include "person.h"
using namespace std;
int main() {
	Person person;
	Person person2{ {2000, 9, 5}, {19351023, "BBPumpkin"} };
	cout << person << endl
		<< person2 << endl;
	Person person3{ {2010, 2, 29}, {0, "Joke"} };
	cout << person3 << endl;
	return 0;
}