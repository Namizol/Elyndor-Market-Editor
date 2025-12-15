Public Class MainForm

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        ConfigForm.Show()
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        SearchForm.Show()
    End Sub

    Private Sub MainForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        RowIndex = 0
        Try
            Me.Icon = Icon.ExtractAssociatedIcon(Application.ExecutablePath)
        Catch
        End Try
        ThemeManager.LoadTheme()
        CheckBoxTheme.Checked = (ThemeManager.CurrentTheme = AppTheme.Dark)
        UpdateThemeToggleText()
        ThemeManager.ApplyTheme(Me)
        If IO.File.Exists("config") Then
            ReloadStartupWindow()
        Else
            MessageBox.Show("Bitte konfigurieren Sie die Datenbank, bevor Tabellen geladen werden!")
        End If
    End Sub

    Private Sub ComboBox1_SelectedValueChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedValueChanged
        MarketName = ComboBox1.Items(ComboBox1.SelectedIndex)
        CurrentMarketTable = conn.GetMarketData(MarketName)
        RefreshGrid()
        DataGridView1.Select()
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        SaveMarket()
    End Sub

    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
        Dim name As String = InputBox("Bitte Markt-Namen eingeben", "Neuer Markt")
        If name.Length > 1 Then
            MarketName = name
            CurrentMarketTable = conn.GetMarketData(name)
            If Not ComboBox1.Items.Contains(name) Then
                ComboBox1.Items.Add(name)
            End If
            ComboBox1.Text = name
        End If
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        CurrentMarketTable.Add(New MARKET_DATA(0, MarketName, 0, "[NEW ITEM]", 0, 0, 0, 0, 0, 0))
        ReindexMarketTable()
        RowIndex = CurrentMarketTable.Count - 1
        RefreshGrid()
        UpdateLog()
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        If DataGridView1.CurrentCell Is Nothing Then
            Exit Sub
        End If
        Dim idx As Integer = DataGridView1.CurrentCell.RowIndex
        If idx < 0 OrElse idx >= CurrentMarketTable.Count Then Exit Sub
        CurrentMarketTable.RemoveAt(idx)
        ReindexMarketTable()
        If RowIndex >= CurrentMarketTable.Count Then
            RowIndex -= 1
        End If
        RefreshGrid()
        UpdateLog()
    End Sub

    Private Sub DataGridView1_CellEnter(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewCellEventArgs) Handles DataGridView1.CellEnter
        Try
            RowIndex = DataGridView1.SelectedCells.Item(0).RowIndex
        Catch ex As Exception
        End Try
        UpdateLog()
    End Sub

    Private Sub DataGridView1_DataError(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewDataErrorEventArgs) Handles DataGridView1.DataError
        ' Nichts tun – DataGrid liefert teils unnötige Meldungen.
    End Sub

    Private Sub CheckBoxTheme_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBoxTheme.CheckedChanged
        If CheckBoxTheme.Checked Then
            ThemeManager.SetTheme(AppTheme.Dark)
        Else
            ThemeManager.SetTheme(AppTheme.Light)
        End If
        UpdateThemeToggleText()
    End Sub

    Private Sub UpdateThemeToggleText()
        CheckBoxTheme.Text = If(ThemeManager.CurrentTheme = AppTheme.Dark, "Dark", "Light")
    End Sub

    Private Sub UpdateLog()
        If CurrentMarketTable Is Nothing OrElse CurrentMarketTable.Count = 0 Then
            If TextBoxLog IsNot Nothing Then TextBoxLog.Text = ""
            Exit Sub
        End If
        If RowIndex < 0 OrElse RowIndex >= CurrentMarketTable.Count Then
            If TextBoxLog IsNot Nothing Then TextBoxLog.Text = ""
            Exit Sub
        End If

        Dim m As MARKET_DATA = CurrentMarketTable(RowIndex)

        Dim priceTotal As Decimal = 0
        Dim huntTotal As Decimal = 0
        Dim arenaTotal As Decimal = 0

        Try
            priceTotal = m.Price * m.PriceRatio
            huntTotal = m.HuntaholicPoint * m.HuntaholicRatio
            arenaTotal = m.ArenaPoint * m.ArenaRatio
        Catch ex As Exception
        End Try

        Dim priceStr As String = m.Price.ToString("N0")
        Dim priceRatioStr As String = m.PriceRatio.ToString("N0")
        Dim priceTotalStr As String = priceTotal.ToString("N0")

        Dim huntStr As String = m.HuntaholicPoint.ToString("N0")
        Dim huntRatioStr As String = m.HuntaholicRatio.ToString("N0")
        Dim huntTotalStr As String = huntTotal.ToString("N0")

        Dim arenaStr As String = m.ArenaPoint.ToString("N0")
        Dim arenaRatioStr As String = m.ArenaRatio.ToString("N0")
        Dim arenaTotalStr As String = arenaTotal.ToString("N0")

        Dim sb As New System.Text.StringBuilder()
        sb.AppendLine("Ingame Kosten (aktuelle Zeile):")
        sb.AppendLine("Preis (" & priceStr & ") x Ratio (" & priceRatioStr & ") = " & priceTotalStr)
        sb.AppendLine("Huntaholic (" & huntStr & ") x Ratio (" & huntRatioStr & ") = " & huntTotalStr)
        sb.AppendLine("Arena (" & arenaStr & ") x Ratio (" & arenaRatioStr & ") = " & arenaTotalStr)

        If TextBoxLog IsNot Nothing Then
            TextBoxLog.Text = sb.ToString()
        End If
    End Sub

    Private Sub LabelTitle_Click(sender As Object, e As EventArgs) Handles LabelTitle.Click

    End Sub
End Class
