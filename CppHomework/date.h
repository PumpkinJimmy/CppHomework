#ifndef _DATE_H
#define _DATE_H
#include <iostream>
using namespace std;
class Date {
public:
	Date(int year = 2020, int month = 1, int day = 1) : year_(year), month_(month), day_(day) {
		if (!(verifyYear() && verifyMonth() && verifyDay())) {
			cout << "Reset to default" << endl;
			year_ = 2020;
			month_ = 1;
			day_ = 1;
		}
	}
	Date(const Date& other) { year_ = other.year_; month_ = other.month_; day_ = other.day_; }
	int GetYear() const { return year_; }
	int GetMonth() const { return month_; }
	int GetDay() const { return day_; }
	void SetYear(int y) {
		if (verifyYear(y)) year_ = y;
	}
	void SetMonth(int m) {
		if (verifyMonth(m)) month_ = m;
	}
	void SetDay(int d) {
		if (verifyDay(year_, month_, d)) day_ = d;
	}
	bool isLeap(int y) {
		if (y % 4 == 0) {
			if (y % 100 == 0) {
				if (y % 400 == 0)
					return true;
				else return false;
			}
			else {
				return true;
			}
		}
		else return false;
	}
	bool isLeap() {
		return isLeap(year_);
	}
	bool verifyYear(int y) {
		if (y > 0) return true;
		else { cout << "Invalid Year: " << y << endl; return false; }
	}
	bool verifyYear() {
		return verifyYear(year_);
	}
	bool verifyMonth(int m) {
		if (m > 0 && m <= 12) return true;
		else { cout << "Invalid Month: " << m << endl; return false; }
	}
	bool verifyMonth() {
		return verifyMonth(month_);
	}
	bool verifyDay(int y, int m, int d) {
		int md = 28;
		switch (m) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			md = 31; break;
		case 4: case 6: case 9: case 11:
			md = 30; break;
		case 2:
			if (isLeap(y)) md = 29; break;
		default:
			cout << "Invalid Month: " << m << endl;
			return false;
		}
		if (d > 0 && d <= md) return true;
		else { cout << "Invalid Day: " << d << endl; return false; }
	}
	bool verifyDay() {
		return verifyDay(year_, month_, day_);
	}
	friend ostream& operator<<(ostream& out, const Date& date);
private:
	int year_;
	int month_;
	int day_;
};
inline ostream& operator<<(ostream& out, const Date& date) {
	out << date.year_ << '-' << date.month_ << '-' << date.day_;
	return out;
}
#endif