// EventFiringObject_Impl.cpp : Implementation of CEventFiringObject
#include "stdafx.h"
#include "EventFiringObject.h"
#include "EventFiringObject_Impl.h"

/////////////////////////////////////////////////////////////////////////////
// CEventFiringObject

STDMETHODIMP CEventFiringObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEventFiringObject
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CEventFiringObject::TestFunction(long lValue)
{
	// TODO: Add your implementation code here
	Fire_Event1(lValue);

	return S_OK;
}
