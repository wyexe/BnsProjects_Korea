using BnsConsole.Code;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BnsConsole
{
    public partial class ServerPrompt : Form
    {
        public ServerPrompt()
        {
            InitializeComponent();
        }

        private void ServerPrompt_Load(object sender, EventArgs e)
        {
            lbGetCardMsg.Visible = false;
            new Thread(_PromptThread).Start();
           
        }
        private string GetIp_By_Config()
        {
            var sbValue = new StringBuilder(64);
            Code.API.GetPrivateProfileString("Config", "Server", "Error", sbValue, 64, System.Environment.CurrentDirectory + @"\\Config.ini");
            switch (sbValue.ToString())
            {
                case "Server":
                    return "121.199.33.217";
                case "Local":
                    return "192.168.0.102";
                case "Test"://return "59.8.163.201";
                    return "120.27.136.99";
                case "Server2":
                   return "153.36.233.103";
                case "MY":
                    return "100.81.157.169";
                default:
                    break;
            }
            MessageBox.Show("Config下的Server是什么鬼?");
            return "";
        }
        private bool IsTestMode()
        {
            var sbValue = new StringBuilder(64);
            Code.API.GetPrivateProfileString("Config", "Test", "0", sbValue, 64, System.Environment.CurrentDirectory + @"\\Config.ini");
            return sbValue.ToString() == "1";
        }
        public bool CheckLeastVersion()
        {
            ConsoleFile.SetGamePath();
            
            string strFilePath = System.Environment.CurrentDirectory + @"\UpdateList.inf";
            if (!HttpDownloadFile(@"http://" + GetIp_By_Config() + ":60888/UpdateList.inf", strFilePath))
                return true;

            if (!File.Exists(strFilePath))
                return false;

            System.Text.StringBuilder sbText = new System.Text.StringBuilder(32);
            Code.API.GetPrivateProfileString("Config", "LeastFileVersion", "0", sbText, 32, strFilePath);
            float fLeastFileVersion = float.Parse(sbText.ToString());

            Code.API.GetPrivateProfileString("Config", "FileVersion", "0", sbText, 32, System.Environment.CurrentDirectory + @"\tmpDownload\UpdateList.inf");
            float fFileVersion = float.Parse(sbText.ToString());

            File.Delete(strFilePath);
            if (fFileVersion < fLeastFileVersion)
            {
                MessageBox.Show("最低版本要求是:" + fLeastFileVersion.ToString());
                return false;
            }

            return true;
        }
        public bool HttpDownloadFile(string url, string path)
        {
            if (GetIp_By_Config().IndexOf("192.168") != -1)
                return false;

            try
            {
                // 先删除该文件
                if (File.Exists(path))
                    File.Delete(path);

                FileStream FStream = new FileStream(path, FileMode.Create);
                long SPosition = 0;
                HttpWebRequest myRequest = (HttpWebRequest)HttpWebRequest.Create(url);
                if (SPosition > 0)
                    myRequest.AddRange((int)SPosition);             //设置Range值

                //向服务器请求,获得服务器的回应数据流
                WebResponse _WebResponse = myRequest.GetResponse();
                long lgSize = _WebResponse.ContentLength;
                Invoke(new MethodInvoker(delegate()
                {
                    lbPrompt.Text = String.Format("正在下载:{0},大小:{1}", FStream.Name, lgSize);
                    lbGetCardMsg.Visible = true;
                }));
                Stream myStream = _WebResponse.GetResponseStream();
                //定义一个字节数据
                byte[] btContent = new byte[512];
                long nRecvSize = 0;
                int intSize = myStream.Read(btContent, 0, 512);
                while (intSize > 0)
                {
                    nRecvSize += intSize;

                    Invoke(new MethodInvoker(delegate()
                    {
                        lbPrompt.Text = String.Format("下载进度:%{0}", nRecvSize * 100 / lgSize);
                        lbGetCardMsg.Visible = true;
                    }));
                   

                    FStream.Write(btContent, 0, intSize);
                    intSize = myStream.Read(btContent, 0, 512);
                }
                //关闭流
                FStream.Close();
                myStream.Close();
                return true;
            }
            catch (Exception ex)
            {
                this.Text = "下载文件失败:" + ex.Message;
                System.Threading.Thread.Sleep(-1);
            }

            return false;
        }

        private bool ExistGameClient()
        {
            var ProcArray = Process.GetProcessesByName("Client");
            return ProcArray.Length > 0;
        }

        private void TermGameClient()
        {
            while (true)
            {
                var ProcArray = Process.GetProcessesByName("Client");
                if (ProcArray.Length == 0)
                    break;


                ProcArray[0].Kill();
                Thread.Sleep(500);
            }
        }

        private void CheckGameClient()
        {
            if (ExistGameClient())
            {
                var TipFrm = new ConsoleForm.TipForm();
                TipFrm.SetText("游戏的进程并没有被关闭! 正在关闭……");
                TermGameClient();
                TipFrm.Close();
            }
            
        }

        private void _PromptThread()
        {
            try
            {
                CheckGameClient();
                Invoke(new MethodInvoker(delegate() { lbPrompt.Text = "优化游戏中……"; }));
                Code.API.Init(Code.ConsoleFile.GetGamePath(), int.Parse(Form1.GetValue_By_Key_For_Config("Optimize")));
                Invoke(new MethodInvoker(delegate() { lbPrompt.Text = "获取公告中……"; }));
                if (!IsTestMode() && !CheckLeastVersion())
                {
                    API.SetGameRun(0x2);
                }
                Code.ConsoleFile.SetMaxFps();

                StringBuilder sbPrompt = new StringBuilder(1024);
                if (Code.API.GetServerTitle(sbPrompt) == 0)
                    Code.API.SetGameRun(0x2);
 
                Invoke(new MethodInvoker(delegate()
                {
                    lbPrompt.Text = sbPrompt.ToString();
                    lbGetCardMsg.Visible = true;
                }));

                if (!File.Exists(System.Environment.CurrentDirectory + @"\\Setting\\Account.txt"))
                    new StreamWriter(System.Environment.CurrentDirectory + @"\\Setting\\Account.txt", false, Encoding.Unicode).Close();
                if (!File.Exists(System.Environment.CurrentDirectory + @"\\Setting\\Card.txt"))
                    new StreamWriter(System.Environment.CurrentDirectory + @"\\Setting\\Card.txt", false, Encoding.Unicode).Close();
                if (!File.Exists(System.Environment.CurrentDirectory + @"\\Setting\\TaskUpgradeSchedule.txt"))
                    new StreamWriter(System.Environment.CurrentDirectory + @"\\Setting\\TaskUpgradeSchedule.txt", false, Encoding.Unicode).Close();
                if (!File.Exists(System.Environment.CurrentDirectory + @"\\Setting\\AccountLog.txt"))
                    new StreamWriter(System.Environment.CurrentDirectory + @"\\Setting\\AccountLog.txt", false, Encoding.Unicode).Close();
                if (!File.Exists(System.Environment.CurrentDirectory + @"\\Setting\\PlayerDaily.txt"))
                    new StreamWriter(System.Environment.CurrentDirectory + @"\\Setting\\PlayerDaily.txt", false, Encoding.Unicode).Close();
                if (!Directory.Exists(System.Environment.CurrentDirectory + @"\\SaveLog"))
                    Directory.CreateDirectory(System.Environment.CurrentDirectory + @"\\SaveLog");
                if (!Directory.Exists(System.Environment.CurrentDirectory + @"\\Setting\\Screenshot\\"))
                    Directory.CreateDirectory(System.Environment.CurrentDirectory + @"\\Setting\\Screenshot\\");
                
                List<Code.CAccount> lst = new List<Code.CAccount>();
                if (!Code.ConsoleFile.GetAccount_By_File(ref lst))
                {
                    MessageBox.Show("木有帐号存在,先添加帐号!");
                    Process.Start(System.Environment.CurrentDirectory + @"\\Setting\\Account.txt");
                    System.Threading.Thread.Sleep(5000);
                    Code.API.SetGameRun(0x2);
                }

                List<CCard> CardList = new List<CCard>();
                if (!ConsoleFile.GetCard_By_File(ref CardList))
                {
                    MessageBox.Show("木有卡号存在,先添加卡号!");
 
                    Process.Start(System.Environment.CurrentDirectory + @"\\Setting\\Card.txt");
                    System.Threading.Thread.Sleep(5000);
                    Code.API.SetGameRun(0x2);
                }

                CCardFunction.GetCardInfo_By_Server(CardList);
    
                // 清理一下
                new SettingConfig().ClearConfig();
            }
            catch (Exception ex)
            {
                MessageBox.Show("_PromptThread:" + ex.Message);
            }
            Invoke(new MethodInvoker(delegate()
            {
                this.Close();
            }));
        }
    }
}
