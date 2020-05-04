using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ConsoleFileServer
{
    class Program
    {
        public static String Compress(String Path)
        {
            var CompressFileName = Path + ".gz";
            if (File.Exists(CompressFileName))
            {
                Console.WriteLine("Delete Old Compress File:" + CompressFileName);
                File.Delete(CompressFileName);
            }

            var fileToCompress = new FileInfo(Path);
            using (FileStream originalFileStream = fileToCompress.OpenRead())
            {
                if ((File.GetAttributes(fileToCompress.FullName) & FileAttributes.Hidden) != FileAttributes.Hidden & fileToCompress.Extension != ".gz")
                {
                    using (FileStream compressedFileStream = File.Create(CompressFileName))
                    {
                        using (GZipStream compressionStream = new GZipStream(compressedFileStream, CompressionMode.Compress))
                        {
                            originalFileStream.CopyTo(compressionStream);
                        }
                        compressedFileStream.Close();
                    }
                }
                originalFileStream.Close();
                File.Delete(Path);
            }

            return CompressFileName;
        }
        static void Main(string[] args)
        {
            Console.WriteLine("是否需要强制更新此版本: y是强制, 不强制的随意!");
            string strIsForce = Console.ReadLine();
            
            SetLockFile(true);
            SetConfigVersion(GetConfigVersion() + 0.01f);
            CompressFile();
            SetMD5();

            if (strIsForce == "y")
                SetForceVersion(GetConfigVersion());

            SetLockFile(false);
            Console.WriteLine("完毕!5s后自动关闭!");
            Thread.Sleep(5 * 1000);
        }

        [DllImport("kernel32")]
        public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        public static extern int GetPrivateProfileString(string section, string key, string def, System.Text.StringBuilder retVal, int size, string filePath);

        static string GetConfigPath()
        {
            return System.Environment.CurrentDirectory + @"\Update\UpdateList.inf";
        }
        static float GetConfigVersion()
        {
            StringBuilder sbVersion = new StringBuilder(32);
            GetPrivateProfileString("Config", "FileVersion", "1.01", sbVersion, 32, GetConfigPath());
            return float.Parse(sbVersion.ToString());
        }
        static void SetForceVersion(float fVersion)
        {
            WritePrivateProfileString("Config", "LeastFileVersion", fVersion.ToString("F2"), GetConfigPath());
        }
        static void SetConfigVersion(float fVersion)
        {
            WritePrivateProfileString("Config", "FileVersion", fVersion.ToString("F2"), GetConfigPath());
        }
        static void SetLockFile(bool bLock)
        {
            WritePrivateProfileString("Config", "Lock", bLock ? "1" : "0", GetConfigPath());
        }

        static void CompressFile()
        {
            string[] strArray = { ".dll", ".exe", ".inf", ".ini", ".xml", ".bnspath" };
            new DirectoryInfo(System.Environment.CurrentDirectory + @"\CopyFile").GetFiles().ToList().ForEach(File_ => 
            {
                var FileName = File_.Name;
                if (strArray.Where(itm => FileName.ToLower().IndexOf(itm) != -1).ToList().Count == 0)
                    return;
                else if (FileName.IndexOf(".gz") != -1)
                    return;

                Compress(String.Format("{0}\\CopyFile\\{1}", System.Environment.CurrentDirectory, FileName));
            });
            
        }

        static void SetMD5()
        {
            DirectoryInfo DirInfo = new DirectoryInfo(System.Environment.CurrentDirectory + @"\CopyFile");
            FileInfo[] FileInfoArray = DirInfo.GetFiles();

            var FileCount = 0;
            for (int i = 0; i < FileInfoArray.Length; i++)
            {
                var FileName = FileInfoArray[i].Name;
                if (FileName.IndexOf(".gz") == -1)
                    continue;

                try
                {
                    if (File.Exists(System.Environment.CurrentDirectory + @"\Update\" + FileName))
                        File.Delete(System.Environment.CurrentDirectory + @"\Update\" + FileName);

                    File.Copy(System.Environment.CurrentDirectory + @"\CopyFile\" + FileName, System.Environment.CurrentDirectory + @"\Update\" + FileName);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("拷贝文件出现异常:" + ex.Message);
                    Console.ReadLine();
                    System.Environment.Exit(0);
                }

                FileCount += 1;
                WritePrivateProfileString("Config", "File" + (i + 1).ToString(), FileName, GetConfigPath());
                WritePrivateProfileString("Config", "File" + (i + 1).ToString() + "Md5", GetMD5HashFromFile(FileInfoArray[i].FullName), GetConfigPath());
            }

            WritePrivateProfileString("Config", "FileCount", FileCount.ToString(), GetConfigPath());
        }

        public static string GetMD5HashFromFile(string fileName)
        {
            try
            {
                Console.WriteLine("FileName:" + fileName);
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
}
