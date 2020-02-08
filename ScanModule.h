#pragma once
#include"AllModule.h"
#include"DataModule.h"

//ScanModule设计成为一个单例模式，整个程序只有一个扫描模块
class ScanModule
{
public:
	void Scan(const string& path);
	void StartScan()
	{
		while (1)
		{
			Scan("C:\\Users\\redee\\Desktop\\project");
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}
	static ScanModule* Instance()
	{
		static ScanModule scanmd;
		static std::thread thd(&StartScan, &scanmd);
		thd.detach();

		return &scanmd;
	}
private:
	ScanModule()
	{
		//_datamd.Init();
	}

	ScanModule(const ScanModule&);

	//DataModule _datamd;
	//vector<string> netrys;
};
