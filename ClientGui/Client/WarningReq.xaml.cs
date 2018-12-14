/////////////////////////////////////////////////////////////////////////////
// WarningReq.xaml.cs - window to view text file           			       //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
//              ycui21@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This file provides WarningReq class to warn the users that the request they want to access has already been deleted
*
* Required Files
* ===============
* WarningReq.xaml.cs, WarningReq.xaml, 
*
* Maintenance History:
* ====================
* 
* ver 1.0 : 27 Nov 2018
* - first release
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
   //////////////////////////////////////////////////////////////////
   /// WarningReq: notify the user the request has already been deleted
   /// 
    public partial class WarningReq : Window
    {
        //----------------<Initialize the window>---------------------
        public WarningReq()
        {
            InitializeComponent();
        }

        //---------------<Close the window>--------------------------
        private void Close_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
