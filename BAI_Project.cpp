// BAI_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "Lidar_sensor.h"
#include "AddElement.h"
#include "Directions_avl.h"
#include "Steps_Instack.h"
#include "calculateAccessibility.h"
#include "Main_process_management.h"
#include "score_by_element.h"
#include <vector>
#include <string.h>
using namespace std;


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
int main()
{
    Main_process_management mpm;
    Steps_Instack S_I;
    AddElement AE;
    Lidar_sensor  LS;
    Directions_avl DA;
    Check_Camera CC;
    calculateAccessibility C; 
    score_by_element sbe;

    double all_params[14];
    double* ptrAll_params = all_params;

    all_params[WIDTH_OF_ROBOT] = 30;
    all_params[height_OF_ROBOT] = 30;

    //����� ���� ������ ������� ����� (����� ���� ����� ��) ���� �������
    all_params[distance_right] = LS.scan(2,0);
    all_params[distance_left] = LS.scan(3,0);
    
    //���� ������ ������� ����� ���� ������ ������.
    all_params[last_left_distance] = all_params[distance_left]; 
    all_params[last_right_distance] = all_params[distance_right];

    
    //����� ����� ���� �1. �"� ���� ���� �����- ���� ����� ������ ���� ������ �����
    all_params[new_direction_l] = 1;
    all_params[new_direction_r] = 1;

    //����� ���� ������� ������ �1.
    all_params[count_step_l]=0;
    all_params[count_step_r]=0;

    // ������� ������ ������ ����� �� ����� �� ������ ��� ������ �����- ����� ���� ������
    mpm.Go_To_Start(&ptrAll_params);

    //���� ������ ����� �� ����� �� ����- ����� �� ������� �0,0  - ����� ������
    all_params[coord_X] = 0;
    all_params[coord_y] = 0;

    //����� ����� ������= 1 ���� ��� ���
    all_params[Status_direction] = 1;

    S_I.AddCoordinate(all_params[coord_X], all_params[coord_y]);
  
    //������� ������
    while (!S_I.SFstack.empty())
    {
        all_params[count_step_l]++;
        all_params[count_step_r]++;

        //����� ������ ����
        mpm.Height_check(&ptrAll_params,&AE);

        //����� ������ �����   
        mpm.Handling_in_new_Directions(&ptrAll_params,&S_I,&DA);

        all_params[distance_straight] = LS.scan(1,(S_I.count_steps-1));

        sbe.loadScoresFromFile("C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/score.txt");
        //����� ����� ����� ������ ���� ������.
        mpm.Open_camera(&ptrAll_params,&CC,&S_I,&AE,&sbe);

        //����� �� ��� ��� ������ ������� ����� ���� �� ����� ������ ����
        if (!mpm.Meeting_point_check(&ptrAll_params,&DA,&S_I))
        {
           mpm.Width_check(&ptrAll_params,&AE);
           mpm.Cehck_slope(&ptrAll_params,&CC,&AE,&sbe);
           mpm.Checking_the_direction_of_continuation(&ptrAll_params, &DA, &AE, &S_I);
        } 
        if (!S_I.SFstack.empty())
        {
            mpm.UpdateCoordinate(&ptrAll_params);
            S_I.AddCoordinate(all_params[coord_X], all_params[coord_y]);
        }
    }
    cout << "finish scan - now calculate final accessibility" << endl;
    C.AccessibilityCalculationlate(&S_I,&AE);

   /* Steps_Instack stack;
    Directions_avl avl;
    stack.AddCoordinate(3, 2 );
    stack.AddCoordinate(4, 2 );
    stack.AddCoordinate(5, 2 );
    stack.print();
    int center = 2;
    stack.SFstack.pop();
    NodeInAVL* result = avl.Insert(stack.SFstack.top().x, stack.SFstack.top().y, 3, 0);
    cout << "  result->x: " << result->x << endl;
    cout << " avl.root->x: " << avl.root->x << endl;
    stack.AddCoordinate(3, 1);
    NodeInAVL* result3 = avl.Insert(8, 1, 3, 0);
    cout  << avl.root->Sright->x << endl;
    NodeInAVL* result2 = avl.Insert(5, 1, 3, 0);
    cout << avl.root->Sright->x<< avl.root->Sleft->x << endl;
    NodeInAVL* result5 = avl.Insert(9, 1, 3, 0);
    cout << avl.root->Sright->x << avl.root->Sleft->x << endl;
    NodeInAVL* result222 = avl.Insert(10, 1, 3, 0);
    cout << avl.root->Sright->x << avl.root->Sleft->x << endl;
    NodeInAVL* result233 = avl.Insert(11, 1, 3, 0);
    cout << avl.root->Sleft->Sleft->x << endl;

    return 0;*/
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
