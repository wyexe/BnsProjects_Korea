using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using BnsConsole.Code;
using System.Diagnostics;

namespace BnsConsole.Setting
{
    public partial class CardSetting : Form
    {
        public CardSetting()
        {
            InitializeComponent();
        }
        private Thread hReadCardThread = null;

        private void CardSetting_Load(object sender, EventArgs e)
        {
            Refush();
            btnAddCard.Enabled = false;
            btnRefush.Enabled = false;
        }

        private void Refush()
        {
            hReadCardThread = new Thread(ReadCardThread);
            hReadCardThread.Start();
        }

        private void ReadCardThread()
        {
            List<CCard> lst = new List<CCard>();
            if(!ConsoleFile.GetCard_By_File(ref lst))
            {
                Invoke(new MethodInvoker(delegate()
                {
                    this.Text = "本地保存的卡号数量=0";
                }));
            }
            else
            {
                Invoke(new MethodInvoker(delegate()
                {
                    this.dataGridView1.Rows.Clear();

                    // ShowData
                    for (int i = 0; i < lst.Count; i++)
                    {
                        this.dataGridView1.Rows.Add();
                        this.dataGridView1.Rows[i].Cells[0].Value = lst[i]._strCard;
                        this.dataGridView1.Rows[i].Cells[1].Value = "…………";
                    }
                }));

                CCardFunction.GetCardInfo_By_Server(lst);
                for (int i = 0; i < lst.Count; i++)
                {
                    var obj = Code.CGlobalVariable.g_WorkCardList.Where((CCard Card) => Card._strCard == lst[i]._strCard).ToList();
                    if (obj.Count > 0)
                    {
                        this.dataGridView1.Rows[i].Cells[1].Value = obj[0]._nCardAllHour;
                        this.dataGridView1.Rows[i].Cells[2].Value = obj[0]._strActiveDate;
                        this.dataGridView1.Rows[i].Cells[3].Value = obj[0]._nCardSurplusHour;
                    }
                }
                Invoke(new MethodInvoker(delegate()
                {
                    if (CGlobalVariable.g_WorkCardList != null && CGlobalVariable.g_WorkCardList.Count > 0)
                        this.Text = "有效卡号数量 = " + CGlobalVariable.g_WorkCardList.Where((CCard Card) => Card._nCardSurplusHour > 0).ToList().Count.ToString();
                    else
                        this.Text = "有效卡号数量 = 0";
                }));
            }
            
            Invoke(new MethodInvoker(delegate()
            {
                btnAddCard.Enabled = true;
                btnRefush.Enabled = true;
            }));
        }

        private void btnAddCard_Click(object sender, EventArgs e)
        {
            Process.Start(System.Environment.CurrentDirectory + @"\Setting\Card.txt");
        }

        private void btnRefush_Click(object sender, EventArgs e)
        {
            btnRefush.Enabled = false;
            Refush();
        }
    }
}
