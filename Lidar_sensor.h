#pragma once
#include <iostream>     
#include <fstream> 
#include <vector>       
#include <string>       
#include <sstream>      
#include <cmath>        
#include <filesystem>
#include <cstdlib>
#include <random>
#include <tuple>
#include <stdexcept>//לזריקת שגיאה מובנית


using namespace std;
namespace fs = std::filesystem;
//
//class Point {
//public:
//    double x;
//    double y;
//    double z;
//
//    Point(double x, double y, double z): x(x),y(y),z(z){}
//};

class Lidar_sensor
{
    const string FILE_path= "C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/points_lidar_for_slope.txt";
    int lineRead = 0;
public:
    Lidar_sensor() {}
    
    pair<double, double> calculateSlopeAndLengthFromRegression();

    double scan(int direction,int count_step);
    double Get_point_off_element(double min, double max);

};