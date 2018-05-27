
#include "stdafx.h"
#include "DrvSqlite.h"


string ShowMemlist(Memlist& tmp)
{
    string res;
    Memlist::const_iterator it = tmp.begin();
    while (it != tmp.end())
    {
        res += *it + "\n";      //cout<<*it<<endl;
        ++it;
    }
    cout << res << endl;
    return res;
}

string ShowMemRow(MemRow& tmp)
{
    string res;
    MemRow::const_iterator map_it = tmp.begin();
    while (map_it != tmp.end())
    {
        //res += map_it->first+" : " +map_it->second+"\n";
        res += map_it->second + " | ";
        ++map_it;
    }
    cout << res << endl;
    return res;
}

string ShowMemTable(MemTable& tmp)
{
    string res;
    MemTable::iterator map_it = tmp.begin();
    while (map_it != tmp.end())
    {
        res += ShowMemRow(map_it->second) + "\n";
        ++map_it;
    }
    cout << res << endl;
    return res;
}

CoreSqliteDrv::CoreSqliteDrv()
{
    state_open = false;
}

CoreSqliteDrv::~CoreSqliteDrv()
{
}

void CoreSqliteDrv::sqlite_error(void)
{
    errMsg = const_cast<char*>(sqlite3_errmsg(sqlite_db));        //��ȡ������Ϣ
    cout << "error��" << errMsg << endl;
}

bool CoreSqliteDrv::open_database(string f_dataname)
{
    int rc = sqlite3_open(f_dataname.c_str(), &sqlite_db);    //�����ݿ�
    if (rc != SQLITE_OK)   //���ݿ��ʧ��
    {
        state_open = false;
        sqlite_error();
        sqlite3_close(sqlite_db);    //�ر����ݿ�����
        return false;
    }
    cout << "open database successfully!" << endl;
    state_open = true;
    return true;
}

bool CoreSqliteDrv::close_database(void)
{
    // �رռ�¼��
    std::cout << "close db" << std::endl;
    if (state_open)
    {
        int res = sqlite3_close(sqlite_db);
        if (res)
        {
            sqlite_error();
            return false;
        }
        state_open = false;
    }
    return true;
}

bool CoreSqliteDrv::Run_SQL(string f_cmdsql)
{
    if (!state_open)
        return false;
    int rc = sqlite3_exec(sqlite_db, f_cmdsql.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK)
    {
        sqlite_error();
        return false;
    }
    return true;
}

int CoreSqliteDrv::GetTableFields(string f_tablename, Memlist& f_resultlist)
{
    if (!state_open)
        return false;
    int row, column;
    char** result = NULL;
    string m_sql = " select * from " + f_tablename;
    sqlite3_get_table(sqlite_db, m_sql.c_str(), &result, &row, &column, 0);
    cout << row << endl;            //����
    cout << column << endl;     //����
    if (row < 1 && column < 1)
        return 0;
    for (int i = 0; i < column; i++)
        f_resultlist.push_back(result[i]);
    return column;  //˳�㷵���ֶ���
}

int CoreSqliteDrv::GetNumRun_SQL(string f_cmdsql)
{
    if (!state_open)
        return -1;  //��ʧ��
    int row, column;
    char** result = NULL;
    sqlite3_get_table(sqlite_db, f_cmdsql.c_str(), &result, &row, &column, 0);
    return row;     //��������
}

string CoreSqliteDrv::GetOneKeyValue(string f_cmdsql, int f_row, int f_col)
{
    if (!state_open)
        return false;
    sqlite3_stmt*  stmt = NULL;
    int rc = sqlite3_prepare_v2(sqlite_db, f_cmdsql.c_str(), f_cmdsql.size(), &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        if (stmt)
            sqlite3_finalize(stmt);
        sqlite_error();
        return false;
    }
    int nColumn = sqlite3_column_count(stmt);
    int vtype, i;
    do
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            for (i = 0; i < nColumn; i++)
            {
                vtype = sqlite3_column_type(stmt, i);
                if (vtype == SQLITE_INTEGER)
                    printf("%s : %d \n", sqlite3_column_name(stmt, i), sqlite3_column_int(stmt, i));
                else if (vtype == SQLITE_TEXT)
                    printf("%s : %s \n", sqlite3_column_name(stmt, i), sqlite3_column_text(stmt, i));
                else if (vtype == SQLITE_NULL)
                    printf("no values\n");
            }
            printf("\n****************\n");
        }
        else if (rc == SQLITE_DONE)
        {
            printf("Select finish\n");
            break;
        }
        else
        {
            printf("Select faile\n");
            sqlite3_finalize(stmt);
            break;
        }
    }
    while (1);
    sqlite3_finalize(stmt);
    return 0;
}

bool CoreSqliteDrv::GetColRowValue(string f_cmdsql, Memlist& f_resultlist, bool f_if)
{
    //��ѯ������,��
    int nRow, nColumn;
    char** dbResult;
    f_resultlist.clear();
    sqlite3_get_table(sqlite_db, f_cmdsql.c_str(), &dbResult, &nRow, &nColumn, NULL);
    if (nColumn > 1 || nRow < 1)    //���û�����ݻ���1�оͲ�����
        return false;
    sqlite3_stmt* querySQL;     //�������ָ���ָ�룬�����ָ������SQL��䷢��ָ��
    if (sqlite3_prepare_v2(sqlite_db, f_cmdsql.c_str(), -1, &querySQL, NULL) == SQLITE_OK)
    {
        //ִ�в�����䣬���������еĲ�ѯ���
        if (f_if)
        {
            //��ȡһ������
            while (sqlite3_step(querySQL) == SQLITE_ROW)
                f_resultlist.push_back((char*) sqlite3_column_text(querySQL, 0));
        }
        else
        {
            if (sqlite3_step(querySQL) == SQLITE_ROW)
            {
                //��ȡһ������
                for (int i = 0; i < nColumn; i++)
                    f_resultlist.push_back((char*) sqlite3_column_text(querySQL, i));
            }
        }
    }
    return true;
}

void CoreSqliteDrv::GetOneTable(string f_cmdsql, MemTable& f_table)
{
    //��ѯ������,��
    int nRow, nColumn;
    char** dbResult;
    sqlite3_get_table(sqlite_db, f_cmdsql.c_str(), &dbResult, &nRow, &nColumn, NULL);
    if (nColumn < 1 || nRow < 1)
        return;
    sqlite3_stmt* querySQL;     //�������ָ���ָ�룬�����ָ������SQL��䷢��ָ��
    if (sqlite3_prepare_v2(sqlite_db, f_cmdsql.c_str(), -1, &querySQL, NULL) == SQLITE_OK)
    {
        //ִ�в�����䣬���������еĲ�ѯ���
        while (sqlite3_step(querySQL) == SQLITE_ROW)
        {
            for (int i = 0; i < nColumn; i++)
            {
                char* field = (char*) sqlite3_column_text(querySQL, i);
                cout << field << " | ";
            }
            cout << endl;
        }
    }
}
/*
    void CoreAccess::GetOneTable(string cmd_sql,Ter_InfoMap& tdb)
    {
    if (!state_open && (cmd_sql.c_str()==NULL) )
    return ;
    try
    {
    if (m_pRecordset->State)
    m_pRecordset->Close();
    m_pRecordset->Open(cmd_sql.c_str(),      // ��ѯDemoTable���������ֶ�
    m_pConnection.GetInterfacePtr(),     // ��ȡ��ӿ��IDispatchָ��
    adOpenDynamic,
    adLockOptimistic,
    adCmdText);
    }
    catch(_com_error& e)
    {
    dump_com_error(e);
    return ;
    }

    _variant_t tmpstr;  //��ʱֵ
    //string tmp_str;
    TerminalInfo tmp_info;
    try
    {
    if(!m_pRecordset->BOF)
    m_pRecordset->MoveFirst();
    else
    return ;        //ShowBox("��������Ϊ��");

    while(!m_pRecordset->adoEOF)
    {
        //��һ��
        tmpstr = m_pRecordset->GetCollect("terminal_group");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_group = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_ip");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_ip = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_mac");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_mac = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_name");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_name = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_nickname");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_nickname = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_pass");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_pass = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_res1");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_res1 = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_res2");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_res2 = (LPCSTR)_bstr_t(tmpstr);
        tmpstr = m_pRecordset->GetCollect("terminal_username");
        if(tmpstr.vt != VT_NULL) tmp_info.terminal_username = (LPCSTR)_bstr_t(tmpstr);

        //�ر��� Ĭ��Ϊδ֪
        tmp_info.terminal_status = E_T_UNKNOWN;//E_T_ONLINE;//E_T_UNKNOWN;

        //����Ŀǰ��ip��key
        tdb.insert(Ter_InfoMap::value_type(tmp_info.terminal_nickname,tmp_info));
        m_pRecordset->MoveNext();   //�¹�
        }
        }
        catch(_com_error& e)
        {
        dump_com_error(e);
        return ;
        }
        return ;
    }

*/

bool CoreSqliteDrv::AddRow(string f_tablename, const MemRow& f_pInfo)
{
    //ȱ���ж� �����Ϸ�
    string t_sql, t_keys, t_value;
    MemRow::const_iterator map_it = f_pInfo.begin();
    while (map_it != f_pInfo.end())
    {
        t_keys += map_it->first + ",";
        t_value += "\'" + map_it->second + "\',";
        ++map_it;
    }
    //ע��Ҫȥ�����һ���ַ�
    t_keys.erase(t_keys.size() - 1, 1);
    t_value.erase(t_value.size() - 1, 1);
    t_sql = "insert into " + f_tablename + " ( " + t_keys + " ) values ( " + t_value + " ) ";
    return Run_SQL(t_sql);
}

bool CoreSqliteDrv::EditRow(string f_tablename, string f_if, const MemRow& f_pInfo)
{
    //ȱ���ж� �����Ϸ�
    string t_sql, t_keys;
    MemRow::const_iterator map_it = f_pInfo.begin();
    while (map_it != f_pInfo.end())
    {
        t_keys += map_it->first + "=\'" + map_it->second + "\',";
        ++map_it;
    }
    //  Update ���� set �ֶ�=ֵ where ����
    //ע��Ҫȥ�����һ���ַ�
    t_keys.erase(t_keys.size() - 1, 1);
    t_sql = "Update " + f_tablename + " set " + t_keys + " where " + f_if;
    return Run_SQL(t_sql);
    return true;
}

bool CoreSqliteDrv::DelRow(string f_tablename, string f_key, string f_value)
{
    string t_sql = " delete from " + f_tablename + " where " + f_key + " = \' " + f_value + "\'";
    return Run_SQL(t_sql);
}



//////////////////////////////////////////////////////////////////////////
CSQLBList::CSQLBList()
{
    m_pDB = NULL;
}

CSQLBList::~CSQLBList()
{
    Close();
}

//�����ݿ��ļ�
bool CSQLBList::Open(const string& dbFileName)
{
    if (m_pDB)
        Close();
    //����������ݿ��ļ�
    int ret = sqlite3_open(dbFileName.c_str(), &m_pDB);
    if (ret != SQLITE_OK)
    {
        //����������ݿ��ļ�ʧ��
        return false;
    }
    return true;
}

//�ر����ݿ��ļ�
void CSQLBList::Close()
{
    if (m_pDB)
    {
        sqlite3_close(m_pDB);
        m_pDB = NULL;
    }
}

//ִ�зǲ�ѯSQL���
int CSQLBList::ExecSQL(const char* strSQL) const
{
    sqlite3_stmt* stmt = NULL;
    int ret = SQLITE_OK;
    ret = sqlite3_prepare_v2(m_pDB, strSQL, -1, &stmt, NULL);
    if (ret != SQLITE_OK)
        return 0;
    sqlite3_step(stmt);
    ret = sqlite3_finalize(stmt);
    return ret;
}

//��ѯ���ݱ�
bool CSQLBList::Query(const char* sql, std::vector<string>& vctPclAddrList) const
{
    sqlite3_stmt* stmt = NULL;
    int ret = sqlite3_prepare_v2(m_pDB, sql, strlen(sql), &stmt, NULL);
    if (ret != SQLITE_OK)
    {
        if (stmt)
            sqlite3_finalize(stmt);
        return false;
    }
    //��ȡ��ѯ���
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        //CKWParcelAdressList pclAddr;
        //pclAddr.pid = sqlite3_column_int(stmt, 0);
        //pclAddr.div_index = sqlite3_column_int(stmt, 1);
        //pclAddr.road_offset = sqlite3_column_int(stmt, 2);
        //pclAddr.road_size = sqlite3_column_int(stmt, 3);
        //pclAddr.bkg_size = sqlite3_column_int(stmt, 4);
        //pclAddr.name_size = sqlite3_column_int(stmt, 5);
        //pclAddr.city = sqlite3_column_int(stmt, 6);
        //vctPclAddrList.push_back(pclAddr);
    }
    if (stmt)
        sqlite3_finalize(stmt);
    return true;
}

bool CSQLBList::QueryByMshCode(DWORD ulMeshCode, std::vector<string>& vctPclAddrList) const
{
    char sql[1024];
    _snprintf_s(sql, sizeof(sql), "SELECT * FROM DBLIST WHERE pid=%d", ulMeshCode);
    return Query(sql, vctPclAddrList);
}
