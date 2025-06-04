#include "AddElement.h"


void AddElement::AddFreeWidth(double x, double y , double w)
{
	try
	{
		listWidth.push_front(Special_parameters(x, y, w));
	}
	catch (...)
	{
	}
}



void AddElement::UpdateHeight(double precent_unlegal)
{
	ScoreFreeHeight += precent_unlegal;
	//לאחר הוספת כל אחוזי כשל גובה- בעת חישוב סופי של אחוזים זה ישוקלל ביחס לכל פסיעות הרובוט
}

void AddElement::Update_Rest_element( double sdisabled, double sb, double sd,int index)
{
	if (vectorElements[index].count_Items == NULL)
		vectorElements[index] = Rest_of_the_parameters(sdisabled, sb, sd);
	else
	{
		int items = vectorElements[index].count_Items;
		vectorElements[index].score_Disabled +=sdisabled;
		vectorElements[index].score_Blind  += sb;
		vectorElements[index].score_Deaf +=sd;
		vectorElements[index].count_Items++;

		if (sd == 1 && sb == 1 && sdisabled == 1)
		{
			vectorElements[index].count_good++;
		}
	}
}

void AddElement::Add_dead_end(double x, double y, int direction,Directions_avl* DA)
{
	NodeInAVL* nil = DA->Checking_and_update_the_legality_of_DeadEnd(x, y, direction);
	if (direction == 1 || direction == 2)
	{
		if (abs(nil->y - y) >= 1000)
			Update_Rest_element(0, 1, 1, DeadEnd);
		else
			Update_Rest_element(1, 1, 1, DeadEnd);
	}
	else if(abs(nil->x-x)>=10)
		Update_Rest_element(0, 1, 1, DeadEnd);
	else
		Update_Rest_element(1, 1, 1, DeadEnd);
}




