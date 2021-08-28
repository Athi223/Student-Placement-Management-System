#include <iostream>
#include "Student.h"
#include <string>
#include <sqlite3.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace std;
namespace py = pybind11;

Student::Student() = default;

Student::Student(string studentName, int studentId, string studentEmail, string studentPhoneNumber, string DOB, string studentDepartment, const Eligibility &studentAcademicDetails) : studentName(studentName), studentId(studentId), studentEmail(studentEmail), studentPhoneNumber(studentPhoneNumber), DOB(DOB), studentDepartment(studentDepartment), studentAcademicDetails(studentAcademicDetails)
{
}

void Student::setName(string s)
{
	studentName = s;
}

string Student::getName()
{
	return studentName;
}

void Student::setID(int i)
{
	studentId = i;
}

int Student::getID()
{
	return studentId;
}

void Student::setEmail(string e)
{
	studentEmail = e;
}

string Student::getEmail()
{
	return studentEmail;
}

void Student::setPhoneNumber(string ph)
{
	studentPhoneNumber = ph;
}

string Student::getPhoneNumber()
{
	return studentPhoneNumber;
}

void Student::setDOB(string d)
{
	DOB = d;
}

string Student::getDOB()
{
	return DOB;
}

void Student::setAcademicDetails(Eligibility e)
{
	studentAcademicDetails.setCGPA(e.getCGPA());
	studentAcademicDetails.setDeadBackLog(e.getDeadBackLog());
	studentAcademicDetails.setLiveBackLog(e.getLiveBackLog());
	studentAcademicDetails.setPassingYear(e.getPassingYear());
	studentAcademicDetails.setYearGap(e.getYearGap());
	studentAcademicDetails.setSkillExpertiseMap(e.getSkillExpertiseMap());
}

Eligibility Student::getAcademicDetails()
{
	return studentAcademicDetails;
}

int Student::callback(void *objPtr, int argc, char **argv, char **azColName)
{
	return ((Student *)objPtr)->callbackHelper(argv);
}

int Student::callbackHelper(char **argv)
{
	Eligibility *tempCriteria = new Eligibility(atof(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), string(argv[6]));
	if ((studentAcademicDetails) == *(tempCriteria))
	{
		potentialOfferId.push_back(atoi(argv[0]));
	}
	delete tempCriteria;
	return 0;
}

vector<int> Student::getApplicableOffers()
{
	string sql(string("SELECT CC.offerId_id, CC.cgpa, CC.liveBacklog, CC.deadBacklog, CC.passingYear, CC.yearGap, CC.skillSet FROM Company_offer INNER JOIN Company_criteria AS CC ON Company_offer.id = CC.offerId_id WHERE cc.offerId_id NOT IN (SELECT offerId_id FROM College_intermediatestudentstatus AS CI WHERE CI.studentId_id == ") + to_string(studentId) + string(");"));
	sqlite3 *DB;
	int exit = 0;
	exit = sqlite3_open("db.sqlite3", &DB);
	string data("CALLBACK FUNCTION");

	if (exit)
	{
		cerr << "Error open DB " << sqlite3_errmsg(DB) << endl;
		return potentialOfferId;
	}
	else
		cout << "Opened Database Successfully!" << endl;

	int rc = sqlite3_exec(DB, sql.c_str(), callback, (void *)this, NULL);
	if (rc != SQLITE_OK)
		cerr << "Error SELECT" << endl;
	else
	{
		cout << "Operation OK!" << endl;
	}
	sqlite3_close(DB);
	return potentialOfferId;
}

int Student::insertCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
	}
	return 0;
}

void Student::apply(int offerId)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	string comma = ",";
	string sql(string("INSERT INTO College_intermediatestudentstatus (offerId_id, status, studentId_id) VALUES (") + to_string(offerId) + string(",0,") + to_string(studentId) + string(");"));

	/* Open database */
	rc = sqlite3_open("db.sqlite3", &db);

	if (rc)
	{
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
		return;
	}
	else
	{
		cout << "Opened database successfully" << endl;
	}
	rc = sqlite3_exec(db, sql.c_str(), insertCallback, 0, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		cerr << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		cout << "Records created successfully" << endl;
	}
	sqlite3_close(db);
}

PYBIND11_MODULE(Student, m)
{
	py::class_<Student>(m, "Student")
		.def(py::init())
		.def(py::init<const string &, const int &, const string &, const string &, const string &, const string &, const Eligibility &>())
		.def("setName", &Student::setName)
		.def("getName", &Student::getName)
		.def("setID", &Student::setID)
		.def("getID", &Student::getID)
		.def("setEmail", &Student::setEmail)
		.def("getEmail", &Student::getEmail)
		.def("setDOB", &Student::setDOB)
		.def("getDOB", &Student::getDOB)
		.def("setPhoneNumber", &Student::setPhoneNumber)
		.def("getPhoneNumber", &Student::getPhoneNumber)
		.def("setAcademicDetails", &Student::setAcademicDetails)
		.def("getAcademicDetails", &Student::getAcademicDetails)
		.def("getApplicableOffers", &Student::getApplicableOffers)
		.def("apply", &Student::apply);
}