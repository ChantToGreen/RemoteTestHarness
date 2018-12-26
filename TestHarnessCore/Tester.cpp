/////////////////////////////////////////////////////////////////////
// Tester.cpp - Prototype for Test Harness child Tester            //
// ver 1.0                                                         //
// Author: Yilin Cui, ycui21@syr.edu                               //
// Origin: Dr. Jim Fawcett's website                               //
/////////////////////////////////////////////////////////////////////

#include "Tester.h"
#include "../Dll_Loader_With_Hosted_Resource/ITests.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Utilities/FileSystem/FileSystem.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>

namespace Testing
{
	//----< initialize paths and logger >------------------------------

	Tester::Tester()
	{
		requestPath("../TestRequests");
		libPath("../testLibraries");
		pLog(Utilities::Logger<0, Utilities::Lock>::getInstance());
		pLog()->setTerminator("");
	}
	//----< deallocate logger >----------------------------------------

	Tester::~Tester()
	{
		delete pLog();
	}

	//---------------------<Load individual test request>---------------
	void Tester::loadTestRequest(const std::string& path)
	{
		std::ifstream in;
		if (!Utilities::openReadFile(requestPath() + "\\" + path, in))
			return;
		std::ostringstream sstr;
		sstr << in.rdbuf();
		TestRequest tr = TestRequest::FromString(sstr.str());
		requests.valueRef().push_back(tr);
		pLog()->write("\n  loaded TestRequest \"" + tr.name() + "\"");
		pLog()->write("\n  " + tr.toString() + "\n");
	}
	//----< load Test Requests >-----------------------------------------
	/*
	* - In Project #4 serialized TestRequests, sent by client, will be
	*   extracted from Comm channel.
	*/
	void Tester::loadTestRequests()
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(requestPath());
		std::ifstream in;
		for (auto file : files)
		{
			if (!Utilities::openReadFile(requestPath() + "\\" + file, in))
				continue;
			std::ostringstream sstr;
			sstr << in.rdbuf();
			TestRequest tr = TestRequest::FromString(sstr.str());
			requests.valueRef().push_back(tr);
			pLog()->write("\n  loaded TestRequest \"" + tr.name() + "\"");
			pLog()->write("\n  " + tr.toString() + "\n");
		}
	}
	//----< load libraries >---------------------------------------------
	/*
	* - Loads all libraries specified in a TestRequest from libPath.
	* - Uses DllLoader member, dllLoader_, to load dlls and extract
	*   their tests.
	*/
	void Tester::loadLibraries(TestRequest& tr)
	{
		std::string path = FileSystem::Path::getFullFileSpec(libPath());
		pLog()->write("\n  loading from \"" + path + "\"");

		std::vector<std::string> files;
		for (Dll dll : tr.request.valueRef())
		{
			files.push_back(dll);
		}

		for (auto file : files)
		{
			pLog()->write("\n    " + file);
			std::string fileSpec = path + "\\" + file;
			dllLoader_.loadAndExtract(fileSpec);
		}
		pLog()->write("\n");
	}
	//----< executes tests >--------------------------
	/*
	* - Executes tests held by DllLoader instance,
	*   using TestUtilities infrastructure.
	*/
	bool Tester::doTests()
	{
		using sPtr = std::shared_ptr<ITest>;
		Utilities::TestExecutive te;
		for (ITest* test : dllLoader_.tests()) {
			// Host passes logger resource pointer to test
			// - test will use logger via this pointer
			test->acceptHostedResource(pLog.valueRef());
			sPtr pTest(test);
			te.registerTest(pTest, pTest->name());
		}
		bool result = te.doTests();
		return result;
	}
}

using namespace Testing;
#ifdef TEST_TESTHARNESSCORE
int main()
{
	std::cout << Utilities::sTitle("Testing Tester using Dll Loader and Logger");

	TestRequest tr;
	Tester tester;

	tester.loadTestRequests();

	for (TestRequest& tr : tester.requests())
	{
		tester.loadLibraries(tr);
		tester.doTests();
		tester.clear();
	}

	Utilities::putline(2);
	getchar();
	return 0;

}
#endif
