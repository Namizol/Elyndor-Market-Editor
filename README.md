# Elyndor Market Editor

Editor für Elyndor-Marktplätze (VB.NET, .NET Framework 4.8) plus C++/CLI-UI-Prototyp. Lädt/editiert/speichert Markteinträge aus SQL Server.

## Features
- Märkte laden/wechseln, Einträge hinzufügen/löschen, SortID automatisch fortlaufend.
- Preise, Huntaholic- und Arena-Werte inkl. Ratios bearbeiten; Ingame-Kosten-Log pro Zeile.
- Suche nach Items (Teilstring oder exakt).
- DB-Konfiguration per Dialog, Speicherung in `config`.

## Voraussetzungen
- Windows, .NET Framework 4.8
- SQL Server mit Tabellen `MarketResource`, `ItemResource`, `StringResource`
- Visual Studio 2022; für C++/CLI: Workload „Desktop development with C++“, MSVC v143 Toolset.

## Projekte
- `Elyndor Market Editor.vbproj` – VB.NET WinForms (Hauptanwendung).
- `ElyndorMarketEditorCpp.vcxproj` – C++/CLI WinForms-Prototyp.

## Start
1) `config` im Programmverzeichnis anlegen oder über „Einstellungen“ füllen:
   ```
   Server=<server>
   User=<user>
   Password=<password>
   Database=<database>
   ```
2) Lösung `Elyndor Market Editor.sln` in Visual Studio öffnen.
3) Konfiguration auf `Debug|x64` (oder benötigte Plattform) stellen.
4) Build & Start.

## Bedienung (VB)
- Markt wählen (Dropdown) → Daten laden.
- „Zeile hinzufügen“ / „Ausgewählte Zeile löschen“ → SortID wird automatisch neu nummeriert.
- „Speichern“ schreibt Markt- und Item-Werte zurück in die DB.
- Log oben zeigt berechnete Ingame-Kosten (Preis/Hunt/Arena jeweils Wert x Ratio, mit Tausendertrennzeichen).
- „Artikel suchen“ öffnet Suchdialog (enthält/exakt).
- „Einstellungen“: DB-Zugangsdaten setzen/speichern.

## Hinweise
- Copy/Paste im Grid kopiert nur Zellen (keine ganzen Zeilen).
- C++/CLI-Projekt kann entladen werden, wenn nicht benötigt, um Toolset-Warnungen zu vermeiden.

## Lizenz
Siehe `LICENSE.txt`.
