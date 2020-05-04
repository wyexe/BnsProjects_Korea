using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace QueryResText
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        
        private List<ResTextContent> VecResChinaText = new List<ResTextContent>();
        private List<ResTextContent> VecResKoreaText = new List<ResTextContent>();
        private void Form1_Load(object sender, EventArgs e)
        {
            Task.Factory.StartNew(() =>
            {
                VecResChinaText = LoadFile(System.Environment.CurrentDirectory + @"\国服资源名字.txt");
                VecResKoreaText = LoadFile(System.Environment.CurrentDirectory + @"\韩服资源名字.txt");
                if (VecResChinaText.Count == 0)
                {
                    MessageBox.Show("'国服资源名字.txt' UnExist!");
                    return;
                }
                if (VecResKoreaText.Count == 0)
                {
                    MessageBox.Show("'韩服资源名字.txt' UnExist!");
                    return;
                }

                txtName.Text = "";
                btnQuery.Enabled = true;
            });
        }

        private List<ResTextContent> LoadFile(String Path)
        {
            var VecResTextContent = new List<ResTextContent>();
            using (StreamReader Reader = new StreamReader(Path, Encoding.Unicode))
            {
                var Text = String.Empty;
                while ((Text = Reader.ReadLine()) != null)
                {
                    var Tup = ResTextContent.Create(Text);
                    if (Tup.Item1)
                    {
                        VecResTextContent.Add(Tup.Item2);
                    }
                }
                Reader.Close();
            }

            return VecResTextContent;
        }

        private void FindInChinaText(String Name)
        {
            VecResChinaText.FindAll(itm => itm.ResName == Name).ToList().ForEach(item => 
            {
                var Element = VecResKoreaText.Find(itm => itm.ResText == item.ResText);
                if (Element != null)
                {
                    txtResult.Text += String.Format("{0}--{1}--{2}\r\n", item.ResName, item.ResText, Element.ResName);
                }
            });
        }

        private void FindInKorText(String Name)
        {
            VecResKoreaText.FindAll(itm => itm.ResName == Name).ToList().ForEach(item => 
            {
                var Element = VecResChinaText.Find(itm => itm.ResText == item.ResText);
                if (Element != null)
                {
                    txtResult.Text += String.Format("{0}--{1}--{2}\r\n", item.ResName, item.ResText, Element.ResName);
                }
            });
        }

        private void btnQuery_Click(object sender, EventArgs e)
        {
            txtResult.Text = "";

            FindInChinaText(txtName.Text.Trim());
            FindInKorText(txtName.Text.Trim());
        }
    }

    public class ResTextContent
    {
        public String ResName { get; set; }
        public String ResText { get; set; }

        public static Tuple<bool, ResTextContent> Create(String Text)
        {
            var Index = Text.IndexOf("--");
            if (Index == -1)
            {
                return Tuple.Create(false, new ResTextContent());
            }

            return Tuple.Create(true, new ResTextContent() { ResName = Text.Substring(0, Index), ResText = Text.Substring(Index + 2) });
        }
    }
}
