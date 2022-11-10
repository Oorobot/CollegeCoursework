namespace SimpleTetris
{
    partial class Single
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Single));
            this.panel1P_next = new System.Windows.Forms.Panel();
            this.label1P_3 = new System.Windows.Forms.Label();
            this.label_1P_Level = new System.Windows.Forms.Label();
            this.label_1P_2 = new System.Windows.Forms.Label();
            this.label_1P_1 = new System.Windows.Forms.Label();
            this.label_1P_Score = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1P_next
            // 
            this.panel1P_next.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1P_next.Location = new System.Drawing.Point(406, 301);
            this.panel1P_next.Name = "panel1P_next";
            this.panel1P_next.Size = new System.Drawing.Size(100, 100);
            this.panel1P_next.TabIndex = 33;
            // 
            // label1P_3
            // 
            this.label1P_3.AutoSize = true;
            this.label1P_3.BackColor = System.Drawing.Color.Transparent;
            this.label1P_3.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1P_3.Location = new System.Drawing.Point(401, 256);
            this.label1P_3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1P_3.Name = "label1P_3";
            this.label1P_3.Size = new System.Drawing.Size(93, 25);
            this.label1P_3.TabIndex = 32;
            this.label1P_3.Text = "下一个";
            // 
            // label_1P_Level
            // 
            this.label_1P_Level.AutoSize = true;
            this.label_1P_Level.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_Level.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_Level.Location = new System.Drawing.Point(435, 195);
            this.label_1P_Level.Name = "label_1P_Level";
            this.label_1P_Level.Size = new System.Drawing.Size(25, 25);
            this.label_1P_Level.TabIndex = 31;
            this.label_1P_Level.Text = "0";
            // 
            // label_1P_2
            // 
            this.label_1P_2.AutoSize = true;
            this.label_1P_2.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_2.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_2.Location = new System.Drawing.Point(401, 156);
            this.label_1P_2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label_1P_2.Name = "label_1P_2";
            this.label_1P_2.Size = new System.Drawing.Size(93, 25);
            this.label_1P_2.TabIndex = 30;
            this.label_1P_2.Text = "难度：";
            // 
            // label_1P_1
            // 
            this.label_1P_1.AutoSize = true;
            this.label_1P_1.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_1.Font = new System.Drawing.Font("黑体", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_1.Location = new System.Drawing.Point(401, 49);
            this.label_1P_1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label_1P_1.Name = "label_1P_1";
            this.label_1P_1.Size = new System.Drawing.Size(93, 25);
            this.label_1P_1.TabIndex = 29;
            this.label_1P_1.Text = "得分：";
            // 
            // label_1P_Score
            // 
            this.label_1P_Score.AutoSize = true;
            this.label_1P_Score.BackColor = System.Drawing.Color.Transparent;
            this.label_1P_Score.Font = new System.Drawing.Font("宋体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_1P_Score.Location = new System.Drawing.Point(435, 85);
            this.label_1P_Score.Name = "label_1P_Score";
            this.label_1P_Score.Size = new System.Drawing.Size(25, 25);
            this.label_1P_Score.TabIndex = 28;
            this.label_1P_Score.Text = "0";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.Control;
            this.button1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("button1.BackgroundImage")));
            this.button1.Location = new System.Drawing.Point(406, 423);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(100, 30);
            this.button1.TabIndex = 26;
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
            this.panel1.TabIndex = 27;
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
            this.button2.Location = new System.Drawing.Point(406, 461);
            this.button2.Margin = new System.Windows.Forms.Padding(4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(100, 30);
            this.button2.TabIndex = 34;
            this.button2.Text = "暂停游戏";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("button3.BackgroundImage")));
            this.button3.Location = new System.Drawing.Point(406, 499);
            this.button3.Margin = new System.Windows.Forms.Padding(4);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(100, 30);
            this.button3.TabIndex = 35;
            this.button3.Text = "返回主窗口";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Single
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(539, 553);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.panel1P_next);
            this.Controls.Add(this.label1P_3);
            this.Controls.Add(this.label_1P_Level);
            this.Controls.Add(this.label_1P_2);
            this.Controls.Add(this.label_1P_1);
            this.Controls.Add(this.label_1P_Score);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.panel1);
            this.KeyPreview = true;
            this.Name = "Single";
            this.Text = "Single";
            this.Load += new System.EventHandler(this.Single_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Single_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Single_KeyUp);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1P_next;
        private System.Windows.Forms.Label label1P_3;
        private System.Windows.Forms.Label label_1P_Level;
        private System.Windows.Forms.Label label_1P_2;
        private System.Windows.Forms.Label label_1P_1;
        private System.Windows.Forms.Label label_1P_Score;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Timer timer1;
    }
}