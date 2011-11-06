#include <stdio.h>

#ifndef _APPINFO_H_
#define _APPINFO_H_
class AppInfo
{
	public:
		int mConnNumber;
		int mGetMem;
		int mReleaseMem;	
		std::string mCfgPath;
		time_t mCfgLastRead;		
		std::list<int> mRatesList;		

		AppInfo();

		void AddRate( int a ){

			mRatesList.push_back( a );
			if( mRatesList.size() > 10 ){
				mRatesList.pop_front();
			}
		}


	static		AppInfo *Instance(){
			static AppInfo o;
			return &o;
		}
};
#endif

