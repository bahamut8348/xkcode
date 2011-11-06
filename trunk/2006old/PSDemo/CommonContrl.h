#pragma  once  


template <class TBase>
class CLocationCbT :  public CComboBoxT< TBase >
{
public:
	Hash<LocationNode, _dword> m_Locations;

	CLocationCbT< TBase >& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}


	CLocationCbT():m_Locations(1000)
	{
		Location location;
		m_Locations = location.GetAllLocations();
	}

	void Init()
	{
		int i = 0;
		for(LocationHash::Iterator Iter = m_Locations.GetHeadIterator(); Iter.Valid(); Iter++)
		{
			LocationNode & node = Iter;
			int idx = this->InsertString(i, node.name);
			SetItemData(idx, (_dword)&node);
			i++;
		}
	}

	DWORD GetValue() const
	{
		int idx = this->GetCurSel();
		LocationNode * pnode =(LocationNode *) GetItemData(idx);
		return pnode->id;
	}

	void SetValue(int n)
	{
		for(int a=0; a<this->GetCount(); a++)
		{
			LocationNode * pnode =(LocationNode *) GetItemData(a);
			if(pnode->id == n)
			{
				SetCurSel(a);
			}
		}
	}
	

};

typedef CLocationCbT<ATL::CWindow> CLocationCb;

template<typename TBase>
class CGenderCbT : public CComboBoxT<TBase>
{
public:

	struct stGender
	{
		int n;
		CString str;
		stGender(){}
		stGender(int _n, LPCTSTR _str)
		{
			n = _n;
			str = _str;
		}
	};

		
	CAtlArray<stGender> m_Genders;

	CGenderCbT< TBase >& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}


	
	void InitGender()
	{
		m_Genders.Add(stGender(0,L"--"));
		m_Genders.Add(stGender(1,L"Male"));
		m_Genders.Add(stGender(2,L"Female"));
		m_Genders.Add(stGender(4,L"Other For Test"));
		
		for(u_int a=0; a<m_Genders.GetCount();a++)
		{
			stGender & g = m_Genders[a];
			int idx = this->InsertString(a, g.str);
			this->SetItemData(idx, g.n);
		}
	}

	DWORD GetValue() const
	{
		return this->GetItemData(this->GetCurSel());
	}

	void SetValue(int n) 
	{
		for(int a=0; a<this->GetCount();a++)
		{
			if(this->GetItemData(a) == n)
			{
				this->SetCurSel(a);
			}
		}
	}
};

typedef  CGenderCbT<CWindow>  CGenderCb;