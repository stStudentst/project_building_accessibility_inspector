#pragma once
#include <iostream>
#include "Directions_avl.h"

class SurfacePoint
{
public:
	double x, y;
	NodeInAVL* nil;

	SurfacePoint(double x, double y, NodeInAVL* nil) : x(x), y(y), nil(nil) {};
	~SurfacePoint() {}
};


