using SimpleTetris;

namespace MyTetris
{
    class BrickFactory
    {
        //成员
        private int x; 
        private int y;
        private int score;
        private int level;
        private Brick brick;
        private Brick next_brick;
        private Square[,] backGround = new Square[20, 14];
        //get/set
        public int X
        {
            get { return x; }
            set { x = value; }
        }
        public int Y
        {
            get { return y; }
            set { y = value; }
        }
        public Brick Brick
        {
            get { return brick; }
            set { brick = value; }
        }
        public Brick Next_Brick
        {
            get { return next_brick; }
            set { next_brick = value; }
        }
        public int Score
        {
            get { return score; }
            set { score = value; }
        }
        public int Level
        {
            get { return level; }
            set { level = value; }
        }
        public Square[,] BackGround
        {
            get { return backGround; }
            set { backGround = value; }
        }
        //构造函数
        public BrickFactory()
        {
            x = 0;
            y = 0;
            score = 0;
            level = 0;
            brick = new Brick();
            next_brick = new Brick();
            for(int row = 0; row < 20; row++)
            {
                for(int col = 0; col < 14; col++)
                {
                    backGround[row, col] = new Square();
                }
            }
        }
        //初始化
        public void init(int f)
        {
            if (f == 1)
            {
                brick = next_brick;
            }
            next_brick = new Brick();
            x = 0;
            y = 0;
        }
        //是否到达底部
        public bool IsDown()
        {
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (brick._Brick[y, x] == 1)
                    {
                        if (y + this.y + 1 >= 20)
                            return false;
                        if (x + this.x >= 14)                     
                            this.x = 13 - x;
                        if (backGround[y + this.y + 1, x + this.x].N == 1)
                            return false;
                    }
                }
            }
            return true;
        }
        //是否可以左移
        public bool IsLeft()
        {
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (brick._Brick[y, x] == 1)
                    {
                        if (x + this.x - 1 < 0)
                        {
                            return false;
                        }
                        if (x + this.x - 1 < 14 &&
                            backGround[y + this.y, x + this.x - 1].N == 1)
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        //是否可以右移
        public bool IsRight()
        {
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    if (brick._Brick[y, x] == 1)
                    {
                        if (x + this.x + 1 >= 14)
                        {
                            return false;
                        }
                        if (x + this.x + 1 > -1 &&
                            backGround[y + this.y, x + this.x + 1].N == 1)
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        //砖块变形
        public void TransformBrick()
        {
            brick.Transform();
            while (!IsDown())
            {
                y--;
            }
            while (!IsLeft())
            {
                x++;
            }
            while (!IsRight())
            {
                x--;
            }
        }
        //计算分数,消去一行加分。
        public int CalculateScore()
        {
            for (int y = 19; y > -1; y--)
            {
                bool isFull = true;
                for (int x = 13; x > -1; x--)
                {
                    if (backGround[y, x].N == 0)
                    {
                        isFull = false;
                        break;
                    }
                }
                if (isFull)
                {
                    score = score + Constant.scorePerLine;
                    for (int yy = y; yy > 0; yy--)
                    {
                        for (int xx = 0; xx < 14; xx++)
                        {
                            Square temp = backGround[yy - 1, xx];
                            backGround[yy, xx] = temp;
                        }
                    }
                    y++;
                }
            }
            return score;
        }
        //难度根据分数提升，最高为10.
        public int CalculateLevel()
        {
            if (level == 10)
                return level;
            else
            {
                level = score / 100;
                return level;
            }
        }
    }
}
