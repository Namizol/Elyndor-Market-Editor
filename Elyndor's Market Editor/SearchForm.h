// Search dialog for items.
#pragma once

#include "Data.h"

#using <System.Drawing.dll>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace ElyndorMarketEditor
{
    public ref class SearchForm : public Form
    {
    public:
        SearchForm()
        {
            InitializeComponent();
        }

    protected:
        ~SearchForm()
        {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container ^ components;
        TextBox ^ txtQuery;
        RadioButton ^ rbPartial;
        RadioButton ^ rbExact;
        Button ^ btnSearch;
        DataGridView ^ dataGridView1;
        Label ^ lblStatus;

        void InitializeComponent()
        {
            this->components = gcnew System::ComponentModel::Container();
            this->txtQuery = gcnew TextBox();
            this->rbPartial = gcnew RadioButton();
            this->rbExact = gcnew RadioButton();
            this->btnSearch = gcnew Button();
            this->dataGridView1 = gcnew DataGridView();
            this->lblStatus = gcnew Label();

            // txtQuery
            this->txtQuery->Location = System::Drawing::Point(12, 12);
            this->txtQuery->Width = 260;

            // radio buttons
            this->rbPartial->Location = System::Drawing::Point(12, 40);
            this->rbPartial->Text = L"Contains";
            this->rbPartial->Checked = true;

            this->rbExact->Location = System::Drawing::Point(90, 40);
            this->rbExact->Text = L"Exact";

            // btnSearch
            this->btnSearch->Location = System::Drawing::Point(200, 40);
            this->btnSearch->Text = L"Search";
            this->btnSearch->Click += gcnew EventHandler(this, &SearchForm::BtnSearch_Click);

            // dataGridView1
            this->dataGridView1->Location = System::Drawing::Point(12, 70);
            this->dataGridView1->Size = System::Drawing::Size(360, 220);
            this->dataGridView1->AutoGenerateColumns = true;
            this->dataGridView1->AllowUserToAddRows = false;
            this->dataGridView1->AllowUserToDeleteRows = false;
            this->dataGridView1->ReadOnly = true;

            // lblStatus
            this->lblStatus->Location = System::Drawing::Point(12, 300);
            this->lblStatus->AutoSize = true;
            this->lblStatus->Text = L"Ready";

            // Form
            this->ClientSize = System::Drawing::Size(384, 331);
            this->Controls->Add(this->txtQuery);
            this->Controls->Add(this->rbPartial);
            this->Controls->Add(this->rbExact);
            this->Controls->Add(this->btnSearch);
            this->Controls->Add(this->dataGridView1);
            this->Controls->Add(this->lblStatus);
            this->Text = L"Search";
        }

        void BtnSearch_Click(Object ^ sender, EventArgs ^ e)
        {
            lblStatus->Text = "Searching...";
            lblStatus->Refresh();

            bool partial = rbPartial->Checked;
            auto result = DataContext::Search(txtQuery->Text, partial);
            dataGridView1->DataSource = result;
            dataGridView1->Refresh();
            lblStatus->Text = "Found " + dataGridView1->RowCount + " occurrences";
        }
    };
}

