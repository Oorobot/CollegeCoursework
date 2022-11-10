using MyTetris;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace SimpleTetris
{
    public partial class Single : Form
    {
        public Single()
        {
            InitializeComponent();
        }

        private Image image1, image1P_next;
        private BrickFactory brickFactory;


        private void Single_Load(object sender, EventArgs e)
        {
            image1 = new Bitmap(panel1.Width, panel1.Height);
            image1P_next = new Bitmap(panel1P_next.Width, panel1P_next.Height);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory);
            base.OnPaint(e);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            label3.Visible = false;
            brickFactory = new BrickFactory();
            label_1P_Score.Text = brickFactory.Score.ToString();
            label_1P_Level.Text = brickFactory.Level.ToString();
            brickFactory.init(0);
            timer1.Interval = Constant.interval;
            timer1.Start();
            this.Focus();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Down(ref brickFactory, label_1P_Score, label3, timer1, image1, panel1, image1P_next, panel1P_next);
            if (brickFactory.Level != 10)
            {
                label_1P_Level.Text = brickFactory.CalculateLevel().ToString();
                timer1.Interval = Constant.interval - Constant.intervalPerLevel * brickFactory.Level;
            }
        }

        private void Single_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.W)
            {
                brickFactory.TransformBrick();
                Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory);
            }
            else if (e.KeyCode == Keys.A)
            {
                if (brickFactory.IsLeft())
                {
                    brickFactory.X--;
                }
                Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory);
            }
            else if (e.KeyCode == Keys.D)
            {
                if (brickFactory.IsRight())
                {
                    brickFactory.X++;
                }
                Draw(image1, image1P_next, panel1, panel1P_next, ref brickFactory);
            }
            else if (e.KeyCode == Keys.S)
            {
                timer1.Stop();
                timer1.Interval = 10;
                timer1.Start();
            }
        }

        private void Single_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.S)
            {
                timer1.Stop();
                timer1.Interval = Constant.interval - Constant.intervalPerLevel * brickFactory.Level;
                timer1.Start();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (button2.Text == "暂停游戏")
            {
                button2.Text = "继续游戏";
                timer1.Stop();
            }
            else
            {
                button2.Text = "暂停游戏";
                timer1.Start();
            }
        }


        private void button3_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
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
                            graphics.FillRectangle(new SolidBrush(brickFactory.BackGround[bgy, bgx].Color), bgx * 20, bgy * 20, 20, 20);
                            graphics.DrawRectangle(new Pen(Color.White), bgx * 20, bgy * 20, 20, 20);
                    }
                }
                for (int row = 0; row < 4; row++)
                {
                    for (int col = 0; col < 4; col++)
                    {
                        if (brickFactory.Brick._Brick[row, col] == 1)
                        {
                            graphics.FillRectangle(new SolidBrush(brickFactory.Brick.Color), (col + brickFactory.X) * 20, (row + brickFactory.Y) * 20, 20, 20);
                            graphics.DrawRectangle(new Pen(Color.White), (col + brickFactory.X) * 20, (row + brickFactory.Y) * 20, 20, 20);

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
