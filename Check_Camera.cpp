#include "Check_Camera.h"


tuple<double, double, bool> Check_Camera::Get_Point_finding(double x, double y, double status_direction, double straight, Steps_Instack* SI)
{
    double C_x, C_y;
    switch (int(status_direction))
    {
    //1=up, 2= down, 3= right, 4= left
    case 1:C_x = LS.Get_point_off_element(x-116,x+116); C_y = LS.Get_point_off_element(y,y+116);
        break;
    case 2: C_x = LS.Get_point_off_element(x-116,x+116);C_y = LS.Get_point_off_element(y - 116,y);
        break;
    case 3:C_x = LS.Get_point_off_element(x, x + 116);C_y = LS.Get_point_off_element(y - 116, y + 116);
        break;
    default:C_x = LS.Get_point_off_element(x - 116, x);C_y = LS.Get_point_off_element(y - 116, y + 116);
        break;
    }

    int B_d=Determine_direction_element(x,y,status_direction,C_x,C_y);
    bool NOW;
    switch (B_d) {

    case front: NOW = true; break;

    case perpendicular_right: Go_to_check(C_x, C_y,SI);
        NOW = true;  break;

    case perpendicular_left: Go_to_check(C_x, C_y,SI);
        NOW = true;  break;

    case forward:NOW = Go_to_Check_straight(x, y, C_x, C_y, status_direction,straight,SI);
        break;
    default: NOW = (Can_Check_later(x, y, C_x, C_y, status_direction,SI))? false: true;
        break;
    }
    return{ C_x, C_y, NOW };
}

int Check_Camera::Determine_direction_element(double x, double y, double status_direction, double E_x, double E_y)
{
    int d;
    switch (int(status_direction))
    {
    case 1:
        if(x == E_x)   (Is_close(y, E_y)) ? d = front : d = forward;

        else if (x > E_x)   (y = E_y) ? d = perpendicular_left : d = diagonalLeft;

        else  (y = E_y)? d = perpendicular_right: d = diagonalRight;

        break;
    case 2:
        if (x == E_x)   (Is_close(y, E_y))? d = front: d = forward;

        else if (x > E_x)   (y == E_y) ? d = perpendicular_right : d = diagonalRight;
       
        else  (y == E_y) ? d = perpendicular_left : d = diagonalLeft;
            
        break;
    case 3:
        if (x == E_x)  (y < E_y) ? d = perpendicular_left : d = perpendicular_right;
       
        else if (y == E_y)  (Is_close(x, E_x)) ? d = front : d = forward;
         
        else  (y < E_y)? d = diagonalLeft : d = diagonalRight;
          
        break;
    default:
        if (x == E_x)  (y < E_y)? d = perpendicular_right: d = perpendicular_left;
       
        else if (y == E_y) (Is_close(x, E_x))? d = front: d = forward;
          
        else  (y > E_y)? d = diagonalLeft: d = diagonalRight;
       
        break;
    }
    return d;
}
        
string Check_Camera::Read_file(string filePath, double* all_params[],Steps_Instack* SI, AddElement* AE,Lidar_sensor* Ls,score_by_element* sbe)
{
    int status_direction_here=(*all_params)[Status_direction];
    ifstream inputFile(filePath);
    string line, readLine;
    int currentLineNumber = 0;
    double C_X, C_Y;
    bool NOW=false;
    if (inputFile.is_open()) {
        while (getline(inputFile, line) && currentLineNumber < l_ELEMENT)
            currentLineNumber++;
        l_ELEMENT++;
        if (ToLower(line) != "nun") {
            auto [c_x, c_y, now] = Get_Point_finding(SI->SFstack.top().x, SI->SFstack.top().y, (*all_params)[Status_direction], (*all_params)[distance_straight], SI);
            C_X = c_x;C_Y = c_y;NOW = now;
            if (NOW) {
                l_ELEMENT++;
                readLine = (ToLower(line));
                if (readLine == "leading marking") {
                    while (getline(inputFile, line) && ToLower(line) == "leading marking")
                        l_ELEMENT++;
                    if (ToLower(line) == "elevator close") {
                        l_ELEMENT++;
                        Check_Elevator(inputFile, 1, AE, sbe);
                    }
                    else
                        l_ELEMENT--;
                }
                else if (readLine == "elevator close")
                    Check_Elevator(inputFile, 0, AE, sbe);
                else if (readLine == "stairs not in stairwell")
                    Check_Stairs_Not_In_Stairwell(inputFile, AE, sbe);
                else if (readLine == "stairlift")
                    Check_Stairlift(inputFile, AE, sbe);
                else if (readLine == "escalator")
                    Check_Escalator(inputFile, AE, sbe);
                else if (readLine == "switch or buttons")
                    Check_SwitchAndButtons(inputFile, AE, sbe);
                else if (readLine == "utility room open")
                    Check_Utility_Room(inputFile, AE, sbe);
                else if (readLine == "dradient")
                    Check_Gradient(inputFile, AE, Ls, sbe, false);
                int d = Determine_direction_element((*all_params)[coord_X], (*all_params)[coord_y], (*all_params)[Status_direction], C_X, C_Y);
                Come_back_to_continue_scanning((*all_params)[coord_X], (*all_params)[coord_y], C_X, C_Y, (*all_params)[Status_direction], d, SI);

            }

            if (l_ELEMENT >= 85) l_ELEMENT = 0;
            inputFile.close();
        }
        else {
            cerr << "orror can't open the file \n" << endl;
        }
        return (NOW) ? readLine : "...";
    }
}

string Check_Camera::ToLower(string str)
{
    // פונקציה להמרת מחרוזת לאותיות קטנות
    transform(str.begin(), str.end(), str.begin(),[](unsigned  char c) {return static_cast<unsigned char>(tolower(c));});
    return str;
}
bool Check_Camera::Is_close(double a, double b) { return abs(a - b) < 71; }

bool Check_Camera::Can_Check_later(double this_x, double this_y, double E_x, double E_y, int sd,Steps_Instack* SI)
{
    
    int Y_length, X_length, third_length,slope_angle;
    Y_length = abs(E_y - this_y);
    X_length = abs(this_x - E_x);
    third_length = sqrt(X_length ^ 2 + Y_length ^ 2);

    switch (sd)
    {
    case (1||2):
        if(Is_close(E_y, this_y))
        {
            slope_angle = (X_length != 0) ? atan(Y_length / X_length) * 180.0 : 0;
            cout << "turn " << slope_angle << " degrees. and go" << third_length << " to arive at Element point" << endl;
            Go_to_check(E_x, E_y, SI);
            return false;
        }
        else
            return true;
    default:
        if (Is_close(E_x,this_x))
        {
            slope_angle = (Y_length!=0)?atan(X_length / Y_length) * 180.0:0;
            cout << "turn " << slope_angle << " degrees. and go" << third_length << " to arive at Element point"<<endl;
            Go_to_check(E_x, E_y, SI);
            return false;
        }
        else
            return true;
    }
       
}

//הכנסת נקודה בדיקה למחסנית פסיעות
void Check_Camera::Go_to_check(double E_x, double E_y, Steps_Instack* SI)
{
    SurfacePoint pointElement(E_x, E_y, nullptr);
    SI->SFstack.push(pointElement);
}

//בדיקה האם לבדוק כרגע אלמנט שמופיע ממולי. (אם הוא ממולי אך רחוק אני אזהה אותו גם בפסיעה הבאה אז אין צורך כרגע) בבדיקה הבאה אני אבדוק.
bool Check_Camera::Go_to_Check_straight(double this_x, double this_y, double E_x, double E_y, double sd,double straight,Steps_Instack* SI)
{
    int l_y = abs(this_y - E_y), l_x = abs(this_x - E_x);
    switch (int(sd))
    {
    case (1||2): 
        if (straight < 70 || l_y < 60)
        {
            Go_to_check(E_x, E_y,SI);
            cout << "go straight" << l_y << "to check element" << endl;
            return true;
        }
        return false;

    default:
        if (straight < 70 || l_x < 60)
        {
            Go_to_check(E_x, E_y,SI);
            cout << "go straight" << l_x << "to check element" << endl;
            return true;
        }
        return false;
    }
}

//חזרה מבדיקה להמשך סריקה
void Check_Camera::Came_Back(double p_x, double p_y, Steps_Instack* SI)
{

    while (/*(!SI->SFstack.empty()) && */ !(SI->SFstack.top().x == p_x && SI->SFstack.top().y == p_y))
    { 
        SI->SFstack.pop();
    }
}




void Check_Camera::Check_Elevator(ifstream& inputFile,int score_leading_marking, AddElement* AE, score_by_element* sbe)
{
    int i = 0;
    string line;
    double score_Disabled = 0, score_Blind = 0, score_Deaf = 0;
    if (inputFile.is_open()) {
        
        cout << "check elevator NOW" << endl;

        if (getline(inputFile, line)) {

            if (ToLower(line) == "true") {
                l_ELEMENT++;
                if (getline(inputFile, line)) {

                    if (ToLower(line) == "true") {
                        l_ELEMENT++;
                        score_Disabled +=sbe->Scores[S_Elevator][i++].scoreDisabled;
                    }
                }
            }
            else if (ToLower(line) == "false") {
                l_ELEMENT++;
                score_Disabled += sbe->Scores[S_Elevator][i++].scoreDisabled;
            }
            if (getline(inputFile, line)) {
                if (ToLower(line) == "elevator open") {
                    l_ELEMENT++;
                    if (getline(inputFile, line)) {
                        l_ELEMENT++;
                        if (stod(line) >= 80)
                            score_Disabled += sbe->Scores[S_Elevator][i++].scoreDisabled;

                        if (getline(inputFile, line)) {
                            l_ELEMENT++;
                            if (stod(line) >= 140)
                                score_Disabled += sbe->Scores[S_Elevator][i++].scoreDisabled;;

                            if (getline(inputFile, line)) {
                                l_ELEMENT++;
                                if (stod(line) >= 100)
                                    score_Disabled += sbe->Scores[S_Elevator][i++].scoreDisabled;;

                                if (getline(inputFile, line)) {
                                    l_ELEMENT++;
                                    if (ToLower(line) == "true")
                                        score_Blind += sbe->Scores[S_Elevator][i++].scoreBlind;

                                    if (getline(inputFile, line)) {
                                        l_ELEMENT++;
                                        if (ToLower(line) == "true")
                                            score_Blind += sbe->Scores[S_Elevator][i++].scoreBlind;

                                        if (getline(inputFile, line)) {
                                            l_ELEMENT++;
                                            if (ToLower(line) == "true")
                                                score_Blind += sbe->Scores[S_Elevator][i++].scoreBlind;

                                            if (getline(inputFile, line)) {
                                                l_ELEMENT++;
                                                if (stod(line) >= 0.8)
                                                    score_Blind += sbe->Scores[S_Elevator][i++].scoreBlind;

                                                if (getline(inputFile, line)) {
                                                    l_ELEMENT++;
                                                    if (stod(line) >= 12 && stod(line) <= 50)
                                                        score_Blind += sbe->Scores[S_Elevator][i++].scoreBlind;

                                                    if (getline(inputFile, line)) {
                                                        l_ELEMENT++;
                                                        if (ToLower(line) == "true")
                                                            score_Blind += sbe->Scores[S_Elevator][i++].scoreBlind;

                                                        if (getline(inputFile, line)) {
                                                            l_ELEMENT++;
                                                            if (ToLower(line) == "true")
                                                                score_Blind += sbe->Scores[S_Elevator][i++].scoreBlind;

                                                            if (getline(inputFile, line)) {
                                                                l_ELEMENT++;
                                                                if (stod(line) >= 3) {

                                                                    if (getline(inputFile, line)) {
                                                                        l_ELEMENT++;
                                                                        if (ToLower(line) == "true") {

                                                                            score_Blind += sbe->Scores[S_Elevator][i].scoreBlind;
                                                                            score_Deaf += sbe->Scores[S_Elevator][i++].score_Deaf;

                                                                            if (getline(inputFile, line)) {
                                                                                l_ELEMENT++;
                                                                                if (stod(line) >= 65 && stod(line) <= 120)
                                                                                    score_Disabled += sbe->Scores[S_Elevator][i++].scoreDisabled;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                else {
                                                                    score_Blind += sbe->Scores[S_Elevator][i].scoreBlind;
                                                                    score_Deaf += sbe->Scores[S_Elevator][i++].score_Deaf;
                                                                }

                                                                if (getline(inputFile, line)) {
                                                                    l_ELEMENT++;
                                                                    if (stod(line) >= 45) {

                                                                        score_Blind += sbe->Scores[S_Elevator][i].scoreBlind;
                                                                        score_Deaf += sbe->Scores[S_Elevator][i++].score_Deaf;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //התייחסות לסימון מוביל. אם לא זוהה לפני זיהוי המעלית ולא אחריה אז אין סימון מוביל ע"פ הדרישות וזה פוגם ברמת נגישות המעלית
            if (score_leading_marking == 0 && getline(inputFile, line) && ToLower(line) != "leading marking")
            {
                score_Blind += sbe->Scores[S_Elevator][i].scoreBlind;
                score_Deaf += sbe->Scores[S_Elevator][i].score_Deaf;
                score_Disabled += sbe->Scores[S_Elevator][i++].scoreDisabled;
            }
            else l_ELEMENT--;
            AE->Update_Rest_element(score_Disabled, score_Blind, score_Deaf, elevator);  }
        else 
        l_ELEMENT--;
    }
     l_ELEMENT= l_ELEMENT;
}


void Check_Camera::Check_Stairs_Not_In_Stairwell(ifstream& inputFile, AddElement* AE, score_by_element* sbe)
{
    int i = 0;
    string line;
    double score_Disabled = 0, score_Blind = 0;
    if (inputFile.is_open()) {

        cout << "check stairs not in stairwell NOW" << endl;

        if (getline(inputFile, line)) {
            l_ELEMENT++;

            if (getline(inputFile, line)) {
                l_ELEMENT++;
                if (ToLower(line) == "true") {
                    score_Blind += sbe->Scores[S_Stairs_not_in_stairwall][i++].scoreBlind;
                    if (ToLower(line) == "true") {
                        score_Blind += 0.1;
                        if (getline(inputFile, line)) {
                            l_ELEMENT++;
                            if (stod(line) >= 60) {
                                score_Blind += sbe->Scores[S_Stairs_not_in_stairwall][i++].scoreBlind;
                            }
                            if (getline(inputFile, line)) {
                                l_ELEMENT++;
                                if (ToLower(line) == "holding_house") {
                                    score_Blind += sbe->Scores[S_Stairs_not_in_stairwall][i++].scoreBlind;
                                }
                                if (getline(inputFile, line)) {
                                    l_ELEMENT++;
                                    if (ToLower(line) == "true") {
                                        score_Blind += sbe->Scores[S_Stairs_not_in_stairwall][i].scoreBlind;
                                        score_Blind += sbe->Scores[S_Stairs_not_in_stairwall][i++].scoreDisabled;
                                    }
                                    
                                }
                            }
                        }
                    }

                }
            }
            if (getline(inputFile, line)) {
                l_ELEMENT++;
                if (ToLower(line) == "stairlift")
                    Check_Stairlift(inputFile,AE,sbe);
            }
             AE->Update_Rest_element(score_Disabled, score_Blind, 1,stairsNotInStairwell);

        }
        else
            l_ELEMENT--;
    }
    l_ELEMENT= l_ELEMENT;
}

void Check_Camera::Check_Stairlift(ifstream& inputFile, AddElement* AE, score_by_element* sbe)
{
    int i = 0;
    string line;
    double score_Disabled = 0;
    if (inputFile.is_open()) {

        cout << "check stairlift" << endl;

        if (getline(inputFile, line)) {
            l_ELEMENT++;
            if (ToLower(line) == "true") {
                if (getline(inputFile, line)) {
                    l_ELEMENT++;
                    if (ToLower(line) == "true")
                         score_Disabled += sbe->Scores[S_Stairlift][i++].scoreDisabled;
                }
            }
            else
                score_Disabled += sbe->Scores[S_Stairlift][i++].scoreDisabled;
            if (getline(inputFile, line)) {
                l_ELEMENT++;
                if (ToLower(line) == "true")
                    score_Disabled += sbe->Scores[S_Stairlift][i++].scoreDisabled;
            }
            if (getline(inputFile, line)) {
                if (ToLower(line) == "stairs not in stairwell") {
                    l_ELEMENT++;
                    Check_Stairs_Not_In_Stairwell(inputFile, AE, sbe);
                }
            }
            AE->Update_Rest_element(score_Disabled, 1, 1, stairlift);
        }
        else
            l_ELEMENT--;
         }
    l_ELEMENT= l_ELEMENT;
}

void Check_Camera::Check_Escalator(ifstream& inputFile, AddElement* AE, score_by_element* sbe)
{
    int i = 0;
    string line;
    double score_Blind = 0;
    if (inputFile.is_open()) {

        cout << "check Escalator NOW" << endl;

        if (getline(inputFile, line)) {
            l_ELEMENT++;
            if (ToLower(line) == "true")
            {
                score_Blind += sbe->Scores[S_Escalator][i++].scoreBlind;

                if (getline(inputFile, line)) {
                    l_ELEMENT++;
                    if(stod(line)>=60)
                        score_Blind += sbe->Scores[S_Escalator][i++].scoreBlind;

                  
                    if (getline(inputFile, line)) {
                        double width = stod(line);
                        l_ELEMENT++;
                        if (getline(inputFile, line))
                        {
                            l_ELEMENT++;
                            if (stod(line) == width)
                                score_Blind += sbe->Scores[S_Escalator][i++].scoreBlind;

                        }
                    }
                }
            }
               
            AE->Update_Rest_element(1, score_Blind, 1, escalator);
        }
        else
            l_ELEMENT--;
    }

}

void Check_Camera::Check_SwitchAndButtons(ifstream& inputFile, AddElement* AE, score_by_element* sbe)
{
    int i = 0;
    string line;
    double score_Disabled = 0;
    if (inputFile.is_open()) {
        l_ELEMENT++;
        cout << "check Gradient NOW" << endl;
        if (getline(inputFile, line)) {
            if (stod(line) >= 65 && stod(line) <= 120)
                score_Disabled += sbe->Scores[S_Switch][i++].scoreDisabled;
        }
        AE->Update_Rest_element(score_Disabled, 1, 1, switchesAndButtons);
    }
    else
        l_ELEMENT--;
}

void Check_Camera::Check_Utility_Room(ifstream& inputFile, AddElement* AE, score_by_element* sbe)
{
    int i = 0;
    string line;
    double score_Disabled = 0, score_Blind = 0;
    if (inputFile.is_open()) {

        cout << "check Utility Room NOW" << endl;

        if (getline(inputFile, line)) {
            l_ELEMENT++;
            if (stod(line)>=45 && stod(line)<=50) {
                score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;
                
                if (getline(inputFile, line)) {
                    l_ELEMENT++;
                    if (ToLower(line) == "true") {

                        if (getline(inputFile, line)) {
                            l_ELEMENT++;
                            if (ToLower(line) == "true") {
                               
                                score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;

                                if (getline(inputFile, line)) {
                                    l_ELEMENT++;
                                    if (ToLower(line) == "true") {
                                        
                                        score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;

                                        if (getline(inputFile, line)) {
                                            l_ELEMENT++;
                                            if (ToLower(line) == "true") {
                                                score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else {
                        i += 3;
                        l_ELEMENT++;
                        score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;
                    }
                    if (getline(inputFile, line)) {
                        l_ELEMENT++;
                        if (stod(line) >= 75)
                            score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;

                        if (getline(inputFile, line)) {
                            l_ELEMENT++;
                            if (ToLower(line) == "true")
                            {
                                if (getline(inputFile, line)) {
                                    l_ELEMENT++;
                                    if (ToLower(line) == "true") {
                                        score_Blind += sbe->Scores[S_UtilityRoom][i].scoreBlind;
                                        score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;
                                       
                                        if (getline(inputFile, line)) {
                                            l_ELEMENT++;
                                            if (stod(line) >= 100)
                                                score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (getline(inputFile, line)) {
                                    l_ELEMENT++;
                                    if (ToLower(line) == "true") {
                                        score_Blind += sbe->Scores[S_UtilityRoom][i].scoreBlind;
                                        score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;

                                        if (getline(inputFile, line)) {
                                            l_ELEMENT++;
                                            if (stod(line) >= 100)
                                            score_Disabled += sbe->Scores[S_UtilityRoom][i++].scoreDisabled;
                                        }
                                    }


                                }
                            }
                        }
                    }
                }
            }
            AE->Update_Rest_element(score_Disabled, score_Blind, 1, utilityRoom);
        }
        else
            l_ELEMENT--;
    }
}

void Check_Camera::Check_Gradient(ifstream& inputFile, AddElement* AE, Lidar_sensor* LS, score_by_element* sbe, bool help)
{
    int i = 0;
    double scoreDisabled = 0, scoreBlind = 0;
    pair<double, double> result = LS->calculateSlopeAndLengthFromRegression();
    double slopePrecent = result.first;
    double lengthSlope = result.second;
    string line;

    if (help) {
        ifstream inputFile("C:/Users/User/Desktop/studies/The Project/Accessibility Inspector/BAI_Project/camera.txt");
        if (inputFile.is_open()) {
            int currentLineNumber = 0;
            while (getline(inputFile, line) && currentLineNumber < l_ELEMENT)
                currentLineNumber++;
            if (ToLower(line) != "gradient")
                help = false;
        }
        else if (slopePrecent >= 5.1)
            help = true;
        if (help) {
            l_ELEMENT++;
            if (getline(inputFile, line)) {
                l_ELEMENT++;
                if (stod(line) >= 1.1) {
                    scoreDisabled += sbe->Scores[S_Gradient][i++].scoreDisabled;
                }
                if (getline(inputFile, line)) {
                    l_ELEMENT++;
                    if (ToLower(line) == "true") {

                        if (getline(inputFile, line)) {
                            l_ELEMENT++;

                            if (ToLower(line) == "true") {
                                if (getline(inputFile, line)) {
                                    l_ELEMENT++;
                                    if (stod(line) >= 85 && stod(line) <= 95) {
                                        scoreDisabled += sbe->Scores[S_Gradient][i].scoreDisabled;
                                        scoreBlind += sbe->Scores[S_Gradient][i++].scoreBlind;
                                    }
                                }
                            }
                        }
                    }
                    else {
                        scoreDisabled += sbe->Scores[S_Gradient][i].scoreDisabled;
                        scoreBlind += sbe->Scores[S_Gradient][i++].scoreBlind;
                    }
                    if (getline(inputFile, line)) {
                        l_ELEMENT++;
                        if (stod(line) >= 4) {
                            scoreDisabled += sbe->Scores[S_Gradient][i].scoreDisabled;
                            scoreBlind += sbe->Scores[S_Gradient][i++].scoreBlind;
                        }
                    }
                    if (slopePrecent >= 10.1)
                        if (lengthSlope<=2.5)
                        {
                            scoreDisabled += sbe->Scores[S_Gradient][i].scoreDisabled;
                            scoreBlind += sbe->Scores[S_Gradient][i++].scoreBlind;
                        }
                }

            }
            AE->Update_Rest_element(scoreDisabled, scoreBlind, 1, gradient);
        }
        else
        {
            l_ELEMENT--;
        }
    }
}


void Check_Camera::Come_back_to_continue_scanning(double this_x, double this_y, double p_x, double p_y, double sd,int d, Steps_Instack* SI)
{
    if(d==diagonalLeft||d==diagonalRight){
        int Y_length, X_length, third_length, slope_angle;
        Y_length = abs(p_y - this_y);
        X_length = abs(this_x - p_x);
        third_length = sqrt(Y_length ^ 2 + X_length ^ 2);

        switch (int(sd))

        {
        case (1 || 2):
            if (Y_length != 0)
            {
                slope_angle = atan(X_length / Y_length) * 180.0;
                cout << "turn " << slope_angle << " degrees. and came back" << third_length << " to continue scaning"<<endl;
            }
                break;
            
        default:
            if (X_length != 0)
            {
                slope_angle = atan(Y_length / X_length) * 180.0;
                cout << "turn " << slope_angle << " degrees. and came back" << third_length << " to continue scaning"<<endl;
            }
                break;
        }
    }
    else
        cout << "come back to continue scaning" << endl;
   
    Came_Back(this_x, this_y, SI);
}


    

