using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace BnsConsole.Code
{
    public class ConsoleFile
    {
        public static bool IsSpecialText_In_LogText(string strText)
        {
            return strText.IndexOf("超过600秒任务") != -1 || strText.IndexOf("超过3分钟不动") != -1;
        }
        public static List<string> ReadFile_By_Path(string strPath)
        {
            List<string> lst = new List<string>();

            using (StreamReader SR = new StreamReader(strPath, Encoding.Unicode))
            {
                string str = string.Empty;
                do
                {
                    str = SR.ReadLine();
                    if (str != null && str.Trim().IndexOf("--") != -1)
                        break;
                    if (str != null && str.Trim() != "" && str.IndexOf("//") == -1)
                        lst.Add(str);
                } while (str != null);
                SR.Close();
            }

            return lst;
        }

        public static void SaveFile_By_Path(string strPath, List<string> lst)
        {
            using (StreamWriter SW = new StreamWriter(strPath, false, Encoding.Unicode))
            {
                lst.ForEach(v => SW.WriteLine(v));
                SW.Close();
            }
        }

        public static string GetFileVersion()
        {
            StringBuilder sbText = new StringBuilder(32);
            API.GetPrivateProfileString("Config", "FileVersion", "0.0", sbText, 32, System.Environment.CurrentDirectory + @"\\tmpDownload\\UpdateList.inf");
            return sbText.ToString();
        }

        public static bool GetCard_By_File(ref List<CCard> RetLst)
        {
            var ListTextCard = ReadFile_By_Path(System.Environment.CurrentDirectory + @"\Setting\Card.txt");

            var ListCard = new List<CCard>();
            ListTextCard.ForEach(itm => 
            {
                if (itm.Length != 0x1C)
                {
                    MessageBox.Show("我去,卡号有这样的嘛[" + itm + "]");
                    return;
                }

                if (API.CheckCard(itm) != 0x1)
                {
                    MessageBox.Show("此卡格式不对[" + itm + "]");
                    return;
                }

                ListCard.Add(new CCard() { _strCard = itm, _nCardAllHour = 0, _nCardSurplusHour = 0, _strActiveDate = "", _strMsg = "" });
            });

            RetLst = ListCard;
            return RetLst.Count > 0;
        }

        public static bool GetAccount_By_File(ref List<CAccount> RetLst)
        {
            var ListAccountText = ReadFile_By_Path(System.Environment.CurrentDirectory + @"\Setting\Account.txt");

            var ListAccount = new List<CAccount>();
            ListAccountText.ForEach(itm => 
            {
                if (itm.IndexOf(",") == -1)
                    return;

                var strArray = itm.Split(new string[] { "," }, StringSplitOptions.None).ToArray();
                if (strArray.Length == 2)
                {
                    if (strArray[1].IndexOf("'") != -1 || strArray[1].IndexOf("\"") != -1)
                    {
                        MessageBox.Show("无效的密码[" + strArray[1] + "]");
                        return;
                    }
                    ListAccount.Add(new CAccount() { strUserName = strArray[0], strPassWord = strArray[1] });
                }
            });

            RetLst = ListAccount;
            return RetLst.Count > 0;
        }

        public static void SaveCard_By_File(List<String> lst)
        {
            using (StreamWriter SW = new StreamWriter(System.Environment.CurrentDirectory + @"\Setting\Card.txt", true, Encoding.Unicode))
            {
                for (int i = 0; i < lst.Count; ++i)
                    SW.WriteLine(lst[i]);

                SW.Close();
            }
        }

        public static string GetGamePath()
        {
            try
            {
                return Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Wow6432Node\plaync\BNS_KOR").GetValue("BaseDir").ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show("无法获取注册表中的游戏路径! :" + ex.Message);
            }
            return "";
        }

        public static string GetNcLanucherPath()
        {
            try
            {
                return Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Wow6432Node\plaync\NCLauncherS").GetValue("BaseDir").ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show("无法获取注册表中的登录器路径! :" + ex.Message);
            }
            return "";
        }

        public static void SetGamePath()
        {
            API.SetParm(0x1, GetGamePath());
            API.SetParm(0x15, GetNcLanucherPath());
        }

        public static void CreateUnExistDir(string strDirecotry)
        {
            if (!Directory.Exists(strDirecotry))
                Directory.CreateDirectory(strDirecotry);
        }

        public static void SetMaxFps()
        {
            try
            {
                string strDucumentPath = System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + @"\BnS\NCSOFT\ClientConfiguration.xml";
                if (!File.Exists(strDucumentPath))
                {
                    string strClientConfigurationPath = System.Environment.CurrentDirectory + @"\tmpDownload\ClientConfiguration.xml";
                    if (!File.Exists(strClientConfigurationPath))
                    {
                        MessageBox.Show("找不到tmp\\ClientConfiguration.xml! 缺少文件!");
                        API.SetGameRun(0x2);
                    }

                    CreateUnExistDir(System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + @"\Bns");
                    CreateUnExistDir(System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + @"\Bns\NCSOFT");

                    File.Copy(strClientConfigurationPath, strDucumentPath);
                }

                XmlDocument XmlDoc = new XmlDocument();
                XmlDoc.Load(strDucumentPath);

                XmlElement XmlRoot = XmlDoc.DocumentElement;
                XmlElement XmlNde = (XmlElement)XmlRoot.SelectSingleNode("/config/option[@name=\"foreground-max-framerate\"]");
                if (XmlNde != null)
                    XmlNde.SetAttribute("value", "20");

                XmlNde = (XmlElement)XmlRoot.SelectSingleNode("/config/option[@name=\"background-max-framerate\"]");
                if (XmlNde != null)
                    XmlNde.SetAttribute("value", "20");

                XmlDoc.Save(strDucumentPath);
            }
            catch (Exception ex)
            {
                MessageBox.Show("设置帧数失败! SetMaxFps:" + ex.Message);
            }
        }


        public static int GetDumpFileCount()
        {
            if (!Directory.Exists(System.Environment.CurrentDirectory + @"\\DumpLog"))
                return 0;

            string[] FileArray = Directory.GetFiles(System.Environment.CurrentDirectory + @"\\DumpLog");

            int nCount = 0;
            foreach (var item in FileArray)
            {
                if (item.IndexOf(".dmp") != -1)
                    nCount++;
            }

            return nCount;
        }

        /// <summary>
        ///  删除过期的截图文件(3天前)!
        /// </summary>
        /// <param name="fnPrint"></param>
        public static void ScreenShotManager(Action<string> fnPrint)
        {
            if (!Directory.Exists(System.Environment.CurrentDirectory + @"\Setting\Screenshot\"))
            {
                Directory.CreateDirectory(System.Environment.CurrentDirectory + @"\Setting\Screenshot\");
            }

            DirectoryInfo Dir = new DirectoryInfo(System.Environment.CurrentDirectory + @"\Setting\Screenshot\");
            foreach (var FileInfo_ in Dir.GetFiles())
            {
                var DateDiff = DateTime.Now - FileInfo_.LastWriteTime;
                if (DateDiff.Days >= 3)
                {
                    fnPrint("正在删除过期的截图文件:" + FileInfo_.Name);
                    File.Delete(FileInfo_.FullName);
                }
            }
        }

        /// <summary>
        /// 删除过期的日志文件(2天前)
        /// </summary>
        /// <param name="fnPrint"></param>
        public static void LogManager(Action<string> fnPrint)
        {
            if (!Directory.Exists(GetGamePath() + @"\bin\Log\"))
            {
                Directory.CreateDirectory(GetGamePath() + @"\bin\Log\");
            }

            DirectoryInfo Dir = new DirectoryInfo(GetGamePath() + @"\bin\Log\");
            foreach (var FileInfo_ in Dir.GetFiles())
            {
                var DateDiff = DateTime.Now - FileInfo_.LastWriteTime;
                if (DateDiff.Days >= 2)
                {
                    fnPrint("正在删除过期的日志文件:" + FileInfo_.Name);
                    File.Delete(FileInfo_.FullName);
                }
            }
        }
    }
}
