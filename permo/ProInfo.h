#pragma once
typedef long long           int64_t;  
typedef unsigned long long  uint64_t;  

class CProInfo
{
public:
	CProInfo(void);
	~CProInfo(void);
	//The process identifier.
	DWORD id;
	//The name of the executable file for the process.
	TCHAR szExeFile[MAX_PATH];
	TCHAR szExeFilePath[MAX_PATH+1];
	//LARGE_INTEGER lastCPUTime;
	//LARGE_INTEGER lastCheckCPUTime;
	//��һ�ε�ʱ��
	int64_t last_time_;  
	int64_t last_system_time_; 
	int cpu;
	BOOL bExit;
	double mem;
	double netup;
	double netdown;
	HICON hIcon;

	bool active;

	// Valid Only When Active
	bool dirty;

	int txRate;
	int rxRate;
	int prevTxRate;	//up
	int prevRxRate; //down

	int pid; // Note: A "Process" item may corresponds to multiple pids
};
