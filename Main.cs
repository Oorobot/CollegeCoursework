using MyTetris;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SimpleTetris
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            GameExplanation form = new GameExplanation();
            form.StartPosition = FormStartPosition.CenterParent;
            form.ShowDialog();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MyTetris.Double form = new MyTetris.Double();
            this.Hide();
            DialogResult d = form.ShowDialog();
            if (d == DialogResult.Cancel)
                this.Visible = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Single form = new Single();
            this.Hide();
            DialogResult d = form.ShowDialog();
            if (d == DialogResult.Cancel)
                this.Visible = true;
        }
    }
}
