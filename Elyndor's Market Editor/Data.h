// Core data and database access layer for the C++17 WinForms port.
#pragma once

#using <System.dll>
#using <System.Data.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Data::SqlClient;
using namespace System::Drawing;

namespace ElyndorMarketEditor
{
    ref class Connection;
    ref class DataContext;

    public ref class MarketData
    {
    private:
        Int64 sortId;
        String ^ name;
        Int64 code;
        String ^ itemName;
        Int64 price;
        Int64 huntaholicPoint;
        Int64 arenaPoint;
        double priceRatio;
        double huntaholicRatio;
        double arenaRatio;

    public:
        MarketData() :
            sortId(0),
            name("[NEW MARKET]"),
            code(0),
            itemName("[NEW ITEM]"),
            price(0),
            huntaholicPoint(0),
            arenaPoint(0),
            priceRatio(0.0),
            huntaholicRatio(0.0),
            arenaRatio(0.0)
        {
        }

        MarketData(
            Int64 sortId,
            String ^ name,
            Int64 code,
            String ^ itemName,
            Int64 price,
            Int64 huntaholicPoint,
            Int64 arenaPoint,
            double priceRatio,
            double huntaholicRatio,
            double arenaRatio) :
            sortId(sortId),
            name(name),
            code(code),
            itemName(itemName),
            price(price),
            huntaholicPoint(huntaholicPoint),
            arenaPoint(arenaPoint),
            priceRatio(priceRatio),
            huntaholicRatio(huntaholicRatio),
            arenaRatio(arenaRatio)
        {
        }

        property Int64 SortID
        {
            Int64 get() { return sortId; }
            void set(Int64 value) { sortId = value; }
        }

        property String ^ Name
        {
            String ^ get() { return name; }
        }

        property Int64 Code
        {
            Int64 get() { return code; }
            void set(Int64 value);
        }

        property String ^ ItemName
        {
            String ^ get() { return itemName; }
            void set(String ^ value) { itemName = value; }
        }

        property Int64 Price
        {
            Int64 get() { return price; }
            void set(Int64 value) { price = value; }
        }

        property Int64 HuntaholicPoint
        {
            Int64 get() { return huntaholicPoint; }
            void set(Int64 value) { huntaholicPoint = value; }
        }

        property Int64 ArenaPoint
        {
            Int64 get() { return arenaPoint; }
            void set(Int64 value) { arenaPoint = value; }
        }

        property double PriceRatio
        {
            double get() { return priceRatio; }
            void set(double value) { priceRatio = value; }
        }

        property double HuntaholicRatio
        {
            double get() { return huntaholicRatio; }
            void set(double value) { huntaholicRatio = value; }
        }

        property double ArenaRatio
        {
            double get() { return arenaRatio; }
            void set(double value) { arenaRatio = value; }
        }
    };

    public ref class SearchData
    {
    private:
        Int64 itemCode;
        Int64 stringCode;
        String ^ stringValue;

    public:
        SearchData(Int64 itemCode, Int64 stringCode, String ^ stringValue) :
            itemCode(itemCode),
            stringCode(stringCode),
            stringValue(stringValue)
        {
        }

        property Int64 ItemCode { Int64 get() { return itemCode; } }
        property Int64 StringCode { Int64 get() { return stringCode; } }
        property String ^ StringValue { String ^ get() { return stringValue; } }
    };

    public ref class Connection
    {
    private:
        String ^ serverIP;
        String ^ username;
        String ^ password;
        String ^ database;
        SqlConnection ^ sql;

        void EnsureOpen();

    public:
        Connection(String ^ serverIP, String ^ username, String ^ password, String ^ database);

        property String ^ ServerIP { String ^ get() { return serverIP; } }
        property String ^ Username { String ^ get() { return username; } }
        property String ^ Password { String ^ get() { return password; } }
        property String ^ Database
        {
            String ^ get() { return database; }
            void set(String ^ value);
        }

        List<String ^> ^ GetMarketsList();
        BindingList<MarketData ^> ^ GetMarketData(String ^ name);
        BindingList<SearchData ^> ^ GetSearchResult(String ^ searchQuery, bool partial);

        void UpdateItemFields(MarketData ^ item);

        String ^ ExecuteScalarString(String ^ query, IEnumerable<SqlParameter ^> ^ parameters);
        void ExecuteNonQuery(String ^ query, IEnumerable<SqlParameter ^> ^ parameters);
    };

    public ref class DataContext abstract sealed
    {
    public:
        static Connection ^ Conn = nullptr;
        static BindingList<MarketData ^> ^ CurrentMarketTable = gcnew BindingList<MarketData ^>();
        static String ^ MarketName = "";

        static bool LoadConfig();
        static void SaveConfig(String ^ server, String ^ user, String ^ password, String ^ database);
        static List<String ^> ^ GetMarkets();
        static bool LoadMarket(String ^ name);
        static void SaveMarket(System::Windows::Forms::ProgressBar ^ progress);
        static BindingList<SearchData ^> ^ Search(String ^ term, bool partial);
        static void SetConnection(Connection ^ connection);
    };
}

