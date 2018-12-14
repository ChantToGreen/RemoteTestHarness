/////////////////////////////////////////////////////////////////////////////
// WarningAccess.xaml.cs - window to view text file           			   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
//              ycui21@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This file provides WarningAccess class to warn the users that they don't have access to certain directory
* 
* Required Files
* ===============
* WarningAccess.xaml.cs, WarningAccess.xaml, 
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
    /// WarningAccess class: warns the user of not having access to
    ///                      certain directory before deny their request
    public partial class WarningAccess : Window
    {
        public WarningAccess()
        {
            InitializeComponent();
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
