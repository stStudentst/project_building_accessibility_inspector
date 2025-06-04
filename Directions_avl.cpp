//#include <iostream>
#include "Directions_avl.h"

int height_level(NodeInAVL* nil)
{
	if (nil == nullptr)
		return 0;
	return(nil->height_level);
}

NodeInAVL* RightRotate(NodeInAVL* p)
{
	NodeInAVL* hl = p->Sleft;
	NodeInAVL* hr = (hl)? hl->Sright:nullptr;

	(hl) ? hl->Sright = p : hl = p;
	p->Sleft = hr;

	p->height_level = 1 + max(height_level(p->Sleft), height_level(p->Sright));
	hl->height_level = 1 + max(height_level(p->Sleft), height_level(p->Sright));

	return hl;
}

NodeInAVL* LeftRotate(NodeInAVL* p)
{
	NodeInAVL* hr = p->Sright;
	NodeInAVL* hl = (hr)? hr->Sleft:nullptr;

	(hr)?hr->Sleft = p:hr=p;
	p->Sright = hl;

	p->height_level = 1 + max(height_level(p->Sleft), height_level(p->Sright));
	hr->height_level = 1 + max(height_level(p->Sleft), height_level(p->Sright));

	return hr;
}
// פונקציה לקבלת מצב באיזון של הצומת 
int BalanceFactor(NodeInAVL* nil)
{
	if (nil == nullptr)
		return 0;
	return height_level(nil->Sleft) - height_level(nil->Sright);
}




NodeInAVL* Directions_avl::SearchPoint(double x, double y, NodeInAVL* nil)
{
	if (nil == nullptr)
		return nullptr;
	if (nil->x > x)
		return SearchPoint(x, y, nil->Sleft);
	if (nil->x < x)
		return SearchPoint(x, y, nil->Sright);
	if (nil->y > y)
		return SearchPoint(x, y, nil->Sleft);
	if (nil->y < y)
		return SearchPoint(x, y, nil->Sright);
	return nil;
}




pair<NodeInAVL*, NodeInAVL*> Directions_avl::AddDirection(double x, double y, int left, int right, pair<NodeInAVL*, NodeInAVL*>p)
{
	if (!p.first)
	{
		NodeInAVL* n = new NodeInAVL(x, y, left, right);
		return { n,n };
	}
	pair <NodeInAVL*, NodeInAVL*> result;
	NodeInAVL* help;
	int i;

	if (x < p.first->x || (x == p.first->x && y < p.first->y)) {
		result = AddDirection(x, y, left, right, { p.first->Sleft,p.second });
		p.first->Sleft = result.first;

	}
	else if (x > p.first->x || (x == p.first->x && y > p.first->y)) {
		result = AddDirection(x, y, left, right, { p.first->Sright,p.second });
		p.first->Sright = result.first;
	}

	if (p.first->Sleft || p.first->Sright) {
		i = max(height_level(p.first->Sleft), height_level(p.first->Sright));
		p.first->height_level = 1 + i;
	}

	NodeInAVL* ina = result.second;
	int b = BalanceFactor(p.first);

	//left left case
	if (b > 1 && (x < p.first->Sleft->x || (x == p.first->Sleft->x && y < p.first->Sleft->y)))
		return { RightRotate(p.first),ina };

	//right right case
	if (b < -1 && (x > p.first->Sright->x || (x == p.first->Sright->x && p.first->y > p.first->Sright->y)))
		return { LeftRotate(p.first) ,ina };

	//left right case
	if (b > 1 && (x > p.first->Sleft->x || (x == p.first->Sleft->x && y > p.first->Sleft->y)))
	{
		help = LeftRotate(p.first->Sleft);
		p.first->Sleft = help;
		return{ RightRotate(p.first),ina };
	}
	// right left case
	if (b < -1 && (x < p.first->Sright->x || (x == p.first->Sright->x && y < p.first->Sright->y)))
	{
		help = RightRotate(p.first->Sright);
		p.first->Sright = help;
		return{ LeftRotate(p.first),ina };
	}

	return { p.first, ina };
}

NodeInAVL* Directions_avl::Find_closes_Button_Y(double x, double y, NodeInAVL* nil)
{

	if (!nil->Sleft && !nil->Sright)
		return nil;
	if (nil->x < x || (nil->Sright && nil->Sright->x == x && nil->Sright->y < y))
		return Find_closes_Button_Y(x, y, nil->Sright);
	if (nil->x > x || (nil->Sleft && nil->Sright->x == x))
		return Find_closes_Button_Y(x, y, nil->Sleft);
	return nil;
}


NodeInAVL* Directions_avl::Find_closes_Above_Y(double x, double y, NodeInAVL* nil)
{
	if (!nil->Sleft && !nil->Sright)
		return nil;
	if (nil->x > x || (nil->Sleft && nil->Sleft->x == x && nil->Sleft->y > y))
		return Find_closes_Above_Y(x, y, nil->Sleft);
	if (nil->x < x || (nil->Sright && nil->Sright->x == x))
		return Find_closes_Above_Y(x, y, nil->Sright);
	return nil;
}

NodeInAVL* Directions_avl::Find_nearest_Button_X(double x, double y, NodeInAVL* nil)
{
	if (!nil)
		return nullptr;

	NodeInAVL* closest = nullptr;NodeInAVL* help = nullptr;

	if (nil->x < x && nil->y == y)
		closest = nil;

	if (nil->x < x)
		help = Find_nearest_Button_X(x, y, nil->Sright);

	if(!help )
			help = Find_nearest_Button_X(x, y, nil->Sleft);

	if (help && (!closest || help->x > closest->x))
	closest = help;
	return closest;
}

NodeInAVL* Directions_avl::Find_nearest_Above_X(double x, double y, NodeInAVL* nil)
{
	if (!nil)
		return nullptr;

	NodeInAVL* closest = nullptr;NodeInAVL* help = nullptr;

	if (nil->x > x && nil->y == y)
		closest = nil;

	if (nil->x > x)
		help = Find_nearest_Above_X(x, y, nil->Sleft);
	
	if(!help)
		help = Find_nearest_Above_X(x, y, nil->Sright);
	
	if (help && (!closest || help->x < closest->x))
		closest = help;
	return closest;
}


NodeInAVL* Directions_avl::Insert(double x, double y, int left, int right)
{
	pair <NodeInAVL*, NodeInAVL*> result = AddDirection(x, y, left, right, { root,nullptr });
	root = result.first;
	return result.second;
}



NodeInAVL* Directions_avl::SearchCoor(double x, double y)
{
	NodeInAVL* help = SearchPoint(x, y, root);
	return help;
}

NodeInAVL* Directions_avl::Checking_and_update_the_legality_of_DeadEnd(double x, double y, int direction)
{
	NodeInAVL* nil;
	switch (direction)
	{
	case 1:nil = Find_closes_Button_Y(x, y, root);break;
	case 2:nil = Find_closes_Above_Y(x, y, root);break;
	case 3:nil = Find_nearest_Button_X(x, y, root);break;
	default:nil = Find_nearest_Above_X(x, y, root);break;
	}
	return nil;
}
