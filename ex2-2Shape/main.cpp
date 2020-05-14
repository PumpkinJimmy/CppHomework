#include <iostream>
#include <regex>
#include "shape.h"
#include "shape_file.h"
using namespace std;
int main()
{
	// Edge Test
	cout << "=== Edge Test ===\n ";
	StraightEdge se{ {0, 0}, {1, 1} };
	ArcEdge ae{ {1, 0}, {0, 1}, {0, 0} };
	ArcEdge aee{ {1, 0}, {0, -1}, {0, 0} };
	se.Print(); cout << endl;
	ae.Print(); cout << endl;
	aee.Print(); cout << endl;
	cout << se.Length() << ' ' << ae.Length() << ' ' << aee.Length() << endl;

	// Circle/Sector Test
	cout << "=== Circle/Sector Test ===\n ";
	Circle circle({ 0, 0 }, 1);
	Sector sector({ 0, 0 }, 1, ae);

	circle.Print();
	sector.Print();

	// MyPolygon Test
	cout << "=== MyPolygon Test ===\n ";
	MyPolygon poly({ {0, 0}, {1, 0}, {2, 1}, {1, 2}, {0, 2}, {-1, 1} });
	poly.Print();

	EdgePtr e1 = make_shared<StraightEdge>(Point(0, 0), Point(1, 0));
	EdgePtr e2 = make_shared<ArcEdge>(Point(1, 0), Point(0, 0), Point(0.5, 0));
	EdgePtr e3 = make_shared<ArcEdge>(Point{ 0, 0 }, Point{ 1, 0 }, Point{ 0.5, 0.5 });
	MyPolygon poly2({ e1, e2 });
	poly2.Print();

	MyPolygon poly3({ e3, e2 });
	poly3.Print();

	// Set Op Test
	cout << "=== Set Op Test ===\n ";
	Circle circle1({ 0, 0 }, 1), circle2({ 1, 0 }, 1), circle3({ 0.5, 0 }, 0.5);
	auto res1 = circle1 & circle2, res3 = circle1 | circle2;
	auto res2 = circle1 & circle3, res4 = circle1 | circle3;
	res1->Print();
	res2->Print();
	res3->Print();
	res4->Print();
	(circle1 | circle3 & circle2)->Print();

	//ShapeFile test
	cout << "=== ShapeFile Test ===" << endl;
	ShapeFile sfile("Shape.txt");
	sfile.Print();
	cout << "--- After Generate() ---" << endl;
	sfile.Generate();
	sfile.Print();

	cout << sfile.GetShapes().size() << " shapes" << endl;
	return 0;
}