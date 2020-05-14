#include <iostream>
#include "identity.h"

int main() {
	cout << Identity() << endl
		<< Identity(2, "BBPumpkin") << endl;
	Identity id1(1, "A"), id2(2, "B");
	Identity id3 = id1;
	cout << id1 << id2 << id3 << endl;
	return 0;
}