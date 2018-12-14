#pragma once
/////////////////////////////////////////////////////////////////////////
// Translater.h - Translates messages to/from managed and native types //
// ver 1.0                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  This C++\Cli Package contains one class, Translater.  It's purpose is to convert
*  managed messages, CsMessage, to native messages, Message, and back.
*
*  Required Files:
* -----------------
*  Translater.h, Translater.cpp
*  CsMessage.h
*  Required References:
* ----------------------
*  CommLibWrapper.dll
*  Message.lib
*  
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 3/27/2018
*  - first release
*/

// Project > Properties > C/C++ > Language > Conformance mode set to No
// Add references to System, System.Core

#include <string>
#include "../CppCommWithFileXfer/MsgPassingComm/IComm.h"
#include "CsMessage.h"
#include "../CommLibWrapper/CommLibWrapper.h"

using namespace System;
using namespace System::Text;

namespace MsgPassingCommunication
{
  public ref class Translater
  {
  public:
    Translater();
    void listen(CsEndPoint^ ep);
    void postMessage(CsMessage^ msg);
    CsMessage^ getMessage();
    CsMessage^ fromMessage(Message& msg);
    Message fromCsMessage(CsMessage^ csMsg);
	void setSendPath(System::String^path);
	void setSavePath(System::String^path);
	void close();
	String^ getSendpath();
	String^ getSavePath();
  private:
    CsEndPoint^ ep_;
    MsgPassingCommunication::CommFactory* pFactory;
    MsgPassingCommunication::IComm* pComm;
  };
  //----< initialize endpoint >----------------------------------------

  Translater::Translater()
  {
    ep_ = gcnew CsEndPoint;
  }
  //----< set client listen endpoint and start Comm >------------------

  void Translater::listen(CsEndPoint^ ep)
  {
    std::cout << "\n  using CommFactory to create instance of Comm on native heap";
    ep_->machineAddress = ep->machineAddress;
    ep_->port = ep->port;
    pFactory = new CommFactory;
    pComm = pFactory->create(Sutils::MtoNstr(ep_->machineAddress), ep_->port);
    pComm->start();
    delete pFactory;
  }
  //----< convert native message to managed message >------------------

  CsMessage^ Translater::fromMessage(Message& msg)
  {
    CsMessage^ csMsg = gcnew CsMessage;
    Message::Attributes attribs = msg.attributes();
    for (auto attrib : attribs)
    {
      csMsg->add(Sutils::NtoMstr(attrib.first), Sutils::NtoMstr(attrib.second));
    }
    return csMsg;
  }
  //----< convert managed message to native message >------------------

  Message Translater::fromCsMessage(CsMessage^ csMsg)
  {
    Message msg;
    auto enumer = csMsg->attributes->GetEnumerator();
    while (enumer.MoveNext())
    {
      String^ key = enumer.Current.Key;
      String^ value = enumer.Current.Value;
      msg.attribute(Sutils::MtoNstr(key), Sutils::MtoNstr(value));
    }
    return msg;
  }

  //------<Set the send path for send files>---------------------------
  inline void Translater::setSendPath(System::String ^ path)
  {
	  if(System::IO::Directory::Exists(path))
	  pComm->setSendFilePath(Sutils::MtoNstr(path));
  }

  //-----<Set the save file path for comm>-----------------------------
  inline void Translater::setSavePath(System::String ^ path)
  {
	  if (System::IO::Directory::Exists(path))
		  pComm->setSaveFilePath(Sutils::MtoNstr(path));
  }

  //-----<Order the comm to stop>-----------------
  inline void Translater::close()
  {
	  pComm->stop();
  }

  //-----<Get CS string for send file path>-----------------
  inline String^ Translater::getSendpath()
  {
	  return Sutils::NtoMstr(pComm->getSendFilePath());
  }

  //-----<Get CS string for save file path>-----------------
  inline String^ Translater::getSavePath()
  {
	  return Sutils::NtoMstr(pComm->getSaveFilePath());
  }
  //----< use Comm to post message >-----------------------------------

  inline void Translater::postMessage(CsMessage^ csMsg)
  {
    std::cout << "\n  posting message in Translater";
    Message msg = this->fromCsMessage(csMsg);
    pComm->postMessage(msg);
  }
  //----< get message from Comm >--------------------------------------

  inline CsMessage^ Translater::getMessage()
  {
    std::cout << "\n  getting message in Translater";
    Message msg = pComm->getMessage();
    return fromMessage(msg);
  }
}