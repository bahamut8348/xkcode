
#define EXPORT_API  extern"C" __declspec( dllexport ) 

#define STD __stdcall

EXPORT_API BOOL STD EnumAdapter( BYTE * buffer, DWORD* buffer_size );