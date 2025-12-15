// Configuration dialog for DB connection settings.
#pragma once

#include "Data.h"

#using <System.Drawing.dll>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace ElyndorMarketEditor
{
    public ref class ConfigForm : public Form
    {
    public:
        ConfigForm()
        {
            InitializeComponent();
        }

    protected:
        ~ConfigForm()
        {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container ^ components;
        TextBox ^ txtServer;
        TextBox ^ txtUser;
        TextBox ^ txtPassword;
        TextBox ^ txtDatabase;
        Button ^ btnSave;
        Label ^ lblServer;
        Label ^ lblUser;
        Label ^ lblPassword;
        Label ^ lblDatabase;

        void InitializeComponent()
        {
            this->components = gcnew System::ComponentModel::Container();
            this->txtServer = gcnew TextBox();
            this->txtUser = gcnew TextBox();
            this->txtPassword = gcnew TextBox();
            this->txtDatabase = gcnew TextBox();
            this->btnSave = gcnew Button();
            this->lblServer = gcnew Label();
            this->lblUser = gcnew Label();
            this->lblPassword = gcnew Label();
            this->lblDatabase = gcnew Label();

            // Labels
            this->lblServer->AutoSize = true;
            this->lblServer->Text = L"Server";
            this->lblServer->Location = System::Drawing::Point(12, 15);

            this->lblUser->AutoSize = true;
            this->lblUser->Text = L"User";
            this->lblUser->Location = System::Drawing::Point(12, 45);

            this->lblPassword->AutoSize = true;
            this->lblPassword->Text = L"Password";
            this->lblPassword->Location = System::Drawing::Point(12, 75);

            this->lblDatabase->AutoSize = true;
            this->lblDatabase->Text = L"Database";
            this->lblDatabase->Location = System::Drawing::Point(12, 105);

            // TextBoxes
            this->txtServer->Location = System::Drawing::Point(90, 12);
            this->txtServer->Width = 250;

            this->txtUser->Location = System::Drawing::Point(90, 42);
            this->txtUser->Width = 250;

            this->txtPassword->Location = System::Drawing::Point(90, 72);
            this->txtPassword->Width = 250;
            this->txtPassword->UseSystemPasswordChar = true;

            this->txtDatabase->Location = System::Drawing::Point(90, 102);
            this->txtDatabase->Width = 250;

            // Button
            this->btnSave->Location = System::Drawing::Point(90, 140);
            this->btnSave->Text = L"Save";
            this->btnSave->Click += gcnew EventHandler(this, &ConfigForm::BtnSave_Click);

            // Form
            this->ClientSize = System::Drawing::Size(360, 180);
            this->Controls->Add(this->lblServer);
            this->Controls->Add(this->lblUser);
            this->Controls->Add(this->lblPassword);
            this->Controls->Add(this->lblDatabase);
            this->Controls->Add(this->txtServer);
            this->Controls->Add(this->txtUser);
            this->Controls->Add(this->txtPassword);
            this->Controls->Add(this->txtDatabase);
            this->Controls->Add(this->btnSave);
            this->Text = L"Configuration";
            this->Load += gcnew EventHandler(this, &ConfigForm::ConfigForm_Load);
        }

        void ConfigForm_Load(Object ^ sender, EventArgs ^ e)
        {
            if (DataContext::Conn != nullptr)
            {
                txtServer->Text = DataContext::Conn->ServerIP;
                txtUser->Text = DataContext::Conn->Username;
                txtPassword->Text = DataContext::Conn->Password;
                txtDatabase->Text = DataContext::Conn->Database;
            }
        }

        void BtnSave_Click(Object ^ sender, EventArgs ^ e)
        {
            try
            {
                DataContext::SaveConfig(txtServer->Text, txtUser->Text, txtPassword->Text, txtDatabase->Text);
                DataContext::SetConnection(gcnew Connection(txtServer->Text, txtUser->Text, txtPassword->Text, txtDatabase->Text));
                MessageBox::Show("Configuration has been successfully saved!");
                this->Close();
            }
            catch (Exception ^ ex)
            {
                MessageBox::Show("There is an error while saving configuration: " + ex->Message);
            }
        }
    };
}

