Imports System.Data.SqlClient

Module Data

    Public conn As Connection
    Public CurrentMarketTable As List(Of MARKET_DATA)
    Public MarketName As String
    Public RowIndex As Int64

    Public Class Connection

        Private _SERVER_IP As String
        Private _USERNAME As String
        Private _PASSWORD As String
        Private _DATABASE As String
        Private SQL As SqlClient.SqlConnection

        Public Sub New(ByVal server_IP As String, _
                        ByVal username As String, _
                        ByVal password As String, _
                        ByVal database As String)

            _SERVER_IP = server_IP
            _USERNAME = username
            _PASSWORD = password
            _DATABASE = database
            SQL = New SqlConnection("Server=" & _SERVER_IP & ";User ID=" & _USERNAME & ";Password=" & _PASSWORD & ";Database=" & _DATABASE)
            Try
                SQL.Open()
                Me.Query("DELETE FROM dbo.MarketResource WHERE code NOT IN (SELECT id FROM dbo.ItemResource)")
            Catch ex As Exception
                MessageBox.Show("Keine Verbindung zum Server! Bitte Konfiguration und SQL-Remote-Login prüfen.")
            End Try

        End Sub

        Public Property ServerIP() As String
            Get
                Return _SERVER_IP
            End Get
            Set(ByVal value As String)
                _SERVER_IP = value
            End Set
        End Property

        Public Property Username() As String
            Get
                Return _USERNAME
            End Get
            Set(ByVal value As String)
                _USERNAME = value
            End Set
        End Property

        Public Property Password() As String
            Get
                Return _PASSWORD
            End Get
            Set(ByVal value As String)
                _PASSWORD = value
            End Set
        End Property

        Public Property Database() As String
            Get
                Return _DATABASE
            End Get
            Set(ByVal value As String)
                Try
                    SQL.ChangeDatabase(value)
                Catch ex As Exception
                End Try
                _DATABASE = value
            End Set
        End Property

        Public Function GetMarketsList() As List(Of String)
            Dim Query As String = "SELECT DISTINCT name FROM dbo.MarketResource ORDER BY name"
            Dim Command As New SqlCommand(Query, SQL)
            Dim rdr As SqlDataReader = Command.ExecuteReader
            Dim lst As New List(Of String)
            Dim i As Int64 = 0
            While rdr.Read
                If Not rdr.IsDBNull(rdr.GetOrdinal("name")) Then
                    lst.Add(rdr.GetSqlString(rdr.GetOrdinal("name")))
                End If
            End While
            rdr.Close()
            Return lst
        End Function

        Public Function GetMarketData(ByVal name) As List(Of MARKET_DATA)
            Dim Query As String = "SELECT a.sort_id as sort_id, " & _
                                    "a.name as name, " & _
                                    "a.code as code, " & _
                                    "c.value as item_name, " & _
                                    "b.price as price, " & _
                                    "b.huntaholic_point as huntaholic_point, " & _
                                    "b.arena_point as arena_point, " & _
                                    "a.price_ratio as price_ratio, " & _
                                    "a.huntaholic_ratio as huntaholic_ratio, " & _
                                    "a.arena_ratio as arena_ratio " & _
                                    "FROM dbo.Marketresource a " & _
                                    "LEFT JOIN dbo.ItemResource b " & _
                                    "ON a.code = b.id " & _
                                    "LEFT JOIN dbo.StringResource c " & _
                                    "ON b.name_id = c.code " & _
                                    "WHERE a.name = '" & name & "' " & _
                                    "ORDER BY a.sort_id"

            Dim Command As New SqlCommand(Query, SQL)
            Dim rdr As SqlDataReader = Command.ExecuteReader
            Dim lst As New List(Of MARKET_DATA)
            While rdr.Read
                If Not rdr.IsDBNull(rdr.GetOrdinal("sort_id")) Then
                    lst.Add(New MARKET_DATA(rdr.GetValue(rdr.GetOrdinal("sort_id")), _
                                            rdr.GetValue(rdr.GetOrdinal("name")), _
                                            rdr.GetValue(rdr.GetOrdinal("code")), _
                                            rdr.GetValue(rdr.GetOrdinal("item_name")), _
                                            rdr.GetValue(rdr.GetOrdinal("price")), _
                                            rdr.GetValue(rdr.GetOrdinal("huntaholic_point")), _
                                            rdr.GetValue(rdr.GetOrdinal("arena_point")), _
                                            rdr.GetValue(rdr.GetOrdinal("price_ratio")), _
                                            rdr.GetValue(rdr.GetOrdinal("huntaholic_ratio")), _
                                            rdr.GetValue(rdr.GetOrdinal("arena_ratio"))))
                End If
            End While
            rdr.Close()
            Return lst
        End Function

        Public Function GetSearchResult(ByVal search_query As String, ByVal search_type As Byte) As List(Of SEARCH_DATA)

            Dim Query As String
            If search_type = 1 Then
                Query = "SELECT a.id as item_code, b.code as string_code, b.value as string_value FROM dbo.ItemResource a LEFT JOIN dbo.StringResource b ON a.name_id = b.code WHERE b.value LIKE '%" & search_query & "%' ORDER BY a.id"
            Else
                Query = "SELECT a.id as item_code, b.code as string_code, b.value as string_value FROM dbo.ItemResource a LEFT JOIN dbo.StringResource b ON a.name_id = b.code WHERE b.value = '" & search_query & "' ORDER BY a.id"
            End If

            Dim Command As New SqlCommand(Query, SQL)
            Dim rdr As SqlDataReader = Command.ExecuteReader
            Dim lst As New List(Of SEARCH_DATA)
            While rdr.Read
                If Not rdr.IsDBNull(rdr.GetOrdinal("item_code")) Then
                    lst.Add(New SEARCH_DATA(rdr.GetValue(rdr.GetOrdinal("item_code")), rdr.GetValue(rdr.GetOrdinal("string_code")), rdr.GetValue(rdr.GetOrdinal("string_value"))))
                End If
            End While
            rdr.Close()
            Return lst

        End Function

        Public Function Query(ByVal query_string As String) As String
            If Not SQL.State = ConnectionState.Open Then
                Try
                    SQL.Open()
                Catch ex As Exception
                End Try
            End If
            Dim Command As New SqlCommand(query_string, SQL)
            Dim rdr As SqlDataReader
            Try
                rdr = Command.ExecuteReader
            Catch ex As Exception
                MessageBox.Show("SQL-Fehler bei Abfrage:" & vbNewLine & query_string & vbNewLine & vbNewLine & ex.Message)
            End Try
            Try
                rdr.Read()
                Dim value As String = rdr.GetValue(0)
                rdr.Close()
                Return value
            Catch ex As Exception
                Try
                    rdr.Close()
                Catch ex2 As Exception
                End Try
                Return ""
            End Try
        End Function

    End Class

    Public Class MARKET_DATA

        Public Sub New(ByVal sort_id As Int64, _
                        ByVal name As String, _
                        ByVal code As Int64, _
                        ByVal item_name As String, _
                        ByVal price As Int64, _
                        ByVal huntaholic_point As Int64, _
                        ByVal arena_point As Int64, _
                        ByVal price_ratio As Decimal, _
                        ByVal huntaholic_ratio As Decimal, _
                        ByVal arena_ratio As Decimal)

            _SORT_ID = sort_id
            _NAME = name
            _CODE = code
            _ITEM_NAME = item_name
            _PRICE = price
            _HUNTAHOLIC_POINT = huntaholic_point
            _ARENA_POINT = arena_point
            _PRICE_RATIO = price_ratio
            _HUNTAHOLIC_RATIO = huntaholic_ratio
            _ARENA_RATIO = arena_ratio

        End Sub

        Private _SORT_ID As Int64
        Private _NAME As String
        Private _CODE As Int64
        Private _ITEM_NAME As String
        Private _PRICE As Int64
        Private _HUNTAHOLIC_POINT As Int64
        Private _ARENA_POINT As Int64
        Private _PRICE_RATIO As Decimal
        Private _HUNTAHOLIC_RATIO As Decimal
        Private _ARENA_RATIO As Decimal

        Public Property SortID() As Int64
            Get
                Return _SORT_ID
            End Get
            Set(ByVal value As Int64)
                _SORT_ID = value
            End Set
        End Property

        Public ReadOnly Property Name() As String
            Get
                Return _NAME
            End Get
        End Property

        Public Property Code() As Int64
            Get
                Return _CODE
            End Get
            Set(ByVal value As Int64)
                _CODE = value
                _ITEM_NAME = conn.Query("SELECT value FROM dbo.StringResource WHERE code = (SELECT name_id FROM dbo.ItemResource WHERE id = " & value & ")")
                _PRICE = conn.Query("SELECT price FROM dbo.ItemResource WHERE id = " & value)
                _HUNTAHOLIC_POINT = conn.Query("SELECT huntaholic_point FROM dbo.ItemResource WHERE id = " & value)
                _ARENA_POINT = conn.Query("SELECT arena_point FROM dbo.ItemResource WHERE id = " & value)
                RefreshGrid()
            End Set
        End Property

        Public ReadOnly Property ItemName() As String
            Get
                Return _ITEM_NAME
            End Get
        End Property

        Public Property Price() As Int64
            Get
                Return _PRICE
            End Get
            Set(ByVal value As Int64)
                _PRICE = value
            End Set
        End Property

        Public Property HuntaholicPoint() As Int64
            Get
                Return _HUNTAHOLIC_POINT
            End Get
            Set(ByVal value As Int64)
                _HUNTAHOLIC_POINT = value
            End Set
        End Property

        Public Property ArenaPoint() As Int64
            Get
                Return _ARENA_POINT
            End Get
            Set(ByVal value As Int64)
                _ARENA_POINT = value
            End Set
        End Property

        Public Property PriceRatio() As Decimal
            Get
                Return _PRICE_RATIO
            End Get
            Set(ByVal value As Decimal)
                _PRICE_RATIO = value
            End Set
        End Property

        Public Property HuntaholicRatio() As Decimal
            Get
                Return _HUNTAHOLIC_RATIO
            End Get
            Set(ByVal value As Decimal)
                _HUNTAHOLIC_RATIO = value
            End Set
        End Property

        Public Property ArenaRatio() As Decimal
            Get
                Return _ARENA_RATIO
            End Get
            Set(ByVal value As Decimal)
                _ARENA_RATIO = value
            End Set
        End Property


    End Class

    Public Class SEARCH_DATA

        Private _ITEM_CODE As Int64
        Private _STRING_CODE As Int64
        Private _STRING_VALUE As String

        Public Sub New(ByVal item_code As Int64, ByVal string_code As Int64, ByVal string_value As String)

            _ITEM_CODE = item_code
            _STRING_CODE = string_code
            _STRING_VALUE = string_value

        End Sub

        Public ReadOnly Property ItemCode() As Int64
            Get
                Return _ITEM_CODE
            End Get
        End Property

        Public ReadOnly Property StringCode() As Int64
            Get
                Return _STRING_CODE
            End Get
        End Property

        Public ReadOnly Property StringValue() As String
            Get
                Return _STRING_VALUE
            End Get
        End Property


    End Class

    Public Sub ReloadStartupWindow()
        Dim server As String = ""
        Dim user As String = ""
        Dim password As String = ""
        Dim database As String = ""

        For Each s As String In IO.File.ReadAllLines("config")
            Dim s1 As String = Split(s, "=")(0)
            Dim s2 As String = Split(s, "=")(1)
            Select Case s1
                Case "Server"
                    server = s2
                Case "User"
                    user = s2
                Case "Password"
                    password = s2
                Case "Database"
                    database = s2
            End Select
        Next

        Try
            conn = New Connection(server, user, password, database)
        Catch ex As Exception
            MessageBox.Show("Keine Verbindung zur Datenbank! Bitte Konfiguration prüfen.")
            MainForm.DataGridView1.DataSource = New List(Of MARKET_DATA)
            MainForm.DataGridView1.Refresh()
            Exit Sub
        End Try

        Dim lst As List(Of String) = conn.GetMarketsList
        MainForm.ComboBox1.Items.Clear()
        For Each s As String In lst
            MainForm.ComboBox1.Items.Add(s)
        Next
        CurrentMarketTable = conn.GetMarketData(MarketName)
        MainForm.DataGridView1.DataSource = CurrentMarketTable
        MainForm.DataGridView1.Refresh()

    End Sub

    Public Sub SaveMarket()
        ReindexMarketTable()
        MainForm.ProgressBar1.Maximum = CurrentMarketTable.Count
        Dim i As Int64 = 1
        conn.Query("DELETE FROM dbo.MarketResource WHERE name='" & MarketName & "'")
        Dim query As String = ""
        For Each m As MARKET_DATA In CurrentMarketTable
            Try
                query = "INSERT INTO dbo.MarketResource VALUES (" & m.SortID & ", '" & m.Name & "', " & m.Code & ", " & Convert.ToString(m.PriceRatio).Replace(",", ".") & ", " & Format(m.HuntaholicRatio, "0.####") & ", " & Format(m.ArenaRatio, "0.####") & ")"
            Catch ex As Exception
            End Try
            conn.Query(query)
            query = "UPDATE dbo.ItemResource SET price = " & m.Price & ", huntaholic_point = " & m.HuntaholicPoint & ", arena_point = " & m.ArenaPoint & " WHERE id = " & m.Code
            conn.Query(query)
            MainForm.ProgressBar1.Value = i
            i += 1
        Next
        MessageBox.Show("Saved!")
    End Sub

    Public Sub RefreshGrid()
        MainForm.DataGridView1.DataSource = New List(Of MARKET_DATA)
        MainForm.DataGridView1.DataSource = CurrentMarketTable.GetRange(0, CurrentMarketTable.Count) 'Dont change just to CurrentMarketTable - weird .NET shit with that datagrid you know...
        Try
            MainForm.DataGridView1.FirstDisplayedScrollingRowIndex = RowIndex
        Catch ex As Exception
        End Try
        MainForm.DataGridView1.Refresh()
    End Sub

    Public Sub ReindexMarketTable()
        If CurrentMarketTable Is Nothing Then Exit Sub
        For idx As Integer = 0 To CurrentMarketTable.Count - 1
            CurrentMarketTable(idx).SortID = idx
        Next
    End Sub

End Module
