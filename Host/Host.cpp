/////////////////////////////////////////////////////////////////////////////
// Host.cpp  - Host execution part               						   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
/////////////////////////////////////////////////////////////////////////////
#include"Host.h"
#include<iostream>
#include"..\CppCommWithFileXfer\MsgPassingComm\Comm.h"
using namespace Testing;
int main(int argc, char** args)
{
	try
	{
		Epoint host("localhost", std::atoi(args[1]));
		std::cout << "Start host\n";
		Host testHost(host, "Host");
		testHost.startProcess();
		std::thread rec = testHost.get();
		std::thread snd = testHost.send();
		if(snd.joinable())
			snd.detach();
		if(rec.joinable())
			rec.join();
		std::cout << "Test harness has been stopped by client, press any key to exit\n";
		getchar();
	}
	catch(std::exception& ex)
	{
		std::cout << "Exception thrown: " << ex.what() << std::endl;
	}
	
}