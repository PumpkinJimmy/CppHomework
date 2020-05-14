#include <regex>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include "shape.h"
#include "shape_file.h"
using namespace std;
double get_double(const string& a, const string& b) {
	double res = 0.0;
	for (auto c : a) {
		res = res * 10 + c - '0';
	}
	if (b.size())
	{
		double mask = 0.1;
		for (int i = 1; i < b.size(); i++) {
			res += mask * (b[i] - '0');
			mask /= 10;
		}
	}
	return res;
}
ShapeFile::ShapeFile(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "File Not Found" << endl;
		exit(-1);
	}
	regex pat_c(R"(C: *\((\d+)(\.\d+)?, *?(\d+)(\.\d+)?\), +(\d+)(\.\d+)?)"),
		pat_p(R"(\((\d+)(\.\d+)?, *(\d+)(\.\d+)?\))");
	smatch sm;
	string text; getline(fin, text);
	while (fin) {
		if (regex_match(text, sm, pat_c)) { // Circle
			Point c; double r;
			for (auto& sub : sm) {
				c.x_ = get_double(sm[1].str(), sm[2].str());
				c.y_ = get_double(sm[3].str(), sm[4].str());
				r = get_double(sm[5].str(), sm[6].str());
			}
			shapes.push_back(make_shared<Circle>(c, r));
		}
		else if (text.size() > 2 && (text[0] == 'P' || text[0] == 'S') && text[1] == ':') { // Polygon
			sregex_iterator pstart(text.begin(), text.end(), pat_p);
			sregex_iterator pend;
			vector<Point> ps;
			Point p;
			for (auto it = pstart; it != pend; it++) {
				if (it->size()) {
					auto& sm = *it;
					p.x_ = get_double(sm[1], sm[2]);
					p.y_ = get_double(sm[3], sm[4]);
					ps.push_back(p);
				}
			}
			if (text[0] == 'P')
				shapes.push_back(make_shared<MyPolygon>(ps));
			else {
				if (ps.size() != 3) {
					fprintf(stderr, "Invalid line:%s\n, required 3 point", text.c_str());
				}
				shapes.push_back(make_shared<Sector>(ArcEdge(ps[1], ps[2], ps[0])));
			}
		}
		else {
			fprintf(stderr, "Invalid line:%s\n", text.c_str());
		}
		getline(fin, text);
	}
}
void ShapeFile::Print() const {
	for (auto& p : shapes) {
		p->Print();
	}
}
void ShapeFile::Generate()
{
	int len = shapes.size();
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < i; j++) {
			auto res = shapes[i] & shapes[j];
			if (res) shapes.push_back(res);
			res = shapes[i] | shapes[j];
			if (res) shapes.push_back(res);
		}
	}
}