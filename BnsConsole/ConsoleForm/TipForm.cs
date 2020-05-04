using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BnsConsole.ConsoleForm
{
    public partial class TipForm : Form
    {
        public TipForm()
        {
            InitializeComponent();
        }

        private void TipForm_Load(object sender, EventArgs e)
        {

        }

        public void SetText(String Text)
        {
            lbText.Text = Text;
        }
    }
}
