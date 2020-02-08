#include"AllModule.h"
#include"DataModule.h"
#include"ScanModule.h"

void TestDirectoryList()
{
	vector<string> dirs;
	vector<string> files;
	DirectoryList("C:\\Users\\redee\\Desktop\\杂货铺", dirs, files);
	for (const auto& e : dirs)
	{
		cout << e << endl;
	}

	for (const auto& e : files)
	{
		cout << e << endl;
	}

}

void TestSqlite()
{
	SqliteModule sq;
	sq.Open("test.db");

	string createtb_sql = "create table tb_doc(id integer primary key autoincrement, doc_path text, doc_name text)";
	sq.ExecuteSql(createtb_sql);


	string insert_sql = "insert into tb_doc(doc_path, doc_name) values('test_path', 'test_document')";
	sq.ExecuteSql(insert_sql);

	insert_sql = "insert into tb_doc(doc_path, doc_name) values('liu\', 'string.h')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc(doc_path, doc_name) values('liu\', 'vector.h')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc(doc_path, doc_name) values('liu\', 'list.h')";
	sq.ExecuteSql(insert_sql);

	string query_sql = "select * from TB_NAME where doc_path = 'liu'";
	int row, col;
	char** ppRet;
	sq.GetTable(query_sql, row, col, ppRet);

	sqlite3_free_table(ppRet);

	AutoGetTable agt(sq, query_sql, row, col, ppRet);
	for (int i = 1; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cout << ppRet[i*col + j] << " ";
		}
		cout << endl;
	}

}

void TestSearch()
{


	ScanModule::Instance();


	vector<std::pair<string, string>> docinfos;
	string key;
	cout << "*========================================================*" << endl;
	cout << "请输入要搜索的关键字：";

	while (std::cin >> key)
	{
		docinfos.clear();
		DataModule::GetInstance()->Search(key, docinfos);
		printf("%-30s %-30s\n", "名称", "路径");
		for (const auto& e : docinfos)
		{
			string prefix, highlight, suffix;
			const string& name = e.first;
			const string& path = e.second;
			DataModule::GetInstance()->PartionHighlight(name, key, prefix, highlight, suffix);

			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix;

			//补齐30空格并对齐
			for (size_t i = name.size(); i < 30; i++)
			{
				cout << " ";
			}


			printf("%-30s\n", path.c_str());
		}
		cout << "*========================================================*" << endl;
		cout << "请输入要搜索的关键字：";
	}
}

void TestScanModule()
{
	//ScanModule scanmd;
	//scanmd.Scan("C:\\Users\\redee\\Desktop\\杂货铺");


}

void TestPinyin()
{
	string allspell = ChineseConvertPinYinAllSpell("拼音 陕西科技大学 xiaan love 刘东旭");
	string initials = ChineseConvertPinYinInitials("拼音 陕西科技大学 xiaan love 刘东旭");
	cout << allspell << endl;
	cout << initials << endl;
}


void TestHighlight()
{
	//使用GBK编码汉字是两个字节
	{
		//高亮关键字
		string key = "陕西科技大学";
		string str = "我爱陕西科技大学13ga79";
		size_t pos = str.find(key);
		string previous, next;
		previous = str.substr(0, pos);
		next = str.substr(pos + key.size(), string::npos);
		cout << previous;
		ColourPrintf(key.c_str());
		cout << next << endl;
	}

	{
	//关键字是拼音，高亮对应的汉字

	//如果改为下面的输入，则算法会出错
	//string key = "shanxikejidaxue";
	//string str = "6我爱陕西科技大学13ga79";
	//此算法是2个字符去取，所以前面如果有一个字符的数字，则会出错
	//如果是ASCII字符就跳一位，如果不是表示是编码的汉字，那么就取两个

	string key = "shanxikejidaxue";
	string str = "6adh123j9我爱陕西科技大学13ga79";
	string previous, next;
	string str_py = ChineseConvertPinYinAllSpell(str);
	string key_py = ChineseConvertPinYinAllSpell(key);
	size_t pos = str_py.find(key_py);
	if (pos == string::npos)
	{
		cout << "拼音不匹配" << endl;
	}
	else
	{
		size_t key_strat = pos;
		size_t key_end = pos + key_py.size();

		size_t str_i = 0, py_i = 0;
		while (py_i < key_strat)
		{
			if (str[str_i] >= 0 && str[str_i] <= 127)
			{
				++str_i;
				++py_i;
			}
			else
			{
				char Chinese[3] = { '\0' };
				Chinese[0] = str[str_i];
				Chinese[1] = str[str_i + 1];
				str_i += 2;

				string py_str = ChineseConvertPinYinAllSpell(Chinese);
				py_i += py_str.size();
			}

		}
		previous = str.substr(0, str_i);

		size_t str_j = str_i, py_j = py_i;

		while (py_j < key_end)
		{
			if (str[str_j] >= 0 && str[str_j] <= 127)
			{
				++str_j;
				++py_j;
			}
			else
			{
				char Chinese[3] = { '\0' };
				Chinese[0] = str[str_j];
				Chinese[1] = str[str_j + 1];
				str_j += 2;

				string py_str = ChineseConvertPinYinAllSpell(Chinese);
				py_j += py_str.size();
			}

		}
		key = str.substr(str_i, str_j - str_i);
		next = str.substr(str_j, string::npos);

		cout << previous;
		ColourPrintf(key.c_str());
		cout << next << endl;
	}
}

	{
		//关键字是拼音首字母，高亮对应的汉字
		string key = "sxkjdx";
		string str = "6adh123j9我爱陕西科技大学13ga79";
		string previous, next;

	}
}

int main()
{
	//TestDirectoryList();
	//TestSqlite();
	//TestScanModule();
	TestSearch();
	//TestPinyin();
	//TestHighlight();


	return 0;

}
