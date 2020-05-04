using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BnsConsole.Code
{
    public class CConsoleContent
    {
        public String AccountName { get; set; }
        public String LogContent { get; set; }
        public int nType { get; set; }

        public static Tuple<bool, CConsoleContent> GetConsoleContent(int nType, String Text)
        {
            if (Text.IndexOf(":") != -1 && (nType > 0 && nType <= 10))
            {
                var Content = CConsoleContent.CreateConsoleContent(Text);
                Content.nType = nType;

                return Tuple.Create(true, Content);
            }
            
            return Tuple.Create(false, new CConsoleContent());
        }

        public static CConsoleContent CreateConsoleContent(String Text)
        {
            var Content = new CConsoleContent();
            Content.AccountName = Text.Substring(0, Text.IndexOf(":"));
            Content.LogContent = Text.Substring(Text.IndexOf(":") + 1);

            return Content;
        }
    }
    public class CConsoleLog
    {
        private Action<int, int, int, String> _SetDgvValuePtr = null;
        private Thread _RefrshFormThread = null;
        private System.Timers.Timer _Timer = null;
        private Queue<CConsoleContent> _Queue = new Queue<CConsoleContent>();

        public const int WM_COPYDATA = 0x4A;
        public struct COPYDATASTRUCT
        {
            public IntPtr dwData;
            public int cbData;
            [MarshalAs(UnmanagedType.LPWStr)]
            public string lpData;
        }
        public void AddToQueue(Message m)
        {
            try
            {
                COPYDATASTRUCT CopyStruct = new COPYDATASTRUCT();
                CopyStruct = (COPYDATASTRUCT)m.GetLParam(CopyStruct.GetType());
                var tup = CConsoleContent.GetConsoleContent(CopyStruct.dwData.ToInt32(), CopyStruct.lpData);
                if (tup.Item1)
                {
                    lock (_Queue) { _Queue.Enqueue(tup.Item2); }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("AddToQueue:" + ex.Message);
            }
        }

        //--------------------------------------------------------------------

        public void Run(Action<int, int, int, String> SetDgvValuePtr)
        {
            if (_RefrshFormThread == null)
            {
                _SetDgvValuePtr = SetDgvValuePtr;
                _RefrshFormThread = new Thread(_RefushThread);
                _RefrshFormThread.Start();
            }
        }

        public void Stop()
        {
            if (_RefrshFormThread != null)
            {
                _RefrshFormThread.Join();
                _Timer.Close();
            }
        }

        private void SetPrintOnLineTimeTimer()
        {
            _Timer = new System.Timers.Timer(60 * 1000) { AutoReset = true, Enabled = true };
            _Timer.Elapsed += ATimer_Elapsed;
        }

        private void ATimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            var OnlineTimeText = new StringBuilder(64);
            lock (Code.CGlobalVariable.g_AccountList)
            {
                try
                {
                    for (int i = 0; i < Code.CGlobalVariable.g_AccountList.Count; i++)
                    {
                        var itm = Code.CGlobalVariable.g_AccountList[i];

                        if (Code.API.GetOnLineMsgByAccount(itm.strUserName, OnlineTimeText) != 0)
                        {
                            _SetDgvValuePtr(0x2, i, 0x4, OnlineTimeText.ToString());
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("ATimer_Elapsed:" + ex.Message);
                }
                
            }
        }

        private Tuple<bool, CConsoleContent> GetAccountMsg()
        {
            lock (_Queue)
            {
                if (_Queue.Count == 0)
                {
                    return Tuple.Create(false, new CConsoleContent());
                }

                return Tuple.Create(true, _Queue.Dequeue());
            }
        }

        public static void Compress(String Path)
        {
            try
            {
                var fileToCompress = new FileInfo(Path);
                using (FileStream originalFileStream = fileToCompress.OpenRead())
                {
                    if ((File.GetAttributes(fileToCompress.FullName) & FileAttributes.Hidden) != FileAttributes.Hidden & fileToCompress.Extension != ".gz")
                    {
                        using (FileStream compressedFileStream = File.Create(fileToCompress.FullName + ".gz"))
                        {
                            using (GZipStream compressionStream = new GZipStream(compressedFileStream, CompressionMode.Compress))
                            {
                                originalFileStream.CopyTo(compressionStream);
                            }
                            compressedFileStream.Close();
                        }
                    }
                    originalFileStream.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Compress:" + ex.Message);
            }
           
        }

        private static ImageCodecInfo GetEncoder(ImageFormat format)
        {
            ImageCodecInfo[] codecs = ImageCodecInfo.GetImageDecoders();

            foreach (ImageCodecInfo codec in codecs)
            {
                if (codec.FormatID == format.Guid)
                {
                    return codec;
                }
            }
            return null;
        }

        public static String BmpToJpg(String Path)
        {
            try
            {
                if (File.Exists(Path + ".bmp"))
                {
                    Bitmap bmp1 = new Bitmap(Path + ".bmp");
                    ImageCodecInfo jgpEncoder = GetEncoder(ImageFormat.Jpeg);

                    // Create an EncoderParameters object.
                    // An EncoderParameters object has an array of EncoderParameter
                    // objects. In this case, there is only one
                    // EncoderParameter object in the array.
                    EncoderParameters myEncoderParameters = new EncoderParameters(1);

                    myEncoderParameters.Param[0] = new EncoderParameter(System.Drawing.Imaging.Encoder.Quality, 50L);
                    bmp1.Save(Path + ".jpg", jgpEncoder, myEncoderParameters);

                    bmp1.Dispose();
                    File.Delete(Path + ".bmp");
                }
                
                return Path + ".jpg";
            }
            catch (Exception ex)
            {
                MessageBox.Show("BmpToJpg:" + ex.Message + ", Path=" + Path);
            }
            return "";
        }

        private static object _SyncRoot = new object();
        public static void SendBugReport(String AccountName, String Body)
        {
            // Copy Log to Current Log\xx.log
            String LogPath = String.Format("{0}\\SaveLog\\{1}.log", System.Environment.CurrentDirectory, AccountName);

            try
            {
                lock (_SyncRoot)
                {
                    try
                    {
                        if (File.Exists(LogPath))
                            File.Delete(LogPath);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Delete LogPath:" + ex.Message + ", Path=" + LogPath);
                    }
                   

                    var ListFile = new List<String>();
                    var AccountLogPath = String.Empty;
                    var ScreenshotPath = String.Empty;
                    var DumpPath = String.Empty;

                    try
                    {
                        AccountLogPath = String.Format("{0}\\bin\\Log\\{1}.log", Code.ConsoleFile.GetGamePath(), AccountName);
                        if (File.Exists(AccountLogPath))
                        {
                            File.Copy(AccountLogPath, LogPath);
                            Compress(LogPath);
                            ListFile.Add(LogPath + ".gz");
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Compress Log:" + ex.Message + ", Path=" + AccountLogPath);
                    }


                    try
                    {
                        ScreenshotPath = BmpToJpg(String.Format("{0}\\Setting\\Screenshot\\{1}", System.Environment.CurrentDirectory, AccountName));
                        if (File.Exists(ScreenshotPath))
                        {
                            Compress(ScreenshotPath);
                            ListFile.Add(ScreenshotPath + ".gz");
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Compress Screenshot:" + ex.Message + ", Path=" + AccountLogPath);
                    }


                    try
                    {
                        DumpPath = String.Format("{0}\\DumpLog\\{1}.dmp", System.Environment.CurrentDirectory, AccountName);
                        if (File.Exists(DumpPath))
                        {
                            Compress(DumpPath);
                            ListFile.Add(DumpPath + ".gz");
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Compress Dump:" + ex.Message + ", Path=" + AccountLogPath);
                    }
                   

                    try
                    {
                        var Ass = Assembly.LoadFrom(System.Environment.CurrentDirectory + "\\CopyFile\\BugReport.dll");
                        var Type = Ass.GetType("BugReport.CCollect");
                        var Obj = Activator.CreateInstance(Type);
                        var MethodPtr = Type.GetMethod("Collect");
                        var wsRetText = MethodPtr.Invoke(Obj, new object[] { Body, ListFile }) as String;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Invalid Message:" + ex.Message);
                    }

                    // delete
                    ListFile.ForEach(Path =>
                    {
                        try
                        {
                            if (File.Exists(Path))
                                File.Delete(Path);
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("Foreach Delete File:" + ex.Message + ", Path=" + Path);
                        }
                        
                    });
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("SendBug2:" + ex.Message);
            }
        }

        private void _RefushThread()
        {
            SetPrintOnLineTimeTimer();
            while (Code.CGlobalVariable.g_bStartGame)
            {
                var tup = GetAccountMsg();
                if (!tup.Item1)
                {
                    Thread.Sleep(100);
                    continue;
                }

                var Content = tup.Item2;
                lock (Code.CGlobalVariable.g_AccountList)
                {
                    var Index = Code.CGlobalVariable.g_AccountList.FindIndex(itm => itm.strUserName == Content.AccountName);
                    if (Index != -1)
                    {
                        // nRetType是由ConsoleLog.h里面的枚举值决定的
                        switch (Content.nType)
                        {
                            case 1: // 状态
                                _SetDgvValuePtr(0x2, Index, 0x2, Content.LogContent);
                                break;
                            case 2: // 日志
                                _SetDgvValuePtr(0x2, Index, 0x3, Content.LogContent);
                                break;
                            case 4: // 上次重登原因
                                if (Code.ConsoleFile.IsSpecialText_In_LogText(Content.LogContent) && Content.LogContent == Code.CGlobalVariable.g_AccountList[Index].strCloseReason)
                                {
                                    Code.CGlobalVariable.g_AccountList[Index].nRepeatCount += 1;
                                    _SetDgvValuePtr(0x2, Index, 0x5, String.Format("#{0} {1}", Code.CGlobalVariable.g_AccountList[Index].nRepeatCount.ToString(), Content.LogContent));
                                }
                                else
                                {
                                    Code.CGlobalVariable.g_AccountList[Index].strCloseReason = Content.LogContent;
                                    _SetDgvValuePtr(0x2, Index, 0x5, Content.LogContent);
                                }
                                break;
                            case 0x5:
                                Task.Factory.StartNew(()=>
                                {
                                    SendBugReport(Content.AccountName, Content.LogContent);
                                });
                                break;
                            default:
                                MessageBox.Show("Content.nType=" + Content.nType.ToString());
                                break;
                        }
                    }
                }
            }
        }
    }
}
