/////////////////////////////////////////////////////////////////////////////
// DeleteConfirm.xaml.cs - window to view text file           			   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
//              ycui21@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This file provides DeleteConfirm class with the functions to let user think twice before they delete
* previous test requests or test logs
* 
* Required Files
* ===============
* DeleteConfirm.xaml.cs, DeleteConfirm.xaml, 
* MainWindow.xaml.cs, MainWindow.xaml
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
    //////////////////////////////////////////////////////////
    ///DeleteConfirm class: give user the opportunity to regret
    ///
    public partial class DeleteConfirm : Window
    {
        private MainWindow mw_ = null;

        //------------------<Initialize the window>-------------
        public DeleteConfirm(MainWindow mw)
        {
            mw_ = mw;
            InitializeComponent();
        }

        //----------------<If user still wants to delete>--------------
        private void Yes_Click(object sender, RoutedEventArgs e)
        {
            mw_.del = true;
            Close();
        }

        //----------------<If user regrets>-------------------
        private void No_Click(object sender, RoutedEventArgs e)
        {
            mw_.del = false;
            Close();
        }
    }
}
