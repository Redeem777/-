#pragma once
#include"AllModule.h"
#include<string>

// SqliteModule是对Sqlite的接口进行一层简单的封装。 
class SqliteModule {
public:
	SqliteModule()
		:_db(nullptr)
	{}

	~SqliteModule() {
		Close();
	}
	void Open(const string& path);
	void Close();
	void ExecuteSql(const string& sql);
	void GetTable(const string& sql, int& row, int& col, char**& ppRet);

	SqliteModule(const SqliteModule&) = delete;
	SqliteModule& operator=(const SqliteModule&) = delete;
private:

	sqlite3* _db;       // 数据库对象 
};

//RAII思想
class AutoGetTable
{
public:
	AutoGetTable(SqliteModule& sm, const string& sql, int& row, int& col, char**& ppRet)
	{
		sm.GetTable(sql, row, col, ppRet);
		_ppRet = ppRet;
	}
	~AutoGetTable()
	{
		sqlite3_free_table(_ppRet);
	}

	AutoGetTable(const AutoGetTable&) = delete;
	AutoGetTable operator=(const AutoGetTable&) = delete;

private:
	char** _ppRet;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//数据管理模块
#define DB_NAME "doc_db"
#define TB_NAME "tb_doc"

//为了方便进行加锁，设计成单例模式
class DataModule
{
public:
	static DataModule* GetInstance()
	{
		static DataModule datamd;
		datamd.Init();

		return &datamd;
	}

	void Init();//建表，打开数据库
	void GetDoc(const string& path, std::set<string>& dbset);//查找某个path下的所有子文档
	void InsertDoc(const string& path, const string& name);
	void DeleteDoc(const string& path, const string& name);
	void Search(const string& key, vector<std::pair<string, string>>& docinfos);

	void PartionHighlight(const string& str, const string& key, string& previous, string& highlight, string& next);

private:
	DataModule()
	{}

	SqliteModule _dbmd;
	std::mutex _mtx;
};
