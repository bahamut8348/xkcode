// EventFiringObject_Impl.h : Declaration of the CEventFiringObject

#ifndef __EVENTFIRINGOBJECT_H_
#define __EVENTFIRINGOBJECT_H_

#include "resource.h"       // main symbols
#include "EventFiringObjectCP.h"

/////////////////////////////////////////////////////////////////////////////
// CEventFiringObject
class ATL_NO_VTABLE CEventFiringObject : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEventFiringObject, &CLSID_EventFiringObject>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CEventFiringObject>,
	public IDispatchImpl<IEventFiringObject, &IID_IEventFiringObject, &LIBID_EVENTFIRINGOBJECTLib>,
	public CProxy_IEventFiringObjectEvents< CEventFiringObject >
{
public:
	CEventFiringObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EVENTFIRINGOBJECT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEventFiringObject)
	COM_INTERFACE_ENTRY(IEventFiringObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CEventFiringObject)
CONNECTION_POINT_ENTRY(DIID__IEventFiringObjectEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IEventFiringObject
public:
	STDMETHOD(TestFunction)(/*[in]*/ long lValue);
};

#endif //__EVENTFIRINGOBJECT_H_
