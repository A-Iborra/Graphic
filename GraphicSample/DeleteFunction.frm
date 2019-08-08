VERSION 5.00
Begin VB.Form DeleteFunction 
   Caption         =   "Delete a function"
   ClientHeight    =   1470
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8055
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   ScaleHeight     =   1470
   ScaleWidth      =   8055
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox expression 
      Height          =   375
      Left            =   240
      TabIndex        =   0
      Top             =   360
      Width           =   7575
   End
   Begin VB.CommandButton Cancel 
      Caption         =   "Cancel"
      Height          =   495
      Left            =   1560
      TabIndex        =   2
      Top             =   840
      Width           =   1215
   End
   Begin VB.CommandButton Delete 
      Caption         =   "Delete"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "Enter the function's expression (leave blank to delete all):"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   5535
   End
End
Attribute VB_Name = "DeleteFunction"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Delete_Click()
Dim f As Object
If Len(expression.Text) = 0 Then
    GraphicSample.GSystemGraphic(currentTabIndex).RemoveAllFunctions
    Unload Me
Else
    Set f = GraphicSample.GSystemGraphic(currentTabIndex).Function(expression.Text)
    If f Is Nothing Then
        MsgBox "The function with the expression: " + expression.Text + " was not found"
    Else
        GraphicSample.GSystemGraphic(currentTabIndex).RemoveFunction f
        Unload Me
    End If
End If
End Sub


Private Sub Cancel_Click()
Unload Me
End Sub


