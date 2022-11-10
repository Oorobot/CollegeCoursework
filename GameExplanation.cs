using System;
using System.Windows.Forms;

namespace SimpleTetris
{
    public partial class GameExplanation : Form
    {
        public GameExplanation()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void GameExplanation_Load(object sender, EventArgs e)
        { 
        }
    }
}
