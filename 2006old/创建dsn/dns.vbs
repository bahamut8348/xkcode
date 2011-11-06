Sub CreateDSN(strDSN, strDbName, strDescription, strDriverPath, strLastUser, strServer, strDriverName)

strPath = "SOFTWARE\ODBC\ODBC.INI\" & strDSN
Set objReg = GetObject("winmgmts:{impersonationLevel=impersonate}//./root/default:StdRegProv")

 strPath = "SOFTWARE\ODBC\ODBC.INI\" & strDSN
 objReg.CreateKey HKEY_LOCAL_MACHINE, strPath

 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "Database", strDbName
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "Description", strDescription
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "Driver", strDriverPath
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "LastUser", strLastUser
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "Server", strServer
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "Trusted_Connection", "No"

 'Ensure that the DSN gets listed in ODBC
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  "SOFTWARE\ODBC\ODBC.INI\ODBC Data Sources", _
  strDSN, strDriverName
  
 Set objReg = Nothing

End Sub


strDriverPath = "C:\WINNT\System32\sqlsrv32.dll"
strDriverName = "SQL Server"

CreateDSN "psserver","psserver","",strDriverPath,"psserver","192.168.1.13",strDriverName
CreateDSN "psplayer","psplayer","",strDriverPath,"psplayer","192.168.1.13",strDriverName

echo "ok"
