#include <iostream>
#include <vector>
#include <array>
#include <sqlite3.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace std;
namespace py = pybind11;

static int verifyCallback(void *data, int argc, char **argv, char **azColName)
{
	vector<int> *data1 = (vector<int> *)data;
	if(atoi(argv[1]) == 1)
		data1[0].push_back(atoi(argv[0]));
	else
		data1[1].push_back(atoi(argv[0]));

	return 0;
}

class Test
{
	public:
	array<vector<int>, 2> verify()
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
};

PYBIND11_MODULE(Test, m)
{
	py::class_<Test>(m, "Test")
		.def(py::init())
		.def("verify", &Test::verify);
}