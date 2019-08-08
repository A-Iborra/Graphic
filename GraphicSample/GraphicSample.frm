VERSION 5.00
Object = "{8CAEFD89-55E6-11D3-8365-006008BD5BC3}#1.0#0"; "Properties.ocx"
Object = "{8CAEFD83-55E6-11D3-8365-006008BD5BC3}#1.0#0"; "FunctioNater.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{8CAEFD70-55E6-11D3-8365-006008BD5BC3}#1.0#0"; "Graphic.ocx"
Begin VB.Form GraphicSample 
   Caption         =   "Graphic Sample"
   ClientHeight    =   12525
   ClientLeft      =   60
   ClientTop       =   360
   ClientWidth     =   17775
   LinkTopic       =   "Form1"
   ScaleHeight     =   12525
   ScaleWidth      =   17775
   StartUpPosition =   3  'Windows Default
   Begin FunctioNater.GSystemFunctioNater GS2DFunction 
      Height          =   4260
      Left            =   120
      TabIndex        =   14
      Top             =   120
      Width           =   5175
      _Property_1_type=   5
      _Property_1     =   0   'False
      variables_type  =   8
      variables       =   $"GraphicSample.frx":0000
      manuallyAddedVariables_type=   8
      manuallyAddedVariables=   "fafbfcfd00000000"
      expression_type =   2
      expression      =   "y = x * cos(x)"
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
      rect            =   "08000000080000006101000040020000"
   End
   Begin FunctioNater.GSystemFunctioNater GS3DFunction 
      Height          =   4260
      Left            =   0
      TabIndex        =   13
      Top             =   5040
      Width           =   5295
      _Property_1_type=   5
      _Property_1     =   0   'False
      variables_type  =   8
      variables       =   $"GraphicSample.frx":141C
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
      containerSize   =   "610100001c010000"
      rect_type       =   4
      rect            =   "00000000500100006101000088030000"
   End
   Begin GSystemPropertiesCtl.GSystemProperties GSystemProperties 
      Left            =   1440
      Top             =   9720
      _Property_1_type=   5
      _Property_1     =   0   'False
      ContainerSize_type=   4
      ContainerSize   =   "8d00000055000000"
      _Property_3_type=   3
      _Property_3     =   ""
      _Property_4_type=   3
      _Property_4     =   ""
      _Property_5_type=   3
      _Property_5     =   ""
      FileType_type   =   3
      FileType        =   ""
   End
   Begin VB.Frame Frames 
      Height          =   2535
      Index           =   2
      Left            =   11880
      TabIndex        =   18
      Top             =   600
      Width           =   3015
      Begin Graphic.GSystemGraphic GSystemGraphic 
         Height          =   2175
         Index           =   2
         Left            =   120
         OleObjectBlob   =   "GraphicSample.frx":3C40
         TabIndex        =   22
         Top             =   240
         Width           =   2775
      End
   End
   Begin VB.Frame Frames 
      Height          =   2535
      Index           =   1
      Left            =   8760
      TabIndex        =   19
      Top             =   600
      Width           =   3015
      Begin Graphic.GSystemGraphic GSystemGraphic 
         Height          =   2175
         Index           =   1
         Left            =   120
         OleObjectBlob   =   "GraphicSample.frx":A85E
         TabIndex        =   21
         Top             =   240
         Width           =   2775
      End
   End
   Begin VB.Frame Frames 
      Height          =   2535
      Index           =   0
      Left            =   5640
      TabIndex        =   17
      Top             =   600
      Width           =   3015
      Begin Graphic.GSystemGraphic GSystemGraphic 
         Height          =   2175
         Index           =   0
         Left            =   120
         OleObjectBlob   =   "GraphicSample.frx":1147C
         TabIndex        =   20
         Top             =   240
         Width           =   2775
      End
   End
   Begin VB.CommandButton textProperties 
      Caption         =   "Text Properties"
      Height          =   495
      Left            =   12720
      TabIndex        =   15
      Top             =   10560
      Width           =   1335
   End
   Begin VB.CommandButton axisTextProperties 
      Caption         =   "Axis Text Properties"
      Height          =   495
      Left            =   11160
      TabIndex        =   12
      Top             =   10560
      Width           =   1335
   End
   Begin VB.CommandButton deleteText 
      Caption         =   "Delete Text"
      Height          =   495
      Left            =   8040
      TabIndex        =   8
      Top             =   11040
      Width           =   1215
   End
   Begin VB.CommandButton editText 
      Caption         =   "Edit Text"
      Height          =   495
      Left            =   6720
      TabIndex        =   11
      Top             =   11040
      Width           =   1215
   End
   Begin VB.CommandButton addTextII 
      Caption         =   "Add Text II"
      Height          =   375
      Left            =   5280
      TabIndex        =   10
      Top             =   11400
      Width           =   1335
   End
   Begin VB.CommandButton addTextI 
      Caption         =   "Add Text I"
      Height          =   375
      Left            =   5280
      TabIndex        =   9
      Top             =   10920
      Width           =   1335
   End
   Begin VB.CommandButton deleteFunction 
      Caption         =   "Delete Function"
      Height          =   495
      Left            =   8040
      TabIndex        =   7
      Top             =   9960
      Width           =   1215
   End
   Begin VB.CommandButton addFunctionII 
      Caption         =   "Add Function II"
      Height          =   375
      Left            =   5280
      TabIndex        =   6
      Top             =   10320
      Width           =   1335
   End
   Begin VB.CommandButton done 
      Caption         =   "Done"
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   9960
      Width           =   1215
   End
   Begin VB.CommandButton showFunctions 
      Caption         =   "Show Functions"
      Height          =   495
      Left            =   9360
      TabIndex        =   5
      Top             =   9960
      Width           =   1335
   End
   Begin VB.CommandButton editFunction 
      Caption         =   "Edit Function"
      Height          =   495
      Left            =   6720
      TabIndex        =   4
      Top             =   9960
      Width           =   1215
   End
   Begin VB.CommandButton addFunctionI 
      Caption         =   "Add Function I"
      Height          =   375
      Left            =   5280
      TabIndex        =   2
      Top             =   9840
      Width           =   1335
   End
   Begin VB.CommandButton plotProperties 
      Caption         =   "Plot properties"
      Height          =   495
      Left            =   12720
      TabIndex        =   1
      Top             =   9960
      Width           =   1335
   End
   Begin VB.CommandButton axisProperties 
      Caption         =   "X-Axis properties"
      Height          =   495
      Left            =   11160
      TabIndex        =   0
      Top             =   9960
      Width           =   1335
   End
   Begin MSComctlLib.TabStrip TabStrip 
      Height          =   9615
      Left            =   5400
      TabIndex        =   16
      Top             =   0
      Width           =   11895
      _ExtentX        =   20981
      _ExtentY        =   16960
      _Version        =   393216
      BeginProperty Tabs {1EFB6598-857C-11D1-B16A-00C0F0283628} 
         NumTabs         =   3
         BeginProperty Tab1 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Typical Graphics"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab2 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "Text Effects"
            ImageVarType    =   2
         EndProperty
         BeginProperty Tab3 {1EFB659A-857C-11D1-B16A-00C0F0283628} 
            Caption         =   "2-D Plots"
            ImageVarType    =   2
         EndProperty
      EndProperty
   End
End
Attribute VB_Name = "GraphicSample"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Public functionExpression As String

Public formLeft As GSystemProperty
Public formTop As GSystemProperty
Public formWidth As GSystemProperty
Public formHeight As GSystemProperty

Public graphicTitle As Object
Public xyCoordinatePlaneText As Object
Public yzCoordinatePlaneText As Object
Public xzCoordinatePlaneText As Object
Public flippedLRxzCoordinatePlaneText As Object
Public directionalText As Object
Public currentTabIndex As Integer

Private Sub Form_Load()
Dim strPosition(2) As String

Move 0, 0

GS2DFunction.DefineIndependentVariable "x", "-3*pi", "3*pi", 3
GS3DFunction.DefineIndependentVariable "x", "-3*pi", "3*pi", 40
GS3DFunction.DefineIndependentVariable "y", "-3*pi", "3*pi", 40

With GSystemProperties
    Set formLeft = .Add
    Set formTop = .Add
    Set formWidth = .Add
    Set formHeight = .Add

    .AddObject GSystemGraphic(0).object
    .AddObject GSystemGraphic(1).object
    .AddObject GSystemGraphic(2).object
    .AddObject GS2DFunction.object
    .AddObject GS3DFunction.object
    .AddObject TabStrip.object

    .FileName = "sampleGraphic"
    If Dir(.FileName) <> "" Then
        .LoadFile
        Move formLeft, formTop
        Height = formHeight
        Width = formWidth
    End If

End With

showFunctions.Caption = "Show functions"
If GSystemGraphic(0).showFunctions Then
    showFunctions.Caption = "Hide Functions"
End If

Set graphicTitle = GSystemGraphic(0).Text("Graphic Title" + Chr(13) + Chr(10) + "On two lines")
If graphicTitle Is Nothing Then

    Set graphicTitle = GSystemGraphic(0).AddText("Graphic Title" + Chr(13) + Chr(10) + "On two lines")

    strPosition(0) = "minx + (maxX - minX)/2"
    strPosition(1) = "maxY"
    strPosition(2) = "0.0"

    graphicTitle.Position = strPosition
    graphicTitle.CoordinatePlane = 8
    graphicTitle.Format = 20

End If

Set directionalText = GSystemGraphic(1).Text("Directional Text")
If directionalText Is Nothing Then
    Set directionalText = GSystemGraphic(1).AddText("Directional Text")
    Dim direction(2) As Double
    Dim color(2) As Double
    direction(0) = 1
    direction(1) = 0
    direction(2) = 1
    color(0) = 1
    color(1) = 0
    color(2) = 0
    directionalText.directionForward = direction
    directionalText.Size = 20
    directionalText.color = color
    direction(0) = -1
    direction(1) = 0
    direction(2) = 1
    directionalText.directionUp = direction
    directionalText.CoordinatePlane = CoordinatePlane_XZ
End If

Set xyCoordinatePlaneText = GSystemGraphic(1).Text("XY Coordinate Plane")
If xyCoordinatePlaneText Is Nothing Then
    Set xyCoordinatePlaneText = GSystemGraphic(1).AddText("XY Coordinate Plane")
    xyCoordinatePlaneText.CoordinatePlane = CoordinatePlane_XY
    xyCoordinatePlaneText.positionX = 0.5
    xyCoordinatePlaneText.positionY = 0.5
    xyCoordinatePlaneText.positionZ = 0#
    xyCoordinatePlaneText.Format = TEXT_FORMAT_CENTER
End If

Set yzCoordinatePlaneText = GSystemGraphic(1).Text("YZ Coordinate Plane")
If yzCoordinatePlaneText Is Nothing Then
    Set yzCoordinatePlaneText = GSystemGraphic(1).AddText("YZ Coordinate Plane")
    yzCoordinatePlaneText.CoordinatePlane = CoordinatePlane_YZ
    yzCoordinatePlaneText.positionX = 0#
    yzCoordinatePlaneText.positionY = 0.5
    yzCoordinatePlaneText.positionZ = 0.75
    yzCoordinatePlaneText.Format = TEXT_FORMAT_CENTER
End If

Set xzCoordinatePlaneText = GSystemGraphic(1).Text("XZ Coordinate Plane")
If xzCoordinatePlaneText Is Nothing Then
    Set xzCoordinatePlaneText = GSystemGraphic(1).AddText("XZ Coordinate Plane")
    xzCoordinatePlaneText.CoordinatePlane = CoordinatePlane_XZ
    xzCoordinatePlaneText.positionX = 0.5
    xzCoordinatePlaneText.positionY = 0#
    xzCoordinatePlaneText.positionZ = 0.5
    xzCoordinatePlaneText.Format = TEXT_FORMAT_CENTER
    Set flippedLRxzCoordinatePlaneText = GSystemGraphic(1).AddText("XZ Coordinate Plane (flipped LR)")
    flippedLRxzCoordinatePlaneText.CoordinatePlane = CoordinatePlane_XZ
    flippedLRxzCoordinatePlaneText.flipHorizontal = True
    flippedLRxzCoordinatePlaneText.positionX = 0.5
    flippedLRxzCoordinatePlaneText.positionY = 0#
    flippedLRxzCoordinatePlaneText.positionZ = 0.25
    flippedLRxzCoordinatePlaneText.Format = TEXT_FORMAT_CENTER
End If

Frames(0).Visible = True
Frames(1).Visible = False
Frames(2).Visible = False

currentTabIndex = 0

End Sub


Private Sub Form_Resize()

done.Top = Height - done.Height - 1536
addFunctionI.Top = Height - editFunction.Height - 1536 - 0.5 * addFunctionI.Height
addFunctionII.Top = addFunctionI.Top + addFunctionI.Height
editFunction.Top = Height - editFunction.Height - 1536
deleteFunction.Top = Height - deleteFunction.Height - 1536
showFunctions.Top = Height - showFunctions.Height - 1536
axisProperties.Top = Height - axisProperties.Height - 1536
axisTextProperties.Top = Height - axisTextProperties.Height - 768
plotProperties.Top = Height - plotProperties.Height - 1536
textProperties.Top = Height - textProperties.Height - 768
addTextI.Top = Height - addTextI.Height - 768 - 0.5 * addTextI.Height
addTextII.Top = addTextI.Top + addTextI.Height
editText.Top = Height - editText.Height - 768
deleteText.Top = Height - deleteText.Height - 768

TabStrip.Height = editFunction.Top - TabStrip.Top - 256
TabStrip.Width = Width - TabStrip.Left - 512

Frames(0).Left = TabStrip.Left + 64
Frames(0).Top = TabStrip.Top + 512
Frames(0).Width = TabStrip.Width - 128
Frames(0).Height = TabStrip.Height - 512 - 64

Dim f As Frame
For Each f In Frames
    f.Top = Frames(0).Top
    f.Left = Frames(0).Left
    f.Width = Frames(0).Width
    f.Height = Frames(0).Height
Next f

GSystemGraphic(0).Left = 0
GSystemGraphic(0).Top = 0
GSystemGraphic(0).Width = Frames(0).Width
GSystemGraphic(0).Height = Frames(0).Height

GSystemGraphic(1).Left = 0
GSystemGraphic(1).Top = 0
GSystemGraphic(1).Width = Frames(1).Width
GSystemGraphic(1).Height = Frames(1).Height

GSystemGraphic(2).Left = 0
GSystemGraphic(2).Top = 0
GSystemGraphic(2).Width = Frames(2).Width
GSystemGraphic(2).Height = Frames(2).Height

End Sub


Private Sub done_Click()
Unload Me
End Sub


Private Sub Form_Unload(Cancel As Integer)
If Cancel <> 1 Then
    formTop = Top
    formLeft = Left
    formHeight = Height
    formWidth = Width
    GSystemProperties.Save
End If
End Sub


Private Sub GS3DFunction_Started(ByVal cntExpectedResults As Long)
GSystemGraphic(currentTabIndex).PrepareForData 0
End Sub


Private Sub GS2DFunction_Started(ByVal cntExpectedResults As Long)
GSystemGraphic(currentTabIndex).PrepareForData 0
End Sub


Public Sub GS3DFunction_Finished()
Dim lightPos(2) As String
lightPos(0) = "MinX + (MaxX - MinX)/2"
lightPos(1) = "MaxY"
lightPos(2) = "MinZ = (MaxZ - MinZ)/2"
GSystemGraphic(currentTabIndex).LightPosition(1) = lightPos
GSystemGraphic(currentTabIndex).LightOn(1) = True
GSystemGraphic(currentTabIndex).FinishedWithData 0
GSystemGraphic(currentTabIndex).Draw 0
End Sub


Private Sub GS2DFunction_Finished()
GSystemGraphic(currentTabIndex).FinishedWithData 0
GSystemGraphic(currentTabIndex).Draw 0
Dim plot As Object
Set plot = GSystemGraphic(currentTabIndex).plot(1)
plot.Name = "2D plot"
End Sub


Private Sub GS3DFunction_TakeResults( _
                    ByVal iterationNo As Long, _
                    ByVal bstrResults As String)
GSystemGraphic(currentTabIndex).TakeDataString bstrResults, 0
End Sub


Private Sub GS2DFunction_TakeResults(ByVal iterationNo As Long, ByVal bstrResults As String)
GSystemGraphic(currentTabIndex).TakeDataString bstrResults, 0
End Sub


Public Sub AddFunctionI_Click()
GSystemGraphic(currentTabIndex).GSystemGraphic(0).AddFunctionInteractive
End Sub


Private Sub addFunctionII_Click()
Dim f As New AddFunction
f.Show 1, Me
If Len(functionExpression) > 0 Then
    GSystemGraphic(currentTabIndex).GSystemGraphic(0).AddFunction functionExpression
End If
End Sub


Private Sub editFunction_Click()
Dim f As New editFunction
f.Show 1, Me
End Sub


Private Sub deleteFunction_Click()
Dim f As New deleteFunction
f.Show 1, Me
End Sub


Private Sub addTextI_Click()
GSystemGraphic(currentTabIndex).AddTextInteractive
End Sub


Private Sub addTextII_Click()
Dim f As New AddText
f.Show 1, Me
End Sub


Private Sub editText_Click()
Dim f As New editText
f.Show 1, Me
End Sub


Private Sub deleteText_Click()
Dim f As New deleteText
f.Show 1, Me
End Sub


Private Sub AxisProperties_Click()
Dim xAxis As Object
Set xAxis = GSystemGraphic(currentTabIndex).Axis("X")
xAxis.EditProperties
End Sub


Private Sub AxisTextProperties_Click()
Dim rText As Object
Set rText = GSystemGraphic(currentTabIndex).Axis("X").RepresentativeText
rText.EditProperties
End Sub



Private Sub PlotProperties_Click()
Dim plot As Object
Set plot = GSystemGraphic(currentTabIndex).plot(1)
If Not plot Is Nothing Then
    plot.EditProperties
Else
    MsgBox "There was no plot # 1 on the graphic"
End If
End Sub


Private Sub showFunctions_Click()
If GSystemGraphic(currentTabIndex).showFunctions Then
    showFunctions.Caption = "Show Functions"
    GSystemGraphic(currentTabIndex).showFunctions = False
Else
    showFunctions.Caption = "Hide Functions"
    GSystemGraphic(currentTabIndex).showFunctions = True
End If
End Sub



Private Sub TabStrip_Click()
Dim f As Frame
For Each f In Frames
    f.Visible = False
Next f
currentTabIndex = TabStrip.SelectedItem.Index - 1
Frames(currentTabIndex).Visible = True
End Sub


Private Sub textProperties_Click()
graphicTitle.EditProperties
End Sub
