#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <ext/hash_map>
#include <ext/hash_set>
#include <arpa/inet.h>

#include "timer.h"
#include "netmanager.h"
#include "appinfo.h"

AppInfo::AppInfo():mConnNumber(0), mGetMem(0), mReleaseMem(0),mCfgLastRead(0){}


