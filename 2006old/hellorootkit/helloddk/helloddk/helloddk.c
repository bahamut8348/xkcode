// helloddk.cpp : Defines the entry point for the console application.

#include <ntddk.h>

VOID OnUnload( IN PDRIVER_OBJECT DriverObject )
{
	DbgPrint("OnUnload called\n");
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath )
{
	DbgPrint("I loaded!\n");

	theDriverObject->DriverUnload  = OnUnload; 

	return STATUS_SUCCESS;
}
