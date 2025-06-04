#include "Lidar_sensor.h"


double generateRandomDouble_straight(int step)
{
   // return static_cast<double>(rand()) / RAND_MAX * 130.0;
    string path = "C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/distanceStraight.txt";
    ifstream inputFile(path);
    string line, readLine;
    int currentLineNumber = 0;
    if (inputFile.is_open()) {
        while (getline(inputFile, line) && currentLineNumber < step)
            currentLineNumber++;

        return stod(line);
    }
}

double generateRandomDouble_Right(int step)
{
    // return static_cast<double>(rand()) / RAND_MAX * 500.0;
    string path = "C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/distanceRight.txt";
    ifstream inputFile(path);
    string line, readLine;
    int currentLineNumber = 0;
    if (inputFile.is_open()) {
        while (getline(inputFile, line) && currentLineNumber < step)
            currentLineNumber++;

        return stod(line);
    }
}

double generateRandomDouble_Left(int step)
{
    string path = "C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/distanceLeft.txt";
    ifstream inputFile(path);
    string line, readLine;
    int currentLineNumber = 0;
    if (inputFile.is_open()) {
        while (getline(inputFile, line) && currentLineNumber < step)
            currentLineNumber++;
        return stod(line);
    }
}

double generateRandomDouble_Top()
{
   return static_cast<double>(rand()) / RAND_MAX * 300.0;
}
double Lidar_sensor::scan(int direction,int count_step)
{
    //direction = 1 ���
    //direction = 2 ����
    //direction = 3 ����
    //direction = 4 �����
   
    double value;
    switch (direction)
    {
    case 1:value = generateRandomDouble_straight(count_step);  break;
    case 2:value = generateRandomDouble_Right(count_step); break;
    case 3:value = generateRandomDouble_Left(count_step);
        if (value ==129) {
            cout << "need check from here" << endl;
        } break;
    default: 
        srand(static_cast<unsigned int>(time(nullptr)));
            value = generateRandomDouble_Top();
        break;
    }
    return value;
}

//����� ���� ����� ������
double Lidar_sensor::Get_point_off_element(double min,double max)
{
    // ����� ����� ������� ������
    random_device rd;
    mt19937 gen(rd());

      //���� ������� ����� ���� ������ ������
    uniform_real_distribution<> distrib(min, max);

    // ���� ���� �����
    return distrib(gen);
}



// ������� ������ ������ �������� �� ������ Z ��� Y ������� �� ������
// ���� ���� ����� ��� ������
// ���� ���: �� ���� ����� ��� �� ����� Z,Y,Z,Y,...
pair<double, double> Lidar_sensor::calculateSlopeAndLengthFromRegression() {
    ifstream file(FILE_path);
    if (!file.is_open()) {
        throw runtime_error("����� ������ �����");
    }

    string line;
    int currentLine = 0;
    while (getline(file, line)&&currentLine<lineRead) 
        currentLine++;
    
    if (currentLine != lineRead) {
        throw out_of_range("���� ���� �� ���� �����");
    }

    vector<double> zs, ys;
    stringstream ss(line);
    double z, y;
    while (ss >> z >> y) {
        zs.push_back(z);
        ys.push_back(y);
    }
    if (zs.size() < 2) {
        throw runtime_error("�� ����� ����� ������ ������");
    }

    int N = zs.size();
    double sumZ = 0, sumY = 0, sumZY = 0, sumZ2 = 0;
    for (int i = 0; i < N; ++i) {
        sumZ += zs[i];
        sumY += ys[i];
        sumZY += zs[i] * ys[i];
        sumZ2 += zs[i] * zs[i];
    }

    double slope = (N * sumZY - sumZ * sumY) / (N * sumZ2 - sumZ * sumZ);
    double slopePercent = slope * 100.0;

    // ����� ���� ������ - ���� ��� ��� ����� ������ �������
    double dz = zs.back() - zs.front();
    double dy = ys.back() - ys.front();
    double length = sqrt(dz * dz + dy * dy);
    (lineRead >= 30) ? lineRead = 0 : lineRead++ ;
    return { slopePercent, length };
}