#pragma once

class CControl : public ACE_Handler
{
public:
	CControl( ACE_HANDLE );
	~CControl(void);

	 virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
	 virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
	 virtual ACE_HANDLE handle (void) const;

	 ACE_Asynch_Read_Stream rs_;
	 ACE_Asynch_Write_Stream ws_;

	 ACE_HANDLE handle_;

	 ACE_Message_Block data_;
	
};
