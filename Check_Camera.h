#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cctype> // ���� std::tolower
//#include <gorithm>
#include <tuple>
#include <cmath>
#include "AddElement.h"
#include "Steps_Instack.h"
#include "Lidar_sensor.h"
#include "score_by_element.h"

using namespace std;

class Check_Camera
{
	//all_params
	#define coord_X 0					// ����� X �����
	#define coord_y 1					// ����� Y �����
	#define distance_straight 2		    //���� �����
	#define Status_direction 13			//����� ���� ���� ������- ������ ���� �� ���������� //up=1=Y++, down=2=y--, right=3=x++, left=4=x--

	//Elements
	#define gradient 0					//����� ����
	#define elevator 1				    //�����
	#define DeadEnd 2					//���� ����
	#define stairlift 3					//�����
	#define stairsNotInStairwell 4	    //������ ����� ��� ���� ������
	#define escalator 5					//������ ����
	#define utilityRoom 6				// ��� �����
	#define switchesAndButtons 7	    // ������ ������ �����

	
	
	#define front 20					//�����
	#define forward	30					//�����
	#define perpendicular_right 40		//���� ������
	#define perpendicular_left 50		//���� ������
	#define diagonalLeft 60				//������ �����
	#define diagonalRight 70			//������ �����

	
	#define S_Elevator 0
	#define S_Stairs_not_in_stairwall 1
	#define S_Stairlift 2
	#define S_Escalator 3
	#define S_Switch 4
	#define S_UtilityRoom 5
	#define S_Gradient 6
	int l_ELEMENT=0;
public:
	 /*l_size_element = 0, l_sound = 0, l_camera, l_touch = 0, l_sizeLidar = 0, l_visual_contrast = 0*/
	Check_Camera() {}
	Lidar_sensor LS;
	
	string Read_file(string filePath, double* all_params[],Steps_Instack* SI, AddElement* AE, Lidar_sensor* Ls,score_by_element* sbe);
	
	tuple<double,double,bool> Get_Point_finding(double x, double y, double status_direction, double straight,Steps_Instack* SI);

	int Determine_direction_element(double x, double y, double status_direction, double E_x, double E_y);
	
	string ToLower(string str);
	bool Can_Check_later(double this_x, double this_y, double E_x, double E_y,int sd,Steps_Instack* SI);
	void Go_to_check(double E_x, double E_y, Steps_Instack* SI);
	bool Go_to_Check_straight(double this_x, double this_y, double E_x, double E_y, double sd,double straight,Steps_Instack* SI);
	void Come_back_to_continue_scanning(double this_x, double this_y, double p_x, double p_y, double sd, int d, Steps_Instack* SI);
	void Check_Elevator(ifstream& inputFile, int score_leading_marking, AddElement* AE, score_by_element* sbe);
	void Check_Stairs_Not_In_Stairwell(ifstream& inputFile, AddElement* AE, score_by_element* sbe);
	void Check_Stairlift(ifstream& inputFile , AddElement* AE, score_by_element* sbe);
	void Check_Escalator(ifstream& inputFile , AddElement* AE, score_by_element* sbe);
	void Check_SwitchAndButtons(ifstream& inputFile, AddElement* AE, score_by_element* sbe);
	void Check_Utility_Room(ifstream&, AddElement* AE, score_by_element* sbe);
	void Check_Gradient(ifstream&, AddElement* AE,Lidar_sensor* LS, score_by_element* sbe, bool KnowPercent);
	void Came_Back( double p_x, double p_y, Steps_Instack* SI);
	bool Is_close(double a, double b);

};