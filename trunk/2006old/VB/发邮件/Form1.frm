VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
Set obj = CreateObject("CDO.Message")
'WScript.Echo logPath & "\backup" & strDate & ".html"
Set objMessage = CreateObject("CDO.Message")
objMessage.Configuration.Fields.Item("http://schemas.microsoft.com/cdo/configuration/sendusing") = 2 'use '1' for local SMTP
objMessage.Configuration.Fields.Item("http://schemas.microsoft.com/cdo/configuration/smtpserver") = strSmtpServer
objMessage.Configuration.Fields.Item("http://schemas.microsoft.com/cdo/configuration/smtpserverport") = strSmtpPort
If strSmtpAuth = "yes" Then
    objMessage.Configuration.Fields.Item("http://schemas.microsoft.com/cdo/configuration/smtpauthenticate") = 1 'use '2' for NTLM authentication
    objMessage.Configuration.Fields.Item("http://schemas.microsoft.com/cdo/configuration/sendusername") = strSmtpUser
    objMessage.Configuration.Fields.Item("http://schemas.microsoft.com/cdo/configuration/sendpassword") = strSmtpPass
End If
If strSmtpSsl = "yes" Then
    objMessage.Configuration.Fields.Item("http://schemas.microsoft.com/cdo/configuration/smtpusessl") = True
End If
objMessage.Configuration.Fields.Update
objMessage.Subject = "Dated Backup Report for " & strDate & "."
objMessage.From = strSendingEmail
objMessage.To = strReportEmail
objMessage.HTMLBody = objExplorer.Document.Body.InnerHTML
'objMessage.AddAttachment = logPath & "\backup" & strDate & ".html"
objMessage.Send
End Sub
