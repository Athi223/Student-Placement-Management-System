#include <iostream>
#include "Eligibility.h"
#include <cstring>
#include <string>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
using namespace std;
namespace py = pybind11;

Eligibility::Eligibility(double cgpa, int lb, int db, int yP, int yG, string skillSet) : CGPA(cgpa), liveBackLog(lb), deadBackLog(db), passingYear(yP), yearGap(yG)
{
	//skill1:level1,skill2:level2...
	char *token;
	const char *delim = ",";
	token = strtok(const_cast<char *>(skillSet.c_str()), delim);
	while (token != NULL)
	{
		string s(token);
		size_t pos = s.find(":");
		skillExpertiseMap[s.substr(0, pos)] = stoi(s.substr(pos + 1, string::npos));
		token = strtok(NULL, delim);
	}
}

Eligibility::Eligibility() = default;

Eligibility::Eligibility(const Eligibility &e) : CGPA(e.CGPA), liveBackLog(e.liveBackLog), deadBackLog(e.deadBackLog), passingYear(e.passingYear), yearGap(e.yearGap)
{
	for (auto it : e.skillExpertiseMap)
	{
		skillExpertiseMap[it.first] = it.second;
	}
}

void Eligibility::operator=(const Eligibility &e)
{
	CGPA = e.CGPA;
	liveBackLog = e.liveBackLog;
	deadBackLog = e.deadBackLog;
	yearGap = e.yearGap;
	passingYear = e.passingYear;
	for (auto it : e.skillExpertiseMap)
	{
		skillExpertiseMap[it.first] = it.second;
	}
}

//student on lhs of == operator
/*
*/
bool Eligibility::operator==(const Eligibility &e) const
{
	if (CGPA >= e.CGPA && liveBackLog <= e.liveBackLog && deadBackLog <= e.deadBackLog && yearGap <= e.yearGap && passingYear == e.passingYear)
	{
		for (auto it : e.skillExpertiseMap)
		{
			auto findIterator = skillExpertiseMap.find(it.first);
			if (findIterator != skillExpertiseMap.end())
			{
				if (findIterator->second < it.second)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void Eligibility::setCGPA(double cgpa)
{
	CGPA = cgpa;
}

double Eligibility::getCGPA()
{
	return CGPA;
}

void Eligibility::setLiveBackLog(int lb)
{
	liveBackLog = lb;
}

int Eligibility::getLiveBackLog()
{
	return liveBackLog;
}

void Eligibility::setDeadBackLog(int db)
{
	deadBackLog = db;
}

int Eligibility::getDeadBackLog()
{
	return deadBackLog;
}

int Eligibility::getYearGap()
{
	return yearGap;
}

void Eligibility::setYearGap(int yg)
{
	yearGap = yg;
}

int Eligibility::getPassingYear()
{
	return passingYear;
}
void Eligibility::setPassingYear(int yp)
{
	passingYear = yp;
}

map<string, int> Eligibility::getSkillExpertiseMap()
{
	return skillExpertiseMap;
}

void Eligibility::setSkillExpertiseMap(map<string, int> sEM)
{
	skillExpertiseMap = sEM;
}

void Eligibility::addSkill(string skillName, string skillLevel)
{
	skillExpertiseMap[skillName] = stoi(skillLevel);
}

PYBIND11_MODULE(Eligibility, m)
{
	py::class_<Eligibility>(m, "Eligibility")
		.def(py::init())
		.def(py::init<const float &, const int &, const int &, const int &, const int &, const string &>())
		.def(py::self == py::self)
		.def("setCGPA", &Eligibility::setCGPA)
		.def("getCGPA", &Eligibility::getCGPA)
		.def("setLiveBackLog", &Eligibility::setLiveBackLog)
		.def("getLiveBackLog", &Eligibility::getLiveBackLog)
		.def("setDeadBackLog", &Eligibility::setDeadBackLog)
		.def("getDeadBackLog", &Eligibility::getDeadBackLog)
		.def("setPassingYear", &Eligibility::setPassingYear)
		.def("getPassingYear", &Eligibility::getPassingYear)
		.def("getYearGap", &Eligibility::getYearGap)
		.def("setYearGap", &Eligibility::setYearGap)
		.def("getSkillExpertiseMap", &Eligibility::getSkillExpertiseMap)
		.def("setSkillExpertiseMap", &Eligibility::setSkillExpertiseMap)
		.def("addSkill", &Eligibility::addSkill);
}