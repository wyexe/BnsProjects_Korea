using BnsConsole.Code;
using BnsConsole.Setting;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BnsConsole
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private CConsoleLog _ConsoleLog = new CConsoleLog();

        private void SetCompileTimeText()
        {
            var sbConsoleDLL = new StringBuilder(64);
            API.GetCompileTime(0, sbConsoleDLL);
            lbConsoleDLL.Text = sbConsoleDLL.ToString();
            sbConsoleDLL.Clear();

            var BnsProjectDLLText = new StringBuilder(64);
            API.GetCompileTime(1, BnsProjectDLLText);
            lbBnsProjectDLL.Text = BnsProjectDLLText.ToString();
            lbBnsProjectDLL.Visible = true;
        }


        [System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")]
        protected override void WndProc(ref Message m)
        {
            if (m.Msg == 0x4A)
            {
                _ConsoleLog.AddToQueue(m);
            }

            base.WndProc(ref m);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                SetCompileTimeText();
                dgvAccount.Columns.Cast<DataGridViewColumn>().ToList().ForEach(f => f.SortMode = DataGridViewColumnSortMode.NotSortable);

                this.Text = "54Bns  Verion:" + Code.ConsoleFile.GetFileVersion();

                停止恶作剧ToolStripMenuItem.Enabled = false;
                menuStrip1.Enabled = false;
                
                new ServerPrompt().ShowDialog();
                new Thread(RefushAccountThread).Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Form1_Load:" + ex.Message);
            }
        }

        private void RefushAccountThread()
        {
            CleanFile();
            CGlobalVariable.g_AccountList.Clear();
            Code.ConsoleFile.GetAccount_By_File(ref CGlobalVariable.g_AccountList);

            Invoke(new MethodInvoker(delegate()
            {
                this.dgvAccount.Rows.Clear();
                for (int i = 0; i < CGlobalVariable.g_AccountList.Count; i++)
                {
                    this.dgvAccount.Rows.Add();

                    var Account = CGlobalVariable.g_AccountList[i];
                    dgvAccount.Rows[i].Cells[0].Value = (i + 1).ToString();
                    dgvAccount.Rows[i].Cells[1].Value = Account.strUserName;
                    dgvAccount.Rows[i].Cells[2].Value = Account.strLog;
                }
                lbPrompt.Text = "刷新完毕…………";

                if (CGlobalVariable.g_WorkCardList != null && CGlobalVariable.g_WorkCardList.Count > 0)
                    lbWorkCardCount.Text = CGlobalVariable.g_WorkCardList.Where((CCard Card) => Card._nCardSurplusHour > 0).ToList().Count.ToString();

                menuStrip1.Enabled = true;
            }));
        }

        private void CleanFile()
        {
            try
            {
                Code.ConsoleFile.ScreenShotManager((string strFileName) =>
                {
                    Invoke(new MethodInvoker(delegate() { this.lbPrompt.Text = strFileName; }));
                });

                Code.ConsoleFile.LogManager((string strFileName) =>
                {
                    Invoke(new MethodInvoker(delegate() { this.lbPrompt.Text = strFileName; }));
                });
            }
            catch (Exception ex)
            {
                MessageBox.Show("CleanFile: " + ex.Message);
            }
        }

        private void 帐号设置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start(System.Environment.CurrentDirectory + @"\\Setting\\Account.txt");
        }

        private void 刷新ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            menuStrip1.Enabled = false;
            new Thread(RefushAccountThread).Start();
        }

        void SetGridViewCell_Color(Color color, int i)
        {
            DataGridViewCellStyle DGVCS = new System.Windows.Forms.DataGridViewCellStyle();
            DGVCS.ForeColor = color;

            for (int k = 0; k < dgvAccount.Columns.Count; k++)
            {
                dgvAccount.Rows[i].Cells[k].Style = DGVCS;
            }
        }

        private void SetDgvValue(int nRetCode,int i, int nColumn, string strValue)
        {
            Invoke(new MethodInvoker(delegate () 
            {
                lock (dgvAccount)
                {
                    if (i < dgvAccount.RowCount)
                    {
                        if (nRetCode == 1) // 已经刷完了,蓝色
                        {
                            if (dgvAccount.Rows[i].Cells[1].Style.ForeColor != Color.Blue)
                                SetGridViewCell_Color(Color.Blue, i);
                        }
                        else if (nRetCode == 2) // 正在刷, 粉红色
                        {
                            if (dgvAccount.Rows[i].Cells[1].Style.ForeColor != Color.Fuchsia)
                                SetGridViewCell_Color(Color.Fuchsia, i);
                            dgvAccount.Rows[i].Cells[nColumn].Value = strValue;
                        }
                    }
                }
            }));
        }


        private void 停止恶作剧ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            API.SetGameRun(0);
        }

        public static string GetValue_By_Key_For_Config(string strKey, string ConfigName = "Config")
        {
            StringBuilder sbValue = new StringBuilder(1024 * 10);
            API.GetPrivateProfileString(ConfigName, strKey, "0", sbValue, 1024 * 10, System.Environment.CurrentDirectory + @"\Config.ini");
            return sbValue.ToString().Trim();
        }

        private void ReadConfig()
        {
            API.SetParm(0x2, GetValue_By_Key_For_Config("SID"));
            API.SetParm(0x3, GetValue_By_Key_For_Config("Script"));
            API.SetParm(0x4, GetValue_By_Key_For_Config("RunGlobalAccount"));
            API.SetParm(0x5, GetValue_By_Key_For_Config("GlobalAccountType"));
            API.SetParm(0x7, GetValue_By_Key_For_Config("SortWindow"));
            API.SetParm(0x8, GetValue_By_Key_For_Config("WarehouseScript"));
            API.SetParm(0x9, GetValue_By_Key_For_Config("IsClose_By_Banned"));
            API.SetParm(0xA, GetValue_By_Key_For_Config("IsPK"));
            API.SetParm(0xB, GetValue_By_Key_For_Config("IsDailyTask"));
            API.SetParm(0xC, GetValue_By_Key_For_Config("FastLoginMode"));
            API.SetParm(0xE, GetValue_By_Key_For_Config("MailScript"));
            API.SetParm(0x10, GetValue_By_Key_For_Config("IsDisableInjectorDlg"));
            API.SetParm(0x11, GetValue_By_Key_For_Config("IsDeletePlayerInSummonerMode"));
            API.SetParm(0x12, GetValue_By_Key_For_Config("GroupTitle"));
            API.SetParm(0x13, this.Handle.ToString());
            API.SetParm(0x14, GetValue_By_Key_For_Config("Accelerate"));
            API.SetParm(0x16, GetValue_By_Key_For_Config("LoginControlHwndTimeout", "NcLauncher"));
            API.SetParm(0x17, GetValue_By_Key_For_Config("LoginTimeout", "NcLauncher"));
            API.SetParm(0x18, GetValue_By_Key_For_Config("DisableLoginButtonTimeout", "NcLauncher"));

            string[] strMailArray = GetValue_By_Key_For_Config("MailName").Split(new string[]{ "," }, StringSplitOptions.RemoveEmptyEntries);
            if (strMailArray.Length >= 16)
            {
                MessageBox.Show("邮寄名称最多16个!");
                Environment.Exit(0);
            }

            foreach (var item in strMailArray)
                API.SetParm(0x6, item);

            string[] strWarehouseArray = GetValue_By_Key_For_Config("Warehouse").Split(new string[] { "-" }, StringSplitOptions.RemoveEmptyEntries);
            if (strWarehouseArray.Length >= 16)
            {
                MessageBox.Show("仓库名称最多16个!");
                Environment.Exit(0);
            }

            foreach (var item in strWarehouseArray)
                API.SetParm(0xF, item);

        }


        private void 开始恶作剧ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!Code.CGlobalVariable.g_bStartGame)
            {
                // 添加到DLL
                for (int i = 0; i < Code.CGlobalVariable.g_AccountList.Count; i++)
                {
                    var Account = Code.CGlobalVariable.g_AccountList[i];
                    API.AccountHandle(0x1, Account.strUserName + "," + Account.strPassWord);
                }
                ReadConfig();

                // 设置禁止启动项
                CGlobalVariable.g_bStartGame = true;
                恶作剧开始ToolStripMenuItem.Enabled = false;
                设置ToolStripMenuItem.Enabled = false;
                刷新ToolStripMenuItem.Enabled = false;
                开始挂机ToolStripMenuItem.Enabled = false;
                一键收邮件ToolStripMenuItem.Enabled = true;

                // 启动刷新线程
                _ConsoleLog.Run(SetDgvValue);
               
            }
            API.SetGameRun(1);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Code.CGlobalVariable.g_bStartGame = false;
            lbPrompt.Text = "正在等待刷新游戏数据线程结束……";
            _ConsoleLog.Stop();

            lbPrompt.Text = "关闭游戏优化……";
            API.SetGameRun(2);
        }

        private void 卡号设置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (CGlobalVariable.g_bStartGame)
            {
                MessageBox.Show("开始游戏后禁止修改卡号!");
                return;
            }

            new CardSetting().ShowDialog();
        }

        private void 控制台设置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new ConsoleSetting().ShowDialog();
        }

        private void 挂机设置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("还没做");
        }

        private int GetSelectIndex()
        {
            lock (dgvAccount)
            {
                if (dgvAccount.SelectedRows.Count > 0)
                    return dgvAccount.SelectedRows[0].Index;

                for (int i = 0; i < dgvAccount.Rows.Count; i++)
                {
                    if (dgvAccount.Rows[i].Selected || dgvAccount.Rows[i].Cells[1].Selected)
                    {
                        return i;
                    }
                }
            }
            return -1;
        }

        private void 删除帐号ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            删除帐号ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() =>
            {
                try
                {
                    int nIndex = GetSelectIndex();
                    if (nIndex == -1)
                    {
                        MessageBox.Show("至少你也要选中一行吧?");
                        return;
                    }

                    lock (Code.CGlobalVariable.g_AccountList)
                    {
                        API.AccountHandle(0x2, Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
                        Code.CGlobalVariable.g_AccountList.RemoveAt(nIndex);
                        Invoke(new MethodInvoker(delegate() { lock (dgvAccount) { dgvAccount.Rows.RemoveAt(nIndex); } })); 
                    }
                    删除帐号ToolStripMenuItem.Enabled = true;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("删除帐号异常:" + ex.Message);
                }
            });
        }

        private void 停止挂机ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            停止挂机ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() => 
            {
                try
                {
                    int nIndex = GetSelectIndex();
                    if (nIndex == -1)
                    {
                        MessageBox.Show("至少你也要选中一行吧?");
                        return;
                    }

                    lock (Code.CGlobalVariable.g_AccountList)
                    {
                        API.AccountHandle(0x4, Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
                    }

                    停止挂机ToolStripMenuItem.Enabled = true;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("停止挂机有异常:" + ex.Message);
                }
            });
        }

        private void 恢复挂机ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            恢复挂机ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() => 
            {
                try
                {
                    int nIndex = GetSelectIndex();
                    if (nIndex == -1)
                    {
                        MessageBox.Show("至少你也要选中一行吧?");
                        return;
                    }

                    lock (Code.CGlobalVariable.g_AccountList)
                    {
                        API.AccountHandle(0x5, Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
                    }
                    恢复挂机ToolStripMenuItem.Enabled = true;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("开始挂机有异常:" + ex.Message);
                }
            });
            
        }

        private void 恢复窗口ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            恢复窗口ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() => 
            {
                int nIndex = GetSelectIndex();
                if (nIndex == -1)
                {
                    MessageBox.Show("至少你也要选中一行吧?");
                    return;
                }

                lock (Code.CGlobalVariable.g_AccountList)
                {
                    API.AccountHandle(0x6, Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
                }
                恢复窗口ToolStripMenuItem.Enabled = true;
            });
            
        }

        private void 开始挂机ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            开始恶作剧ToolStripMenuItem_Click(sender, e);
        }

        private void 冻结游戏ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            冻结游戏ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() => 
            {
                int nIndex = GetSelectIndex();
                if (nIndex == -1)
                {
                    MessageBox.Show("至少你也要选中一行吧?");
                    return;
                }

                lock (Code.CGlobalVariable.g_AccountList)
                {
                    API.AccountHandle(0x7, Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
                }
                冻结游戏ToolStripMenuItem.Enabled = true;
            });
           
        }

        private void 停止窗口排序ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Code.API.SetWindowSortRun(0);
        }

        private void 恢复窗口排序ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var sbValue = new StringBuilder(64);
            Code.API.GetPrivateProfileString("Config", "SortWindow", "0", sbValue, 64, System.Environment.CurrentDirectory + @"\\Config.ini");
            string newvalue = sbValue.ToString();
            int v = Convert.ToInt32(newvalue);
            Code.API.SetWindowSortRun(v);
        }

        private void 一键收邮件ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("确认开启一键收邮件删号", "提示", MessageBoxButtons.YesNo) == System.Windows.Forms.DialogResult.Yes)
                Code.API.SetParm(0xD, "1");
        }

        private void 收邮件停止挂机ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("确认开启一键收邮件停止挂机", "提示", MessageBoxButtons.YesNo) == System.Windows.Forms.DialogResult.Yes)
                Code.API.SetParm(0xD, "2");
        }

        private void 单帐号收邮件删号ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            单帐号收邮件删号ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() => 
            {
                int nIndex = GetSelectIndex();
                if (nIndex == -1)
                {
                    MessageBox.Show("至少你也要选中一行吧?");
                    return;
                }

                lock (Code.CGlobalVariable.g_AccountList)
                {
                    API.AccountHandle(0x8, Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
                }
                单帐号收邮件删号ToolStripMenuItem.Enabled = true;
            });
            
        }

        private void 单帐号收邮件停止挂机ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            单帐号收邮件停止挂机ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() => 
            {
                int nIndex = GetSelectIndex();
                if (nIndex == -1)
                {
                    MessageBox.Show("至少你也要选中一行吧?");
                    return;
                }

                lock (Code.CGlobalVariable.g_AccountList)
                {
                    API.AccountHandle(0x9, Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
                }

                单帐号收邮件停止挂机ToolStripMenuItem.Enabled = true;
            });
           
        }

        private void 所有帐号停止挂机ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            停止挂机ToolStripMenuItem.Enabled = false;
            Task.Factory.StartNew(() =>
            {
                try
                {
                    lock (Code.CGlobalVariable.g_AccountList)
                    {
                        foreach (var itm in Code.CGlobalVariable.g_AccountList)
                        {
                            API.AccountHandle(0x4, itm.strUserName);
                        }
                    }

                    停止挂机ToolStripMenuItem.Enabled = true;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("停止挂机有异常:" + ex.Message);
                }
            });
        }

        private void 恢复优化ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            API.SetGameRun(0x3);
        }

        private void 查找日志ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int nIndex = GetSelectIndex();
            if (nIndex == -1)
            {
                MessageBox.Show("至少你也要选中一行吧?");
                return;
            }

            var FilePath = String.Format("{0}\\bin\\Log\\{1}.log", Code.ConsoleFile.GetGamePath(), Code.CGlobalVariable.g_AccountList[nIndex].strUserName);
            if (!File.Exists(FilePath))
            {
                MessageBox.Show(String.Format("帐号[{0}]不存在日志文件[{1}]!", Code.CGlobalVariable.g_AccountList[nIndex].strUserName, FilePath));
                return;
            }
            var Args = String.Format("/select,\"{0}\"", FilePath);
            Process.Start("explorer.exe", Args);
        }

        private void bugReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int nIndex = GetSelectIndex();
            if (nIndex == -1)
            {
                MessageBox.Show("至少你也要选中一行吧?");
                return;
            }

            lock (Code.CGlobalVariable.g_AccountList)
            {
                Task.Factory.StartNew(() => { CConsoleLog.SendBugReport(Code.CGlobalVariable.g_AccountList[nIndex].strUserName, "Bug Report"); });
            }
        }
    }
}
