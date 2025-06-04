#pragma once
#include <filesystem>
#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <sstream> 
#include <string>
#include <filesystem>
#include "AddElement.h"
#include "Steps_Instack.h"

using namespace std;
namespace fs = std::filesystem;

class calculateAccessibility 
{
	#define NewElwment 90
	#define	Same 80
	#define	Finish 70
	
	#define usualElement 4
	#define gradient 0					//שיפוע הדרך
	#define elevator 1				    //מעלית
	#define DeadEnd 2					//מבוי סתום
	#define stairlift 3					//מעלון
	#define stairsNotInStairwell 4	    //מדרגות שאינן חלק מחדר מדרגות
	#define escalator 5					//מדרגות נעות
	#define utilityRoom 6				// בית שימוש
	#define switchesAndButtons 7	    // מפסקים ולחיצי הפעלה


	double percent_for_disabled = 0, percent_for_blind = 0, percent_for_deaf = 0;
public:
	calculateAccessibility(){}
	

	void Width_C(AddElement* AE, Steps_Instack* SI);
	void Height_C(AddElement* AE, Steps_Instack* SI);
	void Calculate_rest_Element(AddElement* AE);
	string DetermindTypeElement(int e);
	void AccessibilityCalculationlate(Steps_Instack* SI, AddElement* AE);
	
};
