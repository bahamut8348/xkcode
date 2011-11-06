#pragma once


class CTimerTask : public ACE_Event_Handler
{
public:

	

	void Start();

	void call_2(int a, int b);

	virtual int handle_timeout (const ACE_Time_Value &current_time,
		const void *act = 0);

	 virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

	
};

typedef void ( CTimerTask::*ACTION) (int a, int b);



template <typename C, class F>
class Select
{
public:
	Select( C self, F fun)
	{
		this->self = self;
		this->fun = fun;
	}
	
	template<typename A1, typename A2>
	void Execute(A1  a, A2  b)
	{
		(self->*fun)(a,b);
	}

	void Execute(void *a)
	{
		self->*fun(a);
	}

public:
	C self;
	F fun;
};

template <typename C, typename F>
Select<C, F> * MakeEvent(C self, F Fun)
{
	return new Select<C,F>(self, Fun);
}

