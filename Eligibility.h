#pragma once
#include <iostream>

#include <map>
using namespace std;
//add comments
//add namespaces

class Eligibility
{
private:
	//Percentage
	double CGPA;
	int liveBackLog;					//ATKT (not cleared not while application)
	int deadBackLog;					//ATKT (already cleared )
	int passingYear;					//year of graduation(eg .2021)
	int yearGap;						// gap after 12th or 10th standard
	map<string, int> skillExpertiseMap; // mapping between skill(c++, java) and its respective level (in advanced, beginer)

public:
	Eligibility(double, int, int, int, int, string);
	Eligibility();
	Eligibility(const Eligibility &);
	void operator=(const Eligibility &);
	bool operator==(const Eligibility &e) const;
	void setCGPA(double);
	double const getCGPA();
	void setLiveBackLog(int);
	int const getLiveBackLog();
	void setDeadBackLog(int);
	int const getDeadBackLog();
	int const getYearGap();
	void setYearGap(int);
	void addSkill(string, string);
};
