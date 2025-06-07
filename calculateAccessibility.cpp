
#include "calculateAccessibility.h"

using namespace std;
namespace fs = filesystem;

string filePath = "C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/Building_Accessibility_file.txt";
ofstream myFile; 
void Writing(string txt, int f)
{
	if (!myFile.is_open()) {
		// אם הקובץ לא פתוח, ננסה לפתוח אותו בהתאם לקיום
		if (!fs::exists(filePath)) {
			myFile.open(filePath); // יצירת קובץ חדש
			if (myFile.is_open()) {
				cout << "File created successfully: " << filePath << endl;
			}
			else {
				cerr << "Error creating file: " << filePath << endl;
				return;
			}
		}
		else {
			myFile.open(filePath, ios::app); // הוספה לסוף הקובץ הקיים
			if (!myFile.is_open()) {
				cerr << "Error opening file for appending: " << filePath <<endl;
				return;
			}
		}
	}
	
	if (f == Finish) {
		if (myFile.is_open()) {
			myFile.close();
			cout << "File closed. You can find it here: " << filePath << endl;
		}
	}
	else {
		if (myFile.is_open()) {
			myFile << txt << endl; // הוספת המחרוזת ושורה חדשה
			cout << "Wrote to file successfully." << endl;
		}
		else {
			cerr << "Error: File is not open for writing." << endl;
		}
	}
}

//void Writing(string txt, int f)
//{
//	ofstream myFile;
//	if (f == NewElwment)
//	{
//		
//		string path = "C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/my_camera_file.txt";
//		if (fs::exists(path))
//		{
//			ofstream myFile(path); // יצירת קובץ בשם "my_file.txt"
//			cout << "The file was created successfully." << endl;
//		}
//		else {
//			cout << "Error creating file." << endl;
//		}
//	}
//	else if (myFile.is_open())
//		if (f == Same){
//
//			ofstream myFile("my_file.txt", ios::app); // מצב append – מוסיף לסוף
//			myFile << txt;
//			cout << " writed successfully." << endl;
//		}
//		if (f == Finish)
//			myFile.close();
//		
//			
//	else
//		cout << "Error in write to the file" << endl;
//}


void calculateAccessibility::Width_C(AddElement* AE, Steps_Instack* SI)
{
	
	if ( AE->listWidth.empty())
	{
		ostringstream oss;
		oss << "Free Width: 100 percent!";
		Writing(oss.str(),Same);
		return;
	}
		
	int lastX, lastY, lastparameter = 0;

	double unlegal=0;

	bool start = false;
	
	for (Special_parameters& param : AE->listWidth) {
		ostringstream oss;
		//בודק אם נמצא על אותו מסלול- כך בודק אם ההפרש בין הצירויות- נשאר חוקי
		if (lastparameter != 0 && ((lastX == param.x && lastY - param.y < abs(200)) || (lastY == param.y && lastX - param.x < abs(200))))
		{
			if (!start)
			{
				oss << "\n	AT " << lastX << ", " << lastY << "there is a narrowing in width that is too long. Along the points:\n " << param.x << ", " << param.y << ". ";
				start = true;
			}
			else
				oss << param.x << ", " << param.y << ".  ";
			unlegal++;
		}
		else
			start = false;
		//בודק שההיצר ברוחב חוקי
		if (param.s_p < 80)
		{
			if (!start)
				oss << param.x << "' " << param.y << " - ";
			oss << "(At this point the width is too smalland illegal even if it is not too long).\n";
			unlegal += 1;//נורא ביותר שיש היצר כלכך קטן- א"א לעבור
		}
		if (!oss.str().empty())
			Writing(oss.str(), Same);
			
		
		lastparameter = param.s_p;
		lastX = param.x;
		lastY = param.y;
		
	}
	
	
	lastX = 0;
	lastY = 0;
	lastparameter = 0;
	//איזון לפני חזרה 
	percent_for_disabled += ((SI->count_steps-unlegal) / SI->count_steps) * 100;
	Writing(".\n\n",Same);
}

void calculateAccessibility::Height_C(AddElement* AE,Steps_Instack* SI)
{
	ostringstream oss;
	//double ScoreFreeHeight =AE-> Get_scoreFreeHeight();
	 (AE->ScoreFreeHeight == 0)?
		oss << "Free height:\n	100 percent compliant with the rules.\n" :
		oss << "Free height:\n	" <<(AE-> ScoreFreeHeight / SI->count_steps) * 100 << " percent un legal height\n";
	 Writing(oss.str(), NewElwment);
}

void calculateAccessibility::Calculate_rest_Element(AddElement* AE)
{
	
	int s_disabled, s_sd, s_sb;
	//Rest_of_the_parameters* ptrVec = AE->GetVacorElements();
	for (int i = 0;i < 8;i++)
	{
		ostringstream oss;
		
		string typeElement = DetermindTypeElement(i);
		int items = AE->vectorElements[i].count_Items;
		s_disabled = AE->vectorElements[i].score_Disabled / items*100;
		s_sb = AE->vectorElements[i].score_Blind / items * 100;
		s_sd = AE->vectorElements[i].score_Deaf / items * 100;
		
		if(items>0)
		{
			oss << typeElement
				<<s_disabled<< " percent accessibility for people with walking disabilities.\n"
				<<s_sb << "  percent accessibility for the blind.\n"
				<< s_sd<< "  percent accessibility for the deaf.\n"
				<< " In total, there are " << AE->vectorElements[i].count_good << " accessible out of" << items << ". \n\n";
			
			percent_for_disabled += s_disabled;
			percent_for_deaf += s_sd;
			percent_for_blind += s_sb;
		}
		else {
			oss << typeElement << "Not found in the building \n";
			percent_for_disabled += 100;
			percent_for_deaf += 100;
			percent_for_blind += 100;
		}
		Writing(oss.str(), Same);
		
	}
	ostringstream oss;
	oss<< "In summary: the building is " << percent_for_disabled /10 << "% accessible to the disabled, " << percent_for_blind /100/ 9*100
		<< "% to the blind, and " << percent_for_deaf /100/ 9*100 << "% to the deaf.\n";
	Writing(oss.str(), Same);
}

string calculateAccessibility::DetermindTypeElement(int e)
{
	switch (e)
	{
		case gradient:return "\n Gradient:\n	"; break;
		case switchesAndButtons:return "\n Switches and Buttons:\n	"; break;
		case utilityRoom:return "\n Utility Room:\n"; break;
		case escalator:return "\n Escalator:\n	"; break;
		case stairsNotInStairwell:return "Stairs Not In Stairwell:\n"; break;
		case stairlift:return "Stairlift:\n	"; break;
		case elevator:return "Elevator: \n"; break;
		case DeadEnd: return "Dead End:\n"; break;
	}
}

void calculateAccessibility::AccessibilityCalculationlate(Steps_Instack* SI,AddElement* AE)
{
	//קודם לשלוח את מבוי סתום- ואז בדיקה על רוחב
	Writing("Accessibility of the building\n\n", NewElwment);
	Width_C(AE,SI);
	Height_C(AE,SI);
	Calculate_rest_Element( AE);
	Writing("", Finish);
}

