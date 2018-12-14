/////////////////////////////////////////////////////////////////////////////
// DemoReq.cpp : Demonstration of project 3      						   //
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
*  ver 2.0 : 07 Oct 2018
*  - replaced all tests
*  - Ammar's infrastructure has not been modified
*  ver 1.0 - first release
*/

#define IN_DLL
#define ARG_SIZE 256

#include "../ITests.h"
#include"../../CppCommWithFileXfer/Utilities/FileUtilities/FileUtilities.h"
#include <iostream>
#include <string>
#include <string.h>

using namespace Utilities;
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
	pRes_->write("\nDemonstrating Project3 requirement #1: using VS2017 and console project\n");
	pRes_->write("-----------------------------------------------------------------------\n");
	pRes_->write("This is VS2017 and console project, trust me\n");
	pRes_->write("=======================================================================\n");
    return true;
  }
  DLL_DECL std::string name()
  {
	  return "Demo Project3 Requirement #1";
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
  ILog* pRes_ = nullptr;
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

	pRes_->write("\nDemonstrating Project3 requirement #2: using standard I/O stream, new and delete\n");
	pRes_->write("--------------------------------------------------------------------------------\n");
	pRes_->write("Showing the code where uses standard I/O stream\n");
	pRes_->write("Line::110 is where standard I/O stream is used");
	std::string path1 = "../Host/Host.h";
	Utilities::showFileLines(path1, 108, 114);
	pRes_->write("Showing the code where uses shared pointer\n");
	std::string path2 = "../Utilities/TestUtilities/TestUtilities.h";
	pRes_->write("Line::118 is where shared pointer is used\n");
	Utilities::showFileLines(path2, 114, 121);
	pRes_->write("=======================================================================\n");
    return true;
  }
  DLL_DECL std::string name()
  {
    return "Demo Project3 Requirement #2";
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
  ILog* pRes_ = nullptr;
};
//////////////////////////////////////////////////////////////////
// Demo2: demonstration of requirement 3

class Demo3 : public ITest {
public:
  DLL_DECL bool test()
  {
    if (pRes_ == nullptr)
    {
      std::cout << "\n  no logger available";
      return false;
    }

	pRes_->write("\nDemonstrating Project3 requirement #3: provide and use ProcessPoll for TestHarness\n");
	pRes_->write("--------------------------------------------------------------------------------\n");
	pRes_->write("Showing the code fragment where Host spawn and respawn child test process\n");
	std::string path1 = "../Host/Host.h";
	Utilities::showFileLines(path1, 193, 240);
	pRes_->write("To make the test message easy to read, I only use one child process for testing,\nThe second child process can run as well if you have enough test loads");
	pRes_->write("If you close the child process while the Host hasn't received the order to shut down,\n a new child process will be respawned\n");
	pRes_->write("=======================================================================\n");
	return true; 
  }
  DLL_DECL std::string name()
  {
    return "Demo Project3 Requirement #3";
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
  ILog* pRes_ = nullptr;
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

		pRes_->write("\nDemonstrating Project3 requirement #4: provide TestHarness and ChildTester\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Showing whereabout the header files Tester and ChildProcess\n");
		Utilities::showDirContents("../TestHarnessCore", "",{ "*.h" });
		Utilities::showDirContents("../ChildTester","", { "*.h" });
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project3 Requirement #4";
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

		pRes_->write("\nDemonstrating Project3 requirement #5: ChildTester shall use logging facility\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Showing the code were ChildTester uses log and send back log to Client\n");
		std::string path = "../ChildTester/ChildProcess.h";
		Utilities::showFileLines(path, 82, 119);
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project3 Requirement #5";
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

		pRes_->write("\nDemonstrating Project3 requirement #5: Shall have client to send request message\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("Showing the code were ChildTester uses log and send back log to Client\n");
		std::string path = "../ChildTester/ChildProcess.h";
		Utilities::showFileLines(path, 81, 119);
		pRes_->write("Test log of this request can be found in clientSave directory after the test\n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project3 Requirement #6";
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

		pRes_->write("\nDemonstrating Project3 requirement #5: Shall have automated unit test suite\n");
		pRes_->write("--------------------------------------------------------------------------------\n");
		pRes_->write("All above are automated unit test suite, as you can see, it is running on one of the child process in test harness\n");
		pRes_->write("=======================================================================\n");
		return true;
	}
	DLL_DECL std::string name()
	{
		return "Demo Project3 Requirement #7";
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


