VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5205
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6885
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   5205
   ScaleWidth      =   6885
   StartUpPosition =   3  '´°¿ÚÈ±Ê¡
   Begin VB.TextBox Text2 
      Height          =   2415
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   3
      Text            =   "Form1.frx":030A
      Top             =   2760
      Width           =   6855
   End
   Begin VB.TextBox Text1 
      Height          =   1815
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   2
      Top             =   840
      Width           =   6855
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Create Error Message"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   2175
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Test"
      Height          =   375
      Left            =   5640
      TabIndex        =   0
      Top             =   0
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim Right
Dim Bottom
Private Sub Command1_Click()


Set PythonUtils = CreateObject("PythonDemos.Utilities")
response = PythonUtils.SplitString("Hello from VB")
r = Array(1, 2, 3)
MsgBox PythonUtils.GetDict()
'Call PythonUtils.getdict
For Each Item In response
  MsgBox Item, vbInformation, "vb"
Next

End Sub

Private Sub Command2_Click()
    Set PythonUtils = CreateObject("PythonDemos.Utilities")
    n = Text1.Text
    Text2.Text = "ll"
    Text2.Text = PythonUtils.Makeerrmsg(n)
    
End Sub

Private Sub Form_Load()
Right = Form1.Width - Text1.Width
Bottom = Form1.Height - Text2.Height - Text2.Top


End Sub

Private Sub Form_Resize()
Text1.Width = Form1.Width - Right
Text2.Width = Form1.Width - Right
Text2.Height = Form1.Height - Text2.Top - Bottom



End Sub
