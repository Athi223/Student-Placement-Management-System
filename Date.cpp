#include "Date.h"
#include <pybind11/pybind11.h>
namespace py = pybind11;

Date::Date(int d, int m, int y) : day(d), month(m), year(y)
{
}

void Date::setDay(int d)
{
	day = d;
}

void Date::setMonth(int m)
{
	month = m;
}

void Date::setYear(int y)
{
	year = y;
}

int Date::getDay()
{
	return day;
}

int Date::getMonth()
{
	return month;
}

int Date::getYear()
{
	return year;
}

PYBIND11_MODULE(Date, m)
{
	py::class_<Date>(m, "Date")
		.def(py::init())
		.def(py::init<const int &, const int &, const int &>())
		.def("setDay", &Date::setDay)
		.def("setMonth", &Date::setMonth)
		.def("setYear", &Date::setYear)
		.def("getDay", &Date::getDay)
		.def("getMonth", &Date::getMonth)
		.def("getYear", &Date::getYear);
}