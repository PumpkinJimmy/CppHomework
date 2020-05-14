#ifndef _SHAPE_FILE_H
#define _SHAPE_FILE_H
#include <vector>
#include <regex>
#include "shape.h"
class ShapeFile
{
public:
	ShapeFile() {}
	ShapeFile(const char* filename);
	const std::vector<ShapePtr>& GetShapes() const {
		return shapes;
	}
	void Print() const;
	void Generate();
private:
	std::vector<ShapePtr> shapes;
};
#endif