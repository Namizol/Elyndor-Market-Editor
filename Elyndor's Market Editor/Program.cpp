// Entry point for the C++17 / C++/CLI WinForms application.
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String ^> ^)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    ElyndorMarketEditor::MainForm form;
    Application::Run(%form);
    return 0;
}

