using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BnsConsole.Code
{
    public class API
    {
        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention= CallingConvention.Cdecl)]
        public static extern void RechargeCard(string strCardNo,StringBuilder sbRetMsg);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CheckCard(string strCard);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int QueryCardInfo(string strCard, int nIsStart);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetCardInfo(string strCard, StringBuilder sbRetMsg);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int AccountHandle(int nFlag, string strText);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetGameRun(int nRunFlag);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetServerTitle(StringBuilder sbRetMsg);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetBnsLoginKey(StringBuilder sbRetKey);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetParm(int nKey, string strValue);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetWebLoginParm(StringBuilder sbAccountName, StringBuilder sbAccountPass);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Init(String strGamePath, int bOptimize);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetOnLineMsgByAccount(string strAccountName, StringBuilder sbRetMsg);

        [DllImport("kernel32")]
        public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int GetPrivateProfileString(string section, string key, string def, System.Text.StringBuilder retVal, int size, string filePath);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetWindowSortRun(int nRun);

        [DllImport(@"CopyFile\ConsoleDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetCompileTime(int nType, System.Text.StringBuilder retVal);
    }
}
