using BnsConsole.Code;
using BnsConsole.Setting;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace BnsConsole
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            List<CCard> CardList = new List<CCard>();
            if (!ConsoleFile.GetCard_By_File(ref CardList))
            {
                MessageBox.Show("木有卡号存在,先添加卡号!");
            }

            Process instance = RunningInstance(Process.GetCurrentProcess());
            if (instance != null)
            {
                MessageBox.Show("控制台进程已经存在,先打开任务管理器结束掉!");
                HandleRunningInstance(instance);
                return;
            };
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            // BnsLogin BL = new BnsLogin();
            // BL.strAccountName = "sylph906@naver.com";
            // BL.strAccountPass = "zhdk7306";
            // BL.emBnsLoginType = em_BnsLoginType.em_BnsLoginType_ActiveCard;
            try
            {
                string strGamePath = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Wow6432Node\plaync\BNS_KOR").GetValue("BaseDir").ToString();
                if (strGamePath.LastIndexOf(@"\") != 0)
                {
                    strGamePath += "\\";
                }
                if (strGamePath.ToLower().IndexOf("plaync\\bns") == -1)
                {
                    MessageBox.Show("注册表修复错误! 现在的路径是:" + strGamePath + @",正确注册表路径应该是:X:\\XX\\plaync\Bns");
                    return;
                }
                strGamePath += @"bin\";

                if (!File.Exists(strGamePath + "zlib1.dll") && File.Exists(System.Environment.CurrentDirectory + @"\CopyFile\zlib1.dll"))
                {
                    File.Copy(System.Environment.CurrentDirectory + @"\CopyFile\zlib1.dll", strGamePath + "zlib1.dll");

                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Main:" + ex.Message);
            }
           

            Application.Run(new Form1());
           
           
           // Application.Run(BL);

            /*while (true)
            {
                string strCmd = Console.ReadLine();
                if (strCmd == "Show")
                {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);
                    Application.Run(new Form1());
                    //new Form1().Show();
                    continue;
                }
                else if (strCmd == "Exit" || strCmd == "Quit")
                {
                    break;
                }
            }*/
        }

       

        

        static private bool ExistProc(string ProcName)
        {
            return Process.GetProcessesByName(ProcName).Length != 0;
        }

        [DllImport("User32.dll")]
        private static extern bool ShowWindowAsync(IntPtr hWnd, int cmdShow);
        [DllImport("User32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);
        private const int WS_SHOWNORMAL = 1;
        /// <summary>
        /// 得到正在运行的进程
        /// </summary>
        /// <returns></returns>
        public static Process RunningInstance(Process Proc)
        {
            Process[] processes = Process.GetProcessesByName(Proc.ProcessName);

            //遍历正在有相同名字运行的进程
            foreach (Process process in processes)
            {
                //忽略现有的进程
                if (process.Id != Proc.Id)
                {
                    //确保进程从EXE文件运行
                    if (process.MainModule.FileName == Proc.MainModule.FileName)
                    {
                        // 返回另一个进程实例
                        return process;
                    }
                }
            }
            //没有其它的进程，返回Null
            return null;
        }
        /// <summary>
        /// 处理正在运行的进程,也就是将其激活
        /// </summary>
        /// <param name="instance">要处理的进程</param>
        public static void HandleRunningInstance(Process instance)
        {
            //确保窗口没有被最小化或最大化
            ShowWindowAsync(instance.MainWindowHandle, WS_SHOWNORMAL);
            //设置真实进程为foreground window
            SetForegroundWindow(instance.MainWindowHandle);
        }
    }
}
