Public Class ConfigForm

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Try
            SaveConfig(TextBox1.Text, TextBox2.Text, TextBox3.Text, TextBox4.Text)
            MessageBox.Show("Konfiguration wurde erfolgreich gespeichert!")
        Catch ex As Exception
            MessageBox.Show("Beim Speichern der Konfiguration ist ein Fehler aufgetreten: " & ex.Message)
        End Try
        conn = New Connection(TextBox1.Text, TextBox2.Text, TextBox3.Text, TextBox4.Text)
        Try
            ReloadStartupWindow()
        Catch ex As Exception
        End Try
        Me.Hide()
    End Sub

    Private Sub SaveConfig(ByVal server, ByVal user, ByVal password, ByVal database)
        IO.File.WriteAllText("config", "Server=" & server & Chr(13) & Chr(10) & "User=" & user & Chr(13) & Chr(10) & "Password=" & password & Chr(13) & Chr(10) & "Database=" & database)
    End Sub

    Private Sub ConfigForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ' conn kann beim ersten Start Nothing sein -> Felder leer lassen
        If conn IsNot Nothing Then
            TextBox1.Text = conn.ServerIP
            TextBox2.Text = conn.Username
            TextBox3.Text = conn.Password
            TextBox4.Text = conn.Database
        Else
            TextBox1.Text = ""
            TextBox2.Text = ""
            TextBox3.Text = ""
            TextBox4.Text = ""
        End If
        ThemeManager.ApplyTheme(Me)
    End Sub
End Class