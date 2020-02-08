#include"ScanModule.h"
#include"AllModule.h"

void ScanModule::Scan(const string& path)
{
	//扫描进行比对，比对文件系统和数据库
	vector<string> localdirs;
	vector<string> localfiles;
	DirectoryList(path, localdirs, localfiles);

	std::set<string> localset;
	localset.insert(localfiles.begin(), localfiles.end());
	localset.insert(localdirs.begin(), localdirs.end());

	std::set<string> dbset;

	DataModule::GetInstance()->GetDoc(path, dbset);

	auto localit = localset.begin();
	auto dbit = dbset.begin();
	while (localit != localset.end() && dbit != dbset.end())
	{
		if (*localit < *dbit)
		{
			DataModule::GetInstance()->InsertDoc(path, *localit);
			++localit;
		}
		else if (*localit > *dbit)
		{
			DataModule::GetInstance()->DeleteDoc(path, *dbit);
			++dbit;
		}
		else
		{
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end())
	{
		DataModule::GetInstance()->InsertDoc(path, *localit);
		++localit;
	}
	while (dbit != dbset.end())
	{
		DataModule::GetInstance()->DeleteDoc(path, *dbit);
		++dbit;
	}
	//递归扫描对比子目录数据
	for (const auto& subdirs : localdirs)
	{
		string subpath = path;
		subpath += '/';
		subpath += subdirs;
		Scan(subpath);
	}
}
