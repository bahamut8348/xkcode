VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "AutoLogin"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  '窗口缺省
   Begin VB.Frame Frame1 
      Caption         =   "Setting"
      Height          =   1815
      Left            =   120
      TabIndex        =   1
      Top             =   0
      Width           =   4095
      Begin VB.CheckBox Check1 
         Caption         =   "允许自动登录"
         Height          =   255
         Left            =   120
         TabIndex        =   6
         Top             =   1320
         Width           =   1575
      End
      Begin VB.TextBox Text2 
         Height          =   270
         Left            =   1080
         TabIndex        =   5
         Text            =   "123"
         Top             =   840
         Width           =   2775
      End
      Begin VB.TextBox Text1 
         Height          =   270
         Left            =   1080
         TabIndex        =   2
         Text            =   "Admin"
         Top             =   360
         Width           =   2775
      End
      Begin VB.Label Label2 
         Caption         =   "密码"
         Height          =   255
         Left            =   120
         TabIndex        =   4
         Top             =   840
         Width           =   615
      End
      Begin VB.Label Label1 
         Caption         =   "用户名"
         Height          =   255
         Left            =   120
         TabIndex        =   3
         Top             =   360
         Width           =   855
      End
   End
   Begin VB.CommandButton Command1 
      Caption         =   "设置"
      Height          =   495
      Left            =   3120
      TabIndex        =   0
      Top             =   2040
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()


SetAutoLogin Text1.Text, Text2.Text, Check1.Value

MsgBox "设置完成!"
End Sub

Private Sub Form_Load()
Check1.Value = 1
End Sub
