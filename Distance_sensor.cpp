//#include "Distance_sensor.h"
//
//using namespace std;
//
//double generateRandomDouble_straight()
//{
//    return static_cast<double>(rand()) / RAND_MAX * 130.0;
//}
//
//double generateRandomDouble_Sides()
//{
//	return static_cast<double>(rand()) / RAND_MAX * 500.0;
//}
//double generateRandomDouble_Top()
//{
//	return static_cast<double>(rand()) / RAND_MAX * 300.0;
//}
//
////direction = 1 ���
////direction = 2 ����
////direction = 3 ����
////direction = 4 �����
//double Distance_sensor::scan(int direction)
//{
//    srand(static_cast<unsigned int>(time(nullptr))); 
//	double randomValue;
//	switch (direction)
//	{
//	case 1: //���
//		randomValue = generateRandomDouble_straight();
//		break;
//	case 4:
//		randomValue =  generateRandomDouble_Top();
//		break;
//	default:
//		randomValue = generateRandomDouble_Sides();
//		break;
//	}
//	
//	return randomValue;
//}
