VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "Mscomctl.ocx"
Object = "{8CAEFD83-55E6-11D3-8365-006008BD5BC3}#1.0#0"; "FunctioNater.ocx"
Begin VB.Form functions 
   Caption         =   "Form1"
   ClientHeight    =   9375
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   12840
   FillColor       =   &H8000000E&
   ForeColor       =   &H80000008&
   LinkTopic       =   "Form1"
   ScaleHeight     =   9375
   ScaleWidth      =   12840
   StartUpPosition =   3  'Windows Default
   Begin FunctioNater.GSystemFunctioNater GSystemFunctioNater1 
      Height          =   4260
      Left            =   0
      TabIndex        =   14
      Top             =   360
      Width           =   3855
      _Property_1_type=   5
      _Property_1     =   0   'False
      variables_type  =   8
      variables       =   $"FunctionSample.frx":0000
      manuallyAddedVariables_type=   8
      manuallyAddedVariables=   "fafbfcfd00000000"
      expression_type =   2
      expression      =   "z = cos(sqrt(x*x + y*y)) // default 1"
      expressionLabel_type=   2
      expressionLabel =   "Specify some expression here"
      resultsLabel_type=   2
      resultsLabel    =   "results:"
      allowUserProperties_type=   5
      allowUserProperties=   -1  'True
      allowUserPropertiesControl_type=   5
      allowUserPropertiesControl=   -1  'True
      entryFieldVisible_type=   5
      entryFieldVisible=   -1  'True
      resultsFieldVisible_type=   5
      resultsFieldVisible=   -1  'True
      variablesVisible_type=   5
      variablesVisible=   -1  'True
      controlsVisible_type=   5
      controlsVisible =   -1  'True
      startVisible_type=   5
      startVisible    =   -1  'True
      pauseVisible_type=   5
      pauseVisible    =   -1  'True
      resumeVisible_type=   5
      resumeVisible   =   -1  'True
      stopVisible_type=   5
      stopVisible     =   -1  'True
      containerSize_type=   4
      containerSize   =   "010100001c010000"
      rect_type       =   4
      rect            =   "00000000180000000101000050020000"
   End
   Begin FunctioNater.GSystemFunctioNater GSystemFunctioNater2 
      Height          =   4260
      Left            =   4200
      TabIndex        =   15
      Top             =   2400
      Width           =   5175
      _Property_1_type=   5
      _Property_1     =   0   'False
      variables_type  =   8
      variables       =   $"FunctionSample.frx":2824
      manuallyAddedVariables_type=   8
      manuallyAddedVariables=   "fafbfcfd00000000"
      expression_type =   2
      expression      =   "z = sin(sqrt(x*x + y*y)) // default 2"
      expressionLabel_type=   2
      expressionLabel =   "Specify some expression here"
      resultsLabel_type=   2
      resultsLabel    =   "results:"
      allowUserProperties_type=   5
      allowUserProperties=   -1  'True
      allowUserPropertiesControl_type=   5
      allowUserPropertiesControl=   -1  'True
      entryFieldVisible_type=   5
      entryFieldVisible=   -1  'True
      resultsFieldVisible_type=   5
      resultsFieldVisible=   -1  'True
      variablesVisible_type=   5
      variablesVisible=   -1  'True
      controlsVisible_type=   5
      controlsVisible =   -1  'True
      startVisible_type=   5
      startVisible    =   -1  'True
      pauseVisible_type=   5
      pauseVisible    =   -1  'True
      resumeVisible_type=   5
      resumeVisible   =   -1  'True
      stopVisible_type=   5
      stopVisible     =   -1  'True
      containerSize_type=   4
      containerSize   =   "590100001c010000"
      rect_type       =   4
      rect            =   "18010000a000000071020000d8020000"
   End
   Begin VB.TextBox stepCount 
      Height          =   315
      Left            =   6240
      TabIndex        =   13
      Text            =   "10"
      Top             =   1200
      Width           =   795
   End
   Begin VB.TextBox iterationCount 
      Enabled         =   0   'False
      Height          =   315
      Left            =   5760
      TabIndex        =   11
      Text            =   "iterationCount"
      Top             =   1980
      Width           =   1215
   End
   Begin VB.TextBox results 
      Enabled         =   0   'False
      Height          =   315
      Left            =   5760
      TabIndex        =   10
      Text            =   "results"
      Top             =   1620
      Width           =   3195
   End
   Begin VB.CommandButton removeVariable 
      Caption         =   "&remove"
      Height          =   375
      Left            =   900
      TabIndex        =   7
      Top             =   4680
      Width           =   735
   End
   Begin VB.TextBox varName 
      Height          =   315
      Left            =   2100
      TabIndex        =   6
      Text            =   "v"
      Top             =   4740
      Width           =   1215
   End
   Begin VB.CommandButton addVariable 
      Caption         =   "&add"
      Height          =   375
      Left            =   60
      TabIndex        =   5
      Top             =   4680
      Width           =   735
   End
   Begin VB.CommandButton stop 
      Caption         =   "s&top"
      Height          =   435
      Left            =   4980
      TabIndex        =   4
      Top             =   540
      Width           =   735
   End
   Begin VB.CommandButton start 
      Caption         =   "&Start"
      Height          =   435
      Left            =   4140
      TabIndex        =   3
      Top             =   540
      Width           =   735
   End
   Begin MSComctlLib.ListView lvResults 
      Height          =   2835
      Left            =   60
      TabIndex        =   1
      Top             =   5220
      Width           =   4035
      _ExtentX        =   7117
      _ExtentY        =   5001
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   0
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Right Justify
      Caption         =   "iteration #"
      Height          =   315
      Left            =   4980
      TabIndex        =   8
      Top             =   2040
      Width           =   735
   End
   Begin VB.Label Label4 
      Alignment       =   1  'Right Justify
      Caption         =   "result"
      Height          =   315
      Left            =   5160
      TabIndex        =   9
      Top             =   1680
      Width           =   555
   End
   Begin VB.Label Label3 
      Caption         =   "# Iterations for each variable"
      Height          =   255
      Left            =   4080
      TabIndex        =   12
      Top             =   1260
      Width           =   2055
   End
   Begin VB.Label Label2 
      Caption         =   "This is an instance using the buit-in interface"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   0
      TabIndex        =   2
      Top             =   0
      Width           =   3975
   End
   Begin VB.Label Label1 
      Caption         =   "This is an instance using full programmable control"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   4140
      TabIndex        =   0
      Top             =   0
      Width           =   4515
   End
End
Attribute VB_Name = "functions"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'   This sample application users the GSystems FunctioNater object
'   to generate numbers from two types of instances of the object.
'
'   Because there are different ways to use the object, each sample
'   way is illustrated in this example using a distinct part of this file.
'   This will help to keep the clutter down.
'
'   1. The first example is a standard user-manipulated function.
'   In this scenario, the FunctioNater is instantiated on the form
'   with all of it's user-interface components showing.
'   Here the user changes the function and variables definitions
'   and clicks start.
'
'

Public resultCount As Long


Private Sub Form_Load()

GSystemFunctioNater1.PersistFrom "f1"
GSystemFunctioNater2.PersistFrom "f2"

End Sub


Private Sub Form_unload(cancel As Integer)
    GSystemFunctioNater1.PersistTo "f1"
    GSystemFunctioNater2.PersistTo "f2"
End Sub


Private Sub addVariable_Click()
    GSystemFunctioNater1.DefineSimpleVariable varName.Text, "3 * pi"
End Sub


Private Sub removeVariable_Click()
    GSystemFunctioNater1.UndefineVariable varName.Text
End Sub


'   The following code is for manipulating the FunctioNater in it's default
'   state, which is represented by the GSystemFunctioNater1 object on the form.
'
'   The following are the events that you should code to take control of the
'   workings within the FunctioNater object.

'   The function will send the clear event before it attempts to determine the
'   independent variables in the function expression.
'
'   Since this sample will populate a list view with the results the function
'   generates, this is a good time to reset that list-view
'
Private Sub GSystemFunctioNater1_Clear()
'    lvResults.ListItems.Clear
'    lvResults.ColumnHeaders.Clear
End Sub

'
'   When the function is finished defining it's variables, it starts traversing the
'   independent variable values and producing the result.

'   The function sends this message when evaluation of the function starts,
'   passing the estimated number of values that will be produced.

Private Sub GSystemFunctioNater1_Started(ByVal cntExpectedResults As Long)
End Sub


'   As the function parses the expression, it will discover undefined variables
'   in the expression.

'   For example, this is how the function determines the names of the independent
'   variables (variables that you provide the domain for).
'   Each time the function finds one of these, it will send this event to your code
'   in case you would like to do something with the function name.

'   In general, the function object will create the definition it needs in order to
'   run the function (using default values). However, you can explicitly define the
'   variable to the function with the DefineVariable() method on the object.

'   Note that if you DONT define the variable, you should return functionEventRC_NotHandled
'   to tell the function object to use it's own internal definition for the function.
'   If you DO define the variable, return functionEventRC_Handled.

'   Note also that when the variable(s) definition user-interface is showing
'   (showVariables = true), that the user does define the range of these variables.
'   However, the function object is still going to send this event.

Private Sub GSystemFunctioNater1_UndefinedVariable(ByVal variableName As String)

End Sub


Private Sub GSystemFunctioNater1_TakeValues(ByVal iterationNo As Long, ByVal valueCount As Long, theNames() As String, theValues() As Double)

'    Dim colHeader As ColumnHeader
'    Dim li As ListItem
'    Dim value As Variant
'    Dim k As Integer
'    Dim colNumber As Integer
'    Dim foundHeader As Boolean
'
'    Set li = lvResults.ListItems.Add(lvResults.ListItems.Count + 1, "__" + Str$(lvResults.ListItems.Count + 1) + "__", Str$(iterationNo), 0, 0)
'
'    k = 0
'    For Each value In theValues
'        k = k + 1
'        foundHeader = False
'        colNumber = 1
'        For Each colHeader In lvResults.ColumnHeaders
'            If colHeader.Text = theNames(k) Then
'                foundHeader = True
'                Exit For
'            End If
'            colNumber = colNumber + 1
'        Next
'        If Not foundHeader Then
'            lvResults.ColumnHeaders.Add colNumber, "__" + Str$(colNumber) + "__", theNames(k), 800, 0, 0
'        End If
'        If colNumber = 1 Then
'            li.Text = Format$(theValues(1), "#,###,##0.0#")
'        Else
'            li.ListSubItems().Add colNumber - 1, "__" + Str$(colNumber) + "__", Format$(value, "#,###,##0.0##"), 0, 0
'        End If
'    Next value

End Sub


'   If the expression contains a divide by zero, you can take appropriate
'   action upon event notification

Private Sub GSystemFunctioNater1_DivideByZero()

'    MsgBox "There was a divide by zero - stopping"
'
'    GSystemFunctioNater1.stop

End Sub


'   If a math error occurs in the expression due to an invalid argument to one
'   of the standard functions, you can take appropriate action upon event notification

Private Sub GSystemFunctioNater1_InvalidArgument(ByVal functionName As String, ByVal theArgument As Double)


End Sub

'   The following code is used to manipulate a fully programmable
'   instance of the FunctioNater object.

'   This instance will be primarily controlled by your code.

Private Sub start_Click()
    GSystemFunctioNater2.Start
End Sub


Private Sub stop_Click()
    GSystemFunctioNater2.Stop
End Sub

'   Programming for this instance of the FunctioNater object will assist the
'   object in defining the variables.

'   When the object parses the expression, it will send the UndefinedVariable event
'   when it encounters one of the independent variables. When you receive this
'   event, define the variable to the FunctioNater object by calling
'   DefineIndependentVariable on it's interface. The method expects to receive
'   the variable name, the range over which the variable is valid, and the number
'   of steps to take the value along that range.

'   Note the range is specified by two values setting the minimum and maximum values.
'   These values are a string representing a number or expression.


Private Sub GSystemFunctioNater2_Started(ByVal cntExpectedResults As Long)

    resultCount = 0

End Sub

'   Note: Don't be surprised if this event occurs as the FunctioNater is being
'   "loaded" from the saved (persisted) state. At that time, the FunctioNater
'   will parse the function which may cause this event to fire.

Private Sub GSystemFunctioNater2_UndefinedVariable(ByVal variableName As String)
    GSystemFunctioNater2.DefineIndependentVariable variableName, "-3", "3", stepCount.Text
End Sub

Private Sub GSystemFunctioNater2_Parsed()

'    GSystemFunctioNater2.start

End Sub

'   Each time a set of values is ready for use by this application, the TakeValues
'   event is fired.

Private Sub GSystemFunctioNater2_TakeValues(ByVal iterationNo As Long, ByVal valueCount As Long, theNames() As String, theValues() As Double)
'    resultCount = resultCount + 1
'    iterationCount.Text = Str$(resultCount)
End Sub


'   You can also catch the TakeResults event which sends a parseable string representing
'   the results.

Private Sub GSystemFunctioNater2_TakeResults(ByVal iterationNo As Long, ByVal strResults As String)
'    results.Text = strResults
End Sub


'   Don't confuse the Stopped event with the Finished event.
'   The Stopped event happens if the user (or software) has caused the
'   evaluation of the function to stop.
'   The Finished event is sent when the evaluation reaches its end naturally

Private Sub GSystemFunctioNater2_Finished()
End Sub
