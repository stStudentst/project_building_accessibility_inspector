#include "score_by_element.h"


// ������� ������ �� ����� ������ �� �����
void score_by_element::loadScoresFromFile(string filename) {
    ifstream file(filename); // ����� ���� ������
    if (!file.is_open()) {
        cerr << "�����: �� ���� ����� �� �����\n";
        return;
    }

    string line;
    int index = 0;

    // ����� ����� ���� ��� ����
    while (getline(file, line) && index < 7) {
        istringstream iss(line);
        float x, y, z;

        // ����� �� ������� �� ������ ������
        while (iss >> x >> y >> z) {
            Scores[index].emplace_back(x, y, z); // ����� �������� ����� ������
        }
        ++index; // ���� ��� ��� �����
    }

    file.close(); // ����� �����
}