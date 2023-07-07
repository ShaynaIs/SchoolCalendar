#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <iomanip>

using namespace std;

// This object adds some functionality (and easier use) to the c++ date objects
struct date {   
    private:    
        int daysInMonth;
        struct tm date_info;
        // based on code from geeksforgeeks.org to validate date
        bool isValid(int y, int m, int d) {
            if (y > 9999 || y < 1800) return false;
            if (m < 1 || m > 12) return false;
            if (d < 1 || d > 31) return false;

            daysInMonth = 31;
            // Check months with 30 days
            if (m == 4 || m == 6 || m == 9 || m == 11) {
                daysInMonth = 30;
                return (d <= 30);
            }

            // Check February
            if (m == 2) {
                if ((((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))) {
                    daysInMonth = 29;
                    return (d <= 29);
                }
                else {
                    daysInMonth = 28;
                    return (d <= 28);
                }
            }

            return true;
        }
    public:
        // returns -1 if date is invalid and 1 if date is valid
        int set(int y, int m, int d) {
            if (!isValid(y, m, d)) {
                return -1;
            }
            tm time_in = { 0, 0, 0, // second, minute, hour
                           d, m - 1, y - 1900 }; // 1-based day, 0-based month, year since 1900
            time_t time_temp = mktime(&time_in);
            struct tm timeinfo;
            localtime_s(&timeinfo, &time_temp);

            date_info = timeinfo;

            return 1;
        }

        // Checks if input string is correct format
        int checkFormatYYYYMMDD(string s) {
            for (char c : s) {
                if (!(c == '-' || isdigit(c))) {
                    return false;
                }
            }
            return true;
        }
        // Checks if input string is correct format
        int checkFormatMMDDYYYY(string s) {
            for (char c : s) {
                if (!(c == '/' || isdigit(c))) {
                    return false;
                }
            }
            return true;
        }
        // creates a date object from a string "yyyy-mm-dd" or "dd/mm/yyyy"
        int set(string s) {
            // check if yyyy-mm-dd format
            if (checkFormatYYYYMMDD(s)) {
                return set(stoi(s.substr(0, 4)), stoi(s.substr(5, 2)), stoi(s.substr(8, 2)));
            }

            // check if dd/mm/yyyy
            if (checkFormatMMDDYYYY(s)) {
                if (s[1] == '/') s = "0" + s;
                return set(stoi(s.substr(s.size() - 4, 4)), stoi(s.substr(0, 2)), stoi(s.substr(3, 2)));
            }

            return -1;
        }

        // returns the data in the date object
        int getYear() { return 1900+date_info.tm_year; }
        int getMonth() { return date_info.tm_mon+1; }
        string getMonthName() {
            string months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
            return months[getMonth() - 1];
        }
        int getDay() { return date_info.tm_mday; }
        int getDaysInMonth() { return daysInMonth; }
        int getDayOfWeek() {
            //Sunday == 0, Monday == 1, and so on ...
            return date_info.tm_wday;
        }
        string getDayName() {
            string days[7] = { "Sunday", "Monday", "Tuseday", "Wednesday", "Thursday", "Friday", "Saturday" };
            return days[getDayOfWeek()];
        }
        string toString() {
            return to_string(getMonth()) + "/" + to_string(getDay()) + "/" + to_string(getYear());
        }
        string toStringLong() {
           return getDayName() + ", " + getMonthName() + " " + to_string(getDay());
        }
        // compares this date to an input - returns -1 if this is less and 1 if this is more
        int compare(date d) {
            if (getYear() < d.getYear()) return -1;
            if (getYear() > d.getYear()) return 1;
            if (getMonth() < d.getMonth()) return -1;
            if (getMonth() > d.getMonth()) return 1;
            if (getDay() < d.getDay()) return -1;
            if (getDay() > d.getDay()) return 1;
            return 0;
        }

        // Changes to next day
        void addDay() {
            if (isValid(getYear(), getMonth(), getDay() + 1)) {
                set(getYear(), getMonth(), getDay() + 1);
                return;
            }
            if (isValid(getYear(), getMonth() + 1, 1)) {
                set(getYear(), getMonth() + 1, 1);
                return;
            }
            set(getYear() + 1, 1, 1);
            return;
        }

        // Changes to next day
        void addDay(int amount) {
            for (int i = 0; i < amount; i++) {
                addDay();
            }
        }

        // Changes to prev day
        void minusDay() {
            if (isValid(getYear(), getMonth(), getDay() - 1)) {
                set(getYear(), getMonth(), getDay() - 1);
                return;
            }
            if (isValid(getYear(), getMonth() - 1, 31)) {
                set(getYear(), getMonth() - 1, 31);
                return;
            }
            if (isValid(getYear(), getMonth() - 1, 30)) {
                set(getYear(), getMonth() - 1, 30);
                return;
            }
            if (isValid(getYear(), getMonth() - 1, 29)) {
                set(getYear(), getMonth() - 1, 29);
                return;
            }
            if (isValid(getYear(), getMonth() - 1, 28)) {
                set(getYear(), getMonth() - 1, 28);
                return;
            }
            set(getYear() - 1, 12, 31);
            return;
        }

        // Changes to prev day
        void minusDay(int amount) {
            for (int i = 0; i < amount; i++) {
                minusDay();
            }
        }
};

// Struct to contain all info necessary for an assignment
struct calenderItem {
    bool done = false;
    date due;
    string className;
    string type;
    string details;
    calenderItem(bool b, date d, string n, string t, string de) {
        done = b;
        due = d;
        className = n;
        type = t;
        details = de;
        // example = calenderItem(false, due, "CSE 130", "Final Project", "Insert Details Here")
    }
    string toString() {
        return className + " " + type;
    }
    string toStringLong() {
        return toString() + "\n\t" + details + "\n\tDue on: " + due.toStringLong();
    }
};

// Function to compare assignments in order to sort them
bool compareCalenderItem(calenderItem a, calenderItem b) {
    return (a.due.compare(b.due) == -1);
}

// Menu options
int menu() {
    int i;
    cout << endl << "Menu:" << endl;
    cout << "1. View calendar" << endl;
    cout << "2. View assignement list" << endl;
    cout << "3. Edit calendar" << endl;
    cout << "4. Exit" << endl;
    cout << "Please enter the number associated with the correct menu action: ";
    cin >> i;
    if (i == 1 || i == 2) return i;
    if (i == 4) return 0;
    if (i < 1 || i > 4) {
        cout << "Please try again";
        return menu();
    }
    cout << endl;
    cout << "Would you like to..." << endl;
    cout << "1. Add a single item" << endl;
    cout << "2. Bulk upload items" << endl;
    cout << "3. Edit existing item" << endl;
    cout << "4. Delete item" << endl;
    cout << "5. Back" << endl;
    cout << "Please enter the number associated with the correct menu action: ";
    cin >> i;
    if (i == 5) return menu();
    if (i < 1 || i > 5) {
        cout << "Please try again";
        return menu();
    }
    return i + 2;
}

// Display the calender
void display(vector<calenderItem> v) {
    // Check if calender is empty
    if (empty(v)) {
        cout << "You finished all your assignments!!" << endl;
        return;
    }

    // Sort items in order so easier to deal with
    sort(v.begin(), v.end(), compareCalenderItem);

    // Start displaying
    
    // Get todays date - https://stackoverflow.com/questions/8343676/how-to-get-current-date-and-time
    date start;
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);    
    start.set(1900 + newtime.tm_year, 1 + newtime.tm_mon, newtime.tm_mday);


    // Set iterator
    int iter = 0;
    
    // Display past OVERDUE items
    if (v[iter].due.compare(start) == -1) {
        cout << "===========================================================================" << endl;
        cout << "                            Overdue/Missed Assignments: " << endl;
        cout << "===========================================================================" << endl;
        while (iter < v.size() && v[iter].due.compare(start) == -1) {
            cout << v[iter].toString() << endl;
            iter++;
        }
    }

    // Check if remaining items
    if (iter >= v.size()) {
        cout << endl << "You have no more assignments!!" << endl;
        return;
    }

    // Display calender

    // Get Sunday date
    date currDay;
    string days[7] = { "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY" };
    char nextMonth;
    do {
        // Check if remaining items
        if (iter >= v.size()) {
            cout << endl << "You have no more assignments!!" << endl;
            return;
        }
        // Get Sunday date
        currDay.set(start.getYear(), start.getMonth(), start.getDay());
        currDay.minusDay(start.getDayOfWeek());

        // Display calender heading
        cout << endl << start.getMonthName() << endl;
        for (string d : days) {
            cout << "||" << setw(15) << left << d;
        }
        cout << "||" << endl;
        cout << setfill('-') << setw(120) << left << " " << endl << setfill(' ') << endl;

        //  Loop through month by week
        for (int j = start.getDay(); j < start.getDaysInMonth(); j += 7) {
            // Check if remaining items
            if (iter >= v.size()) {
                cout << endl << "You have no more assignments!!" << endl;
                return;
            }
            cout << setfill(' ') << "||";
            // Maps day of week with items
            map<int, vector<string>> itemsByDayOfWeek;
            int most = 3;
            for (int i = 0; i < 7; i++) {
                itemsByDayOfWeek[i] = vector<string>();
                while (iter < v.size() && v[iter].due.compare(currDay) == 0) {
                    // Check if string is too long
                    string both = v[iter].className + " " + v[iter].type;
                    if (both.size() < 15) {
                        itemsByDayOfWeek[i].push_back(both);
                    }
                    else {
                        string temp[2] = { v[iter].className, v[iter].type };
                        for (string t : temp) {
                            for (int k = 0; k < t.size(); k += 15) {
                                itemsByDayOfWeek[i].push_back(t.substr(k, k + 15));
                            }
                        }
                    }
                    iter++;
                }
                if (itemsByDayOfWeek[i].size() > most) {
                    most = itemsByDayOfWeek[i].size();
                }
                // Print first row
                cout << setw(15) << currDay.getDay() << "||";
                currDay.addDay();
            }
            currDay.minusDay(6); // Reset currday
            cout << endl;

            // Print following rows
            // Loop through most amount of rows
            for (int i = 0; i < most; i++) {
                cout << "||";
                // Loop through days of week
                for (int d = 0; d < 7; d++) {
                    if (!itemsByDayOfWeek[d].empty()) {
                        cout << setw(15) << left << itemsByDayOfWeek[d][0] << "||";
                        itemsByDayOfWeek[d].erase(itemsByDayOfWeek[d].begin());
                    }
                    else {
                        cout << setw(17) << right << "||";
                    }
                    if (currDay.getDayOfWeek() == 6) {
                        cout << endl;
                    }
                }
                cout << endl;
            }


            // Print line
            cout << setfill('-') << setw(120) << left << " " << endl << setfill(' ') << endl;
            currDay.addDay(6);
        }
        cout << endl;

        // Ask user if should display next month
        string temp;
        cout << "Would you like to view next month? ";
        cin >> temp;
        nextMonth = temp[0];

        // Reset start day
        start.set(currDay.getYear(), currDay.getMonth(), currDay.getDay());

        cout << endl;
    } while (nextMonth == 'y' || nextMonth == 'Y');
}

// Function to print assignments as list organized by date
void printAssigments(vector<calenderItem> v) {
    if (v.empty()) {
        cout << "There are no assignments." << endl;
        return;
    }
    sort(v.begin(), v.end(), compareCalenderItem);
    cout << endl << "All Assignments\n----------------" << endl;
    for (calenderItem c : v) {
        cout << c.toStringLong() << endl;
    }
    cout << endl;
}

// Function to create a calender item from user input
calenderItem getItem() {
    // Instantiate variables
    string doneString, dateString, className, type, details;
    bool done = false;
    date due;
    

    cout << "\nEnter in a single item into calender: " << endl;
    cin.ignore();

    // Get completion status
    cout << "Is the assignment completed? Enter 'true' if completed or 'false' if not: ";
    cin >> doneString;
    if (doneString[0] == 't') done = true; // Already set to false otherwise

    // Get date due
    int validDate;
    do {
        cout << "Enter the date it is due (YYYY-MM-DD format): ";
        cin >> dateString;
        validDate = due.set(dateString);
        if (validDate == -1) cout << "Error... Invalid Date" << endl;;
    } while (validDate == -1);
    
    char ETC[100];

    // Get class name
    cout << "Class name (ex CSE 130): ";
    cin >> className;

    // Get assignment type
    cin.getline(ETC, 30);
    cout << "Type (ex assignment, test, paper, project): ";
    cin >> type;

    // Get decription
    cout << "Description: ";
    cin >> details;
    cin.getline(ETC, 100);

    calenderItem temp = calenderItem(done, due, className, type, details);

    cout << "\nYou are adding:\n" << temp.toStringLong();

    
    return temp;
}

// Function to download the current state of the items to csv file
void downloadAssignments(vector<calenderItem> v) {
    ofstream myFile;
    myFile.open("download.csv", ios_base::app);         //ios_base::app adds onto the file without overwriting. Stands for append
    
    string temp;
    cout << "Items will be downloaded to 'download.csv'." << endl;
    cout << "Enter 'CANCEL' to cancel the download: ";
    cin >> temp;
    if (temp == "CANCEL") return;

    for (calenderItem c : v) {
        myFile << c.done << "," << c.due.toString() << "," << c.className << "," << c.type << "," << c.details << endl;
    }

    myFile.close();
}

// Function to upload a csv file to a vector
vector<calenderItem> bulkUpload() {
    // Print template for user to fill out using excel
    char download, verify; //used to initiate download of template
    cout << "Enter 'd' to download template (anything else to skip): ";
    cin >> download;
    if (download == 'd') {
        cout << "Are you sure? If you previously own a template and download another, it will overwrite all of the previous information. Enter 'y' to confirm: ";
        cin >> verify;
        if (verify == 'y') {
            ofstream MyFile("template.csv");    //if d was used again it won't redownload, it will overwrite. Good to know for us as well as users too
            MyFile << "done, date, class, type, details,\n";
            MyFile << "false, 22-11-14, CSE 130, Project, example,\n";
            MyFile << "true, 23-11-23, CSE 130, Project, maybe\n";
        }
    }

    // Describe each field and acceptable inputs
    cout << "\n***To use the template, OPEN it as an Excel file and SAVE it as a csv file***" << endl;
    cout << "\n\nEach item has 5 fields:" << endl;
    cout << "\tCompletion Status\t- whether these item has been completed (True / False)" << endl;;
    cout << "\tDue Date\t\t- the date the item is due" << endl;
    cout << "\tClass Name\t\t- the class that assigned this item (ex CSE 130)" << endl;
    cout << "\tType\t\t\t- ex assignment, test, discussion board, paper, project etc" << endl;
    cout << "\tDescription\t\t- any added details about the item" << endl;

    // Access file
    string fileName;
    ifstream Catalog; // to read the file

    do {
        cout << "\n***Place the new file in the same folder as this file***" << endl;
        cout << "File name: ";
        cin >> fileName;

        Catalog.open(fileName);
        
        // Validation
        if (Catalog.fail()) {
            cout << "Something went wrong with " << fileName << endl;
            cout << "Try again... Make sure to include the ENTIRE file name" << endl;
        }
    } while (Catalog.fail());
    
    // Instantiate variables
    vector<calenderItem> v; // Vector to read in items to
    string temp, doneString, dateString, className, type, details;
    bool done;
    
    // Skip first line
    if (Catalog.peek() != EOF) {
        getline(Catalog, temp, '\n');
    }

    // Read in the rest
    while (Catalog.peek() != EOF) {

        getline(Catalog, doneString, ',');                           //reads lines from users file name input
        getline(Catalog, dateString, ',');
        getline(Catalog, className, ',');
        getline(Catalog, type, ',');
        getline(Catalog, details, '\n'); // if has "" ignore inside

        // Get correct format for done and date
        if (doneString[0] == 't' || doneString[0] == 'T') done = true;
        else done = false;
        date due;
        int validDate = due.set(dateString);
        if (validDate == -1) cout << "Invalid date... Check item..." << endl;

        v.push_back(calenderItem(done, due, className, type, details));
    }

    Catalog.close();

    cout << "Completed!" << endl;
    return v;
}

// Function for user to select an item
int findItem(vector<calenderItem> v) {
    // List assignments to chose from
    cout << endl << "Items: " << endl;
    for (int i = 0; i < v.size(); i++) {
        cout << (i + 1) << ": ";
        cout << v[i].toStringLong() << endl;
    }

    // Get assignment number from user 
    int itemNum;
    cout << endl <<  "Enter the number of the item to edit: ";
    cin >> itemNum;

    // Confirm
    cout << endl << "Item selected: " << itemNum << endl;
    cout << v[itemNum-1].toStringLong() << endl;;
    string temp;
    cout << "Would you like to modify this item? ";
    cin >> temp;
    if (temp[0] == 'Y' || temp[0] == 'y') {
        return itemNum-1;
    }
    else {
        cout << "Would you like to try again? " << endl;
        cin >> temp;
        if (temp[0] == 'Y' || temp[0] == 'y') {
            findItem(v);
        }
        else {
            return -1;
        }
    }
    return -1;
}

// Function for user to edit an item
void editItem(calenderItem& c) {
    // Get field to edit from user
    int i;
    cout << "\nWould you like to edit the..." << endl;
    cout << "1. Completion status" << endl;
    cout << "2. Due date" << endl;
    cout << "3. Class name" << endl;
    cout << "4. Item type" << endl;
    cout << "5. Description" << endl;
    cout << "6. Exit" << endl;
    cout << "Input the corresponding number: ";
    cin >> i;
    if (i == 6) return;
    if (i < 1 || i > 5) editItem(c);
    string temp;

    // Edit corect field
    switch (i) {
    case 1:
        cout << "What is the new completion status (Input 'true' if the item is complete)? ";
        cin >> temp;
        if (temp[0] == 't' || temp[0] == 'T') c.done = true;
        else c.done = false;
        break;
    case 2:
        cout << "What is the new due date (yyyy-mm-dd)? ";
        cin >> temp;
        date due;
        due.set(temp);
        c.due = due;
        break;
    case 3:
        cout << "What is the new class name? ";
        cin >> c.className;
        break;
    case 4:
        cout << "What is the new item type (ex test, project, hw)? ";
        cin >> c.type;
        break;
    case 5:
        cout << "What is the new description? ";
        cin >> c.details;
        break;
    }

    // repeat if would liek to edit more
    cout << "Would you like to continue editting this item? ";
    cin >> temp;
    if (temp[0] == 't' || temp[0] == 'T') editItem(c);
}

// Function to edit item with referance to vector
void editItemInVector(vector<calenderItem> &v) {
    int i = findItem(v);
    if (i == -1) return;
    editItem(v[i]);
}

// Function to delete item from vector
void deleteItem(vector<calenderItem> &v) {
    int i = findItem(v);
    if (i == -1) return;
    v.erase(v.begin() + i);
    cout << "\nItem #" << i + 1 << " erased" << endl;
}

// Main
int main()
{
    cout << "Hello Student!" << endl;
    vector<calenderItem> calender;
    vector<calenderItem> newItems;
    int action;

    // Prints menu and then executes correct action based on user input
    do {
        cout << endl << "Press ENTER to continue...";
        cin.get();

        action = menu();

        switch (action) {
        case 1:
            display(calender);
            break;
        case 2:
            printAssigments(calender);
            break;
        case 3:
            calender.push_back(getItem());
            break;
        case 4:
            // bulk upload
            newItems = bulkUpload();
            calender.insert(calender.end(), newItems.begin(), newItems.end());
            newItems.clear();
            break;
        case 5:
            editItemInVector(calender);
            break;
        case 6: 
            deleteItem(calender);
            break;
        }
        
        cin.ignore();

    } while (action <= 6 && action >= 1);

    // Download current state if user wants
    string temp;
    cout << "\nWould you like to download the current state of your calender items? ";
    cin >> temp;
    cin.ignore();
    if (temp[0] == 'y' || temp[0] == 'Y') {
        downloadAssignments(calender);
    }

    cout << endl << "Have a good day!" << endl;
}
