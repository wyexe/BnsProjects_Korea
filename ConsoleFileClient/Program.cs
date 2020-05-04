using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ConsoleFileClient
{
    public class _ConsoleFileSrc
    {
        public string _strFileName = string.Empty;
        public string _strFileMd5 = string.Empty;

        public _ConsoleFileSrc(string strFileName)
        {
            _strFileName = strFileName;
            _strFileMd5 = GetMD5HashFromFile(System.Environment.CurrentDirectory + @"\tmpDownload\" + _strFileName);
        }

        public _ConsoleFileSrc()
        {

        }

        public static string GetMD5HashFromFile(string fileName)
        {
            try
            {
                FileStream file = new FileStream(fileName, System.IO.FileMode.Open);
                MD5 md5 = new MD5CryptoServiceProvider();
                byte[] retVal = md5.ComputeHash(file);
                file.Close();
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < retVal.Length; i++)
                {
                    sb.Append(retVal[i].ToString("x2"));
                }
                return sb.ToString();
            }
            catch (Exception ex)
            {
                Console.WriteLine("计算文件md5错误:" + "GetMD5HashFromFile() fail,error:" + ex.Message);
                
            }
            return "";
        }  
    }
    class Program
    {
        public static readonly String ClientVersion = "1.01";
        private static string strIp = "";
        private static bool SetIp_By_Config()
        {
            var sbValue = new StringBuilder(64);
            GetPrivateProfileString("Config", "Server", "Error", sbValue, 64, System.Environment.CurrentDirectory + @"\\Config.ini");
            switch (sbValue.ToString())
            {
                case "Server": case "Local":
                    Program.strIp = "121.199.33.217";
                    break;
                case "Test":
                    //Program.strIp = "59.8.163.201";
                    Program.strIp = "120.27.136.99";
                    break;
                case "Server2":
                    Program.strIp = "153.36.233.103";
                    break;
                case "MY":
                    Program.strIp = "120.24.4.137";
                    break;
                default:
                    return false;
            }
            return sbValue.ToString() != "Error";
        }

        public static void Decompress(FileInfo fileToDecompress)
        {
            using (FileStream originalFileStream = fileToDecompress.OpenRead())
            {
                string currentFileName = fileToDecompress.FullName;
                string newFileName = currentFileName.Remove(currentFileName.Length - fileToDecompress.Extension.Length);

                using (FileStream decompressedFileStream = File.Create(newFileName))
                {
                    using (GZipStream decompressionStream = new GZipStream(originalFileStream, CompressionMode.Decompress))
                    {
                        decompressionStream.CopyTo(decompressedFileStream);
                        Console.WriteLine("Decompressed: {0}", fileToDecompress.Name);
                    }
                }
            }
        }

        private static void DecompressFile()
        {
            var DirInfo = new DirectoryInfo(System.Environment.CurrentDirectory + @"\tmpDownload\");
            var FileArray = DirInfo.GetFiles();

            foreach (var item in FileArray)
            {
                if (item.Name.IndexOf(".gz") == -1)
                    continue;

                Decompress(item);
            }
        }

        static void Main(string[] args)
        {
            // Create My Directory
            CreateDirectory();

            if (!SetIp_By_Config())
            {
                Console.WriteLine("Config里的Server=这个填的是什么鬼?");
                Console.Read();
                return;
            }

            // Download Md5 file
            do
            {
                HttpDownloadFile(@"http://" + Program.strIp + ":60888/UpdateList.inf", System.Environment.CurrentDirectory + @"\UpdateList.inf");
            } while (GetUpdateListValue("Lock") == "1" && Sleep(5 * 1000));

            TerminateProcess("BnsConsole");
            TerminateProcess("Client");
            TerminateProcess("WebLogin");

            // Comp Client Md5
            if (ClientVersion != GetUpdateListValue("ConsoleFileClientVersion"))
            {
                Console.WriteLine(String.Format("自动更新器有新的版本[{0}]发布了……旧版本[{1}]已经抛弃了!", GetTmpUpdateListValue("ConsoleFileClientVersion"), ClientVersion));
                File.Delete(System.Environment.CurrentDirectory + @"\UpdateList.inf");
                Console.ReadLine();
                return;
            }
            if (!CheckUpdateFile())
            {
                Console.WriteLine("文件版本已经是最新的了, 不必下载了!");
                File.Delete(System.Environment.CurrentDirectory + @"\UpdateList.inf");
                MoveLpk();
                Process.Start(System.Environment.CurrentDirectory + @"\BnsConsole.exe", " -UpdateRun");
                System.Threading.Thread.Sleep(5000);
                return;
            }

            // Comp tmpDownload File
            var DownloadList = GetDownloadFile(GetTmpDownloadFileMd5(), GetServerFileMd5());
            if (!DownLoadFile(DownloadList))
            {
                Console.WriteLine("更新失败! 等待10s后重新请求");
                File.Delete(System.Environment.CurrentDirectory + @"\UpdateList.inf");
                return;
            }

           

            // Move File to Directory
            MoveFileToDirectory();
            if (CheckUpdateFile())
            {
                Console.WriteLine("更新失败!");
                System.Threading.Thread.Sleep(-1);
            }


            File.Delete(System.Environment.CurrentDirectory + @"\UpdateList.inf");
            // Move Sp File
            MoveLpk();

            

            Console.WriteLine("更新完毕!");
            var ProcArray = Process.GetProcessesByName("BnsConsole");
            
            Process.Start(System.Environment.CurrentDirectory + @"\BnsConsole.exe"," -UpdateRun");
            System.Threading.Thread.Sleep(5000);
        }

        public static bool CheckUpdateFile()
        {
            // 2个版本不一样的时候需要更新
            var ServerFileVersion = GetUpdateListValue("FileVersion");
            var LocalFileVersion = GetTmpUpdateListValue("FileVersion");
            if (ServerFileVersion != LocalFileVersion)
            {
                Console.WriteLine(String.Format("本地文件版本:{0}, 不等于服务器文件版本{1}", LocalFileVersion, ServerFileVersion));
                return true;
            }

            // 把tmpDownload的文件和真实计算文件的MD5 对比一下!
            var tmpDownLoadFileList = GetTmpDownloadFileMd5();
            var ServerDownLoadFileList = GetServerFileMd5();
            if (tmpDownLoadFileList.Count != ServerDownLoadFileList.Count)
            {
                Console.WriteLine(String.Format("本地文件数量={0}, 不等于服务器文件数量={1}", tmpDownLoadFileList.Count, ServerDownLoadFileList.Count));
                return true;
            }

            // 检查MD5
            foreach (var ServerItem in ServerDownLoadFileList)
            {
                var LocalItem = tmpDownLoadFileList.Find(itm => itm._strFileName == ServerItem._strFileName);
                if (LocalItem == null)
                {
                    Console.WriteLine(String.Format("Server文件{0}, 本地不存在!", ServerItem._strFileName));
                    return true;
                }

                if (LocalItem._strFileMd5 != ServerItem._strFileMd5)
                {
                    Console.WriteLine(String.Format("Server文件{0}的MD5 != 本地文件的MD5", ServerItem._strFileName));
                    return true;
                }
            }

            return false;
        }

        public static void TerminateProcess(string strName)
        {
            var ProcArray = Process.GetProcessesByName(strName);
            do
            {
                if (ProcArray.Length > 0)
                    ProcArray[0].CloseMainWindow();

                Console.WriteLine("正在关闭" + strName);
                Thread.Sleep(1000);
                ProcArray = Process.GetProcessesByName(strName);
            } while (ProcArray.Length > 0);
        }

        static void MoveLpk()
        {
            try
            {
               // var strGamePath = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Wow6432Node\plaync\BNS_KOR").GetValue("BaseDir").ToString();
               // strGamePath += @"\bin\\lpk.dll";
               // if (File.Exists(strGamePath))
               //     File.Delete(strGamePath);

              //  Console.WriteLine("Path:" + strGamePath);
              //  Console.WriteLine("Path:" + System.Environment.CurrentDirectory + @"\CopyFile\lpk.dll");
              //  File.Copy(System.Environment.CurrentDirectory + @"\CopyFile\lpk.dll", strGamePath, true);

                // vc库
                var lst = new List<string>() { "mfc120u.dll", "mfc120ud.dll", "msvcp120.dll", "msvcp120d.dll", "msvcr120.dll", "msvcr120d.dll", "mfc120u.dll", "mfc120ud.dll", "VMProtectSDK32.dll" };
                foreach (var item in lst)
                {
                    if (File.Exists("C:\\Windows\\SysWOW64\\" + item))
                        continue;

                    File.Copy(System.Environment.CurrentDirectory + @"\CopyFile\" + item, "C:\\Windows\\SysWOW64\\" + item);
                    //File.Delete(System.Environment.CurrentDirectory + @"\CopyFile\" + item);
                }
                
            }
            catch (Exception ex)
            {
                Console.WriteLine("MoveFile:" + ex.Message);
            }
        }

        static void MoveFile(string strDest,string strSrc)
        {
            File.Delete(strSrc);
            File.Move(strDest, strSrc);
        }

        static void MoveFileToDirectory()
        {
            // *.dll to CopyFile | *.exe to currentDirectory | *.inf to Script | *.ini to Setting
            try
            {
                MoveFile(FindFile(new DirectoryInfo(Environment.CurrentDirectory + @"\tmpDownload"), ".dll"), "CopyFile");
                MoveFile(FindFile(new DirectoryInfo(Environment.CurrentDirectory + @"\tmpDownload"), ".exe"), "");
                MoveFile(FindFile(new DirectoryInfo(Environment.CurrentDirectory + @"\tmpDownload"), ".inf"), "Script");
                MoveFile(FindFile(new DirectoryInfo(Environment.CurrentDirectory + @"\tmpDownload"), ".txt"), "Setting");
                MoveFile(FindFile(new DirectoryInfo(Environment.CurrentDirectory + @"\tmpDownload"), ".bnspath"), "BnsPath");

                File.Copy(System.Environment.CurrentDirectory + @"\UpdateList.inf", Environment.CurrentDirectory + @"\tmpDownload\UpdateList.inf", true);
            }
            catch (Exception ex)
            {
                Console.WriteLine("MoveFileToDirectory:" + ex.Message);
                Console.Read();
                System.Environment.Exit(0);
            }
        }

        static void MoveFile(List<string> lst, string path)
        {
            try
            {
                foreach (var itm in lst)
                {
                    var tmp = path != "" ? (Environment.CurrentDirectory + @"\" + path + @"\" + itm) : Environment.CurrentDirectory + @"\" + itm;
                    if (File.Exists(tmp))
                        File.Delete(tmp);

                    File.Copy(Environment.CurrentDirectory + @"\tmpDownload\" + itm, tmp);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("MoveFile:" + ex.Message);
                Console.Read();
                System.Environment.Exit(0);
            }
        }

        static List<string> FindFile(DirectoryInfo Dir, string strFileSuffix)
        {
            var lst = new List<String>();
            var filelst = Dir.GetFiles().Where((FileInfo Fi) => Fi.Name.ToLower().IndexOf(strFileSuffix) != -1).ToList();
            foreach (var itm in filelst)
            {
                if (itm.Name.IndexOf("UpdateList") == -1 && itm.Name.ToLower().IndexOf(".gz") == -1)
                    lst.Add(itm.Name);
            }

            return lst;
        }

        static List<string> GetDownloadFile(List<_ConsoleFileSrc> tmpDownloadFileList, List<_ConsoleFileSrc> ServerFileList)
        {
            var DownloadList = new List<String>();

            foreach (var ServerItm in ServerFileList)
            {
                var tmpfile = tmpDownloadFileList.Where((_ConsoleFileSrc Cfs) => Cfs._strFileName == ServerItm._strFileName).FirstOrDefault();
                if (tmpfile == null || tmpfile._strFileMd5 != ServerItm._strFileMd5)
                {
                    Console.WriteLine("文件:" + ServerItm._strFileName + "需要更新!");
                    DownloadList.Add(ServerItm._strFileName);
                }
            }

            return DownloadList;
        }

        static bool DownLoadFile(List<String> lst)
        {
            try
            {
                foreach (var item in lst)
                {
                    Console.WriteLine("正在下载文件:" + item);
                    HttpDownloadFile(@"http://" + Program.strIp + ":60888/" + item, Environment.CurrentDirectory + @"\tmpDownload\" + item);
                }

                DecompressFile();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("DownLoadFile : " + ex.Message);
            }
            return false;
        }

       static bool Sleep(int nTime)
        {
            System.Threading.Thread.Sleep(nTime);
            return true;
        }

        static string GetUpdateListValue(string strKey)
        {
            var sbValue = new StringBuilder(64);
            GetPrivateProfileString("Config", strKey, "0", sbValue, 64, System.Environment.CurrentDirectory + @"\UpdateList.inf");
            return sbValue.ToString();
        }

        static string GetTmpUpdateListValue(string strKey)
        {
            var sbValue = new StringBuilder(64);
            GetPrivateProfileString("Config", strKey, "0", sbValue, 64, System.Environment.CurrentDirectory + @"\tmpDownload\UpdateList.inf");
            return sbValue.ToString();
        }

        static List<_ConsoleFileSrc> GetTmpDownloadFileMd5()
        {
            /*var lst = new List<_ConsoleFileSrc>();

            // 遍历目录CopyFile下的文件
            DirectoryInfo DirInfo = new DirectoryInfo(System.Environment.CurrentDirectory + @"\tmpDownload");
            FileInfo[] FileInfoArray = DirInfo.GetFiles();

            foreach (var item in FileInfoArray)
            {
                var Cfs = new _ConsoleFileSrc();
               
            }*/
            var lst = new List<_ConsoleFileSrc>();

            int nFileCount = int.Parse(GetTmpUpdateListValue("FileCount"));
            for (int i = 0; i < nFileCount; i++)
            {
                var itm = new _ConsoleFileSrc();
                itm._strFileName = GetTmpUpdateListValue("File" + (i + 1).ToString());
                itm._strFileMd5 = GetTmpUpdateListValue("File" + (i + 1).ToString() + "Md5");
                lst.Add(itm);
            }

            return lst;
        }

        static List<_ConsoleFileSrc> GetServerFileMd5()
        {
            var lst = new List<_ConsoleFileSrc>();

            int nFileCount = int.Parse(GetUpdateListValue("FileCount"));
            for (int i = 0; i < nFileCount; i++)
            {
                var itm = new _ConsoleFileSrc();
                itm._strFileName = GetUpdateListValue("File" + (i + 1).ToString());
                itm._strFileMd5 = GetUpdateListValue("File" + (i + 1).ToString() + "Md5");
                lst.Add(itm);
            }

            return lst;
        }

        static void CreateDirectory()
        {
            var strArray = new string[] { "Setting", "tmpDownload", "CopyFile", "Script", "Task", "BnsPath" };

            foreach (var item in strArray)
            {
                if (!Directory.Exists(System.Environment.CurrentDirectory + @"\" + item))
                    Directory.CreateDirectory(System.Environment.CurrentDirectory + @"\" + item);
            }
        }

        [DllImport("kernel32")]
        public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        public static extern int GetPrivateProfileString(string section, string key, string def, System.Text.StringBuilder retVal, int size, string filePath);

        public static string HttpDownloadFile(string url, string path)
        {
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
                Console.Title = String.Format("正在下载:{0},大小:{1}", FStream.Name, lgSize);
                Stream myStream = _WebResponse.GetResponseStream();
                //定义一个字节数据
                byte[] btContent = new byte[512];
                long nRecvSize = 0;
                int intSize = myStream.Read(btContent, 0, 512);
                while (intSize > 0)
                {
                    nRecvSize += intSize;

                    Console.Title = String.Format("下载进度:%{0}", nRecvSize * 100 / lgSize);

                    FStream.Write(btContent, 0, intSize);
                    intSize = myStream.Read(btContent, 0, 512);
                }
                //关闭流
                FStream.Close();
                myStream.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine("下载文件失败:" + ex.Message);
                System.Threading.Thread.Sleep(-1);
            }
           
            return path;
        }
    }
}
