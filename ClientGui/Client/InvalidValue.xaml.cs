/////////////////////////////////////////////////////////////////////////////
// InvalidValue.xaml.cs - window to view text file           			   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
//              ycui21@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This file provides InvalidValue class with the functions to alert the user of inputing invalid value
* This prevent user input the file name with illegal characters and empty author name
* 
* Required Files
* ===============
* InvalidValue.xaml.cs, InvalidValue.xaml, 
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
    /////////////////////////////////////////////////////////////////
    // InvalidValue: to alert user of inputing invalid value
    //
    public partial class InvalidValue : Window
    {
        //----------<Initialize the window>-------------
        public InvalidValue()
        {
            InitializeComponent();
        }

        //----------<Close the window>-------------------
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
