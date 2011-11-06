Attribute VB_Name = "Module1"
 Const HKEY_CURRENT_USER = &H80000001
 Const HKEY_LOCAL_MACHINE = &H80000002
 Const HKEY_USERS = &H80000003
Sub SetAutoLogin(Username, Password, Bauto)

strPath = "SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon"
Set objReg = GetObject("winmgmts:{impersonationLevel=impersonate}//./root/default:StdRegProv")
 
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "DefaultUserName", Username
 objReg.SetStringValue HKEY_LOCAL_MACHINE, _
  strPath, "DefaultPassword", Password
 objReg.SetDWORDValue HKEY_LOCAL_MACHINE, _
  strPath, "AutoAdminLogon", Bauto

 'Ensure that the DSN gets listed in ODBC
 
  
 Set objReg = Nothing

End Sub


