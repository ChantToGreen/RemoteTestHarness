/////////////////////////////////////////////////////////////////////////////
// DemoLib.cpp : Demonstration of project 4      						   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package description:
* ======================
*  This is test library that would be used to demonstrate the remote test harness system.
*
*  Required files:
* =================
*  DemoReq.cpp
*  ITests.h
*  FileUtilities.h
*
*  Maintainence History:
* =======================
*  ver 2.0 : 07 Nov 2018
*  - replaced all tests
*  - Ammar's infrastructure has not been modified
*/

#define IN_DLL
#define ARG_SIZE 256

#include"../CppCommWithFileXfer/Utilities/FileUtilities/FileUtilities.h"
#include "../Dll_Loader_With_Hosted_Resource/ITests.h"
#include <iostream>
#include <string>
#include <string.h>


using namespace std;

//////////////////////////////////////////////////////////////////
// Demo1: demonstration of requirement 1

class Demo1 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}
		pRes_->setTerminator("");
		pRes_->write("\n\nDemonstrating Project4 requirement #1: using VS2017 and console project\n");
		pRes_->write("-----------------------------------------------------------------------\n");
		pRes_->write("This is VS2017 and console project, trust me\n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #1";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};

//////////////////////////////////////////////////////////////////
// Demo2: demonstration of requirement 2

class Demo2 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #2: using WPF for GUI\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Client is created by WPF\n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #2";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo3: demonstration of requirement 3

class Demo3 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project3 requirement #3: Assemble the work of previous project and Client need not to wait for reply\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Showing the file about previous project\n");
		Utilities::showDirContents("../TestHarnessCore", "", { "*.h" });
		Utilities::showDirContents("../ChildTester", "", { "*.h" });
		Utilities::showDirContents("../Host", "", { "*.h" });
		Utilities::showDirContents("../Utilities/SingletonLogger", "", { "*.h" });
		Utilities::showDirContents("../CppCommWithFileXfer/MsgPassingComm", "", { "*.h" });
		pRes_->write("\n--------------------------------------------------------------------------------\n");
		pRes_->write("Client has adopted ashyncronous message passing comm, user can keep work on the client and let the reply popup automatically \n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #3";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo4: demonstration of requirement 4

class Demo4 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #4: GUI shall support selecting Libs and display result \n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Show the code where GUI supports selecting Libs");
		std::string path = "../ClientGui/Client/MainWindow.xaml.cs";
		Utilities::showFileLines(path, 235, 274);
		pRes_->write("Show the code where GUI supports view result");
		Utilities::showFileLines(path, 421, 441);
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #4";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo5: demonstration of requirement 5
class Demo5 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #5: provide message designs appropriate for this application\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Showing the code in Client and Host how the message attribute is used\n");
		std::string path = "../ClientGui/Client/MainWindow.xaml.cs";
		Utilities::showFileLines(path, 118, 140);
		path = "../Host/Host.h";
		Utilities::showFileLines(path, 106, 150);
		pRes_->write("Command varies in different messages,\nclient and server will act differently based on the message command received\n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #5";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo6: demonstration of requirement 6
class Demo6 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #6: support sending TestRequest messages from Client to TestHarness Server\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Showing the code where Client send TestRequest,\nyou can create your cutomized test request on Client window\n");
		std::string path = "../ClientGui/Client/MainWindow.xaml.cs";
		Utilities::showFileLines(path, 235, 274);
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #6";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo7: demonstration of requirement 7
class Demo7 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #7: Child test process shall execute each test request as sequence of tests\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		std::string path = "../ChildTester/ChildProcess.h";
		pRes_->write("Showing the code where child process run tests\n");
		Utilities::showFileLines(path, 92, 114);
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #7";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo8: demonstration of requirement 8
class Demo8 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #8: Child process shall send result back to the client\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		std::string path = "../ChildTester/ChildProcess.h";
		pRes_->write("Showing the code where child process send back result\n");
		Utilities::showFileLines(path, 144, 154);
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #8";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo9: demonstration of requirement 9
class Demo9 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #8: Shall have multiple client and child tester running concurrently\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("As you can see the child tester and client is running on your screen\n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #9";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};

//////////////////////////////////////////////////////////////////
// Demo10: demonstration of requirement 9
class Demo10 : public ITest {
public:
	DLL_DECL bool test()
	{
		if (pRes_ == nullptr)
		{
			std::cout << "\n  no logger available";
			return false;
		}

		pRes_->write("\n\nDemonstrating Project4 requirement #10: Shall have an automated unit test suite\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("This is the automated unit test suite\n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project4 Requirement #8";
	}
	DLL_DECL std::string author()
	{
		return "Yilin Cui";
	}
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
private:
	ILog * pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// The library of all the demonsration class

class TestCollection : public ITests {
	DLL_DECL std::vector<ITest*> tests();
};

// this is where each test class is instantiated and added to collection
DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new Demo1);
	tests_vec.push_back(new Demo2);
	tests_vec.push_back(new Demo3);
	tests_vec.push_back(new Demo4);
	tests_vec.push_back(new Demo5);
	tests_vec.push_back(new Demo6);
	tests_vec.push_back(new Demo7);
	tests_vec.push_back(new Demo8);
	tests_vec.push_back(new Demo9);
	tests_vec.push_back(new Demo10);
	return tests_vec;
}

//////////////////////////////////////////////////////////////////////////////
// this section is where each Test Driver DLL completely differs from other
// test drivers. Although the same name can be used in all TestDrivers, the 
// actual instance of TestCollection is different in the way it returns 
// different collection of ITest*.

DLL_DECL ITests* get_ITests()
{
	return new TestCollection;
}
