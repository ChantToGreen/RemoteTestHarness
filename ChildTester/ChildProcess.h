/////////////////////////////////////////////////////////////////////////////
// ChildProcess.h - Child tester which can process test request 		   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module provide a host class which will act as the worker for test harness,
* single thread besides Comm system, will run the test and send back result to the client
* Required Files:
* ===============
* Comm.h, Message.h, Tester.h,
* FileUtilities.h, ISingletonLogger.h,
* SingletonLoggerFactory.h, TestRequest.h
* ChildProcess.h
*
* Maintenance History:
* ====================
*
* ver 1.0 : 06 Nov 2018
* - first release
*/
#pragma once
#include<iostream>
#include<fstream>
#include"../CppCommWithFileXfer/Message/Message.h"
#include"../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include"../TestHarnessCore/Tester.h"
#include"../TestRequest/TestRequest.h"
#include"../Utilities/FileUtilities/FileUtilities.h"
#include"../Utilities/SingletonLogger/ISingletonLogger.h"
#include"../Utilities/SingletonLogger/SingletonLoggerFactory.h"


namespace Testing
{
	using Comm = MsgPassingCommunication::Comm;
	using Msg = MsgPassingCommunication::Message;
	using Epoint = MsgPassingCommunication::EndPoint;
	///////////////////////////////////////////////////////////////
	// ChildTester class, worker class for testing,
	// do the test and send back the test result
	//
	class ChildTester
	{
	public:
		ChildTester(const std::string& IP,const Epoint& src, const Epoint& mother);
		void work();
		
	private:
		std::string CreateLog(const std::string &path);
		void SendLog(const Epoint&des, const std::string&file);
		void SendReady(const Epoint&des);
		Comm comm_;
		Epoint src_;
		Epoint mother_;
		Tester tst_;
	};
	//-----------------------<Constructor, also start the comm once initialized>---------------
	inline ChildTester::ChildTester(const std::string& IP, const Epoint& src, const Epoint& mother) :src_(src),mother_(mother),comm_(src, IP)
	{
		comm_.start();
		comm_.setSendFilePath("../ChildSendFile");
		comm_.setSaveFilePath("../ChildSaveFile");
		
	}

	//--------------------<Send the ready message to the host>----------------
	inline void ChildTester::SendReady(const Epoint&des)
	{
		Msg msg;
		msg.from(src_);
		msg.to(des);
		msg.command("Ready");
		comm_.postMessage(msg);

	}

	//--------------------<Keep extract test request, do the test, return the result to client, notify the host it is ready>------------
	inline void ChildTester::work()
	{
		Utilities::ILogger<0, Lock>* plog = Utilities::SingletonLoggerFactory<0, Lock>::getInstance();
		SendReady(mother_);
		Msg msg;
		while (true)
		{
			std::cout << "\nWaiting for test request...\n===================================\n";
			msg = comm_.getMessage();
			if (msg.command() == "do test")
			{
				std::string path = msg.name();
				std::string log = CreateLog(path);
				std::ofstream fout(comm_.getSendFilePath()+"/"+log, std::ios::out);		
				plog->addStream(&fout);
				tst_.loadTestRequest(path);
				for (TestRequest& tr : tst_.requests())
				{
					tst_.loadLibraries(tr);
					bool result=tst_.doTests();
					if(result)
						plog->write("\n----------------\n All tests passed");
					else
						plog->write("\n----------------------\n At least one test failed");
					plog->write("\n----------------------\n");
					tst_.clear();
				}
				fout.close();
				SendLog(msg.from(), log);
				SendReady(mother_);
				plog->removeStream(&fout);
				tst_.clearReq();
			}
			if (msg.command() == "stop")
			{
				std::cout << "tester stopped\n===================================\n";
				comm_.stop();
				break;
			}

		}
	}

	//----------------------------<Create log name for the test result>------------------
	inline std::string ChildTester::CreateLog(const std::string & file)
	{
		std::ifstream in;
		Utilities::openReadFile(file, in);
		std::ostringstream sstr;
		sstr << in.rdbuf();
		std::string req = sstr.str();
		int i = 0;
		while (req[i] != ',')
		{
			++i;
		}
		req = req.substr(0, i);
		std::string filename = req+ "_" + "report" + ".txt";
		return filename;

	}

	//---------------------------<Send the log back to the client>--------------------
	inline void ChildTester::SendLog(const Epoint & des, const std::string & fileName)
	{
		Msg msg;
		msg.to(des);
		msg.from(src_);
		msg.file(fileName);
		msg.value("sendingFile");
		msg.command("display log");
		comm_.postMessage(msg);
	}

	
	
	
}