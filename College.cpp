#include "College.h"
#include <sqlite3.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace std;
namespace py = pybind11;

// Constructor definition
College::College(string mCollegeName, int mCollegeCode, string mCollegeEmail, string mPhoneNumber) : collegeName(mCollegeName), collegeCode(mCollegeCode), collegeEmail(mCollegeEmail), phoneNumber(mPhoneNumber) {}

void College::setName(string s)
{
	collegeName = s;
}

void College::setCollegeCode(int code)
{
	collegeCode = code;
}

void College::setEmail(string e)
{
	collegeEmail = e;
}

void College::setPhoneNumber(string pn)
{
	phoneNumber = pn;
}

string College::getName()
{
	return collegeName;
}

int College::getCollegeCode()
{
	return collegeCode;
}

string College::getEmail()
{
	return collegeEmail;
}

string College::getPhoneNumber()
{
	return phoneNumber;
}

static int verifyCallback(void *data, int argc, char **argv, char **azColName)
{
	vector<int> *data1 = (vector<int> *)data;
	if(atoi(argv[1]) == 1)
		data1[0].push_back(atoi(argv[0]));
	else
		data1[1].push_back(atoi(argv[0]));

	return 0;
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char *)data);

	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

array<vector<int>, 2> College::verify()
{
	// Open SQLite database
	sqlite3 *DB;
	int exit = 0;
	exit = sqlite3_open("db.sqlite3", &DB);
	array<vector<int>, 2> data;
	// verify all pending student profiles
	string sql("SELECT SS.id, SS.cgpa = CS.cgpa AND SS.liveBacklog = CS.liveBacklog AND SS.deadBacklog = CS.deadBacklog AND SS.prn = CS.prn AND SS.yearGap = CS.yearGap AND SS.passingYear = CS.passingYear AND SS.department = CS.department AS result FROM (Student_student AS stud INNER JOIN Student_eligibility AS elig ON stud.id = elig.studentId_id) AS SS INNER JOIN College_studentinfo AS CS ON SS.prn = CS.prn WHERE SS.id = stud.id AND SS.verificationStatus = 3;");
	try
	{
		if (exit)
			throw exit;

		std::cout << "Opened database successfully" << std::endl;

		int rc = sqlite3_exec(DB, sql.c_str(), verifyCallback, &data, NULL);

		if (rc != SQLITE_OK)
		{
			throw (short) -1;
		}
		
		cout << "Operation OK!" << endl;
	}
	catch(int exit) {
			std::cout << "Exception caught" << std::endl;
			std::cerr << "Error open DB" << sqlite3_errmsg(DB) << std::endl;
	}

	catch(short rc) {
			std::cout << "Exception caught" << endl;
			std::cerr << "Error in auto verification" << endl;
	}

	// Close sqlite
	sqlite3_close(DB);
	return data;
}

void College::viewEnrolledStudents()
{

	// Open SQLite database
	sqlite3 *DB;
	int exit = 0;
	exit = sqlite3_open("db.sqlite3", &DB);
	string data("");

	// Pass queries to sql throught 
	string sql("SELECT S.studentId_id, A.first_name, A.last_name, S.department FROM STUDENT_student S, auth_user A where S.id=A.id");

	try
	{
		if (exit)
			throw exit;

		std::cout << "Opened database successfully" << std::endl;

		int rc = sqlite3_exec(DB, sql.c_str(), callback, (void *)data.c_str(), NULL);
		std::cout << std::endl;


		if (rc != SQLITE_OK)
		{
			throw (short) -1;
		}
		
		cout << "Operation OK!" << endl;
	}

	catch(int exit) {
			std::cout << "Exception caught" << std::endl;
			std::cerr << "Error open DB" << sqlite3_errmsg(DB) << std::endl;
	}

	catch(short rc) {
			std::cout << "Exception caught" << endl;
			std::cerr << "Error getting count " << endl;
	}

	// Close sqlite
	sqlite3_close(DB);
}

void College::updateStudentPlacedStatus()
{
	// Open SQLite database
	sqlite3 *DB;
	int exit = 0;
	exit = sqlite3_open("db.sqlite3", &DB);
	string data("");

	// Update all records of the students placed from temporary table to the college database
	string sql("UPDATE Student_student SET placedOffer_id = COALESCE(placedOffer_id, (SELECT offerId_id FROM College_intermediatestudentstatus WHERE status = 1 AND Student_student.id = College_intermediatestudentstatus.studentId_id));");

	try
	{
		if (exit)
			throw exit;

		std::cout << "Opened database successfully" << std::endl;

		int rc = sqlite3_exec(DB, sql.c_str(), NULL, NULL, NULL);
		std::cout << std::endl;


		if (rc != SQLITE_OK)
		{
			throw (short) -1;
		}
		
		cout << "Operation OK!" << endl;
	}

	catch(int exit) {
			std::cout << "Exception caught" << std::endl;
			std::cerr << "Error open DB" << sqlite3_errmsg(DB) << std::endl;
	}

	catch(short rc) {
			std::cout << "Exception caught" << endl;
			std::cerr << "Error updating placed students " << endl;
	}

	// Close sqlite
	sqlite3_close(DB);
}

PYBIND11_MODULE(College, m)
{
	py::class_<College>(m, "College")
		.def(py::init())
		.def(py::init<const string &, const int &, const string &, const string &>())
		.def("setName", &College::setName)
		.def("getName", &College::getName)
		.def("setCollegeCode", &College::setCollegeCode)
		.def("getCollegeCode", &College::getCollegeCode)
		.def("setEmail", &College::setEmail)
		.def("getEmail", &College::getEmail)
		.def("setPhoneNumber", &College::setPhoneNumber)
		.def("getPhoneNumber", &College::getPhoneNumber)
		.def("verify", &College::verify)
		.def("viewEnrolledStudents", &College::viewEnrolledStudents)
		.def("updateStudentPlacedStatus", &College::updateStudentPlacedStatus);
}