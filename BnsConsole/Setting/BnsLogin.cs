using BnsConsole.Code;
using Microsoft.Win32;
using mshtml;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BnsConsole.Setting
{
    public enum em_BnsLoginType
    {
        em_BnsLoginType_LoginGame = 0x1,
        em_BnsLoginType_ActiveCard  = 0x2
    }
    public partial class BnsLogin : Form
    {
        public BnsLogin()
        {
            InitializeComponent();
        }

        private void BnsLogin_Load(object sender, EventArgs e)
        {
            this.Hide();
            webBrowser1.AllowWebBrowserDrop = false;
            webBrowser1.WebBrowserShortcutsEnabled = false;
            webBrowser1.IsWebBrowserContextMenuEnabled = false;

            lbAccount.Text = "当前登录帐号:\r\n" + strAccountName;
            new Action(() =>
            {
                if (emBnsLoginType == em_BnsLoginType.em_BnsLoginType_LoginGame)
                    BnsLoginThread();

            }).BeginInvoke(r =>
            {
                if (r.IsCompleted)
                {
                    Invoke(new MethodInvoker(delegate()
                    {
                        this.Close();
                    }));
                }
            }, null);
        }

        public static void Display_Image_And_Flash()
        {
            try
            {
                RegistryKey regkey = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Internet Explorer\Main", true);

                regkey.SetValue("Display Inline Images", "no", RegistryValueKind.String);

                regkey = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Internet Explorer\Main", true);

                regkey.SetValue("Play_Animations", "no", RegistryValueKind.String);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Display_Image_And_Flash:" + ex.Message);
            }
        }
        public string strAccountName = string.Empty;
        public string strAccountPass = string.Empty;
        public em_BnsLoginType emBnsLoginType = em_BnsLoginType.em_BnsLoginType_LoginGame;
        public int nActiveCardResult = 0;
        public static void Enable_ImaGe_And_Flash()
        {
            try
            {
                RegistryKey regkey = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Internet Explorer\Main", true);

                regkey.SetValue("Display Inline Images", "yes", RegistryValueKind.String);

                regkey = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Internet Explorer\Main", true);

                regkey.SetValue("Play_Animations", "yes", RegistryValueKind.String);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Enable_ImaGe_And_Flash:" + ex.Message);
            }
        }

        [DllImport("wininet.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool InternetGetCookieEx(string pchURL, string pchCookieName, StringBuilder pchCookieData, ref uint pcchCookieData, int dwFlags, IntPtr lpReserved);
        [DllImport("wininet.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern int InternetSetCookieEx(string lpszURL, string lpszCookieName, string lpszCookieData, int dwFlags, IntPtr dwReserved);
        const int INTERNET_COOKIE_HTTPONLY = 0x00002000;
        public static string GetGlobalCookies(string uri)
        {
            uint datasize = 1024;
            StringBuilder cookieData = new StringBuilder((int)datasize);
            if (InternetGetCookieEx(uri, null, cookieData, ref datasize, INTERNET_COOKIE_HTTPONLY, IntPtr.Zero) && cookieData.Length > 0)
            {
                return cookieData.ToString();
            }
            else
            {
                return null;
            }
        }
        public static void SetGlobalCookies(string uri)
        {
            uint datasize = 1024;
            StringBuilder cookieData = new StringBuilder((int)datasize);
            InternetSetCookieEx(uri, null, cookieData.ToString(), INTERNET_COOKIE_HTTPONLY, IntPtr.Zero);
        }
        private bool GetLoginKey(ref String strToken)
        {
            try
            {
                var WC = new WebClient();
                WC.Headers.Add("Cookie", GetGlobalCookies("http://bns.plaync.com"));
                var Stm = WC.OpenRead("http://bns.plaync.com/api/launcher/launcher?serverType=live");
                var Sr = new StreamReader(Stm);

                var strHtml = Sr.ReadToEnd();
                if (strHtml.IndexOf("fail") != -1)
                    return false;

                if (strHtml.IndexOf("success") != -1)
                {
                    strHtml = strHtml.Substring(strHtml.IndexOf("token"));
                    strHtml = strHtml.Substring(8);
                    strToken = strHtml.Substring(0, strHtml.IndexOf("\""));
                    return true;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("网页登录错误:" + ex.Message);
            }
            return false;
        }

        private bool IsHasVer()
        {
            bool bRetCode = false;
            Invoke(new MethodInvoker(delegate()
            {
                try { bRetCode = webBrowser1.Document.GetElementById("captcha_img") != null; }
                catch (Exception) { }
            }));
            return bRetCode;
        }
        private Image GetWebImage(HTMLDocument doc, IHTMLControlElement ImgeTag)
        {
            HTMLBody body = (HTMLBody)doc.body;
            IHTMLControlRange rang = (IHTMLControlRange)body.createControlRange();
            IHTMLControlElement Img = ImgeTag; //图片地址

            Image oldImage = Clipboard.GetImage();
            rang.execCommand("Unselect");
            rang.add(Img);
            rang.execCommand("Copy", false, null);  //拷贝到内存
            Image numImage = Clipboard.GetImage();

            try
            {
                if (oldImage != null)
                    Clipboard.SetImage(oldImage);
            }
            catch (Exception)
            {
                // MessageBox.Show("GetWebImage:" + ex.Message);
            }

            return numImage;
        }
        private void GetVerCodeSrc()
        {
            Invoke(new MethodInvoker(delegate()
            {
                try { pictureBox1.ImageLocation = webBrowser1.Document.GetElementById("captcha_img").GetAttribute("src"); lbMsg.Text = "出现了验证码!"; }
                catch (Exception) { }
                /*IHTMLDocument3 Doc = CorssDomainHelper.GetDocumentFromWindow(webBrowser1.Document.Window.DomWindow as IHTMLWindow2);
                var ImagePic = Doc.getElementById("captcha_img") as IHTMLControlElement;
                if (ImagePic != null)
                {
                    
                   / * Image img = GetWebImage(Doc as HTMLDocument, ImagePic);
                    if (img != null)
                    {
                        img.Save("2.jpg");
                        MessageBox.Show("1");
                    }* /
                }*/
            }));

            int nCount = 0;
            while (!m_bVer && nCount++ <= 10 * 60 * 10) // 10m 内没人处理, 就继续下去!
            {
                if (nCount % (10 * 60) == 0)
                    Invoke(new MethodInvoker(delegate() { lbMsg.Text = "已经过" + (nCount / 10 / 60).ToString() + "分钟还没动静!"; }));
                else if (nCount % 10 == 0)
                    Invoke(new MethodInvoker(delegate() { lbMsg.Text = "已经过" + (nCount / 10).ToString() + "秒还没动静!"; }));

                Thread.Sleep(100);
            }

            Invoke(new MethodInvoker(delegate()
            {
                m_bVer = false;
                txtVerCode.Text = "";
                pictureBox1.Image = null;
            }));
            Thread.Sleep(5000);
        }

        private void BnsLoginThread()
        {
            int nCount = 1;
            StringBuilder sbRetMsg = new StringBuilder(1024);

            Display_Image_And_Flash();
            String strToken = String.Empty;

/*
            Invoke(new MethodInvoker(delegate() { webBrowser1.Navigate("http://bns.plaync.com/index?redirect=false"); }));
            Thread.Sleep(5000);
            Invoke(new MethodInvoker(delegate() { webBrowser1.Navigate("javascript:GNBLogin(); void 0;"); }));
            Thread.Sleep(5000);
*/
           // SetGlobalCookies("http://bns.plaync.com");
            do
            {
                if (nCount++ >= 8)
                {
                    MessageBox.Show("帐号:" + strAccountName + " 登录失败!自己找原因!");
                    break;
                }

                Invoke(new MethodInvoker(delegate() { try { webBrowser1.Navigate("http://bns.plaync.com/index?redirect=false"); lbMsg.Text = "正在访问官网!"; } catch (Exception) { } }));
                Thread.Sleep(5000 + nCount * 1000);
                Invoke(new MethodInvoker(delegate()
                {
                    try
                    {
                        webBrowser1.Document.GetElementById("id").SetAttribute("value", strAccountName);
                        webBrowser1.Document.GetElementById("pwd").SetAttribute("value", strAccountPass);
                        webBrowser1.Document.GetElementById("login").InvokeMember("Click");
                        lbMsg.Text = "输入帐号密码等待登录!";
                    }
                    catch (Exception)
                    {
                        
                    }
                }));
                Thread.Sleep(5000 + nCount * 1000);

                // 判断是否出现验证码!
                if (IsHasVer())
                {
                    GetVerCodeSrc();
                }
                Invoke(new MethodInvoker(delegate() { webBrowser1.Stop(); }));
            } while (!GetLoginKey(ref strToken));
            SetGlobalCookies("http://bns.plaync.com");
            Invoke(new MethodInvoker(delegate() { try { webBrowser1.Navigate("javascript:GNBLogout(); void 0;"); } catch (Exception) { } }));
            Thread.Sleep(5 * 1000);

            Invoke(new MethodInvoker(delegate() { try { webBrowser1.Navigate("www.baidu.com"); } catch (Exception) { } }));
            Thread.Sleep(2 * 1000);

            Enable_ImaGe_And_Flash();
            sbRetMsg.Append(nCount >= 5 ? "0" : strToken);
            Code.API.GetBnsLoginKey(sbRetMsg);
        }

        private bool m_bVer = false;
        private void btnSumit_Click(object sender, EventArgs e)
        {
            try
            {
                webBrowser1.Document.GetElementById("login_name").SetAttribute("value", strAccountName);
                webBrowser1.Document.GetElementById("password").SetAttribute("value", strAccountPass);
                webBrowser1.Document.GetElementById("capt").SetAttribute("value", txtVerCode.Text.Trim());
                webBrowser1.Document.GetElementById("btnSubmit").InvokeMember("Click");
                m_bVer = true;
            }
            catch(Exception)
            {
                MessageBox.Show("btnSumit_Click");
            }
        }

        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
           
        }

        private void SetAllWebItemSelf(HtmlElementCollection items)
        {
            
        }

        private void webBrowser1_NewWindow(object sender, CancelEventArgs e)
        {
           
        }
    }
}
