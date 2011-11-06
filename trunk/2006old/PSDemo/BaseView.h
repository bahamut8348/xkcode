#pragma once


enum BaseView_Type
{
	BV_ROOMLIST = 0x01,
	BV_CHAT = 0x02,
	BV_GAME = 0x03, 
};


class  CBaseView
{
public :
	virtual _dword getType() const = 0;
	virtual ~CBaseView(){}
};

