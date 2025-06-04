#include <iostream>
#include "Main_process_management.h"


using namespace std;

void Main_process_management::Go_To_Start(double* all_params[])
{
	int steps = ((*all_params)[distance_left] + (*all_params)[distance_right] + (*all_params)[WIDTH_OF_ROBOT]) / 2;
	int h = steps;
	if ((*all_params)[distance_left] > (*all_params)[distance_right])
	{
		steps = steps - (*all_params)[distance_right];
		//Go to the left side the number of steps in steps
		//          ----
	}
	else
	{
		steps = steps - (*all_params)[distance_left];
		//Go to the right side the number of steps in steps
		//          ----D_AVL
	}
	(*all_params)[distance_left] = h;
	(*all_params)[distance_right] = h;
}


void Main_process_management::Handling_in_new_Directions(double* all_params[], Steps_Instack* SI, Directions_avl* D_AVL)
{
	(*all_params)[last_left_distance] = (*all_params)[distance_left];
	(*all_params)[last_right_distance] = (*all_params)[distance_right];
	(*all_params)[distance_right] = LS.scan(2, SI->count_steps);
	(*all_params)[distance_left] = LS.scan(3, SI->count_steps);
	
	// Check and update for the both sides
	if ((*all_params)[last_left_distance] - (*all_params)[distance_left] > 40
		&& (*all_params)[last_right_distance] - (*all_params)[distance_right] > 40
		&& (*all_params)[new_direction_r] == 1 && (*all_params)[new_direction_l] == 1)
		Adding_Direction(all_params, true, true, SI, D_AVL);
	else
	{
		// Check and update for the left side
		if ((*all_params)[distance_left] - (*all_params)[last_left_distance] > 40 )
		{
			(*all_params)[count_step_l] = 1;
			(*all_params)[new_direction_l] = 1;
		}
		else if ((*all_params)[last_left_distance] - (*all_params)[distance_left] > 40
			&& (*all_params)[new_direction_l] == 1)
			Adding_Direction(all_params, false, true, SI, D_AVL);
			
		
		//Check and update for the right side
		 if ((*all_params)[distance_right] - (*all_params)[last_right_distance] > 40)
		{
			(*all_params)[count_step_r] = 1;
			(*all_params)[new_direction_r] = 1;
		}
		else if ((*all_params)[last_right_distance] - (*all_params)[distance_right] > 40
			&& (*all_params)[new_direction_r] == 1)
			Adding_Direction(all_params, true, false, SI, D_AVL);
	}
}

void Main_process_management::Adding_Direction(double* all_params[], bool r, bool l, Steps_Instack* SI, Directions_avl* D_AVL)

{
	int numL = 0, numR = 0, D_r = 0, D_l = 0;
	int dir = (*all_params)[Status_direction];
	SurfacePoint* h;
	NodeInAVL* new_nil;

	switch (dir) {
	case 1:  D_r = 3; D_l = 4; break;
	case 2:  D_r = 4; D_l = 3; break;
	case 3:  D_r = 2; D_l = 1; break;
	default: D_r = 1; D_l = 2; break;
	}

	if (r) numR = floor((*all_params)[count_step_r] / 2);
	if (l) numL = floor((*all_params)[count_step_l] / 2);

	if (r && l && numL == numR) {
		h = SI->Get_Cooridnate_to_addDirection(numL);
		if (h)
		{
			new_nil = D_AVL->Insert(h->x, h->y, D_l, D_r);
			h->nil = new_nil;
			(*all_params)[new_direction_l] = 0;
			(*all_params)[new_direction_r] = 0;
		}
	}
	else
	{ 
		if (r) {
			h = SI->Get_Cooridnate_to_addDirection(numR);
			if (h)
			{
				new_nil = D_AVL->Insert(h->x, h->y, 0, D_r);
				h->nil = new_nil;
				(*all_params)[new_direction_r] = 0;
			}
		}
		if (l) {
			h = SI->Get_Cooridnate_to_addDirection(numL);
			if (h)
			{
				new_nil = D_AVL->Insert(h->x, h->y, D_l, 0);
				h->nil = new_nil;
				(*all_params)[new_direction_l] = 0;
			}
		}
	}
	
}


void Main_process_management::Height_check(double* all_params[],AddElement* AE)
{
	double height = LS.scan(4,0);
	if ((*all_params)[height_OF_ROBOT] + height < 177)
		AE->UpdateHeight(0.5);
	else if ((*all_params)[height_OF_ROBOT] + height < 200)
		AE->UpdateHeight(0.85);
}


void Main_process_management::Width_check(double* all_params[], AddElement* AE)
{
	double width = (*all_params)[WIDTH_OF_ROBOT] + (*all_params)[distance_right] + (*all_params)[distance_right];
	if (width < 130)
		AE->AddFreeWidth((*all_params)[coord_X], (*all_params)[coord_y], width);
}

void Main_process_management::Dead_end_check(double* all_params[], Directions_avl* D_AVL, AddElement* AE)
{
	double width = (*all_params)[WIDTH_OF_ROBOT] + (*all_params)[distance_left] + (*all_params)[distance_right];
	double last_width= (*all_params)[WIDTH_OF_ROBOT] + (*all_params)[last_left_distance] + (*all_params)[last_right_distance];
	if(D_AVL->root)
		if (width < 100 && last_width < 100)
			AE->Add_dead_end((*all_params)[coord_X], (*all_params)[coord_y], (*all_params)[Status_direction],D_AVL);
}

void Main_process_management::UpdateCoordinate(double* all_params[])
{
	if ((*all_params)[Status_direction] == 1)
		(*all_params)[coord_y] += 59;
	else if ((*all_params)[Status_direction] == 2)
		(*all_params)[coord_y] -= 59;
	else if ((*all_params)[Status_direction] == 3)
		(*all_params)[coord_X] += 59;
	else
		(*all_params)[coord_X] -= 59;
}


void Main_process_management::Open_camera(double* all_params[],Check_Camera* CC, Steps_Instack* SI,AddElement* AE,score_by_element* sbe)
{
	string element=CC->Read_file(filepath,all_params,SI,AE,&LS,sbe);
	if (element !="..." && element == "stairs not in stairwell")
		(*all_params)[distance_straight] == 0;
}



void Main_process_management::Go_back(double* all_params[],Steps_Instack* SI)
{
	SurfacePoint* s = SI->Get_coordinate_to_Keep_scan();
	if ((s)&&(s->nil))
	{
		(*all_params)[coord_X] = s->nil->x;
		(*all_params)[coord_y] = s->nil->y;
		if (s->nil->Dleft>0)
		{
			(*all_params)[Status_direction] = s->nil->Dleft;
			if (s->nil->Dright == 0)
				s->nil = nullptr;
			else s->nil->Dleft = 0;
		}
		else
		{
			if(s){
				(*all_params)[Status_direction] = s->nil->Dright;
				s->nil = nullptr;
			}
		}
		(*all_params)[count_step_r] = 0;
		(*all_params)[count_step_l] = 0;
		(*all_params)[new_direction_l] = 0;
		(*all_params)[new_direction_r] = 0;
	}
}

bool Main_process_management::Meeting_point_check(double* all_params[],Directions_avl* D_AVL,Steps_Instack* SI)
{
	NodeInAVL* nil = D_AVL->SearchCoor((*all_params)[coord_X], (*all_params)[coord_y]);
	if (nil == nullptr)
		return false;
	else
	{
		int dir = (*all_params)[Status_direction];
		switch (dir)
		{
			case 1:  if (nil->Dleft == 2) nil->Dleft = 0; else if (nil->Dright == 2) nil->Dright = 0; break;
			case 2:  if (nil->Dleft == 1) nil->Dleft = 0; else if (nil->Dright == 1) nil->Dright = 0; break;
			case 3:  if (nil->Dleft == 4) nil->Dleft = 0; else if (nil->Dright == 4) nil->Dright = 0; break;
			default: if (nil->Dleft == 3) nil->Dleft = 0; else if (nil->Dright == 3) nil->Dright = 0; break;
		}
		Go_back(all_params,SI);
		return true;
	}

}


bool Main_process_management::Checking_the_direction_of_continuation(double* all_params[], Directions_avl* D_AVL, AddElement* AE,Steps_Instack* SI)
{
	//בדיקה אם יכול להמשיך מסלול
	
	if ((*all_params)[distance_straight] < 70)
	{
		if ((*all_params)[new_direction_l]==1 && (*all_params)[new_direction_r]==1)
		{
			Adding_Direction(all_params, true, true, SI, D_AVL);
		}
		else if ((*all_params)[new_direction_l]==1)
		{
			Adding_Direction(all_params, false, true, SI, D_AVL);
		}
		else if ((*all_params)[new_direction_r]==1)
		{
			Adding_Direction(all_params, true, false, SI, D_AVL);
		}

		//עדכון מבוי סתום בעת הצורך
		Dead_end_check(all_params,D_AVL,AE);
		Go_back(all_params,SI);
		return false;
	}
	return true;
}

void Main_process_management::Cehck_slope(double* all_params[],Check_Camera* CC,AddElement* AE,score_by_element* sbe)
{
	pair<double, double>result = LS.calculateSlopeAndLengthFromRegression();
	if (result.first > 5)
	{
		ifstream inputFile(filepath);
		CC->Check_Gradient(inputFile,AE,&LS,sbe,true);
	}
}
	





