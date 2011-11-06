#pragma once


namespace PyPSClient2
{
	
	static Address StringToAddr(const std::wstring & str)
	{
		return ClientCommon::StrToAddress(str.c_str());
	}


	struct AreaInfo_ : public stCP_AreaInfo
	{
		std::wstring get_AreaName() const
		{
			return this->Desp;
		}

		void set_AreaName( const std::wstring & s)
		{
			STRCPYS(Desp, s.c_str());
		}
	};
	
	
	struct  PyPSUID
	{
		_dword mAccountID;
		_word mSSID;
		_dword mSeq;
		_byte mIDType;
		_qword mTimeStamp;
	
		std::wstring __str__() const
		{
			_char buf[1024];
			::_tprintf(buf, L"<PSUID(%d,%d,%d)", mIDType, mAccountID, mSSID);
			return buf;
		}

		void set(_dword accid, _word ssid, _dword seq, _byte idtype, _qword timestamp)
		{
			mAccountID		= accid;
			mSeq			= seq;
			mSSID			= ssid;
			mIDType			= idtype;
			mTimeStamp		= timestamp;
		}

		operator PSFC::Common::PSUID () const
		{
			return PSFC::Common::PSUID::CreatePSUID(mIDType, mAccountID, mSeq,mTimeStamp, mSSID);
		}

		static PyPSUID CreateFromPSUID(const PSFC::Common::PSUID & uid)
		{
			PyPSUID ret;
			ret.set(uid.AccountID(),uid.SSAccountID(), uid.SequenceNumber(), uid.IDType(), uid.TimeStamp());
			return ret;
		}

	};

}