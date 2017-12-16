<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
   Inherits System.Windows.Forms.Form

   'Form overrides dispose to clean up the component list.
   <System.Diagnostics.DebuggerNonUserCode()>
   Protected Overrides Sub Dispose(ByVal disposing As Boolean)
      Try
         If disposing AndAlso components IsNot Nothing Then
            components.Dispose()
         End If
      Finally
         MyBase.Dispose(disposing)
      End Try
   End Sub

   'Required by the Windows Form Designer
   Private components As System.ComponentModel.IContainer

   'NOTE: The following procedure is required by the Windows Form Designer
   'It can be modified using the Windows Form Designer.  
   'Do not modify it using the code editor.
   <System.Diagnostics.DebuggerStepThrough()>
   Private Sub InitializeComponent()
      Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
      Me.AxGSystemGraphic1 = New AxGraphic.AxGSystemGraphic()
      CType(Me.AxGSystemGraphic1, System.ComponentModel.ISupportInitialize).BeginInit()
      Me.SuspendLayout()
      '
      'AxGSystemGraphic1
      '
      Me.AxGSystemGraphic1.Enabled = True
      Me.AxGSystemGraphic1.Location = New System.Drawing.Point(48, 37)
      Me.AxGSystemGraphic1.Name = "AxGSystemGraphic1"
      Me.AxGSystemGraphic1.OcxState = CType(resources.GetObject("AxGSystemGraphic1.OcxState"), System.Windows.Forms.AxHost.State)
      Me.AxGSystemGraphic1.Size = New System.Drawing.Size(1102, 569)
      Me.AxGSystemGraphic1.TabIndex = 0
      '
      'Form1
      '
      Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
      Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
      Me.ClientSize = New System.Drawing.Size(1233, 633)
      Me.Controls.Add(Me.AxGSystemGraphic1)
      Me.Name = "Form1"
      Me.Text = "Form1"
      CType(Me.AxGSystemGraphic1, System.ComponentModel.ISupportInitialize).EndInit()
      Me.ResumeLayout(False)

   End Sub

   Friend WithEvents AxGSystemGraphic1 As AxGraphic.AxGSystemGraphic
End Class
