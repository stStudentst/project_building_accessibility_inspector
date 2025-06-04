#pragma once
#include "Directions_avl.h"
#include "SurfacePoint.h"
#include <stack>

using namespace std;

//מחלקת נקודה על פני השטח  

class Steps_Instack
{
	
public:
	int count_steps = 0;
	Steps_Instack(){}
	//מחסנית מסוג המחלקה הנ"ל אליה נכניס את כל הנקודות בשביל מהלך סריקה נכונה
	stack<SurfacePoint> SFstack;
	stack<SurfacePoint>SFhelp_addDirection;
	void AddCoordinate(double x, double y);
	SurfacePoint* Get_Cooridnate_to_addDirection(int num);
	SurfacePoint* Get_coordinate_to_Keep_scan();
	void print();
};

