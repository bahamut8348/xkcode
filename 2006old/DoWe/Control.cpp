#include "StdAfx.h"
#include "Control.h"

CControl::CControl( ACE_HANDLE handle):
	handle_(handle),
		data_(1024)
{
	rs_.open(*this);
	rs_.read(data_, 9);
}
CControl::~CControl(void)
{
}

void CControl::handle_read_stream( const ACE_Asynch_Read_Stream::Result &result )
{
	size_t byte_to_read = result.bytes_to_read() - result.bytes_transferred();
	if(byte_to_read == 0)
	{
		data_.rd_ptr()[data_.length()] = 0;
		ACE_DEBUG(( LM_INFO, L"OK\n"));
		ACE_DEBUG(( LM_INFO, L"handle_read_stream %d, %C\n", result.bytes_transferred(),  data_.rd_ptr() ));
	}
	else
	{
		rs_.readv(data_, byte_to_read);
	}

	if(!result.success())
	{
		ACE_DEBUG (( LM_INFO, L"handle_read_stream Error: %m\n"));
		
	}
	
}

void CControl::handle_write_stream( const ACE_Asynch_Write_Stream::Result &result )
{
	ACE_DEBUG (( LM_INFO, L"handle_write_stream\n"));
}

ACE_HANDLE CControl::handle( void ) const
{
	return handle_;
}