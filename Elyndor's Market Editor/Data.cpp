#include "Data.h"

#using <System.Core.dll>

using namespace System::IO;
using namespace System::Windows::Forms;

namespace ElyndorMarketEditor
{
    void MarketData::Code::set(Int64 value)
    {
        code = value;
        if (DataContext::Conn != nullptr)
        {
            try
            {
                DataContext::Conn->UpdateItemFields(this);
            }
            catch (...)
            {
            }
        }
    }

    Connection::Connection(String ^ serverIP, String ^ username, String ^ password, String ^ database)
    {
        this->serverIP = serverIP;
        this->username = username;
        this->password = password;
        this->database = database;
        String ^ connString = String::Format("Server={0};User ID={1};Password={2};Database={3}", serverIP, username, password, database);
        sql = gcnew SqlConnection(connString);
        EnsureOpen();
    }

    void Connection::EnsureOpen()
    {
        if (sql->State != System::Data::ConnectionState::Open)
        {
            sql->Open();
        }
    }

    void Connection::UpdateItemFields(MarketData ^ item)
    {
        List<SqlParameter ^> ^ parms = gcnew List<SqlParameter ^>();
        parms->Add(gcnew SqlParameter("@code", item->Code));
        item->ItemName = ExecuteScalarString(
            "SELECT value FROM dbo.StringResource WHERE code = (SELECT name_id FROM dbo.ItemResource WHERE id = @code)",
            parms);
        item->Price = Convert::ToInt64(ExecuteScalarString("SELECT price FROM dbo.ItemResource WHERE id = @code", parms));
        item->HuntaholicPoint = Convert::ToInt64(ExecuteScalarString("SELECT huntaholic_point FROM dbo.ItemResource WHERE id = @code", parms));
        item->ArenaPoint = Convert::ToInt64(ExecuteScalarString("SELECT arena_point FROM dbo.ItemResource WHERE id = @code", parms));
    }

    List<String ^> ^ Connection::GetMarketsList()
    {
        EnsureOpen();
        List<String ^> ^ lst = gcnew List<String ^>();
        SqlCommand ^ cmd = gcnew SqlCommand("SELECT DISTINCT name FROM dbo.MarketResource ORDER BY name", sql);
        SqlDataReader ^ rdr = nullptr;
        try
        {
            rdr = cmd->ExecuteReader();
            while (rdr->Read())
            {
                if (!rdr->IsDBNull(rdr->GetOrdinal("name")))
                {
                    lst->Add(rdr->GetString(rdr->GetOrdinal("name")));
                }
            }
        }
        finally
        {
            if (rdr != nullptr) rdr->Close();
        }
        return lst;
    }

    BindingList<MarketData ^> ^ Connection::GetMarketData(String ^ name)
    {
        EnsureOpen();
        String ^ query =
            "SELECT a.sort_id AS sort_id, a.name AS name, a.code AS code, c.value AS item_name, "
            "b.price AS price, b.huntaholic_point AS huntaholic_point, b.arena_point AS arena_point, "
            "a.price_ratio AS price_ratio, a.huntaholic_ratio AS huntaholic_ratio, a.arena_ratio AS arena_ratio "
            "FROM dbo.Marketresource a "
            "LEFT JOIN dbo.ItemResource b ON a.code = b.id "
            "LEFT JOIN dbo.StringResource c ON b.name_id = c.code "
            "WHERE a.name = @name "
            "ORDER BY a.sort_id";

        SqlCommand ^ cmd = gcnew SqlCommand(query, sql);
        cmd->Parameters->AddWithValue("@name", name);
        SqlDataReader ^ rdr = nullptr;
        BindingList<MarketData ^> ^ lst = gcnew BindingList<MarketData ^>();
        try
        {
            rdr = cmd->ExecuteReader();
            while (rdr->Read())
            {
                if (!rdr->IsDBNull(rdr->GetOrdinal("sort_id")))
                {
                    lst->Add(gcnew MarketData(
                        rdr->GetInt64(rdr->GetOrdinal("sort_id")),
                        rdr->GetString(rdr->GetOrdinal("name")),
                        rdr->GetInt64(rdr->GetOrdinal("code")),
                        rdr->IsDBNull(rdr->GetOrdinal("item_name")) ? "[UNKNOWN]" : rdr->GetString(rdr->GetOrdinal("item_name")),
                        rdr->IsDBNull(rdr->GetOrdinal("price")) ? 0 : rdr->GetInt64(rdr->GetOrdinal("price")),
                        rdr->IsDBNull(rdr->GetOrdinal("huntaholic_point")) ? 0 : rdr->GetInt64(rdr->GetOrdinal("huntaholic_point")),
                        rdr->IsDBNull(rdr->GetOrdinal("arena_point")) ? 0 : rdr->GetInt64(rdr->GetOrdinal("arena_point")),
                        rdr->IsDBNull(rdr->GetOrdinal("price_ratio")) ? 0.0 : Convert::ToDouble(rdr->GetValue(rdr->GetOrdinal("price_ratio"))),
                        rdr->IsDBNull(rdr->GetOrdinal("huntaholic_ratio")) ? 0.0 : Convert::ToDouble(rdr->GetValue(rdr->GetOrdinal("huntaholic_ratio"))),
                        rdr->IsDBNull(rdr->GetOrdinal("arena_ratio")) ? 0.0 : Convert::ToDouble(rdr->GetValue(rdr->GetOrdinal("arena_ratio")))
                    ));
                }
            }
        }
        finally
        {
            if (rdr != nullptr) rdr->Close();
        }
        return lst;
    }

    BindingList<SearchData ^> ^ Connection::GetSearchResult(String ^ searchQuery, bool partial)
    {
        EnsureOpen();
        String ^ query;
        if (partial)
        {
            query = "SELECT a.id AS item_code, b.code AS string_code, b.value AS string_value "
                    "FROM dbo.ItemResource a "
                    "LEFT JOIN dbo.StringResource b ON a.name_id = b.code "
                    "WHERE b.value LIKE '%' + @query + '%' ORDER BY a.id";
        }
        else
        {
            query = "SELECT a.id AS item_code, b.code AS string_code, b.value AS string_value "
                    "FROM dbo.ItemResource a "
                    "LEFT JOIN dbo.StringResource b ON a.name_id = b.code "
                    "WHERE b.value = @query ORDER BY a.id";
        }

        SqlCommand ^ cmd = gcnew SqlCommand(query, sql);
        cmd->Parameters->AddWithValue("@query", searchQuery);
        SqlDataReader ^ rdr = nullptr;
        BindingList<SearchData ^> ^ lst = gcnew BindingList<SearchData ^>();
        try
        {
            rdr = cmd->ExecuteReader();
            while (rdr->Read())
            {
                if (!rdr->IsDBNull(rdr->GetOrdinal("item_code")))
                {
                    lst->Add(gcnew SearchData(
                        rdr->GetInt64(rdr->GetOrdinal("item_code")),
                        rdr->IsDBNull(rdr->GetOrdinal("string_code")) ? 0 : rdr->GetInt64(rdr->GetOrdinal("string_code")),
                        rdr->IsDBNull(rdr->GetOrdinal("string_value")) ? "" : rdr->GetString(rdr->GetOrdinal("string_value"))
                    ));
                }
            }
        }
        finally
        {
            if (rdr != nullptr) rdr->Close();
        }
        return lst;
    }

    String ^ Connection::ExecuteScalarString(String ^ query, IEnumerable<SqlParameter ^> ^ parameters)
    {
        EnsureOpen();
        SqlCommand ^ cmd = gcnew SqlCommand(query, sql);
        for each (SqlParameter ^ p in parameters)
        {
            cmd->Parameters->Add(p);
        }
        Object ^ result = cmd->ExecuteScalar();
        return result == nullptr || result == DBNull::Value ? "" : result->ToString();
    }

    void Connection::ExecuteNonQuery(String ^ query, IEnumerable<SqlParameter ^> ^ parameters)
    {
        EnsureOpen();
        SqlCommand ^ cmd = gcnew SqlCommand(query, sql);
        for each (SqlParameter ^ p in parameters)
        {
            cmd->Parameters->Add(p);
        }
        cmd->ExecuteNonQuery();
    }

    void Connection::Database::set(String ^ value)
    {
        try
        {
            sql->ChangeDatabase(value);
            database = value;
        }
        catch (...)
        {
        }
    }

    bool DataContext::LoadConfig()
    {
        if (!File::Exists("config"))
        {
            return false;
        }

        String ^ server = "";
        String ^ user = "";
        String ^ password = "";
        String ^ database = "";

        for each (String ^ line in File::ReadAllLines("config"))
        {
            if (!line->Contains("=")) continue;
            array<String ^> ^ parts = line->Split('=');
            if (parts->Length < 2) continue;
            String ^ key = parts[0];
            String ^ value = parts[1];
            if (key->Equals("Server", StringComparison::InvariantCultureIgnoreCase)) server = value;
            else if (key->Equals("User", StringComparison::InvariantCultureIgnoreCase)) user = value;
            else if (key->Equals("Password", StringComparison::InvariantCultureIgnoreCase)) password = value;
            else if (key->Equals("Database", StringComparison::InvariantCultureIgnoreCase)) database = value;
        }

        try
        {
            Conn = gcnew Connection(server, user, password, database);
        }
        catch (Exception ^ ex)
        {
            MessageBox::Show("No connection to a database! Please check your config!" + Environment::NewLine + ex->Message);
            return false;
        }
        return true;
    }

    void DataContext::SaveConfig(String ^ server, String ^ user, String ^ password, String ^ database)
    {
        String ^ content =
            "Server=" + server + Environment::NewLine +
            "User=" + user + Environment::NewLine +
            "Password=" + password + Environment::NewLine +
            "Database=" + database;
        File::WriteAllText("config", content);
    }

    void DataContext::SetConnection(Connection ^ connection)
    {
        Conn = connection;
    }

    List<String ^> ^ DataContext::GetMarkets()
    {
        if (Conn == nullptr) return gcnew List<String ^>();
        return Conn->GetMarketsList();
    }

    bool DataContext::LoadMarket(String ^ name)
    {
        if (Conn == nullptr) return false;
        try
        {
            MarketName = name;
            CurrentMarketTable = Conn->GetMarketData(name);
            return true;
        }
        catch (Exception ^ ex)
        {
            MessageBox::Show("Failed to load market: " + ex->Message);
            return false;
        }
    }

    void DataContext::SaveMarket(ProgressBar ^ progress)
    {
        if (Conn == nullptr || CurrentMarketTable == nullptr) return;

        if (progress != nullptr)
        {
            progress->Value = 0;
            progress->Maximum = CurrentMarketTable->Count;
        }

        List<SqlParameter ^> ^ deleteParams = gcnew List<SqlParameter ^>();
        deleteParams->Add(gcnew SqlParameter("@name", MarketName));
        Conn->ExecuteNonQuery("DELETE FROM dbo.MarketResource WHERE name = @name", deleteParams);

        int i = 0;
        for each (MarketData ^ m in CurrentMarketTable)
        {
            List<SqlParameter ^> ^ insertParams = gcnew List<SqlParameter ^>();
            insertParams->Add(gcnew SqlParameter("@sort_id", m->SortID));
            insertParams->Add(gcnew SqlParameter("@name", m->Name));
            insertParams->Add(gcnew SqlParameter("@code", m->Code));
            insertParams->Add(gcnew SqlParameter("@price_ratio", m->PriceRatio));
            insertParams->Add(gcnew SqlParameter("@huntaholic_ratio", m->HuntaholicRatio));
            insertParams->Add(gcnew SqlParameter("@arena_ratio", m->ArenaRatio));

            Conn->ExecuteNonQuery(
                "INSERT INTO dbo.MarketResource (sort_id, name, code, price_ratio, huntaholic_ratio, arena_ratio) "
                "VALUES (@sort_id, @name, @code, @price_ratio, @huntaholic_ratio, @arena_ratio)",
                insertParams);

            List<SqlParameter ^> ^ updateParams = gcnew List<SqlParameter ^>();
            updateParams->Add(gcnew SqlParameter("@price", m->Price));
            updateParams->Add(gcnew SqlParameter("@huntaholic_point", m->HuntaholicPoint));
            updateParams->Add(gcnew SqlParameter("@arena_point", m->ArenaPoint));
            updateParams->Add(gcnew SqlParameter("@code", m->Code));

            Conn->ExecuteNonQuery(
                "UPDATE dbo.ItemResource SET price = @price, huntaholic_point = @huntaholic_point, arena_point = @arena_point WHERE id = @code",
                updateParams);

            ++i;
            if (progress != nullptr)
            {
                progress->Value = i;
                progress->Refresh();
            }
        }

        MessageBox::Show("Saved!");
    }

    BindingList<SearchData ^> ^ DataContext::Search(String ^ term, bool partial)
    {
        if (Conn == nullptr) return gcnew BindingList<SearchData ^>();
        return Conn->GetSearchResult(term, partial);
    }
}

