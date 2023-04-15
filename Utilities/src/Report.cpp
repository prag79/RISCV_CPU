#include "Report.h"

/** openFile()
* Opens file
* @return bool
**/
bool Report::openFile()
{
	mFileHandler.open(mFileName, std::fstream::trunc | std::fstream::out);
	if (mFileHandler.is_open())
	{
		return true;
	}
	else {
		//TODO: Report Error
		return false;
	}
}

bool Report::openReadModeFile()
{
	mFileHandler.open(mFileName, std::fstream::in);
	if (mFileHandler.is_open())
	{
		return true;
	}
	else {
		//TODO: Report Error
		return false;
	}
}
/** writeFile()
* @param transType - File Type(READ/WRITE/TRANSACTIONS)
* @param time       - time stamp
* @param numTransaction - Number of transaction
* @return void
**/
void Report::writeFile(const std::string transType, const double_t time, const uint64_t numTransaction, std::string space)
{
	if ((enHdrTrans == false) && (space == ","))
	{
		mFileHandler << "Bank Number" << space << "Channel Number" << space << "Number of Transaction" << std::endl;
		enHdrTrans = true;
	}
	mFileHandler << time << space << numTransaction<<std::endl;
}

void Report::writeFile(const std::string transType,const int bankNum, const int chanNum, const uint64_t numTransaction, std::string space)
{
	if ((enHdrBank == false) && (space == ","))
	{
		mFileHandler << "Bank Number" << space << "Channel Number" << space << "Number of Transaction" << std::endl;
		enHdrBank = true;
	}
	mFileHandler << bankNum <<space <<chanNum << space << numTransaction << std::endl;
}

void Report::writeFile(const std::string transType, const double_t time, const int chanNum, const uint64_t numTransaction, std::string space)
{
	if ((enHdrChan == false) && (space == ","))
	{
		mFileHandler << "Time" << space << "Channel Number" << space << "Number of Transaction" << std::endl;
		enHdrChan = true;
	}
	mFileHandler << time << space << chanNum << space << numTransaction << std::endl;
}

void Report::writeFile(const double_t time, const double busUtilization, std::string space)
{
	if ((enHdrBus == false) && (space == ","))
	{
		mFileHandler << "Time" << space << "Bus Utilization" << std::endl;
		enHdrBus = true;
	}
	mFileHandler << time << space << busUtilization << std::endl;
}

void Report::writeFile(const uint16_t slotNum, const double_t latency, std::string space)
{
	if ((enHdrLat == false) && (space == ","))
	{
		mFileHandler << "Slot Number" << space << "Latency" << std::endl;
		enHdrLat = true;
	}
	mFileHandler <<(uint32_t)slotNum << space << latency << std::endl;
}

void Report::writeFile(const double_t time, const int chanNum, const double_t cmdTime, std::string space)
{
	if ((enHdrCmdTime == false) && (space == ","))
	{
		mFileHandler << "Time" << space << "Channel Number" << space << "Command Time" <<std::endl;
		enHdrCmdTime = true;
	}
	mFileHandler << time << space << chanNum << space << cmdTime << std::endl;
}
void Report::readFile(const uint32_t& slotNum, const double_t& latency)
{
}

void Report::writeFile(const uint8_t chanNum, const double_t startTime, const double_t endTime, std::string space)
{
	if ((enHdrActivity == false) && (space == ","))
	{
		mFileHandler << "Channel Number" << space << "Start Time" << space << "End Time" << std::endl;
		enHdrActivity = true;
	}
	mFileHandler << (uint32_t)chanNum << space << startTime << space << endTime<< std::endl;
}
void Report::writeFile(const uint64_t numCmd)
{
	
	mFileHandler <<numCmd << std::endl;
}
//bool closeFile();
/** closeFile()
* Closes file
* @return bool
**/
bool Report::closeFile()
{
	if (mFileHandler.is_open())
	{
		mFileHandler.close();
		return true;
	}else {
		//TODO: Report Error
		return false;
	}
}

/** isOpen()
* Checks if file is open
* @return bool
**/
bool Report::isOpen()
{
	if (mFileHandler.is_open())
		return true;
	else
		return false;
}