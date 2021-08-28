#pragma once
#include <iostream>
#include <vector>
#include <array>
using namespace std;

class College
{
private:
	string collegeName;
	int collegeCode;
	string collegeEmail;
	string phoneNumber;

public:

	// Constructors
	College() = default; 									 	// Default 
	College(string, int, string, string);						// Parameterized
	College(const College&) = default;							// Copy

	// Setter and Getter methods
	void setName(string);
	void setCollegeCode(int);
	void setEmail(string);
	void setPhoneNumber(string);
	string getName();
	int getCollegeCode();
	string getEmail();
	string getPhoneNumber();
	array<vector<int>, 2> verify();
	void viewEnrolledStudents();
	void updateStudentPlacedStatus();
	//Destructor
	~College() = default;
};
