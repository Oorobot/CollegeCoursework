using System;
using System.Drawing;

namespace SimpleTetris
{
    //俄罗斯方块
    class Brick
    {
        //成员
        private int[] type = new int[2];
        private int[,] brick = new int[4, 4];
        private Color color;
        //get/set
        public int[] Type
        {
            get { return type; }
            set { type = value; }
        }
        public int[,] _Brick
        {
            get { return brick; }
            set { brick = value; }
        }
        public Color Color
        {
            get { return color; }
            set { color = value; }
        }

        //成员函数
        public Brick()
        {
            Random random = new Random();
            type[0] = random.Next(7);
            type[1] = random.Next(4);
            color = Constant.AllColor[type[0]];
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    brick[j, i] = Constant.AllBrick[type[0], type[1], j, i];
                }
            }
        }

        public void Transform()
        {
            type[1] = (type[1] + 1) % 4;
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    brick[y, x] = Constant.AllBrick[type[0], type[1], y, x];
                }
            }
        }
    }

    //格子
    class Square
    {
        private int n = 0;
        private Color color = Color.White;
        public int N
        {
            get { return n; }
            set { n = value; }
        }
        public Color Color
        {
            get { return color; }
            set { color = value; }
        }
        public Square()
        {
            n = 0;
            color = Color.White;
        }
    }
}
