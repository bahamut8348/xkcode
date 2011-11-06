VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Config DSN"
   ClientHeight    =   2355
   ClientLeft      =   5730
   ClientTop       =   4170
   ClientWidth     =   4410
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2355
   ScaleWidth      =   4410
   Begin VB.Frame Frame2 
      Height          =   1095
      Left            =   1920
      TabIndex        =   6
      Top             =   1080
      Width           =   2415
      Begin VB.Label Label3 
         Caption         =   "For PSServer"
         Height          =   375
         Left            =   240
         TabIndex        =   7
         Top             =   360
         Width           =   2055
      End
   End
   Begin VB.CommandButton Command1 
      Caption         =   "CreateDSN"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   1200
      Width           =   1455
   End
   Begin VB.Frame Frame1 
      Height          =   975
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   4335
      Begin VB.TextBox dbname 
         Height          =   270
         Left            =   1200
         TabIndex        =   5
         Text            =   "Text2"
         Top             =   600
         Width           =   2895
      End
      Begin VB.TextBox dbserver 
         Height          =   270
         Left            =   1200
         TabIndex        =   3
         Text            =   "Text1"
         Top             =   240
         Width           =   2895
      End
      Begin VB.Label Label2 
         Caption         =   "DbName:"
         Height          =   255
         Left            =   120
         TabIndex        =   4
         Top             =   600
         Width           =   735
      End
      Begin VB.Label Label1 
         Caption         =   "DbServer:"
         Height          =   255
         Left            =   120
         TabIndex        =   2
         Top             =   240
         Width           =   855
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()

strDriverPath = "C:\WINNT\System32\sqlsrv32.dll"
strDriverName = "SQL Server"

strServer = dbserver.Text
strLastUser = "psplayer"
strDbName = dbname.Text

strDSN = "psplayer"
strdesp = "desp"
Call CreateDSN(strDSN, strDbName, strdesp, strDriverPath, strLastUser, strServer, strDriverName)

strDbName = "game"
strDSN = "psserver"
strLastUser = "psserver"
Call CreateDSN(strDSN, strDbName, strdesp, strDriverPath, strLastUser, strServer, strDriverName)

MsgBox "OK", vbInformation, "Create DSN"


End Sub

Private Sub Form_Load()
dbname.Text = "psplayer"
dbserver.Text = "192.168.1.13"
End Sub
