/////////////////////////////////////////////////////////////////////////////
// Host.h - Host for remote test harness system 						   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module provide a host class which provide means to create new child tester,
* receive command from client and assign test to availabal child process
* Required Files:
* ===============
* Comm.h, Message.h, Cpp11-BlockingQueue.h, 
* FileSystem.h, FileUtilities.h, Host.h,
* Process.h, TestRequest.h
*
* Maintenance History:
* ====================
* 
* ver 1.0 : 06 Nov 2018
* - Add function allows admin to shut down Host
* - first release
*/
#pragma once
#include"../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include"../CppCommWithFileXfer/Message/Message.h"
#include"../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include"../Utilities/FileSystem/FileSystem.h"
#include"../Utilities/FileUtilities/FileUtilities.h"
#include"../Process/Process/Process.h"
#include"../TestRequest/TestRequest.h"
#include<thread>
#include<iostream>
#include<vector>
#include<fstream>
#include<set>
namespace Testing
{
	using Comm = MsgPassingCommunication::Comm;
	using Msg = MsgPassingCommunication::Message;
	using Epoint = MsgPassingCommunication::EndPoint;
	//////////////////////////////////////////////////////////////////////////
	// Host class: 
	// - this class will spawn finite number of child process to
	//   run tests, when one child crushed, host will create new one 
	//   for replacement
	// - It will listen to both client and child process to assign test task
	//   for availbale child process
	// - Host can not shut down it self, it will either crush or shut down on
	//   on client command.
	class Host
	{
	public:
		Host(const Epoint& ep, const std::string& IP);
		void setSavePath(const std::string&path);
		void setSendPath(const std::string&path);
		std::thread get();
		std::thread send();
		void startProcess();
		void stopAllChildren();

	private:
		bool stop_ = false;
		size_t stopIssued_ = 0;
		bool dllExist(const Msg&msg);
		void respawnChild();
		void receive();
		void post();
		std::string savePath_ = "../testLibraries";
		std::string sendPath_ = "../HostSend";
		BlockingQueue<Msg> readyQueue_;
		BlockingQueue<Msg> requestQueue_;
		Epoint src_;
		Comm comm_;
		bool respawn_ = true;
		size_t proNum_ = 2;
		int startChildport_ = 8090;
		std::set<std::string> clients_;
	};

	//----------------<Constructor for host, also start the comm>-----------------
	inline Host::Host(const Epoint& ep, const std::string& IP) :src_(ep),comm_(ep, IP)
	{
		comm_.start();
		comm_.setSaveFilePath(savePath_);
		comm_.setSendFilePath(sendPath_);
		std::cout << "Host started at: " << ep.address << ":" << ep.port << "\n";
	}

	//----------------------<Set the save file path for host>---------------
	inline void Host::setSavePath(const std::string&path)
	{
		savePath_ = path;
		comm_.setSaveFilePath(path);
	}

	//----------------------<Set the send file path for host>---------------
	inline void Host::setSendPath(const std::string&path)
	{
		sendPath_ = path;
		comm_.setSendFilePath(path);
	}

	//-------------------<Defines action for receiver thread>---------------
	inline void Host::receive()
	{
		Msg msg;
		while (true&&!stop_)
		{
			if (stop_&&requestQueue_.size() >= proNum_)
				break;
			msg = comm_.getMessage();
			std::cout << "Received: "<<msg.command() << std::endl;
			if (msg.command() == "stop"&&readyQueue_.size()>= proNum_&&!stop_)
			{
				std::cout << "Stop order received! Host will be shut down\n===================================\n";
				requestQueue_.enQ(msg);
				stop_ = true;
				if (requestQueue_.size() >= proNum_)
					break;
				continue;
			}
			if (msg.command() == "Ready")
			{
				std::cout << "Child tester: "<<msg.from().toString()<<" is now available\n===================================\n";
				readyQueue_.enQ(msg);
				continue;
			}
			if (msg.command() == "testing"&&!stop_)
			{
				clients_.insert(msg.from().toString());
				if (msg.file().substr(msg.file().length() - 3, 3) == "txt")
				{
					Msg newMsg;
					std::string path = savePath_ + '\\' + msg.file();
					newMsg.name(path);
					newMsg.command("do test");
					newMsg.from(msg.from());
					requestQueue_.enQ(newMsg);
					continue;
				}
			}
			if (stop_&&stopIssued_ == proNum_)
				break;
		}
		std::cout << "Receiver thread stopped!\n";
	}


	//-------------<Create a receiver thread to get message>--------------
	inline std::thread Host::get()
	{
		auto tproc = [&]() {receive(); };
		std::thread receiver(tproc);
		return receiver;
	}


	//-------------<Action for receiver thread to receive message from client and child process, and also send back last message to admin>-----------
	inline void Host::post()
	{
		Msg msg;
		while (true)
		{
			std::cout << "Host is ready to post request\n===================================\n";
			msg = requestQueue_.deQ();
			std::cout << "Received msg from: " << msg.from().toString() << " with command: " << msg.command() << "\n";
			if (msg.command() == "stop")
			{
				stopAllChildren();
				Msg lastCall;
				lastCall.command("server stopped");
				lastCall.from(msg.to());
				for (auto cl : clients_)
				{
					lastCall.to(Epoint::fromString(cl));
					comm_.postMessage(lastCall);
				}
				comm_.stop();
				respawn_ = false;
				break;
			}
			if (msg.command() == "Close client")
				clients_.erase(msg.from().toString());
			if (msg.command() == "do test")
			{
				std::cout << "msg: " << msg.command() << "\n";
				if (!dllExist(msg))
				{
					std::cout << "Dll received does not match the test request,\n";
					std::cout << "request has been re-enqueued to wait for dlls";
					std::cout << "\n==============================================\n";
					requestQueue_.enQ(msg);
					continue;
				}
				Msg ready = readyQueue_.deQ();
				msg.to(ready.from());
				comm_.postMessage(msg);
				std::cout << "Assign test request to child tester: " << ready.from().toString() << "\n===================================\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}

	//---------------<create a sender thread to post message to client>------------
	inline std::thread Host::send()
	{
		auto tpoc = [&]() {post(); };
		std::thread sender(tpoc);
		return sender;
	}

	//---------------------<Spawn finite number of child process>------------------
	inline void Host::startProcess()
	{
		size_t i = 0;
		while(i<proNum_)
		{
			if (!respawn_) { std::cout << "No more child to spawn\n"; return; }
			i++;
			Process p;
			p.title("Child testing process");
			std::string appPath = "../Debug/ChildTester.exe";
			p.application(appPath);
			int portal = (int)i + 8085;
			std::string cmd = std::to_string(portal);
			p.commandLine(cmd);
			std::cout << "Child tester with address: localhost:" << cmd << " has been spawned\n===================================\n";
			Epoint child("localhost", portal);
			p.create();
			CBP callBack = [&]() {respawnChild(); };
			p.setCallBackProcessing(callBack);
			p.registerCallback();

		}
	}

	

	//--------------------<Respawn child process while test is not done>---------------
	inline void Host::respawnChild()
	{
		if (!respawn_) { std::cout << "No more child to respawn\n"; return; }
		Process p;
		p.title("Child testing process");
		std::string appPath = "../Debug/ChildTester.exe";
		p.application(appPath);
		int portal = startChildport_;
		startChildport_ += 5;
		std::string cmd = std::to_string(portal);
		p.commandLine(cmd);
		std::cout << "Child tester with address: localhost:" << cmd << " has been spawned\n";
		Epoint child("localhost", portal);
		p.create();
		CBP callBack = [&]() {respawnChild(); };
		p.setCallBackProcessing(callBack);
		p.registerCallback();
	
	}

	//------------------<Check the dlls stated in the test request exists or not, if not, the host will re-enqueue the message untill all dlls arrived>---------
	inline bool Host::dllExist(const Msg & msg)
	{
		Msg temp = msg;
		std::string path = temp.name();
		std::ifstream fin(path, std::ios::in);
		std::string line;
		if (fin.good())
		{
			std::getline(fin, line);
		}
		TestRequest rq = TestRequest::FromString(line);
		for (std::string dll : rq.request.valueRef())
		{
			std::string path = savePath_ + '/' + dll;
			if (!FileSystem::File::exists(path))
				return false;
		}
		return true;
	}

	//--------------<Send stop message to all child process>---------------
	inline void Host::stopAllChildren()
	{
		while (readyQueue_.size()!=0)
		{
			Msg msg = readyQueue_.deQ();
			msg.to(msg.from());
			msg.command("stop");
			comm_.postMessage(msg);

		}
		std::cout << "Stop message posted to all children\n";
	}


}