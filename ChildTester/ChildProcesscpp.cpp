/////////////////////////////////////////////////////////////////////////////
// ChildProcess.cpp - test worker execution file                       	   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
/////////////////////////////////////////////////////////////////////////////
#include"../ChildTester/ChildProcess.h"
#include"../CppCommWithFileXfer/Message/Message.h"
#include"../CppCommWithFileXfer/MsgPassingComm/Comm.h"
using namespace Testing;
using namespace MsgPassingCommunication;
int main(int args, char**argv)
{
	try
	{
		size_t potal = std::stoi(argv[0]);
		std::cout << "potal: " << argv[0] << "\n";
		EndPoint tester("localhost", potal);
		EndPoint host("localhost", 8083);
		ChildTester CTester("tester", tester, host);
		CTester.work();
		std::cout << "Test harness has been stopped by client, press any key to exit\n";
		getchar();
		return 0;
	}
	catch (std::exception& ex)
	{
		std::cout << "Exception thrown: " << ex.what() << std::endl;
	}

}