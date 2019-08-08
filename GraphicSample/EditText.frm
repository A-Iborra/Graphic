VERSION 5.00
Begin VB.Form EditText 
   Caption         =   "Edit Text"
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
   Begin VB.CommandButton Edit 
      Caption         =   "Edit"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "Enter the Text:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   5535
   End
End
Attribute VB_Name = "EditText"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Edit_Click()
Dim f As Object
Set f = GraphicSample.GSystemGraphic(currentTabIndex).Text(expression.Text)
If Not f Is Nothing Then
    f.EditProperties
    Unload Me
Else
    MsgBox "The Text with the value: " + expression.Text + " was not found"
End If
End Sub


Private Sub Cancel_Click()
Unload Me
End Sub

Private Sub Form_Load()
Edit.Enabled = False
End Sub

Private Sub expression_Change()
Edit.Enabled = Len(expression.Text) > 0
End Sub
