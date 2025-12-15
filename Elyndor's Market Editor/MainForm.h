// Main window for the C++17 WinForms application.
#pragma once

#include "Data.h"
#include "ConfigForm.h"
#include "SearchForm.h"

#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>
#using <Microsoft.VisualBasic.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace Microsoft::VisualBasic;

namespace ElyndorMarketEditor
{
    public ref class MainForm : public System::Windows::Forms::Form
    {
    public:
        MainForm(void)
        {
            InitializeComponent();
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container ^ components;
        ComboBox ^ comboBoxMarkets;
        DataGridView ^ dataGridView1;
        Button ^ btnSave;
        Button ^ btnAdd;
        Button ^ btnDelete;
        Button ^ btnNewMarket;
        Button ^ btnConfig;
        Button ^ btnSearch;
        ProgressBar ^ progressBar1;
        Label ^ lblMarkets;

        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->comboBoxMarkets = gcnew ComboBox();
            this->dataGridView1 = gcnew DataGridView();
            this->btnSave = gcnew Button();
            this->btnAdd = gcnew Button();
            this->btnDelete = gcnew Button();
            this->btnNewMarket = gcnew Button();
            this->btnConfig = gcnew Button();
            this->btnSearch = gcnew Button();
            this->progressBar1 = gcnew ProgressBar();
            this->lblMarkets = gcnew Label();

            // comboBoxMarkets
            this->comboBoxMarkets->Anchor = AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;
            this->comboBoxMarkets->FormattingEnabled = true;
            this->comboBoxMarkets->Location = System::Drawing::Point(12, 28);
            this->comboBoxMarkets->Name = L"comboBoxMarkets";
            this->comboBoxMarkets->Size = System::Drawing::Size(460, 21);
            this->comboBoxMarkets->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::ComboBoxMarkets_SelectedIndexChanged);

            // dataGridView1
            this->dataGridView1->Anchor = AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right;
            this->dataGridView1->Location = System::Drawing::Point(12, 60);
            this->dataGridView1->Name = L"dataGridView1";
            this->dataGridView1->Size = System::Drawing::Size(760, 340);
            this->dataGridView1->AutoGenerateColumns = true;
            this->dataGridView1->AllowUserToAddRows = false;
            this->dataGridView1->AllowUserToDeleteRows = false;
            this->dataGridView1->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView1->CellEnter += gcnew DataGridViewCellEventHandler(this, &MainForm::DataGridView1_CellEnter);
            this->dataGridView1->CellEndEdit += gcnew DataGridViewCellEventHandler(this, &MainForm::DataGridView1_CellEndEdit);
            this->dataGridView1->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            // btnSave
            this->btnSave->Anchor = AnchorStyles::Bottom | AnchorStyles::Left;
            this->btnSave->Location = System::Drawing::Point(12, 410);
            this->btnSave->Name = L"btnSave";
            this->btnSave->Size = System::Drawing::Size(75, 23);
            this->btnSave->Text = L"Save";
            this->btnSave->UseVisualStyleBackColor = true;
            this->btnSave->Click += gcnew EventHandler(this, &MainForm::BtnSave_Click);

            // btnAdd
            this->btnAdd->Anchor = AnchorStyles::Bottom | AnchorStyles::Left;
            this->btnAdd->Location = System::Drawing::Point(93, 410);
            this->btnAdd->Name = L"btnAdd";
            this->btnAdd->Size = System::Drawing::Size(75, 23);
            this->btnAdd->Text = L"Add";
            this->btnAdd->UseVisualStyleBackColor = true;
            this->btnAdd->Click += gcnew EventHandler(this, &MainForm::BtnAdd_Click);

            // btnDelete
            this->btnDelete->Anchor = AnchorStyles::Bottom | AnchorStyles::Left;
            this->btnDelete->Location = System::Drawing::Point(174, 410);
            this->btnDelete->Name = L"btnDelete";
            this->btnDelete->Size = System::Drawing::Size(75, 23);
            this->btnDelete->Text = L"Delete";
            this->btnDelete->UseVisualStyleBackColor = true;
            this->btnDelete->Click += gcnew EventHandler(this, &MainForm::BtnDelete_Click);

            // btnNewMarket
            this->btnNewMarket->Anchor = AnchorStyles::Bottom | AnchorStyles::Left;
            this->btnNewMarket->Location = System::Drawing::Point(255, 410);
            this->btnNewMarket->Name = L"btnNewMarket";
            this->btnNewMarket->Size = System::Drawing::Size(90, 23);
            this->btnNewMarket->Text = L"New Market";
            this->btnNewMarket->UseVisualStyleBackColor = true;
            this->btnNewMarket->Click += gcnew EventHandler(this, &MainForm::BtnNewMarket_Click);

            // btnConfig
            this->btnConfig->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            this->btnConfig->Location = System::Drawing::Point(480, 26);
            this->btnConfig->Name = L"btnConfig";
            this->btnConfig->Size = System::Drawing::Size(90, 23);
            this->btnConfig->Text = L"Config";
            this->btnConfig->UseVisualStyleBackColor = true;
            this->btnConfig->Click += gcnew EventHandler(this, &MainForm::BtnConfig_Click);

            // btnSearch
            this->btnSearch->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            this->btnSearch->Location = System::Drawing::Point(576, 26);
            this->btnSearch->Name = L"btnSearch";
            this->btnSearch->Size = System::Drawing::Size(90, 23);
            this->btnSearch->Text = L"Search";
            this->btnSearch->UseVisualStyleBackColor = true;
            this->btnSearch->Click += gcnew EventHandler(this, &MainForm::BtnSearch_Click);

            // progressBar1
            this->progressBar1->Anchor = AnchorStyles::Bottom | AnchorStyles::Right;
            this->progressBar1->Location = System::Drawing::Point(480, 410);
            this->progressBar1->Name = L"progressBar1";
            this->progressBar1->Size = System::Drawing::Size(292, 23);

            // lblMarkets
            this->lblMarkets->AutoSize = true;
            this->lblMarkets->Location = System::Drawing::Point(12, 9);
            this->lblMarkets->Name = L"lblMarkets";
            this->lblMarkets->Text = L"Markets";

            // MainForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(784, 441);
            this->Controls->Add(this->lblMarkets);
            this->Controls->Add(this->progressBar1);
            this->Controls->Add(this->btnSearch);
            this->Controls->Add(this->btnConfig);
            this->Controls->Add(this->btnNewMarket);
            this->Controls->Add(this->btnDelete);
            this->Controls->Add(this->btnAdd);
            this->Controls->Add(this->btnSave);
            this->Controls->Add(this->dataGridView1);
            this->Controls->Add(this->comboBoxMarkets);
            this->Name = L"MainForm";
            this->Text = L"Elyndor's Market Editor (C++17)";
            this->Load += gcnew EventHandler(this, &MainForm::MainForm_Load);
        }

        void PopulateMarkets()
        {
            auto markets = DataContext::GetMarkets();
            comboBoxMarkets->Items->Clear();
            for each (String ^ s in markets)
            {
                comboBoxMarkets->Items->Add(s);
            }
            if (markets->Count > 0)
            {
                comboBoxMarkets->SelectedIndex = 0;
            }
        }

        void RefreshGrid(Int32 rowIndex)
        {
            dataGridView1->DataSource = nullptr;
            dataGridView1->DataSource = DataContext::CurrentMarketTable;
            try
            {
                if (rowIndex >= 0 && rowIndex < dataGridView1->RowCount)
                {
                    dataGridView1->FirstDisplayedScrollingRowIndex = rowIndex;
                    dataGridView1->Rows[rowIndex]->Selected = true;
                }
            }
            catch (...)
            {
            }
            dataGridView1->Refresh();
        }

        // Event handlers
        void MainForm_Load(Object ^ sender, EventArgs ^ e)
        {
            if (DataContext::LoadConfig())
            {
                PopulateMarkets();
            }
            else
            {
                MessageBox::Show("Please set a database config before loading tables!");
            }
        }

        void ComboBoxMarkets_SelectedIndexChanged(Object ^ sender, EventArgs ^ e)
        {
            if (comboBoxMarkets->SelectedIndex < 0) return;
            DataContext::LoadMarket(comboBoxMarkets->Text);
            RefreshGrid(0);
            dataGridView1->Focus();
        }

        void BtnSave_Click(Object ^ sender, EventArgs ^ e)
        {
            DataContext::SaveMarket(progressBar1);
        }

        void BtnAdd_Click(Object ^ sender, EventArgs ^ e)
        {
            DataContext::CurrentMarketTable->Add(gcnew MarketData(
                DataContext::CurrentMarketTable->Count,
                DataContext::MarketName,
                0,
                "[NEW ITEM]",
                0,
                0,
                0,
                0.0,
                0.0,
                0.0));
            RefreshGrid(DataContext::CurrentMarketTable->Count - 1);
        }

        void BtnDelete_Click(Object ^ sender, EventArgs ^ e)
        {
            if (dataGridView1->SelectedRows->Count == 0) return;
            int idx = dataGridView1->SelectedRows[0]->Index;
            if (idx >= 0 && idx < DataContext::CurrentMarketTable->Count)
            {
                DataContext::CurrentMarketTable->RemoveAt(idx);
                int newIndex = idx;
                if (newIndex >= DataContext::CurrentMarketTable->Count) newIndex = DataContext::CurrentMarketTable->Count - 1;
                if (newIndex < 0) newIndex = 0;
                RefreshGrid(newIndex);
            }
        }

        void BtnNewMarket_Click(Object ^ sender, EventArgs ^ e)
        {
            String ^ name = Interaction::InputBox("Please enter market name", "New market", "");
            if (String::IsNullOrWhiteSpace(name)) return;

            DataContext::MarketName = name;
            DataContext::CurrentMarketTable = gcnew BindingList<MarketData ^>();
            if (!comboBoxMarkets->Items->Contains(name))
            {
                comboBoxMarkets->Items->Add(name);
            }
            comboBoxMarkets->Text = name;
            RefreshGrid(0);
        }

        void BtnConfig_Click(Object ^ sender, EventArgs ^ e)
        {
            ConfigForm ^ cfg = gcnew ConfigForm();
            cfg->ShowDialog(this);
            PopulateMarkets();
        }

        void BtnSearch_Click(Object ^ sender, EventArgs ^ e)
        {
            SearchForm ^ search = gcnew SearchForm();
            search->Show(this);
        }

        void DataGridView1_CellEnter(Object ^ sender, DataGridViewCellEventArgs ^ e)
        {
            // Keep a simple row index in case we need to restore position later.
        }

        void DataGridView1_CellEndEdit(Object ^ sender, DataGridViewCellEventArgs ^ e)
        {
            if (e->RowIndex < 0 || e->ColumnIndex < 0) return;
            auto row = dataGridView1->Rows[e->RowIndex];
            auto item = dynamic_cast<MarketData ^>(row->DataBoundItem);
            if (item == nullptr) return;

            String ^ columnName = dataGridView1->Columns[e->ColumnIndex]->DataPropertyName;
            if (String::IsNullOrEmpty(columnName))
            {
                columnName = dataGridView1->Columns[e->ColumnIndex]->Name;
            }

            if (columnName == "Code")
            {
                if (DataContext::Conn != nullptr)
                {
                    try
                    {
                        DataContext::Conn->UpdateItemFields(item);
                    }
                    catch (...)
                    {
                    }
                }
                RefreshGrid(e->RowIndex);
            }
        }
    };
}

