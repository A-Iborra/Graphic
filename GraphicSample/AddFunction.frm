VERSION 5.00
Begin VB.Form AddFunction 
   Caption         =   "Add a function"
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
   Begin VB.CommandButton Add 
      Caption         =   "Add"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "Enter the function's expression:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   5535
   End
End
Attribute VB_Name = "AddFunction"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim priorFunctionExpression As String

Private Sub Add_Click()
Unload Me
End Sub

Private Sub Cancel_Click()
GraphicSample.functionExpression = priorFunctionExpression
Unload Me
End Sub

Private Sub Form_Load()
Add.Enabled = False
priorFunctionExpression = functionExpression
End Sub

Private Sub expression_Change()
GraphicSample.functionExpression = expression.Text
If Len(GraphicSample.functionExpression) > 0 Then
    Add.Enabled = True
Else
    Add.Enabled = False
End If
End Sub
