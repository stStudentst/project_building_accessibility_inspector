//#include "Directions_avl.h"
//#include "SurfacePoint.h"
#include "Steps_Instack.h"

void Steps_Instack::AddCoordinate(double x, double y)
{
	SurfacePoint sf(x, y, nullptr);
	SFstack.push(sf);
	count_steps++;
}


SurfacePoint* Steps_Instack::Get_Cooridnate_to_addDirection(int num)
{
	SurfacePoint* s = nullptr;
	while (num > 0 && !SFstack.empty())
	{
		SFhelp_addDirection.push(SFstack.top());
		SFstack.pop();
		num--;
	}
	s = &(SFstack.top());

	while (!SFhelp_addDirection.empty())
	{
		SFstack.push(SFhelp_addDirection.top());
		SFhelp_addDirection.pop();
	}
	
	return s;
}

SurfacePoint* Steps_Instack::Get_coordinate_to_Keep_scan()
{
	while (!SFstack.empty()&& (SFstack.top().nil == nullptr))
	{
		SFstack.pop();
	}
	if(!SFstack.empty())
		return &SFstack.top();
	return nullptr;
}

void Steps_Instack::print()
{
	int i;
	stack<SurfacePoint> Stemp;
	int size = SFstack.size();
	for ( i = 0; i < size ; i++)
	{
		cout <<"x:" <<SFstack.top().x << " " <<"y:"<< SFstack.top().y<<  endl;
		Stemp.push(SFstack.top());
		SFstack.pop();
	}
	for ( i = 0; i < size; i++)
	{
		SFstack.push(Stemp.top());
		Stemp.pop();
	}

}
