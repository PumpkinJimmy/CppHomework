#ifndef _IDENTITY_H
#define _IDENTITY_H
#include <cstring>
#include <iomanip>
using namespace std;
class Identity
{
public:
	
	Identity(int number, const char* name) : number_(number), name_(nullptr), is_retain(false){
		int len = strlen(name);
		name_ = new char[len + 1];
		strcpy_s(name_, len+1, name);
	}
	Identity() : Identity(1, "Default Name") {}
	
	Identity(const Identity& other) {
		copy(other);
		cout << "Invoke copy constructor, origin:" << hex << "0x" << int(other.name_) <<  " new: " << "0x" << int(name_) << endl << dec;
	}
	Identity(Identity&& tmp) {
		tmp.retain();
		name_ = tmp.name_;
		cout << "Invoke move constructor, origin:" << hex << int(tmp.name_) << " new: " << hex << "0x" << int(name_) << endl << dec;
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
		cout << "Invoke deconstructor, name_ ptr: " << hex << "0x" << int(name_) << endl << dec;
		if (!is_retain) {
			delete name_;
			cout << "Delete " << hex << "0x" << int(name_) << endl << dec;
		}
		else cout << "Retain " << hex << "0x" << int(name_) << endl << dec;
	}
	void copy(const Identity& other) {
		number_ = other.number_;
		int len = strlen(other.name_);
		delete name_;
		name_ = new char[len + 1];
		strcpy_s(name_, len + 1, other.name_);
	}
	void retain() { is_retain = true; }
	void release() { is_retain = false;  }
	friend ostream& operator<<(ostream& out, const Identity& id);
private:
	int number_;
	char* name_;
	bool is_retain;
};
inline ostream& operator<<(ostream& out, const Identity& id) {
	out << id.name_ << ": " << id.number_;
	return out;
}
#endif