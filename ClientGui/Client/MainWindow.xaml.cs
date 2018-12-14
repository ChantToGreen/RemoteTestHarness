/////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - The main window for client           			   //
// ver 1.0                                                                 //
// Environment: VS2017, WIN10, Surface Pro M3                              //
// Application: Remote Test Harness                                        //
// Author:      Yilin Cui                                                  //
//              ycui21@syr.edu                                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This file provides the functions needed for Client , allows client to send test libraries and
* requests to the test Host. It uses Translater to translate C# message and CPP message back and forth
* 
* Required Files
* ===============
* MainWindow.xaml.cs, MainWindow.xaml,
* Translater.h, CsMessage.h, 
* TextView.xaml.cs,TextView.xaml
* DeleteConfirm.xaml.cs, DeleteConfirm.xaml 
* InvalidValue.xaml.cs, InvalidValue.xaml
* WarningAccess.xaml.cs, WarningAccess.xaml
* WarningReq.xaml.cs, WarningReq.xaml
* WarningRes.xaml.cs, WarningRes.xaml
* 
* Maintenance History:
* ====================
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using MsgPassingCommunication;
using System.Threading;

namespace Client
{
    ////////////////////////////////////////////////////////////////////////////////
    /// MainWindow class: where UI component and functions are bound together
    ///                   Allows user to use this window to create/send new test request
    ///                   view history test request and test log. Also it provides function
    ///                   to manage the server, even though it can only shut down  now       
    public partial class MainWindow : Window
    {
        public InvalidValue iw { get; set; } = null;
        public WarningReq warningWin { get; set; } = null;
        public WarningRes ws { get; set; } = null;
        public WarningAccess wa { get; set; } = null;
        public TextView textWin { get; set; } = null;
        public DeleteConfirm dw { get; set; } = null;
        private string clientSend_ = "../ClientSend/";
        private string clientSave_ = "../ClientSave/";
        private string currentDir_ = "../ClientSend";
        public string author { get; set; } = "Yilin Cui";
        private string defAuthor_ = "Yilin Cui";
        private List<Char> invalidChar_;
        private List<string> selectedLibs_;
        public bool del { get; set; } = false;
        public bool serverRunning { get; set; } = true;
        private Translater translater_;
        private CsEndPoint client_;
        private CsEndPoint server_;
        private Thread receiver_;
        private Dictionary<string, Action<CsMessage>> messageDispatcher_;
        private int pendingRequest_ = 0;

        //------------<Initialize client>-----------------------
        public MainWindow()
        {
            invalidChar_ = new List<char>();
            invalidChar_.Add('\\');
            invalidChar_.Add('|');
            invalidChar_.Add('/');
            invalidChar_.Add('*');
            invalidChar_.Add('?');
            invalidChar_.Add('<');
            invalidChar_.Add('>');
            client_ = new CsEndPoint();
            client_.machineAddress = "localhost";
            Console.WriteLine(App.args[0]);
            int cmdPort = Int32.Parse(App.args[0]);
            Console.WriteLine("Port: {0}",cmdPort);
            client_.port = cmdPort;
            server_ = new CsEndPoint();
            server_.machineAddress= "localhost";
            server_.port = 8083;
            selectedLibs_ = new List<string>();
            initializeDispather();
            InitializeComponent();
            loadDirectories();
            loadRequests();
            loadHistoryRes();
            translater_ = new Translater();
            translater_.listen(client_);
            translater_.setSavePath("../ClientSave");
            translater_.setSendPath("../ClientSend");
            receiver_ = new Thread(processMsg);
            receiver_.IsBackground = true;
            receiver_.Start();
            ClientArress.Text = client_.machineAddress + ":" + client_.port.ToString();
            ServerArress.Text=server_.machineAddress + ":" + server_.port.ToString();
            //if(App.args.ToList<string>().Count>2)
            //    demo(App.args[1], App.args[2]);
        }

        //---------------<Initialize messageDispather>---------
        private void initializeDispather()
        {
            messageDispatcher_ = new Dictionary<string, Action<CsMessage>>();
            messageDispatcher_["display log"] = (CsMessage msg) =>
            {
                --pendingRequest_;
                Console.WriteLine("Showing log!");
                string file = msg.attributes["sendingFile"];
                TestHistory.Items.Add(file);
                System.IO.StreamReader sr = new System.IO.StreamReader(clientSave_ + file);
                string lines = sr.ReadToEnd();
                sr.Close();
                textWin = new TextView();
                textWin.TextViewer.Text = lines;
                textWin.Show();

            };
            messageDispatcher_["server stopped"] = (CsMessage msg) =>
              {
                  ServerStat.Text = ("(Stopped)");
              };
        }


        //-------------<the process for incoming message>--------------
        private void processMsg()
        {
            while(true)
            {
                CsMessage msg = translater_.getMessage();
                if (msg.attributes.ContainsKey("command"))
                {
                    if(msg.attributes["command"]=="Close client")
                    {
                        translater_.close();
                        break;
                    }
                    string key = msg.attributes["command"];
                    Dispatcher.Invoke(messageDispatcher_[key], new object[] { msg });
                }
            }
        }

        //-----------------<Stop receiver thread and gracefully close the window>----------
        private void Window_Closed(object sender, EventArgs e)
        {
            CsMessage msg = new CsMessage();
            msg.add("command", "Close client");
            msg.add("to", CsEndPoint.toString(server_));
            msg.add("from", CsEndPoint.toString(client_));
            if (serverRunning)
                translater_.postMessage(msg);
            Thread.Sleep(10);
            msg.attributes["to"] = CsEndPoint.toString(client_);
            translater_.postMessage(msg);
        }


        //---------------<Navigate into the directory and restore the navigator when read access violation exception occured>---------
        private void Directory_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (Dir.SelectedIndex == -1) return;
            string backup = currentDir_;
            try
            {
                if (Dir.SelectedValue.ToString() == "..")
                {
                    String parent = goAncestor();
                    loadDirectories(parent);
                    Console.WriteLine("Parent: {0}", parent);
                    return;
                }
                string path = currentDir_ + "\\" + Dir.SelectedValue.ToString();
                Console.WriteLine(path);
                if (!System.IO.Directory.Exists(path)) return;
                loadDirectories(path);
            }
            catch(Exception ex)
            {
                Console.Write(ex.Message);
                wa = new WarningAccess();
                wa.Show();
                loadDirectories(backup);
            }
        }


        //-----------------<Add lib to the editing request>-------------
        private void Libs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (Libs.SelectedIndex == -1)
                return;
            string lib = Libs.SelectedValue.ToString();
            string clientPath = System.IO.Path.GetFullPath("../ClientSend");
            string currPath = System.IO.Path.GetFullPath(currentDir_);
            Console.WriteLine(currPath);
            Console.WriteLine(clientPath);
            if (currPath != clientPath)
            {
                string filepath = clientSend_ + lib;
                if (System.IO.File.Exists(filepath))
                {
                    System.IO.File.Delete(filepath);
                }
                System.IO.File.Copy(currentDir_ + "\\" + lib, filepath);
            }
            Req.Items.Add(lib);
            selectedLibs_.Add(lib);
            if (!selectedLibs_.Contains(lib))
            {
                Req.Items.Add(lib);
                selectedLibs_.Add(lib);
            }

        }

        //---------------<Send out the request>------------
        private void sendReq_Click(object sender, RoutedEventArgs e)
        {
            if(!serverRunning)return;
            string req = ReqName.Text;
            if (selectedLibs_.Count == 0||req.Length==0)
            {
                warningWin = new WarningReq();
                warningWin.Show();
                return;
            }
            foreach(char ch in invalidChar_)
            {
                if (req.Contains(ch))
                {
                    warningWin = new WarningReq();
                    warningWin.Show();
                    return;
                }
            }
            string path = clientSend_ + req + ".txt";
            System.IO.StreamWriter sw = new System.IO.StreamWriter(path);
            sw.Write(req + ", "+author);
            string dateTime = System.DateTime.Now.ToString();
            sw.Write(", "+dateTime);
            foreach(string lib in selectedLibs_)
            {
                sw.Write(", "+lib);
                sendFile(lib);
            }
            sw.Close();
            if(!requestHistory.Items.Contains(req+".txt"))
                requestHistory.Items.Add(req + ".txt");
            ++pendingRequest_;
            sendFile(req + ".txt", "testing");
            selectedLibs_.Clear();
            Req.Items.Clear();
            ReqName.Clear();

        }


        //-------------<Send the file>--------------
        private void sendFile(string file,string command="")
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(server_));
            msg.add("from", CsEndPoint.toString(client_));
            msg.add("command", command);
            msg.add("sendingFile", file); ;
            translater_.postMessage(msg);
        }

        //-------------<Remove the library from editing request>----------
        private void removeLib_Click(object sender, RoutedEventArgs e)
        {
            if (selectedLibs_.Count == 0) return;
            if (Req.SelectedIndex == -1) return;
            foreach(string lib in selectedLibs_)
            {
                if(lib==Req.SelectedValue.ToString())
                {
                    selectedLibs_.Remove(lib);
                    break;
                }
            }
            Req.Items.Remove(Req.SelectedItem);
        }


        //-------------<View history request>--------------------
        private void requestHistory_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (requestHistory.SelectedIndex == -1)
                return;
            string path = clientSend_ + requestHistory.SelectedValue.ToString();
            if (!System.IO.File.Exists(path))
            {
                requestHistory.Items.Remove(requestHistory.SelectedValue.ToString());
                ws = new WarningRes();
                ws.Show();
                return;
            }
            textWin = new TextView();
            System.IO.StreamReader sr = new System.IO.StreamReader(path);
            string lines = sr.ReadToEnd();
            sr.Close();
            textWin.TextViewer.Text = lines;
            textWin.Show();
        }

        //-------------<View history request>--------------------
        private void Req_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (selectedLibs_.Count == 0) return;
            if (Req.SelectedIndex == -1) return;
            foreach (string lib in selectedLibs_)
            {
                if (lib == Req.SelectedValue.ToString())
                {
                    selectedLibs_.Remove(lib);
                    break;
                }
            }
            Req.Items.Remove(Req.SelectedItem);
        }

        //-------------<Resend history request>--------------------
        private void Resend_Click(object sender, RoutedEventArgs e)
        {
            if (requestHistory.SelectedIndex == -1|| !serverRunning) return;
            string path = clientSend_ + requestHistory.SelectedValue.ToString();
            if(!System.IO.File.Exists(path))
            {
                requestHistory.Items.Remove(requestHistory.SelectedValue.ToString());
                ws = new WarningRes();
                ws.Show();
                return;
            }
            ++pendingRequest_;
            sendFile(requestHistory.SelectedValue.ToString(), "testing");
        }

        //-------------<Navigate down through directory tree>--------------------
        private void SelectDir_Click(object sender, RoutedEventArgs e)
        {
            if (Dir.SelectedIndex == -1) return;
            string backup = currentDir_;
            try
            {
                if (Dir.SelectedValue.ToString() == "..")
                {
                    String parent = goAncestor();
                    loadDirectories(parent);
                    Console.WriteLine("Parent: {0}", parent);
                    return;
                }
                string path = currentDir_ + "\\" + Dir.SelectedValue.ToString();
                Console.WriteLine(path);
                if (!System.IO.Directory.Exists(path)) return;
                loadDirectories(path);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                wa = new WarningAccess();
                wa.Show();
                loadDirectories(backup);
            }
        }

        //-----------<Search the ancestor directory>--------------
        private string goAncestor()
        {
            if (System.IO.Directory.GetParent(currentDir_)==null) return currentDir_;
            string parent = System.IO.Directory.GetParent(currentDir_).FullName;
            return parent;
                
        }

        //-----------<Add library to editing request>--------------
        private void SelectFile_Click(object sender, RoutedEventArgs e)
        {
            if (Libs.SelectedIndex == -1)
                return;
            string lib = Libs.SelectedValue.ToString();
            string clientPath = System.IO.Path.GetFullPath("../ClientSend");
            string currPath = System.IO.Path.GetFullPath(currentDir_);
            Console.WriteLine(currPath);
            Console.WriteLine(clientPath);
            if (currPath != clientPath)
            {
                string filepath = clientSend_ + lib;
                if (System.IO.File.Exists(filepath))
                {
                    System.IO.File.Delete(filepath);
                }
                System.IO.File.Copy(currentDir_ + "\\" + lib, filepath);
            }
            if (!selectedLibs_.Contains(lib))
            {
                Req.Items.Add(lib);
                selectedLibs_.Add(lib);
            }
        }

        //---------------<Review history test log>-----------------
        private void TestHistory_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (TestHistory.SelectedIndex == -1)
                return;
            string path = clientSave_ + TestHistory.SelectedValue.ToString();
            if(!System.IO.File.Exists(path))
            {
                TestHistory.Items.Remove(TestHistory.SelectedValue.ToString());
                ws = new WarningRes();
                ws.Show();
                return;
            }
            textWin = new TextView();
            System.IO.StreamReader sr = new System.IO.StreamReader(path);
            string lines=sr.ReadToEnd();
            sr.Close();
            textWin.TextViewer.Text = lines;
            textWin.Show();
            
        }

        //---------------------<View history test log>-------------------------
        private void ViewText_Click(object sender, RoutedEventArgs e)
        {
            if (TestHistory.SelectedIndex == -1)
                return;
            string path = clientSave_ + TestHistory.SelectedValue.ToString();
            if (!System.IO.File.Exists(path))
            {
                TestHistory.Items.Remove(TestHistory.SelectedValue.ToString());
                ws = new WarningRes();
                ws.Show();
                return;
            }
            textWin = new TextView();
            System.IO.StreamReader sr = new System.IO.StreamReader(path);
            string lines = sr.ReadToEnd();
            sr.Close();
            textWin.TextViewer.Text= lines;
            textWin.Show();
        }

       //-----------------------<Load the diretories and libs in the GUI>------------------
        private void loadDirectories(string path= "../ClientSend")
        {
                Dir.Items.Clear();
                Libs.Items.Clear();
                currentDir_ = path;
                string[] dirs = System.IO.Directory.GetDirectories(path);
                Dir.Items.Add("..");
                foreach (string dir in dirs)
                {
                    System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(dir);
                    Dir.Items.Add(di.Name);
                }
                string[] files = System.IO.Directory.GetFiles(currentDir_, "*.dll");
                foreach (string file in files)
                {
                    string name = System.IO.Path.GetFileName(file);
                    Libs.Items.Add(name);
                }

        }

        //--------------------<Load history requests in the GUI>------------------
        private void loadRequests()
        {
            requestHistory.Items.Clear();
            string path = "../ClientSend";
            string[] files = System.IO.Directory.GetFiles(path, "*.txt");
            foreach(string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                requestHistory.Items.Add(name);
            }
        }

        //--------------------<Load history test logs in the GUI>------------------
        private void loadHistoryRes()
        {
            TestHistory.Items.Clear();
            string path = "../ClientSave";
            string[] files = System.IO.Directory.GetFiles(path, "*.txt");
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                TestHistory.Items.Add(name);
            }
        }

       
        //-----------------------<Delete the history request from GUI and folder>--------------
        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            if (requestHistory.SelectedIndex == -1) return;
            string path = clientSend_ + requestHistory.SelectedValue.ToString();
            if (!System.IO.File.Exists(path))
            {
                requestHistory.Items.Remove(requestHistory.SelectedItem);
                ws = new WarningRes();
                ws.Show();
                return;
            }
            dw = new DeleteConfirm(this);
            dw.ShowDialog();
            if (del)
            {
                System.IO.File.Delete(path);
                requestHistory.Items.Remove(requestHistory.SelectedItem);
            }

        }


        //-------------------------<View history request in TextViewer>-------------------
        private void ViewReq_Click(object sender, RoutedEventArgs e)
        {
            if (requestHistory.SelectedIndex == -1)
                return;
            string path = clientSend_ + requestHistory.SelectedValue.ToString();
            if (!System.IO.File.Exists(path))
            {
                Console.WriteLine(path);
                requestHistory.Items.Remove(requestHistory.SelectedValue.ToString());
                ws = new WarningRes();
                ws.Show();
                return;
            }
            textWin = new TextView();
            System.IO.StreamReader sr = new System.IO.StreamReader(path);
            string lines = sr.ReadToEnd();
            sr.Close();
            textWin.TextViewer.Text = lines;
            textWin.Show();

        }


        //--------------------<Delete history test log from GUI and folder>------------
        private void DeleteRes_Click(object sender, RoutedEventArgs e)
        {
            if (TestHistory.SelectedIndex == -1) return;
            string path = clientSave_ + TestHistory.SelectedValue.ToString();
            if (!System.IO.File.Exists(path))
            {
                TestHistory.Items.Remove(TestHistory.SelectedItem);
                ws = new WarningRes();
                ws.Show();
                return;
            }
            dw = new DeleteConfirm(this);
            dw.ShowDialog();
            if (del)
            {
                System.IO.File.Delete(path);
                TestHistory.Items.Remove(TestHistory.SelectedItem);
            }
        }


        //--------------<Change the author for the request>------------------
        private void ComfirmAuthor_Click(object sender, RoutedEventArgs e)
        {
            if(AuthorName.Text=="")
            {
                AuthorName.Text = author;
                iw = new InvalidValue();
                iw.Show();
                return;
            }
            author = AuthorName.Text;
        }

        
        //------------<Restore author to default value>-------------
        private void DefaultAuthor_Click(object sender, RoutedEventArgs e)
        {
            author = defAuthor_;
            AuthorName.Text = author;
        }


        //-----------<Shut down server gracefully>----------
        private void KillServer_Click(object sender, RoutedEventArgs e)
        {
            if (!serverRunning) return;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(server_));
            msg.add("from", CsEndPoint.toString(client_));
            msg.add("command", "stop");
            translater_.postMessage(msg);

        }

        //------------<Send demo>--------------
        public void demo(string req, string lib)
        {
            if (req == "no demo") return;
            sendFile(lib);
            sendFile(req, "testing");
            requestHistory.Items.Add(req);
        }
    }
}
