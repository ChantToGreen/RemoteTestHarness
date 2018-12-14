/////////////////////////////////////////////////////////////////////////////
// App.xaml.cs - configures the startup environment for Client 			   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
//              ycui21@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This file provides App class: configures the startup environment for Client
* 
* Required Files
* ===============
* App.xaml.cs, App.xaml, 
*
* Maintenance History:
* ====================
* 
* ver 1.0 : 27 Nov 2018
* - first release
*/
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    /////////////////////////////////////////////////////////////
    /// App class: configures the startup environment for Client
    /// 
    public partial class App : Application
    {
        static public string[] args { get; set; }
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            args = e.Args;

        }
    }
}
