Imports System.Text

Public Class SearchForm

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Label2.Text = "Suche..."
        Label2.Refresh()
        If RadioButton1.Checked = True Then
            DataGridView1.DataSource = conn.GetSearchResult(TextBox1.Text, 1)
        Else
            DataGridView1.DataSource = conn.GetSearchResult(TextBox1.Text, 2)
        End If
        DataGridView1.Refresh()
        Label2.Text = "Gefunden: " & DataGridView1.RowCount & " Treffer"
    End Sub
End Class