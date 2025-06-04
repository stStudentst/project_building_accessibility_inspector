#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <list>


namespace struct_Elements
{

	
	//class for Special parameters that require different calculation
	class Special_parameters 
	{
	public:
		double x,y, s_p;
		Special_parameters(double x, double y, double sp ): x(x),y( y), s_p(sp) {}
	};


	// class for the rest
	class Rest_of_the_parameters 
	{
	public:
		//   ניקוד לחירש   ניקוד לעיוור    ניקוד לנכה
		double score_Disabled, score_Blind, score_Deaf;
		int count_Items,count_good;
		Rest_of_the_parameters(){}
		Rest_of_the_parameters( double sdisabled, double sb, double sd) :
			score_Disabled(sdisabled),
			score_Blind(sb),
			score_Deaf(sd),
			count_Items(1) {
			count_good = (sd == 1 && sdisabled == 1 && sb == 1) ? 1 : 0;
		}
	};
};

#pragma once
