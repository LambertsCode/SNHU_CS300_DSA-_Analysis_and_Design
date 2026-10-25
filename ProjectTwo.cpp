//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Leah Miller
// Date        : 10/17/26
// Description : ABCU advising program. Loads Computer Science course data
//               from a CSV file into a binary search tree, then lets an
//               advisor print the sorted course list or look up one course.
//============================================================================

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

//============================================================================
// Data structures
//============================================================================

// holds the data from a single line of the input file
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// one node of the binary search tree, ordered by courseNumber
struct Node {
    Course course;
    Node* left;    // subtree of smaller course numbers
    Node* right;   // subtree of larger course numbers

    explicit Node(const Course& aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

//============================================================================
// Binary search tree of courses
//============================================================================

class CourseTree {
public:
    // constructor
    CourseTree() {
        root = nullptr;
    }

    // deconstructor
    ~CourseTree() {
        clearCourses();
    }

    void clearCourses();
    void insertCourse(const Course& course);
    const Course* findCourse(const string& courseNumber) const;
    void printCourseList() const;

private:
    Node* root;

    static void deleteTree(Node* node);
    static Node* insertNode(Node* node, const Course& course);
    static void inOrder(const Node* node);
};

// frees every node and leaves the tree empty
void CourseTree::clearCourses() {
    deleteTree(root);
    root = nullptr;
}

// post-order delete: children are freed before their parent
void CourseTree::deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// adds a course to the tree
void CourseTree::insertCourse(const Course& course) {
    root = insertNode(root, course);
}

// walks down from the node: smaller course numbers go left, all others right
Node* CourseTree::insertNode(Node* node, const Course& course) {
    if (node == nullptr) {
        return new Node(course);
    }

    if (course.courseNumber < node->course.courseNumber) {
        node->left = insertNode(node->left, course);
    }
    else {
        node->right = insertNode(node->right, course);
    }
    return node;
}

// returns a pointer to the matching course, or nullptr if it is not in the tree
const Course* CourseTree::findCourse(const string& courseNumber) const {
    const Node* current = root;

    while (current != nullptr) {
        if (courseNumber == current->course.courseNumber) {
            return &current->course;
        }
        else if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}

// prints list of courses in order
void CourseTree::printCourseList() const {
    inOrder(root);
}

// in-order traversal visits courses already sorted
void CourseTree::inOrder(const Node* node) {
    if (node == nullptr) {
        return;
    }
    inOrder(node->left);
    cout << node->course.courseNumber << ": " << node->course.courseName << endl;
    inOrder(node->right);
}

//============================================================================
// Helper functions
//============================================================================

// splits a line into its comma-separated tokens
vector<string> splitLine(const string& line, char delimiter) {
    vector<string> tokens;
    string token;

    for (char character : line) {
        if (character == delimiter) {
            tokens.push_back(token);
            token.clear();
        }
        else {
            token += character;
        }
    }
    tokens.push_back(token);
    return tokens;
}
// converts a string to all uppercase
string toUpperCase(string text) {
    for (char& character : text) {
        character = static_cast<char>(toupper(static_cast<unsigned char>(character)));
    }
    return text;
}

//============================================================================
// File loading
//============================================================================

// opens the file, reads each line, parses it, and checks format error 1
// (every line needs at least a course number and a name)
// returns an empty vector if the file cannot be opened or a line is malformed
vector<vector<string>> readCourseLines(const string& filePath) {
    vector<vector<string>> parsedLines;
    int lineNumber = 0;

    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cout << "Error: could not open file " << filePath << endl;
        return {};
    }

    string rawLine;
    while (getline(inputFile, rawLine)) {
        lineNumber++;

        if (rawLine.empty()) {
            continue;   // skip blank lines
        }

        vector<string> tokens = splitLine(rawLine, ',');

        // ignore empty fields at the end of the line
        while (!tokens.empty() && tokens.back().empty()) {
            tokens.pop_back();
        }

        // check 1: at least two parameters per line
        if (tokens.size() < 2) {
            cout << "Format error on line " << lineNumber
                << ": expected at least 2 fields, found " << tokens.size() << endl;
            return {};
        }

        parsedLines.push_back(tokens);
    }

    return parsedLines;
}

// creates one course object from the tokens of one line
Course buildCourse(const vector<string>& tokens) {
    Course newCourse;
    newCourse.courseNumber = tokens[0];
    newCourse.courseName = tokens[1];

    // everything after the first two tokens is a prerequisite
    for (size_t i = 2; i < tokens.size(); i++) {
        newCourse.prerequisites.push_back(tokens[i]);
    }
    return newCourse;
}

// loads the file into the tree and checks format error 2
// returns true only if the whole file loaded without errors
bool loadCourses(const string& filePath, CourseTree& courses) {
    courses.clearCourses();

    vector<vector<string>> parsedLines = readCourseLines(filePath);
    if (parsedLines.empty()) {
        return false;
    }

    for (const vector<string>& tokens : parsedLines) {
        courses.insertCourse(buildCourse(tokens));
    }

    // check 2: every prerequisite must exist as a course in the file
    for (const vector<string>& tokens : parsedLines) {
        for (size_t i = 2; i < tokens.size(); i++) {
            if (courses.findCourse(tokens[i]) == nullptr) {
                cout << "Format error: prerequisite" << tokens[i]
                    << " listed for course " << tokens[0]
                    << " does not exist as a course in the file" << endl;
                courses.clearCourses();
                return false;
            }
        }
    }

    return true;
}

//===========================================================================
// Printing
//===========================================================================

// prints one course and its prerequisites
void printCourse(const CourseTree& courses, const string& courseNumber) {
    const Course* course = courses.findCourse(courseNumber);
    if (course == nullptr) {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << course->courseNumber << ": " << course->courseName << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites:" << endl;
    for (const string& prereqNumber : course->prerequisites) {
        const Course* prereq = courses.findCourse(prereqNumber);
        if (prereq != nullptr) {
            cout << "  " << prereq->courseNumber << ": " << prereq->courseName << endl;
        }
    }
}

//============================================================================
// Menu
//============================================================================

void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

// reads the menu choice, returning -1 if the input is not a number
int readChoice() {
    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        choice = -1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

int main() {
    CourseTree courses;
    bool loaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    // shows menu after each choice until '9' entered
    while (choice != 9) {
        displayMenu();
        choice = readChoice();

        if (choice == 1) {
            cout << "Enter the file name: ";
            string filePath;
            getline(cin, filePath);

            loaded = loadCourses(filePath, courses);
            if (loaded) {
                cout << "Courses loaded." << endl;
            }
        }
        else if (choice == 2) {
            if (!loaded) {
                cout << "Please load the data first (option 1)." << endl;
            }
            else {
                courses.printCourseList();
            }
        }
        else if (choice == 3) {
            if (!loaded) {
                cout << "Please load the data first (option 1)." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                string courseNumber;
                getline(cin, courseNumber);
                printCourse(courses, toUpperCase(courseNumber));
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << "That is not a valid option." << endl;
        }
    }

    return 0;
}