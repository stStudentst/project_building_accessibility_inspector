#pragma once
#include <iostream>
using namespace std;


class NodeInAVL
{
	

public:
	double x, y;
	int height_level, Dright, Dleft;

	//D= direction and S=son
	NodeInAVL* Sright;
	NodeInAVL* Sleft;

	NodeInAVL(double x, double y, int right, int left) :x(x), y(y), Dleft(left), Dright(right), Sright(nullptr), Sleft(nullptr), height_level(1) {};
	~NodeInAVL() {};
};
class Directions_avl
{


public:

	NodeInAVL* root;
	Directions_avl() :root(nullptr) {}
	pair<NodeInAVL*, NodeInAVL*> AddDirection(double x, double y, int left, int right, pair<NodeInAVL*, NodeInAVL*>p);
	NodeInAVL* SearchPoint(double x, double y, NodeInAVL* nil);

	NodeInAVL* Find_closes_Button_Y(double x, double y, NodeInAVL* nil);
	NodeInAVL* Find_closes_Above_Y(double x, double y, NodeInAVL* nil);
	NodeInAVL* Find_nearest_Button_X(double x, double y, NodeInAVL* nil);
	NodeInAVL* Find_nearest_Above_X(double x, double y, NodeInAVL* nil);



	NodeInAVL* Insert(double x, double y, int left, int right);
	NodeInAVL* SearchCoor(double x, double y);
	NodeInAVL* Checking_and_update_the_legality_of_DeadEnd(double x, double y, int direction);

	~Directions_avl() {}

};

