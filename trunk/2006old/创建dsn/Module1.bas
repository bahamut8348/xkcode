Attribute VB_Name = "Module1"
 Const HKEY_CURRENT_USER = &H80000001
 Const HKEY_LOCAL_MACHINE = &H80000002
 Const HKEY_USERS = &H80000003
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
