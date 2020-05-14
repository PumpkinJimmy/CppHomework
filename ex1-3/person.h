#ifndef _PERSON_H
#define _PERSON_H
#include "date.h"
#include "identity.h"
class Person {
public:
	Person(const Date& birthday, const Identity& id) : birthday_(birthday), id_(id) {}
	Person(Date&& birthday, Identity&& id) : birthday_(birthday), id_(move(id)) {}
	Person() {}
	Person(Person&& tmp): birthday_(tmp.birthday_), id_(move(tmp.id_)){
		cout << "Invoke move constructor" << endl;
	}
	const Date& GetBirthday() const { return birthday_; }
	const Identity& GetId() const { return id_; }
private:
	Date birthday_;
	Identity id_;
};
inline ostream& operator<<(ostream& out, const Person& p) {
	out << '(' << p.GetId() << ") Birthday: " << p.GetBirthday();
	return out;
}
#endif