using SimpleTetris;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace MyTetris
{
    public partial class Double : Form
    {

        public Double()
        {
            InitializeComponent();
        }

        private Image image1, image2, image1P_next, image2P_next;
        private BrickFactory brickFactory1, brickFactory2;

        private void Double_Load(object sender, EventArgs e)
        {
            image1 = new Bitmap(panel1.Width, panel1.Height);
            image2 = new Bitmap(panel2.Width, panel2.Height);
            image1P_next = new Bitmap(panel1P_next.Width, panel1P_next.Height);
            image2P_next = new Bitmap(panel2P_next.Width, panel2P_next.Height);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory1);
            Draw(image2, image2P_next, panel2, panel2P_next, ref brickFactory2);
            base.OnPaint(e);
        }
        /// <summary>
        /// 开始游戏
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            label3.Visible = false;
            label4.Visible = false;

            brickFactory1 = new BrickFactory();
            label_1P_Score.Text = brickFactory1.Score.ToString();
            label_1P_Level.Text = brickFactory1.Level.ToString();
            brickFactory1.init(0);
            brickFactory2 = new BrickFactory();
            label_2P_Score.Text = brickFactory2.Score.ToString();
            label_2P_Level.Text = brickFactory2.Level.ToString();
            brickFactory2.init(0);

            timer1.Interval = Constant.interval;
            timer2.Interval = Constant.interval;
            timer1.Start();
            timer2.Start();
            this.Focus();
        }
        /// <summary>
        /// 暂停游戏
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            if (button2.Text == "暂停游戏")
            {
                button2.Text = "继续游戏";
                timer1.Stop();
                timer2.Stop();
            }
            else
            {
                button2.Text = "暂停游戏";
                timer1.Start();
                timer2.Start();
            }
        }
        //每一个时间间隔下降一格
        private void timer1_Tick(object sender, EventArgs e)
        {
            Down(ref brickFactory1, label_1P_Score, label3, timer1, image1, panel1, image1P_next, panel1P_next);
            if (brickFactory1.Level != 10)
            {
                label_1P_Level.Text = brickFactory1.CalculateLevel().ToString();
                timer1.Interval = Constant.interval - Constant.intervalPerLevel * brickFactory1.Level;
            }
        }
        private void timer2_Tick(object sender, EventArgs e)
        {
            Down(ref brickFactory2, label_2P_Score, label4, timer2, image2, panel2, image2P_next, panel2P_next);
            if (brickFactory2.Level != 10)
            {
                label_2P_Level.Text = brickFactory2.CalculateLevel().ToString();
                timer2.Interval = Constant.interval - Constant.intervalPerLevel * brickFactory2.Level;
            }
        }
        //KeyDown
        private void Double_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.W)
            {
                brickFactory1.TransformBrick();
                Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory1);
            }
            else if (e.KeyCode == Keys.A)
            {
                if (brickFactory1.IsLeft())
                {
                    brickFactory1.X--;
                }
                Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory1);
            }
            else if (e.KeyCode == Keys.D)
            {
                if (brickFactory1.IsRight())
                {
                    brickFactory1.X++;
                }
                Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory1);
            }
            else if (e.KeyCode == Keys.S)
            {
                timer1.Stop();
                timer1.Interval = 10;
                timer1.Start();
            }
            else if (e.KeyCode == Keys.I)
            {
                brickFactory2.TransformBrick();
                Draw(image2, image2P_next, panel2, panel2P_next, ref brickFactory2);
            }
            else if (e.KeyCode == Keys.J)
            {
                if (brickFactory2.IsLeft())
                {
                    brickFactory2.X--;
                }
                Draw(image2, image2P_next, panel2, panel2P_next, ref brickFactory2);
            }
            else if (e.KeyCode == Keys.L)
            {
                if (brickFactory2.IsRight())
                {
                    brickFactory2.X++;
                }
                Draw(image2, image2P_next, panel2, panel2P_next, ref brickFactory2);
            }
            else if (e.KeyCode == Keys.K)
            {
                timer2.Stop();
                timer2.Interval = 10;
                timer2.Start();
            }
        }
        //KeyUp
        private void Double_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.S)
            {
                timer1.Stop();
                timer1.Interval = Constant.interval - Constant.intervalPerLevel * brickFactory1.Level;
                timer1.Start();
            }
            if (e.KeyCode == Keys.K)
            {
                timer2.Stop();
                timer2.Interval = Constant.interval - Constant.intervalPerLevel * brickFactory2.Level;
                timer2.Start();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Info info = new Info();
            info.InfoEvent += TcpInfo;
            info.StartPosition = FormStartPosition.CenterParent;
            info.ShowDialog();
        }

        void TcpInfo(String ip, String port)
        {
            Console.WriteLine(ip);
            Console.WriteLine(port);
        }

        private void Down(ref BrickFactory brickFactory, Label Score_label, Label gameOver,
                                Timer timer, Image i, Panel p, Image i_next, Panel p_next)
        {
            if (brickFactory.IsDown())
            {
                brickFactory.Y++;
            }
            else
            {
                if (brickFactory.Y == 0)
                {
                    timer.Stop();
                    gameOver.Visible = true;
                    return;
                }

                for (int y = 0; y < 4; y++)
                {
                    for (int x = 0; x < 4; x++)
                    {
                        if (brickFactory.Brick._Brick[y, x] == 1)
                        {
                            brickFactory.BackGround[brickFactory.Y + y, brickFactory.X + x].N = brickFactory.Brick._Brick[y, x];
                            brickFactory.BackGround[brickFactory.Y + y, brickFactory.X + x].Color = brickFactory.Brick.Color;
                        }
                    }
                }
                Score_label.Text = brickFactory.CalculateScore().ToString();
                brickFactory.init(1);
                timer.Start();
            }
            Draw(i, i_next, p, p_next, ref brickFactory);
        }

        private void Draw(Image image, Image image_next, Panel panel, Panel panel_next, ref BrickFactory brickFactory)
        {
            Graphics graphics = Graphics.FromImage(image);
            Graphics graphics_next = Graphics.FromImage(image_next);
            graphics.Clear(this.BackColor);
            graphics_next.Clear(this.BackColor);
            if (brickFactory != null)
            {
                for (int bgy = 0; bgy < 20; bgy++)
                {
                    for (int bgx = 0; bgx < 14; bgx++)
                    {
                        if (brickFactory.BackGround[bgy, bgx].N == 1)
                        {
                            graphics.FillRectangle(new SolidBrush(brickFactory.BackGround[bgy, bgx].Color), bgx * 20, bgy * 20, 20, 20);
                            graphics.DrawRectangle(new Pen(Color.Black), bgx * 20, bgy * 20, 20, 20);
                        }
                    }
                }
                for (int row = 0; row < 4; row++)
                {
                    for (int col = 0; col < 4; col++)
                    {
                        if (brickFactory.Brick._Brick[row, col] == 1)
                        {
                            graphics.FillRectangle(new SolidBrush(brickFactory.Brick.Color), (col + brickFactory.X) * 20, (row + brickFactory.Y) * 20, 20, 20);
                            graphics.DrawRectangle(new Pen(Color.Black), (col + brickFactory.X) * 20, (row + brickFactory.Y) * 20, 20, 20);
                        }
                        if (brickFactory.Next_Brick._Brick[row, col] == 1)
                            graphics_next.FillRectangle(new SolidBrush(brickFactory.Next_Brick.Color), col * 20, row * 20, 20, 20);
                    }
                }
            }
            Graphics graphics1 = panel.CreateGraphics();
            Graphics graphics2 = panel_next.CreateGraphics();
            graphics1.DrawImage(image, 0, 0);
            graphics2.DrawImage(image_next, 0, 0);
        }
    }
}
