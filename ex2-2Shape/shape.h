#pragma once
#ifndef _SHAPE_H
#define _SHAPE_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#define SQ(x) ((x) * (x))
using namespace std;
const double pi = 3.1415926;

/*
 * 表示点的类
 * 交作业的类，实际上下面Vec可以完全取代这个类
 */
class Point
{
public:
	Point(double x = 0, double y = 0) :x_(x), y_(y) {}
	void Print() {
		cout << *this << endl;
	}
	friend ostream& operator<<(ostream& out, const Point& p);
	double x_;
	double y_;
	double dst(const Point& other) const{
		return sqrt(pow(other.y_ - y_, 2) + pow(other.x_ - x_, 2));
	}
	bool operator==(const Point& other) const {
		return x_ == other.x_ && y_ == other.y_;
	}
	bool operator!=(const Point& other) const {
		return x_ != other.x_ || y_ != other.y_;
	}
};
struct Vec
{
	Vec(double _x = 0, double _y = 0) :x(_x), y(_y) {}
	Vec(const Point& p) : x(p.x_), y(p.y_) {}
	Vec(const Point& st, const Point& ed) : x(ed.x_ - st.x_), y(ed.y_ - st.y_) {}
	double x;
	double y;
	double norm() const { return sqrt(SQ(x) + SQ(y)); }
	double normalize() {
		double len = norm();
		if (len == 0) {
			cout << "Invalid Vec Op: Normalize a zero vec" << endl;
			exit(-1);
		}
		x /= len;
		y /= len;
		return len;
	}
	Vec operator+(const Vec& other) const {
		return Vec(x + other.x, y + other.y);
	}
	Vec operator-(const Vec& other) const {
		return Vec(x - other.x, y - other.y);
	}
	Vec operator*(double scale) const {
		return Vec(x * scale, y * scale);
	}
	double dot(const Vec& other) const {
		return x * other.x + y * other.y;
	}
	double cross(const Vec& other) const {
		return x * other.y - y * other.x;
	}
	friend Vec operator*(double scale, const Vec& vec);
};
inline Vec operator*(double scale, const Vec& vec) {
	return vec * scale;
}
ostream& operator<<(ostream& out, const Point& p);

class Edge
{
public:
	virtual double Length() const = 0;
	virtual void Print() const = 0;
	virtual Point GetSt() const = 0;
	virtual Point GetEd() const = 0;
};
typedef shared_ptr<Edge> EdgePtr;
class StraightEdge : public Edge
{
public:
	StraightEdge(){}
	StraightEdge(const Point& st, const Point& ed) : st_(st), ed_(ed) {
	}
	virtual double Length() const{
		return sqrt((ed_.y_ - st_.y_) * (ed_.y_ - st_.y_) +
			(ed_.x_ - st_.x_) * (ed_.x_ - st_.x_));
	}
	virtual void Print() const{
		cout << "[StraigtEdge:" << st_ << "->" << ed_ << ']';
	}
	virtual Point GetSt() const { return st_; }
	virtual Point GetEd() const { return ed_; }
private:
	Point st_;
	Point ed_;
};
/*
 * 表示弧的类
 * 起点和顺序很重要！！
 * ArcEdge表示的是从起点*逆时针*到终点的那一段弧
 */
class ArcEdge : public Edge
{
public:
	ArcEdge(){}
	ArcEdge(const Point& st, const Point& ed, const Point& center) : st_(st), ed_(ed), center_(center) {
		if (abs(st_.dst(center_) - ed_.dst(center_)) > 1e-3) {
			cout << "Invalid ArcEdge Argument:" << st_ << ' ' << ed_ << ' ' << center_ << endl;
			exit(-1);
		}
		//cout << "Invalid Arc:";
		//Print();
		//cout << endl;
		//exit(1);

	}
	virtual double Length() const {
		double r = st_.dst(center_);
		double angle = GetAngle();
		return angle * r;
	}
	virtual void Print() const{
		cout << "[ArcEdge:" << st_ << "->" << ed_ << ",center:" << center_ << ']';
	}
	virtual Point GetSt() const { return st_; }
	virtual Point GetEd() const { return ed_; }
	double GetAngle() const{
		double r = st_.dst(center_);
		double cross = Vec(center_, st_).cross(Vec(center_, ed_));
		double dot = (st_.x_-center_.x_) * (ed_.x_ - center_.x_) + (st_.y_ - center_.y_) * (ed_.y_ - center_.y_);
		double angle = acos(dot / r / r);
		return cross < 0 ? 2 * pi - angle : angle;
	}
	double GetRadius() const {
		return st_.dst(center_);
	}
	Point st_;
	Point ed_;
	Point center_;
};

class Shape
{
public:
	static const int InvalidValue = -1;
	virtual void Print() const = 0;
	virtual double Circumference() const = 0;
	virtual double Area() const = 0;
};
typedef shared_ptr<Shape> ShapePtr;
class Circle : public Shape
{
public:
	Circle() : radius_(0) {}
	Circle(Point center, double radius) :center_(center), radius_(radius) {}
	virtual void Print() const;
	virtual double Circumference() const;
	virtual double Area() const;
	double GetRadius() const { return radius_; }
	Point GetCenter() const { return center_; }
private:
	double radius_;
	Point center_;
};
class Sector : public Circle
{
public:
	Sector() : Circle() {}
	Sector(ArcEdge arc): Circle(arc.center_, arc.GetRadius()), arc_(arc){
	}
	Sector(Point center, double radius, ArcEdge arc) : Circle(center, radius), arc_(arc) {
		if (arc_.center_ != center || arc_.st_.dst(center) != radius) {
			cout << "Invalid Init of Sector, Argument:" << center << ' ' << radius << ' ';
			arc.Print();
			cout << endl;
		}
	}
	virtual void Print() const;
	virtual double Circumference() const;
	virtual double Area() const;
private:
	ArcEdge arc_;
};
class MyPolygon : public Shape
{
public:
	MyPolygon() {}
	MyPolygon(const vector<Point>& ps);
	MyPolygon(const vector<EdgePtr>& edges);
	virtual void Print() const;
	virtual double Circumference() const;
	virtual double Area() const;
private:
	vector<EdgePtr> edges_;
};
ShapePtr operator&(ShapePtr p1, ShapePtr p2);
ShapePtr operator&(const Shape& p1, const Shape& p2);
ShapePtr operator&(ShapePtr p1, const Shape& p2);
ShapePtr operator&(const Shape& p1, ShapePtr p2);
ShapePtr operator|(ShapePtr p1, ShapePtr p2);
ShapePtr operator|(const Shape& p1, const Shape& p2);
ShapePtr operator|(ShapePtr p1, const Shape& p2);
ShapePtr operator|(const Shape& p1, ShapePtr p2);
#endif