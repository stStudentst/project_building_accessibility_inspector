#include "score_by_element.h"


// פונקציה שקוראת את הקובץ וממלאת את המערך
void score_by_element::loadScoresFromFile(string filename) {
    ifstream file(filename); // פתיחת קובץ לקריאה
    if (!file.is_open()) {
        cerr << "שגיאה: לא ניתן לפתוח את הקובץ\n";
        return;
    }

    string line;
    int index = 0;

    // קריאת הקובץ שורה אחר שורה
    while (getline(file, line) && index < 7) {
        istringstream iss(line);
        float x, y, z;

        // קריאת כל שלישייה של מספרים מהשורה
        while (iss >> x >> y >> z) {
            Scores[index].emplace_back(x, y, z); // הוספת השלישייה למערך המתאים
        }
        ++index; // מעבר לתא הבא במערך
    }

    file.close(); // סגירת הקובץ
}