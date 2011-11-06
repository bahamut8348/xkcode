#include "StdAfx.h"
#include "sqlconvert.h"

static object datetime_datetime;

struct time_to_python
{
	static PyObject* convert(COleDateTime const& time)
	{
		SYSTEMTIME st;
		time.GetAsSystemTime(st);
		object result = datetime_datetime(
			(int)st.wYear
			, (int)st.wMonth
			, (int)st.wDay
			, st.wHour
			, st.wMinute
			, st.wSecond
			, st.wMilliseconds *1000
			);

		return incref(result.ptr());
	}
};

void initConvert()
{
	//object datetime = import("datetime").attr("__dict__");

	//datetime_datetime = datetime["datetime"];

	to_python_converter<COleDateTime, time_to_python>();

}