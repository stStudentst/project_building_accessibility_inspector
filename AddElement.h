#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "Directions_avl.h"
#include "StructElements.h"

using namespace std;
using namespace struct_Elements;
#define DeadEnd 2	//מבוי סתום

class AddElement
{
	
public:
	double ScoreFreeHeight = 0.0;
	
	AddElement() {}
	list<Special_parameters> listWidth;
	Rest_of_the_parameters vectorElements[8];

	void UpdateHeight(double precent_unlegal);
	void AddFreeWidth(double x, double y, double w);
	void Update_Rest_element( double sdisabled, double sb, double sd, int index);
	void Add_dead_end(double x, double y, int direction,Directions_avl* DA);

};

