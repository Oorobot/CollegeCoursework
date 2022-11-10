using System;
using System.Windows.Forms;

namespace SimpleTetris
{
    public delegate void TcpInfo(string ip,string port);
    public partial class Info : Form
    {
        public Info()
        {
            InitializeComponent();
        }

        public event TcpInfo InfoEvent;
        private void button1_Click(object sender, EventArgs e)
        {
            InfoEvent(textBox1.Text,textBox2.Text);
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
