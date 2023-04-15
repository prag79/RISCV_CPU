/*******************************************************************
 * File : Report.h
 *
 * Copyright(C) crossbar-inc. 2016  
 * 
 * ALL RIGHTS RESERVED.
 *
 * Description: This file contains detail of file management used to store 
 * performance metrics.
 ********************************************************************/

#ifndef REPORTS_H_
#define REPORTS_H_
#include<iostream>
#include<fstream>
#include<string>
#include<cstdint>

class Report
{

public:
	Report(const std::string filename)
		:mFileName(filename)
	{
		enHdrChan = false;
		enHdrTrans = false;
		enHdrBank = false;
		enHdrBus = false;
		enHdrLat = false;
		enHdrCmdTime = false;
		enHdrActivity = false;
	}
	~Report()
	{
		
	}

	bool openFile();
	bool openReadModeFile();
	void writeFile(const std::string transType,const double_t time, const uint64_t numTransaction, std::string space);
	void writeFile(const std::string transType, const int bankNum, const int chanNum, const uint64_t numTransaction, std::string space);
	void writeFile(const std::string transType, const double_t time, const int chanNum, const uint64_t numTransaction, std::string space);
	void writeFile(const uint16_t slotNum, const double_t latency, std::string space);
	void writeFile(const double_t time, const double busUtilization, std::string space);
	void writeFile(const double_t time, const int chanNum, const double_t chanUtil, std::string space);
	void writeFile(const uint8_t chanNum, const double_t startTime, const double_t endTime, std::string space);
	void writeFile(const uint64_t numCmd);
	void readFile(const uint32_t& slotNum, const double_t& latency);
	bool closeFile();
	bool isOpen();
private:
    std::string mFileName;
	std::ofstream mFileHandler;
	bool enHdrChan;
	bool enHdrTrans;
	bool enHdrBank;
	bool enHdrBus;
	bool enHdrLat;
	bool enHdrCmdTime;
	bool enHdrActivity;
};

#endif