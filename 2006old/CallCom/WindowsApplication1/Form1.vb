Public Class Form1

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim vss = CreateObject("SourceSafe")
        vss.Open("\\lechuck\vss\srcsafe.ini", "keger", "")
        Dim project = vss.VSSItem("$/PSDash")
        Dim items = project.Items()
        Dim i
        For Each i In items

        Next

    End Sub
End Class
