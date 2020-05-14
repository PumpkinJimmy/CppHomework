#include <cmath>
#include <functional>
#include "shape.h"

ostream& operator<<(ostream& out, const Point& p)
{
out << '(' << p.x_ << ',' << p.y_ << ')';
return out;
}
ShapePtr CircleSetOp(const Circle* c1, const Circle* c2, function<bool(double,double)> op) {
	if (!c1 || !c2) {
		// fprintf(stderr, "Invalid Shape And Op\n");
		return nullptr;
	}
	double d = c1->GetCenter().dst(c2->GetCenter()), r1 = c1->GetRadius(), r2 = c2->GetRadius();
	if (d >= r1 + r2) {
		return nullptr;
	}
	else if (d <= abs(r1 - r2)) {
		const Circle* resp = (op(r1, r2) ? c2 : c1);
		return make_shared<Circle>(*resp);
	}
	else {
		// prepare center value
		Point ct1 = c1->GetCenter(), ct2 = c2->GetCenter();
		double a1 = ct1.x_, b1 = ct1.y_;
		double a2 = ct2.x_, b2 = ct2.y_;
		// prepare cross line: Dx + Ey + F = 0
		double D = -2 * a1 + 2 * a2, E = -2 * b1 + 2 * b2,
			F = a1 * a1 + b1 * b1 - a2 * a2 - b2 * b2 - r1 * r1 + r2 * r2;
		// 根轴方向向量
		Vec vec_cross(E, -D);
		vec_cross.normalize();
		// prepare center line: Ax + By + C = 0
		double A = b1 - b2, B = a2 - a1, C = a1 * b2 - a2 * b1;
		// 圆心连线方向向量
		Vec vec_center(a2 - a1, b2 - b1);
		vec_center.normalize();
		// 根轴与圆心连线的交点
		Point cross((B * F + C * E) / (A * E - B * D), (F * A + C * D) / (B * D - A * E));

		// 公共弦长的一半
		double hl = sqrt(SQ(r1) - SQ(cross.x_ - a1) - SQ(cross.y_ - b1));
		// 找出两个交点
		Vec vec_p1 = Vec(cross) + vec_cross * hl, vec_p2 = Vec(cross) - vec_cross * hl;
		Point pa(vec_p1.x, vec_p1.y), pb(vec_p2.x, vec_p2.y);
		//
		EdgePtr e1, e2;
		if (op((vec_p1 - Vec(ct1)).cross(vec_p2 - Vec(ct1)), 0)) {
			e1 = make_shared<ArcEdge>(pa, pb, ct1);
			e2 = make_shared<ArcEdge>(pb, pa, ct2);
		}
		else {
			e1 = make_shared<ArcEdge>(pb, pa, ct1);
			e2 = make_shared<ArcEdge>(pa, pb, ct2);
		}
		return make_shared<MyPolygon>(vector<EdgePtr>{ e1, e2 });
	}
}
ShapePtr CircleIntersection(const Circle* c1, const Circle* c2) {
	return CircleSetOp(c1, c2, greater<double>());
}
ShapePtr CircleUnion(const Circle* c1, const Circle* c2) {
	return CircleSetOp(c1, c2, less<double>());
}
ShapePtr operator&(ShapePtr p1, ShapePtr p2) {
	auto c1 = dynamic_cast<const Circle*>(p1.get());
	auto c2 = dynamic_cast<const Circle*>(p2.get());
	return CircleIntersection(c1, c2);
}
ShapePtr operator&(const Shape& p1, const Shape& p2) {
	return CircleIntersection(dynamic_cast<const Circle*>(&p1), dynamic_cast<const Circle*>(&p2));
}
ShapePtr operator&(ShapePtr p1, const Shape& p2)
{
	return CircleIntersection(dynamic_cast<const Circle*>(p1.get()), dynamic_cast<const Circle*>(&p2));
}
ShapePtr operator&(const Shape& p1, ShapePtr p2)
{
	return p2 & p1;
}
ShapePtr operator|(ShapePtr p1, ShapePtr p2) {
	auto c1 = dynamic_cast<const Circle*>(p1.get());
	auto c2 = dynamic_cast<const Circle*>(p2.get());
	return CircleUnion(c1, c2);
}
ShapePtr operator|(const Shape& p1, const Shape& p2) {
	return CircleUnion(dynamic_cast<const Circle*>(&p1), dynamic_cast<const Circle*>(&p2));
}
ShapePtr operator|(ShapePtr p1, const Shape& p2)
{
	return CircleIntersection(dynamic_cast<const Circle*>(p1.get()), dynamic_cast<const Circle*>(&p2));
}
ShapePtr operator|(const Shape& p1, ShapePtr p2)
{
	return p2 | p1;
}
double getArea(const Point& a, const Point& b, const Point& c) {
	Vec v1(c, a), v2(c, b);
	double cross = v1.cross(v2);
	return cross < 0 ? -0.5 * cross : 0.5 * cross;
}
void Circle::Print() const
{
	cout << "[Circle:" << center_ << " radius: " << radius_ << ']' << endl
		<< "\tCircumference: " << Circumference() << endl
		<< "\tArea: " << Area() << endl << endl;
}

double Circle::Circumference() const
{
	return 2 * pi * radius_;
}
double Circle::Area() const
{
	return pi * radius_ * radius_;
}


void Sector::Print() const {
	cout << "[Sector:" << GetCenter()  << " radius: " << GetRadius() << ']' << endl
		<< "\tFrom " << arc_.st_ << " to " << arc_.ed_ << endl
		<< "\tCircumference: " << Circumference() << endl
		<< "\tArea: " << Area() << endl << endl;
}
double Sector::Circumference() const{
	return arc_.Length() + 2 * GetRadius();
}
double Sector::Area() const {
	double r = GetRadius();
	return 0.5 * arc_.Length() * r;
}
MyPolygon::MyPolygon(const vector<Point>& ps) {
	if (ps.size() < 3) {
		cout << "Invalid Polygon Edges: Too less points" << endl;
		exit(-1);
	}
	for (int i = 0; i < ps.size(); i++) {
		int nxt = (i + 1) % ps.size();
		edges_.push_back(make_shared<StraightEdge>(ps[i], ps[nxt]));
	}
}
MyPolygon::MyPolygon(const vector<EdgePtr>& edges) : edges_(edges) {
	if (edges_.size() < 2) {
		cout << "Invalid Polygon Edges: Too less edges" << endl;
		exit(-1);
	}
	for (int i = 0; i < edges_.size(); i++) {
		int nxt = (i + 1) % edges_.size();
		if (edges_[i]->GetSt() != edges_[nxt]->GetEd()) {
			cout << "Invalid Polygon Edges: Not closed" << endl;
			exit(-1);
		}
	}
}
void MyPolygon::Print() const {
	cout << "[Polygon with " << edges_.size() << " edges]" << endl
		<< "\tEdges:\n";
	for (auto& ep : edges_) {
		cout << "\t\t"; ep->Print(); cout << endl;
	}
	cout << "\tCircumference: " << Circumference() << endl
	<< "\tArea: " << Area() << endl << endl;

}

double MyPolygon::Circumference() const 
{
	double res = 0;
	for (auto& ep : edges_) {
		res += ep->Length();
	}
	return res;
}
double MyPolygon::Area() const 
{
	double res = 0;
	if (edges_.size() == 2)
	{
		ArcEdge* e1 = dynamic_cast<ArcEdge*>(edges_[0].get());
		ArcEdge* e2 = dynamic_cast<ArcEdge*>(edges_[1].get());
		if (!e1 && !e2) {
			return Shape::InvalidValue;
		}
		else if (e1 && e2) {
			// both arc
			Sector sc(*e1), sc2(*e2);
			return sc.Area() - getArea(e1->center_, e1->st_, e1->ed_) + sc2.Area() - getArea(e2->center_, e2->st_, e2->ed_);
		}
		else {
			if (e2) swap(e1, e2);
			Sector sc(*e1);
			return sc.Area() - getArea(e1->center_, e1->st_, e1->ed_);
		}
	}
	else
	{
		for (int i = 0; i < edges_.size(); i++) {
			auto ep = edges_[i];
			auto st = ep->GetSt(), ed = ep->GetEd();
			res += getArea({ 0,0 }, st, ed);
		}
		return res;
	}
}