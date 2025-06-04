#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Distance_sensor.h"
#include "Directions_avl.h"
#include "SurfacePoint.h"
#include "Steps_Instack.h"
#include "StructElements.h"
#include "AddElement.h"
#include "Lidar_sensor.h"
#include "Check_Camera.h"
#include "score_by_element.h"

using namespace std;

class Main_process_management
{
	#define coord_X 0		       // ����� X �����
	#define coord_y 1              // ����� Y �����
	#define distance_straight 2	   //���� �����
	#define distance_right 3	   //���� �����
	#define distance_left 4		   //���� �����
	#define last_right_distance 5  //���� ���� �����
	#define last_left_distance 6   //���� ���� �����
	#define count_step_l 7         //���� ���� ������� ���� �� ����
	#define count_step_r 8         //���� ���� ������� ���� �� ����
	#define new_direction_l 9      //����� ����� �� ���� ������� ���� �����  1=��� 0 == ��
	#define new_direction_r 10     //����� ����� �� ���� ������� ���� ����� 1=��� 0 == ��
	#define WIDTH_OF_ROBOT 11      //���� �� ������
	#define height_OF_ROBOT 12     //���� �� ������
	#define Status_direction 13    //����� ���� ���� ������- ������ ���� �� ���������� //up=1=Y++, down=2=y--, right=3=x++, left=4=x--
	
public:
	Main_process_management() {}
	Lidar_sensor LS;
	
	string filepath = "C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/camera.txt";

	void Go_To_Start(double* all_params[]);
	void Handling_in_new_Directions(double* all_params[], Steps_Instack* SI, Directions_avl* D_AVL);
	void Adding_Direction(double* all_params[], bool r, bool l, Steps_Instack* SI, Directions_avl* D_AVL);

	void Open_camera(double* all_params[],Check_Camera* CC, Steps_Instack* SI,AddElement* AE,score_by_element* sbe);
	
	void Height_check(double* all_params[],AddElement* AE);
	void Width_check(double* all_params[], AddElement* AE);
	void Dead_end_check(double* all_params[], Directions_avl* D_AVL, AddElement* AE);
	void UpdateCoordinate(double* all_params[]);
	void Go_back(double* all_params[],Steps_Instack* SI);
	bool Meeting_point_check(double* all_params[], Directions_avl* D_AVL,Steps_Instack* SI);
	bool Checking_the_direction_of_continuation(double* all_params[],Directions_avl* D_AVL, AddElement* AE, Steps_Instack* SI);
	void Cehck_slope(double* all_params[],Check_Camera* CC,AddElement* AE,score_by_element* sbe);
};

