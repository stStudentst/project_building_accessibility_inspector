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
////direction = 1 ישר
////direction = 2 ימין
////direction = 3 שמאל
////direction = 4 למעלה
//double Distance_sensor::scan(int direction)
//{
//    srand(static_cast<unsigned int>(time(nullptr))); 
//	double randomValue;
//	switch (direction)
//	{
//	case 1: //ישר
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
