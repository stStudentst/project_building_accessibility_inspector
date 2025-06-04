#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
struct Triple {
    float scoreDisabled, scoreBlind, score_Deaf;

    Triple(float x, float y, float z) : scoreDisabled(x), scoreBlind(y), score_Deaf(z) {}
};

class score_by_element
{
public:
    vector<Triple>Scores[7];
    score_by_element(){}
    void loadScoresFromFile(string filename);

};

