#ifndef OBJ_TRIANGLE_H
#define OBJ_TRIANGLE_H

#include <array>

// By default counterclockwise winding is front face
struct ObjTriangle {
    ObjTriangle() 
	{
			std::array<int, 3> a = {{0,0,0}};
			std::array<int, 3> b = {{0,0,0}};
			x = a;
			texID = b;
    }

    int & operator[](int i) {
        return x[i];
    }

    std::array<int, 3> x;
    std::array<int, 3> texID;
	std::array<int, 3> normalID;
};

#endif // OBJ_TRIANGLE_H
