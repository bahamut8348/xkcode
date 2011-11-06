// test_asio.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost;

void print( asio::deadline_timer* t , int *count)
{
	_tprintf(L"Count: %d\n", *count);
	t->expires_at(t->expires_at()+boost::posix_time::seconds(1));
	if(*count == 0) return;
	t->async_wait(boost::bind(print, t, count));
	*count -=1;
}

class Printer
{
public:
	int m_count;
	asio::deadline_timer *m_t;
	Printer(asio::deadline_timer *t) : m_count(5), m_t(t)
	{
	
	}

	void print()
	{
		_tprintf(L"Count: %d\r\n", m_count);
		m_count--;
		if(m_count > 0)
		{
			m_t->expires_at(m_t->expires_at() + boost::posix_time::seconds(1));
			m_t->async_wait(boost::bind(&Printer::print, this));

		}
	}
};

class Counter
{
public:
	Counter(asio::io_service & io):a(0), b(0)
		,timer(io, boost::posix_time::seconds(1))
		,timer2(io, boost::posix_time::seconds(1))
		,strand_(io)
	{


		timer.async_wait(boost::bind(&Counter::Add, this));
		timer2.async_wait(boost::bind(&Counter::Add2, this));
	}

	void Add()
	{
		a++;
		if(a>10000) return;
		timer.expires_at(timer.expires_at()+boost::posix_time::microseconds(1));
		timer.async_wait(boost::bind(&Counter::Add, this));
		
	}
	void Add2()
	{
		b++;
		if(b>10000) return;
		timer2.expires_at(timer.expires_at()+boost::posix_time::microseconds(1));
		timer2.async_wait(boost::bind(&Counter::Add2, this));

	}

	void Wait()
	{
		_tprintf(L"Counter Result: %d %d\n", a,b);
	}


	asio::deadline_timer timer;
	asio::deadline_timer timer2;
	asio::strand strand_;
int a, b;
};


class TimeServer
{
public:
	TimeServer(asio::io_service & io)
	{
		using asio::ip::tcp;
		tcp::resolver resolver(io);

		tcp::resolver::query query("www.163.com", "daytime");
		tcp::resolver::iterator endpoint_iterator;
		try
		{
			endpoint_iterator = resolver.resolve(query);
		}
		catch (std::exception& e)
		{
			printf("Exception: %s", e.what());
		}
		
		tcp::resolver::iterator end;
		
		while(endpoint_iterator != end)
		{
			USES_CONVERSION;
	//boost:	asio::error error = boost::asio::error::host_not_found;
			tcp::socket sock(io);
//			try
			{
				sock.connect(*endpoint_iterator);
				//sock.read_some();
			}
//			catch (asio::error & error)
			{
//				printf("%s\n",error.what());
			}
			
			
//			tcp::resolver::endpoint_type & ep = (*endpoint_iterator).endpoint();
			
//			_tprintf(L"%s\n",A2W( ep.address().to_string().c_str()) );

			endpoint_iterator++;
		}
		
		

	};
	





};


int _tmain(int argc, _TCHAR* argv[])
{
	asio::io_service io;
	
	
	//TimeServer ts(io);


	io.run();
	return 0;
}

