Imports System.Drawing
Imports System.Windows.Forms

Public Enum AppTheme
    Light
    Dark
End Enum

Public Class ThemePalette
    Public Background As Color
    Public Surface As Color
    Public Panel As Color
    Public Border As Color
    Public TextPrimary As Color
    Public TextSecondary As Color
    Public Accent As Color
    Public AccentText As Color
    Public Neutral As Color
    Public NeutralHover As Color
    Public PrimaryHover As Color
    Public PrimaryActive As Color
    Public Success As Color
    Public Danger As Color
    Public HeaderBack As Color
    Public HeaderText As Color
    Public GridLine As Color
    Public AltRow As Color
    Public Selection As Color
    Public SelectionText As Color
    Public InputBack As Color
End Class

Public Module ThemeManager
    Private themeLoaded As Boolean = False
    Public CurrentTheme As AppTheme = AppTheme.Dark
    Private LightPalette As ThemePalette
    Private DarkPalette As ThemePalette

    Private ReadOnly themeFile As String = "theme"

    Public Sub LoadTheme()
        If themeLoaded Then Return
        InitPalettes()
        Dim fileValue As String = ""
        Try
            If IO.File.Exists(themeFile) Then
                fileValue = IO.File.ReadAllText(themeFile).Trim().ToLowerInvariant()
            End If
        Catch ex As Exception
        End Try

        If fileValue = "dark" Then
            CurrentTheme = AppTheme.Dark
        Else
            CurrentTheme = AppTheme.Light
        End If
        themeLoaded = True
    End Sub

    Public Sub SetTheme(ByVal theme As AppTheme)
        CurrentTheme = theme
        Try
            IO.File.WriteAllText(themeFile, If(theme = AppTheme.Dark, "Dark", "Light"))
        Catch ex As Exception
        End Try
        For Each f As Form In Application.OpenForms
            ApplyTheme(f)
        Next
    End Sub

    Public Sub ToggleTheme()
        If CurrentTheme = AppTheme.Light Then
            SetTheme(AppTheme.Dark)
        Else
            SetTheme(AppTheme.Light)
        End If
    End Sub

    Public Sub ApplyTheme(ByVal form As Form)
        If Not themeLoaded Then LoadTheme()
        Dim p As ThemePalette = GetPalette()

        form.BackColor = p.Background
        form.ForeColor = p.TextPrimary

        For Each ctrl As Control In form.Controls
            ApplyControlTheme(ctrl, p)
        Next
    End Sub

    Private Sub ApplyControlTheme(ByVal ctrl As Control, ByVal p As ThemePalette)
        If TypeOf ctrl Is Panel Then
            ctrl.BackColor = p.Panel
            ctrl.ForeColor = p.TextPrimary
        ElseIf TypeOf ctrl Is Label Then
            ctrl.ForeColor = p.TextPrimary
            ctrl.BackColor = Color.Transparent
        ElseIf TypeOf ctrl Is Button Then
            StyleButton(CType(ctrl, Button), p)
        ElseIf TypeOf ctrl Is ComboBox Then
            Dim c As ComboBox = CType(ctrl, ComboBox)
            c.BackColor = p.InputBack
            c.ForeColor = p.TextPrimary
            c.FlatStyle = FlatStyle.Standard
        ElseIf TypeOf ctrl Is TextBox Then
            Dim t As TextBox = CType(ctrl, TextBox)
            t.BackColor = p.InputBack
            t.ForeColor = p.TextPrimary
            t.BorderStyle = BorderStyle.FixedSingle
        ElseIf TypeOf ctrl Is ProgressBar Then
            ctrl.BackColor = p.Panel
            ctrl.ForeColor = p.Accent
        ElseIf TypeOf ctrl Is DataGridView Then
            StyleGrid(CType(ctrl, DataGridView), p)
        End If

        If ctrl.HasChildren Then
            For Each child As Control In ctrl.Controls
                ApplyControlTheme(child, p)
            Next
        End If
    End Sub

    Private Sub StyleButton(ByVal btn As Button, ByVal p As ThemePalette)
        btn.FlatStyle = FlatStyle.Flat
        btn.FlatAppearance.BorderSize = 0
        Dim back As Color = btn.BackColor

        ' Map existing colors to theme roles
        If back = Color.FromArgb(37, 99, 235) Then
            btn.BackColor = p.Accent
            btn.ForeColor = p.AccentText
            btn.FlatAppearance.MouseOverBackColor = p.PrimaryHover
            btn.FlatAppearance.MouseDownBackColor = p.PrimaryActive
        ElseIf back = Color.FromArgb(34, 197, 94) Then
            btn.BackColor = p.Success
            btn.ForeColor = p.AccentText
            btn.FlatAppearance.MouseOverBackColor = p.PrimaryHover
            btn.FlatAppearance.MouseDownBackColor = p.PrimaryActive
        ElseIf back = Color.FromArgb(239, 68, 68) Then
            btn.BackColor = p.Danger
            btn.ForeColor = p.AccentText
            btn.FlatAppearance.MouseOverBackColor = p.PrimaryHover
            btn.FlatAppearance.MouseDownBackColor = p.PrimaryActive
        Else
            btn.BackColor = p.Neutral
            btn.ForeColor = p.TextPrimary
            btn.FlatAppearance.MouseOverBackColor = p.NeutralHover
            btn.FlatAppearance.MouseDownBackColor = p.PrimaryActive
        End If
    End Sub

    Private Sub StyleGrid(ByVal grid As DataGridView, ByVal p As ThemePalette)
        grid.BackgroundColor = p.Surface
        grid.BorderStyle = BorderStyle.None
        grid.EnableHeadersVisualStyles = False
        grid.GridColor = p.GridLine
        grid.ColumnHeadersBorderStyle = DataGridViewHeaderBorderStyle.None

        Dim headerStyle As New DataGridViewCellStyle()
        headerStyle.BackColor = p.HeaderBack
        headerStyle.ForeColor = p.HeaderText
        headerStyle.Font = New Font("Segoe UI Semibold", grid.Font.Size, FontStyle.Bold)
        headerStyle.SelectionBackColor = p.HeaderBack
        headerStyle.SelectionForeColor = p.HeaderText
        headerStyle.Alignment = DataGridViewContentAlignment.MiddleLeft
        headerStyle.WrapMode = DataGridViewTriState.[True]
        grid.ColumnHeadersDefaultCellStyle = headerStyle

        Dim altStyle As New DataGridViewCellStyle()
        altStyle.BackColor = p.AltRow
        altStyle.SelectionBackColor = p.Selection
        altStyle.SelectionForeColor = p.SelectionText
        grid.AlternatingRowsDefaultCellStyle = altStyle

        Dim cellStyle As New DataGridViewCellStyle()
        cellStyle.BackColor = p.Surface
        cellStyle.ForeColor = p.TextPrimary
        cellStyle.SelectionBackColor = p.Selection
        cellStyle.SelectionForeColor = p.SelectionText
        cellStyle.WrapMode = DataGridViewTriState.[False]
        grid.DefaultCellStyle = cellStyle
    End Sub

    Private Function GetPalette() As ThemePalette
        If CurrentTheme = AppTheme.Dark Then
            Return DarkPalette
        End If
        Return LightPalette
    End Function

    Private Sub InitPalettes()
        ' Rappelz-inspirierte Paletten (Gold/Blau)
        LightPalette = New ThemePalette() With {
            .Background = Color.FromArgb(244, 244, 240),
            .Surface = Color.FromArgb(250, 250, 247),
            .Panel = Color.FromArgb(234, 233, 227),
            .Border = Color.FromArgb(210, 204, 190),
            .TextPrimary = Color.FromArgb(42, 38, 32),
            .TextSecondary = Color.FromArgb(94, 87, 76),
            .Accent = Color.FromArgb(34, 102, 180),
            .AccentText = Color.White,
            .Neutral = Color.FromArgb(224, 221, 213),
            .NeutralHover = Color.FromArgb(214, 210, 202),
            .PrimaryHover = Color.FromArgb(54, 126, 205),
            .PrimaryActive = Color.FromArgb(28, 86, 150),
            .Success = Color.FromArgb(52, 168, 83),
            .Danger = Color.FromArgb(203, 67, 53),
            .HeaderBack = Color.FromArgb(229, 223, 210),
            .HeaderText = Color.FromArgb(42, 38, 32),
            .GridLine = Color.FromArgb(214, 210, 202),
            .AltRow = Color.FromArgb(239, 237, 232),
            .Selection = Color.FromArgb(220, 210, 185),
            .SelectionText = Color.FromArgb(42, 38, 32),
            .InputBack = Color.White
        }

        DarkPalette = New ThemePalette() With {
            .Background = Color.FromArgb(18, 20, 28),
            .Surface = Color.FromArgb(26, 28, 38),
            .Panel = Color.FromArgb(24, 26, 36),
            .Border = Color.FromArgb(58, 58, 70),
            .TextPrimary = Color.FromArgb(230, 224, 210),
            .TextSecondary = Color.FromArgb(171, 164, 150),
            .Accent = Color.FromArgb(226, 173, 68),
            .AccentText = Color.FromArgb(24, 24, 24),
            .Neutral = Color.FromArgb(48, 52, 64),
            .NeutralHover = Color.FromArgb(63, 68, 82),
            .PrimaryHover = Color.FromArgb(239, 193, 98),
            .PrimaryActive = Color.FromArgb(203, 152, 52),
            .Success = Color.FromArgb(70, 181, 126),
            .Danger = Color.FromArgb(229, 80, 70),
            .HeaderBack = Color.FromArgb(30, 33, 45),
            .HeaderText = Color.FromArgb(230, 224, 210),
            .GridLine = Color.FromArgb(65, 70, 84),
            .AltRow = Color.FromArgb(32, 36, 48),
            .Selection = Color.FromArgb(61, 52, 36),
            .SelectionText = Color.FromArgb(236, 230, 215),
            .InputBack = Color.FromArgb(35, 38, 50)
        }
    End Sub
End Module

