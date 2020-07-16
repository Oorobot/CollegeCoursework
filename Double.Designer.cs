using SimpleTetris;
using System.Net.Sockets;

namespace MyTetris
{
    partial class Double
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Double));
            this.button1 = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.label_1P_Score = new System.Windows.Forms.Label();
            this.label_1P_1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label_2P_1 = new System.Windows.Forms.Label();
            this.label_2P_Score = new System.Windows.Forms.Label();
            this.label_1P_2 = new System.Windows.Forms.Label();
            this.label_2P_2 = new System.Windows.Forms.Label();
            this.label_1P_Level = new System.Windows.Forms.Label();
            this.label_2P_Level = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.label1P_3 = new System.Windows.Forms.Label();
            this.label2P_3 = new System.Windows.Forms.Label();
            this.panel1P_next = new System.Windows.Forms.Panel();
            this.panel2P_next = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.Control;
            this.button1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("button1.BackgroundImage")));
            this.button1.Location = new System.Drawing.Point(408, 488);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(100, 30);
            this.button1.TabIndex = 0;
            this.button1.Text = "开始游戏";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.White;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.label3);
            this.panel1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.panel1.Location = new System.Drawing.Point(15, 45);
            this.panel1.Margin = new System.Windows.Forms.Padding(4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(380, 500);
            this.panel1.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 16.2F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label3.Location = new System.Drawing.Point(86, 227);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(215, 28);
            this.label3.TabIndex = 0;
            this.label3.Text = "游戏结束！！！";
            this.label3.Visible = false;
            // 
            // button2
            // 
            this.button2.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("button2.BackgroundImage")));
            this.button2.Location = new System.Drawing.Point(582, 488);
            this.button2.Margin = new System.Windows.Forms.Padding(4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(100, 30);
            this.button2.TabIndex = 2;
            this.button2.Text = "暂停游戏";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // timer2
            // 
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // label_1P_Score
            // 
            this.label_1P_Score.AutoSize = true;
            this.label_1P_Score.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_Score.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_Score.Location = new System.Drawing.Point(437, 89);
            this.label_1P_Score.Name = "label_1P_Score";
            this.label_1P_Score.Size = new System.Drawing.Size(25, 25);
            this.label_1P_Score.TabIndex = 7;
            this.label_1P_Score.Text = "0";
            // 
            // label_1P_1
            // 
            this.label_1P_1.AutoSize = true;
            this.label_1P_1.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_1.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_1.Location = new System.Drawing.Point(403, 53);
            this.label_1P_1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label_1P_1.Name = "label_1P_1";
            this.label_1P_1.Size = new System.Drawing.Size(93, 25);
            this.label_1P_1.TabIndex = 8;
            this.label_1P_1.Text = "得分：";
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.White;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2.Controls.Add(this.label4);
            this.panel2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.panel2.Location = new System.Drawing.Point(690, 45);
            this.panel2.Margin = new System.Windows.Forms.Padding(4);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(380, 500);
            this.panel2.TabIndex = 10;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 16.2F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label4.Location = new System.Drawing.Point(89, 225);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(215, 28);
            this.label4.TabIndex = 13;
            this.label4.Text = "游戏结束！！！";
            this.label4.Visible = false;
            // 
            // label_2P_1
            // 
            this.label_2P_1.AutoSize = true;
            this.label_2P_1.BackColor = System.Drawing.Color.Transparent;
            this.label_2P_1.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_2P_1.Location = new System.Drawing.Point(589, 53);
            this.label_2P_1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label_2P_1.Name = "label_2P_1";
            this.label_2P_1.Size = new System.Drawing.Size(93, 25);
            this.label_2P_1.TabIndex = 12;
            this.label_2P_1.Text = "得分：";
            // 
            // label_2P_Score
            // 
            this.label_2P_Score.AutoSize = true;
            this.label_2P_Score.BackColor = System.Drawing.Color.Transparent;
            this.label_2P_Score.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_2P_Score.Location = new System.Drawing.Point(622, 89);
            this.label_2P_Score.Name = "label_2P_Score";
            this.label_2P_Score.Size = new System.Drawing.Size(25, 25);
            this.label_2P_Score.TabIndex = 11;
            this.label_2P_Score.Text = "0";
            // 
            // label_1P_2
            // 
            this.label_1P_2.AutoSize = true;
            this.label_1P_2.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_2.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_2.Location = new System.Drawing.Point(403, 160);
            this.label_1P_2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label_1P_2.Name = "label_1P_2";
            this.label_1P_2.Size = new System.Drawing.Size(93, 25);
            this.label_1P_2.TabIndex = 15;
            this.label_1P_2.Text = "难度：";
            // 
            // label_2P_2
            // 
            this.label_2P_2.AutoSize = true;
            this.label_2P_2.BackColor = System.Drawing.Color.Transparent;
            this.label_2P_2.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_2P_2.Location = new System.Drawing.Point(589, 160);
            this.label_2P_2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label_2P_2.Name = "label_2P_2";
            this.label_2P_2.Size = new System.Drawing.Size(93, 25);
            this.label_2P_2.TabIndex = 16;
            this.label_2P_2.Text = "难度：";
            // 
            // label_1P_Level
            // 
            this.label_1P_Level.AutoSize = true;
            this.label_1P_Level.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_Level.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_Level.Location = new System.Drawing.Point(437, 199);
            this.label_1P_Level.Name = "label_1P_Level";
            this.label_1P_Level.Size = new System.Drawing.Size(25, 25);
            this.label_1P_Level.TabIndex = 17;
            this.label_1P_Level.Text = "0";
            // 
            // label_2P_Level
            // 
            this.label_2P_Level.AutoSize = true;
            this.label_2P_Level.BackColor = System.Drawing.Color.Transparent;
            this.label_2P_Level.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_2P_Level.Location = new System.Drawing.Point(622, 199);
            this.label_2P_Level.Name = "label_2P_Level";
            this.label_2P_Level.Size = new System.Drawing.Size(25, 25);
            this.label_2P_Level.TabIndex = 18;
            this.label_2P_Level.Text = "0";
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.Color.Transparent;
            this.button3.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("button3.BackgroundImage")));
            this.button3.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.button3.Location = new System.Drawing.Point(1038, 12);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(30, 25);
            this.button3.TabIndex = 21;
            this.button3.UseVisualStyleBackColor = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label1P_3
            // 
            this.label1P_3.AutoSize = true;
            this.label1P_3.BackColor = System.Drawing.Color.Transparent;
            this.label1P_3.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1P_3.Location = new System.Drawing.Point(403, 260);
            this.label1P_3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1P_3.Name = "label1P_3";
            this.label1P_3.Size = new System.Drawing.Size(93, 25);
            this.label1P_3.TabIndex = 23;
            this.label1P_3.Text = "下一个";
            // 
            // label2P_3
            // 
            this.label2P_3.AutoSize = true;
            this.label2P_3.BackColor = System.Drawing.Color.Transparent;
            this.label2P_3.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2P_3.Location = new System.Drawing.Point(589, 260);
            this.label2P_3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2P_3.Name = "label2P_3";
            this.label2P_3.Size = new System.Drawing.Size(93, 25);
            this.label2P_3.TabIndex = 24;
            this.label2P_3.Text = "下一个";
            // 
            // panel1P_next
            // 
            this.panel1P_next.BackColor = System.Drawing.Color.Transparent;
            this.panel1P_next.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1P_next.Location = new System.Drawing.Point(408, 305);
            this.panel1P_next.Name = "panel1P_next";
            this.panel1P_next.Size = new System.Drawing.Size(100, 100);
            this.panel1P_next.TabIndex = 25;
            // 
            // panel2P_next
            // 
            this.panel2P_next.BackColor = System.Drawing.Color.Transparent;
            this.panel2P_next.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2P_next.Location = new System.Drawing.Point(582, 305);
            this.panel2P_next.Name = "panel2P_next";
            this.panel2P_next.Size = new System.Drawing.Size(100, 100);
            this.panel2P_next.TabIndex = 26;
            // 
            // Double
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(1082, 553);
            this.Controls.Add(this.panel2P_next);
            this.Controls.Add(this.panel1P_next);
            this.Controls.Add(this.label2P_3);
            this.Controls.Add(this.label1P_3);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label_2P_Level);
            this.Controls.Add(this.label_1P_Level);
            this.Controls.Add(this.label_2P_2);
            this.Controls.Add(this.label_1P_2);
            this.Controls.Add(this.label_2P_1);
            this.Controls.Add(this.label_2P_Score);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.label_1P_1);
            this.Controls.Add(this.label_1P_Score);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.ControlText;
            this.KeyPreview = true;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Double";
            this.Text = "俄罗斯方块";
            this.Load += new System.EventHandler(this.Double_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Double_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Double_KeyUp);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.Label label_1P_1;
        private System.Windows.Forms.Label label_1P_Score;
        private System.Windows.Forms.Label label_2P_1;
        private System.Windows.Forms.Label label_2P_Score;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label_1P_2;
        private System.Windows.Forms.Label label_2P_2;
        private System.Windows.Forms.Label label_1P_Level;
        private System.Windows.Forms.Label label_2P_Level;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label1P_3;
        private System.Windows.Forms.Label label2P_3;
        private System.Windows.Forms.Panel panel1P_next;
        private System.Windows.Forms.Panel panel2P_next;
    }
}

