using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BnsConsole.Code
{
    public class CCardFunction
    {
        delegate CCard GetCard_By_Text(string str);
        public static List<CCard> GetCardInfo_By_Server(List<CCard> lst)
        {
            try
            {
                GetCard_By_Text fnGetCard_By_Text = (str) =>
                {
                    CCard Card = new CCard();
                    // Card--0720--2015:1:1--0720
                    string[] strArray = str.Split(new string[] { "--" }, StringSplitOptions.RemoveEmptyEntries);
 
                    Card._strCard = strArray[0];
                    Card._nCardAllHour = int.Parse(strArray[1]);
                    Card._nCardSurplusHour = int.Parse(strArray[2]);
                    if (Card._nCardSurplusHour != 0)
                        Card._strActiveDate = strArray[3];
                    else
                        Card._strActiveDate = "此卡无效!";

                    return Card;
                };

                // push Card
                API.QueryCardInfo("", 0x2); // Clear
                for (int i = 0; i < lst.Count; i++)
                    API.QueryCardInfo(lst[i]._strCard, 0);

                // Query Card by Server
                int nRetCount = API.QueryCardInfo("", 1);
                if (nRetCount == 0)
                {
                    MessageBox.Show("查询卡号失败!");
                    return CGlobalVariable.g_WorkCardList;
                }

                Code.CGlobalVariable.g_WorkCardList.Clear();
                for (int i = 0; i < lst.Count; i++)
                {
                    StringBuilder sbRetMsg = new StringBuilder(1024);
                    if (API.GetCardInfo(lst[i]._strCard, sbRetMsg) == 1)
                    {
                        lst[i] = fnGetCard_By_Text(sbRetMsg.ToString());
                        Code.CGlobalVariable.g_WorkCardList.Add(lst[i]);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("GetCardInfo_By_Server:" + ex.Message);
            }
            return CGlobalVariable.g_WorkCardList;
        }
    }
    public class CCard
    {
        private string strCard = string.Empty;

        public string _strCard
        {
            get { return strCard; }
            set { strCard = value; }
        }
        private int nCardAllHour = 0;

        public int _nCardAllHour
        {
            get { return nCardAllHour; }
            set { nCardAllHour = value; }
        }
        private int nCardSurplusHour = 0;

        public int _nCardSurplusHour
        {
            get { return nCardSurplusHour; }
            set { nCardSurplusHour = value; }
        }
        private string strActiveDate = string.Empty;

        public string _strActiveDate
        {
            get { return strActiveDate; }
            set { strActiveDate = value; }
        }
        private string strMsg = string.Empty;

        public string _strMsg
        {
            get { return strMsg; }
            set { strMsg = value; }
        }
    }

    public class CAccount
    {
        private string _strUserName = string.Empty;

        public string strUserName
        {
            get { return _strUserName; }
            set { _strUserName = value; }
        }
        private string _strPassWord = string.Empty;

        public string strPassWord
        {
            get { return _strPassWord; }
            set { _strPassWord = value; }
        }
        private CCard _Card = new CCard();

        public CCard Card
        {
            get { return _Card; }
            set { _Card = value; }
        }
        private string _strLog = string.Empty;

        public string strLog
        {
            get { return _strLog; }
            set { _strLog = value; }
        }

        private string strCloseReason_ = string.Empty;

        public string strCloseReason
        {
            get { return strCloseReason_; }
            set { strCloseReason_ = value; }
        }

        private int nRepeatCount_ = 0;

        public int nRepeatCount
        {
            get { return nRepeatCount_; }
            set { nRepeatCount_ = value; }
        }
    }

    public class CAccountLog
    {
        private string _strAccount = string.Empty;

        public string strAccount
        {
            get { return _strAccount; }
            set { _strAccount = value; }
        }
        private int _nPlayerIndex = 0;

        public int nPlayerIndex
        {
            get { return _nPlayerIndex; }
            set { _nPlayerIndex = value; }
        }
        private DateTime _dtTime = new DateTime();

        public DateTime dtTime
        {
            get { return _dtTime; }
            set { _dtTime = value; }
        }
    }
}
