#ifndef _IDENTITY_H
#define _IDENTITY_H
#include <cstring>
#include <iomanip>
using namespace std;
class Identity
{
public:
	Identity(int number, const char* name) : number_(number), name_(nullptr){
		int len = strlen(name);
		name_ = new char[len + 1];
		strcpy_s(name_, len+1, name);
	}
	Identity() : Identity(1, "Default Name") {}
	void copy(const Identity& other) {
		number_ = other.number_;
		int len = strlen(other.name_);
		delete name_;
		name_ = new char[len + 1];
		strcpy_s(name_, len+1, other.name_);
		cout << "Invoke copy constructor, name_ ptr: " << ios_base::hex << int(name_) << endl;
	}
	Identity(const Identity& other) {
		copy(other);
	}
	bool operator==(const Identity& other) const {
		return (number_ == other.number_ && strcmp(name_, other.name_) == 0);
	}
	Identity& operator=(const Identity& other) {
		if (other == *this) { return *this;  }
		copy(other);
		return *this;
	}
	~Identity() {
		cout << "Invoke deconstructor, name_ ptr: " << ios_base::hex << int(name_) << endl;
		delete name_;
	}
	friend ostream& operator<<(ostream& out, const Identity& id);
private:
	int number_;
	char* name_;
};
inline ostream& operator<<(ostream& out, const Identity& id) {
	out << id.name_ << ": " << id.number_ << endl;
	return out;
}
#endif