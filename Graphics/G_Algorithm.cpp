#include "G_Algorithm.h"

void DDALine(HDC hdc, int x0, int y0, int x1, int y1) {
  float dx, dy, k;
  dx = float(x1) - float(x0);
  dy = float(y1) - float(y0);
  k = dy / dx;         //б��
  if (fabs(k) <= 1) {  //��ֱ��б�ʾ���ֵС��1ʱ��y��x�仯����
    float y = float(y0);
    if (x1 >= x0) {  //��һ�����ڵڶ������ʱ
      for (int x = x0; x <= x1; x++) {
        SetPixel(hdc, x, int(y + 0.5), 0);
        y += k;  //ÿ�μ�k��ȡ��������ص�
      };
    } else {  //��һ�����ڵڶ����ұ�ʱ
      for (int x = x0; x >= x1; x--) {
        SetPixel(hdc, x, int(y + 0.5), 0);
        y -= k;
      };
    }
  } else {  //ֱ��б�ʾ���ֵ����1ʱ��x��y�仯����
    float x = float(x0);
    int y = 0;
    k = 1 / k;
    if (y1 >= y0) {  //��һ�����ڵڶ�������ʱ
      for (int y = y0; y <= y1; y++) {
        SetPixel(hdc, int(x + 0.5), y, 0);
        x += k;
      };
    } else {  //��һ�����ڵڶ���������ʱ
      for (int y = y0; y >= y1; y--) {
        SetPixel(hdc, int(x + 0.5), y, 0);
        x -= k;
      };
    }
  }
}

void midLine(HDC hdc, int x0, int y0, int x1, int y1) {
  int d0, d1, d2, a, b;
  int xmax = x1 > x0 ? x1 : x0;
  int xmin = x1 > x0 ? x0 : x1;
  int ymax = y1 > y0 ? y1 : y0;
  int ymin = y1 > y0 ? y0 : y1;
  if (x0 == x1) {  //��б��Ϊ0ʱ
    while (ymin <= ymax) {
      SetPixel(hdc, xmin, ymin, 0);
      ymin++;
    }
  } else {
    float dx = float(x1) - float(x0);
    float dy = float(y1) - float(y0);
    float k = dy / dx;  //б��
    if (x1 > x0) {      //���㲻ͬ����µ�a��b
      a = y0 - y1;
      b = x1 - x0;
    } else {
      a = y1 - y0;
      b = x0 - x1;
    }
    if (k >= 0 && k <= 1) {  //б����[0,1]ʱ
      d0 = 2 * a + b;
      d1 = 2 * (a + b);
      d2 = 2 * a;  //��ͬ����µ���ֵ
      for (; xmin <= xmax; xmin++) {
        SetPixel(hdc, xmin, ymin, 0);
        if (d0 < 0) {  // d0<0,d0+d1����y+1
          d0 += d1;
          ymin++;
        } else {
          d0 += d2;
        }
      }
    } else if (k > 1) {  //б����[1,+��]ʱ
      d0 = a + 2 * b;
      d1 = 2 * (a + b);
      d2 = 2 * b;  //��ͬ����µ���ֵ
      for (; ymin <= ymax; ymin++) {
        SetPixel(hdc, xmin, ymin, 0);
        if (d0 <= 0) {
          d0 += d2;
        } else {
          d0 += d1;
          xmin++;
        }
      }
    } else if (k < 0 && k >= -1) {  //б����[-1,0]ʱ
      d0 = -2 * a + b;
      d1 = -2 * a + 2 * b;
      d2 = -2 * a;
      for (; xmin <= xmax; xmin++) {
        SetPixel(hdc, xmin, ymax, 0);
        if (d0 < 0) {
          d0 += d1;
          ymax--;
        } else {
          d0 += d2;
        }
      }
    } else if (k < -1) {  //б����[-��,-1]ʱ
      d0 = a - 2 * b;
      d1 = 2 * (a - b);
      d2 = -2 * b;
      for (; ymin <= ymax; ymin++) {
        SetPixel(hdc, xmax, ymin, 0);
        if (d0 < 0) {
          d0 += d1;
          xmax--;
        } else {
          d0 += d2;
        }
      }
    }
  }
}

void BresenhamLine(HDC hdc, int x1, int y1, int x2, int y2) {
  int x, y, dx, dy, s1, s2, p, temp, interchange, i;
  x = x1;
  y = y1;
  dx = abs(x2 - x1);
  dy = abs(y2 - y1);
  if (x2 > x1)  // x2����+1������-1
    s1 = 1;
  else
    s1 = -1;

  if (y2 > y1)  // y2����+1������-1
    s2 = 1;
  else
    s2 = -1;

  if (dy > dx) {
    temp = dx;
    dx = dy;
    dy = temp;  //���dy>dx���򽻻�dy��dx��ֵ
    interchange = 1;
  } else
    interchange = 0;

  p = 2 * dy - dx;  //��ʼ����б�ֵ
  for (i = 1; i <= dx; i++) {
    SetPixel(hdc, x, y, 0);
    if (p >= 0) {
      if (interchange == 0)  //��dy<dxʱ
        y = y + s2;
      else  //��dy>dxʱ
        x = x + s1;
      p = p - 2 * dx;
    }
    if (interchange == 0)  //��dy<dxʱ
      x = x + s1;
    else  //��dy>dxʱ
      y = y + s2;
    p = p + 2 * dy;
  }
}

void circle(HDC hdc, int a, int b, int r) {
  int y = r, x = 0, d = 1 - r;
  while (y >= x) {  //����Բ�ĶԳ���
    SetPixel(hdc, a + x, b + y, 0);
    SetPixel(hdc, a - x, b + y, 0);
    SetPixel(hdc, a + x, b - y, 0);
    SetPixel(hdc, a - x, b - y, 0);
    SetPixel(hdc, a + y, b + x, 0);
    SetPixel(hdc, a + y, b - x, 0);
    SetPixel(hdc, a - y, b + x, 0);
    SetPixel(hdc, a - y, b - x, 0);
    if (d < 0) {
      d = d + 2 * x + 3;
      x++;
    } else {
      d = d + 2 * (x - y) + 5;
      x++;
      y--;  //��d<0ʱ��y--
    }
  }
}

void PolyScan(const std::vector<POINT> &Poly, const HDC &hdc) {
  int yMin = Poly.front().y, yMax = Poly.front().y;
  int i, NumOfEdge = Poly.size() - 1;
  std::vector<POINT> Pp;
  //����ɨ��߽�
  for (auto &v : Poly) {
    if (v.y > yMax) yMax = v.y;
    if (v.y < yMin) yMin = v.y;
    Pp.push_back(v);
  }
  Pp.pop_back();
  //��ʼ�����Ա߱�
  ET *AEL = new ET;
  AEL->next = nullptr;
  //��������ʼ���ߵķ����
  ET **NET = new ET*[yMax];
  for (i = yMin; i < yMax; i++) {
    NET[i] = new ET;
    NET[i]->next = nullptr;
  }
  //ɨ�貢�����ߵķ����
  for (i = yMin; i < yMax; i++) {
    for (int j = 0; j < NumOfEdge; j++) {
      if (Pp[j].y == i) {
        if (Pp[(j - 1 + NumOfEdge) % NumOfEdge].y > Pp[j].y) {
          ET *p = new ET;
          p->x = (float)Pp[j].x;
          p->ymax = Pp[(j - 1 + NumOfEdge) % NumOfEdge].y;
          p->dx = (float)(Pp[(j - 1 + NumOfEdge) % NumOfEdge].x - Pp[j].x) /
                  (float)(Pp[(j - 1 + NumOfEdge) % NumOfEdge].y - Pp[j].y);
          p->next = NET[i]->next;
          NET[i]->next = p;
        }
        if (Pp[(j + 1 + NumOfEdge) % NumOfEdge].y > Pp[j].y) {
          ET *p = new ET;
          p->x = (float)Pp[j].x;
          p->ymax = Pp[(j + 1 + NumOfEdge) % NumOfEdge].y;
          p->dx = (float)(Pp[(j + 1 + NumOfEdge) % NumOfEdge].x - Pp[j].x) /
                  (float)(Pp[(j + 1 + NumOfEdge) % NumOfEdge].y - Pp[j].y);
          p->next = NET[i]->next;
          NET[i]->next = p;
        }
      }
    }
  }
  //���ݱߵķ������֮ɨ��ʱ�������Ա߱�
  for (i = yMin; i < yMax; i++) {
    ET *p = AEL->next;
    while (p) {//�����µĽ��㣬���»��Ա߱�
      p->x = p->x + p->dx;
      p = p->next;
    }
    ET *eq = AEL;
    p = AEL->next;
    eq->next = nullptr;
    while (p) {//����
      while (eq->next && p->x >= eq->next->x) eq = eq->next;
      ET *s = p->next;
      p->next = eq->next;
      eq->next = p;
      p = s;
      eq = AEL;
    }
    ET *q = AEL;
    p = q->next;
    while (p) { //��ɨ���ߵ�����Ա߱���ymax�Ľ��
      if (p->ymax == i) {
        q->next = p->next;
        delete p;
        p = q->next;
      } else {
        q = q->next;
        p = q->next;
      }
    }
    p = NET[i]->next;//���ߵķ�����е��½�㰴��������
    q = AEL;
    while (p) {
      while (q->next && p->x >= q->next->x) q = q->next;
      ET *s = p->next;
      p->next = q->next;
      q->next = p;
      p = s;
      q = AEL;
    }
    p = AEL->next;
    while (p && p->next) {//��ɫ
      for (float j = p->x; j <= p->next->x; j++) SetPixel(hdc, (int)j, i, 0);
      p = p->next->next;
	}
  }
}

void ScanLineFill4(int x, int y, COLORREF bordercolor, COLORREF fillcolor,
                   HDC hdc) {
  int xr, xl;
  bool spanNeedFill = false;
  POINT seed;
  std::vector<POINT> Seed;
  seed.x = x;
  seed.y = y;
  Seed.push_back(seed);
  while (!Seed.empty()) {
    seed = Seed.back();
    x = seed.x;
    y = seed.y;
    Seed.pop_back();
    while (GetPixel(hdc, x, y) == fillcolor) {
      SetPixel(hdc, x, y, bordercolor);
      x++;
    }
    xr = x - 1;
    x = seed.x - 1;
    while (GetPixel(hdc, x, y) == fillcolor) {
      SetPixel(hdc, x, y, bordercolor);
      x--;
    }
    xl = x + 1;
    x = xl;
    y++;
    while (x < xr) {
      while (GetPixel(hdc, x, y) == fillcolor) {
        x++;
        spanNeedFill = true;
      }
      if (spanNeedFill) {
        seed.x = x - 1;
        seed.y = y;
        Seed.push_back(seed);
        spanNeedFill = false;
      }
      while (GetPixel(hdc, x, y) != fillcolor && x < xr) x++;
    }
    x = xl;
    y = y - 2;
    while (x < xr) {
      while (GetPixel(hdc, x, y) == fillcolor) {
        x++;
        spanNeedFill = true;
      }
      if (spanNeedFill) {
        seed.x = x - 1;
        seed.y = y;
        Seed.push_back(seed);
        spanNeedFill = false;
      }
      while (GetPixel(hdc, x, y) != fillcolor && x < xr) x++;
    }
  }
}

int encode(int x, int y, int L, int R, int T, int B) {
  int c = 0;
  if (x < L) c |= LEFT;
  if (x > R) c |= RIGHT;
  if (y < B) c |= BOTTOM;
  if (y > T) c |= TOP;
  return c;
}

void CS_LineClip(int &x1, int &y1, int &x2, int &y2, int L, int R, int T,
                 int B) {
  int code1, code2, code, x = 0, y = 0;
  code1 = encode(x1, y1, L, R, B, T);
  code2 = encode(x2, y2, L, R, B, T);  // �˵��������
  while (code1 != 0 || code2 != 0)     // ֱ������ȫ�ɼ���
  {
    if ((code1 & code2) != 0) return;  // �ų�����Ȼ���ɼ������
    code = code1;
    if (code1 == 0) code = code2;  // ����ڴ�����ĵ�
    //��˳���⵽�˵�ı��벻Ϊ0���Ű��߶����Ӧ�Ĵ��ڱ߽��󽻡�
    if ((LEFT & code) != 0)  // �߶��봰������ӳ����ཻ
    {
      x = L;
      y = y1 + (y2 - y1) * (L - x1) / (x2 - x1);
    } else if ((RIGHT & code) != 0)  // �߶��봰���ұ��ӳ����ཻ
    {
      x = R;
      y = y1 + (y2 - y1) * (R - x1) / (x2 - x1);
    } else if ((BOTTOM & code) != 0)  // �߶��봰���±��ӳ����ཻ
    {
      y = B;
      x = x1 + (x2 - x1) * (B - y1) / (y2 - y1);
    } else if ((TOP & code) != 0)  // �߶��봰���ϱ��ӳ����ཻ
    {
      y = T;
      x = x1 + (x2 - x1) * (T - y1) / (y2 - y1);
    }
    if (code == code1) {
      x1 = x;
      y1 = y;
      code1 = encode(x, y, L, R, B, T);
    }  //��ȥP1������
    else {
      x2 = x;
      y2 = y;
      code2 = encode(x, y, L, R, B, T);
    }  //��ȥP2������
  }
}

void InitBoundary(line &Clip) {
  int L, R, B, T;
  if (Clip.x0 <= Clip.x1 && Clip.y0 <= Clip.y1) {
    L = Clip.x0;
    R = Clip.x1;
    B = Clip.y0;
    T = Clip.y1;
  } else if (Clip.x0 > Clip.x1 && Clip.y0 <= Clip.y1) {
    L = Clip.x1;
    R = Clip.x0;
    B = Clip.y0;
    T = Clip.y1;
  } else if (Clip.x0 <= Clip.x1 && Clip.y0 > Clip.y1) {
    L = Clip.x0;
    R = Clip.x1;
    B = Clip.y1;
    T = Clip.y0;
  } else {
    L = Clip.x1;
    R = Clip.x0;
    B = Clip.y1;
    T = Clip.y0;
  }
  Clip.x0 = L;
  Clip.y0 = T;
  Clip.x1 = R;
  Clip.y1 = B;
}

void Intersection(POINT s, POINT p, POINT x, POINT y, POINT &i) {
  if (x.x == y.x) {
    i.x = x.x;
    i.y = s.y + (x.x - s.x) * (s.y - p.y) / (s.x - p.x);
  } else {
    i.y = x.y;
    i.x = s.x + (x.y - s.y) * (s.x - p.x) / (s.y - p.y);
  }
}

bool Inside(POINT p, int edge, char position) {
  if (position == 'T') {
    if (p.y <= edge) return true;
  } else if (position == 'R') {
    if (p.x <= edge) return true;
  } else if (position == 'B') {
    if (p.y >= edge) return true;
  } else if (position == 'L') {
    if (p.x >= edge) return true;
  }
  return false;
}

void SoutherlandHodgmanClip(const std::vector<POINT> &Poly, std::vector<POINT> &out,
                            line Clip) {
  //ʹ��ǰ�ȳ�ʼ��Clip��ʹ��InitBoundary(Clip);
  out.clear();
  std::vector<POINT> In;
  for (auto &v : Poly) In.push_back(v);
  POINT s, i, x, y;
  char position = 'L';
  x.x = Clip.x0;
  x.y = Clip.y1;
  y.x = Clip.x0;
  y.y = Clip.y0;
  s = In.back();
  for (auto &v : In) {
    if (Inside(v, Clip.x0, position)) {
      if (Inside(s, Clip.x0, position)) {
        out.push_back(v);
      } else {
        Intersection(s, v, x, y, i);
        out.push_back(i);
        out.push_back(v);
      }
    } else {
      if (Inside(s, Clip.x0, position)) {
        Intersection(s, v, x, y, i);
        out.push_back(i);
      }
    }
    s = v;
  }
  if (out.size() != 0) {
    In.clear();
    for (auto &v : out) In.push_back(v);
    out.clear();
    position = 'T';
    x.x = Clip.x0;
    x.y = Clip.y0;
    y.x = Clip.x1;
    y.y = Clip.y0;
    s = In.back();
    for (auto &v : In) {
      if (Inside(v, Clip.y0, position)) {
        if (Inside(s, Clip.y0, position)) {
          out.push_back(v);
        } else {
          Intersection(s, v, x, y, i);
          out.push_back(i);
          out.push_back(v);
        }
      } else {
        if (Inside(s, Clip.y0, position)) {
          Intersection(s, v, x, y, i);
          out.push_back(i);
        }
      }
      s = v;
    }
  }
  if (out.size() != 0) {
    In.clear();
    for (auto &v : out) {
      In.push_back(v);
    }
    out.clear();
    position = 'R';
    x.x = Clip.x1;
    x.y = Clip.y0;
    y.x = Clip.x1;
    y.y = Clip.y1;
    s = In.back();
    for (auto &v : In) {
      if (Inside(v, Clip.x1, position)) {
        if (Inside(s, Clip.x1, position)) {
          out.push_back(v);
        } else {
          Intersection(s, v, x, y, i);
          out.push_back(i);
          out.push_back(v);
        }
      } else {
        if (Inside(s, Clip.x1, position)) {
          Intersection(s, v, x, y, i);
          out.push_back(i);
        }
      }
      s = v;
    }
  }
  if (out.size() != 0) {
    In.clear();
    for (auto &v : out) {
      In.push_back(v);
    }
    out.clear();
    position = 'B';
    x.x = Clip.x1;
    x.y = Clip.y1;
    y.x = Clip.x0;
    y.y = Clip.y1;
    s = In.back();
    for (auto &v : In) {
      if (Inside(v, Clip.y1, position)) {
        if (Inside(s, Clip.y1, position)) {
          out.push_back(v);
        } else {
          Intersection(s, v, x, y, i);
          out.push_back(i);
          out.push_back(v);
        }
      } else {
        if (Inside(s, Clip.y1, position)) {
          Intersection(s, v, x, y, i);
          out.push_back(i);
        }
      }
      s = v;
    }
  }
  if (out.size() != 0) {
    s = out.front();
    out.push_back(s);
  }
}

void Pan_Rotate_Zoom(int P_x, int P_y, int R_x, int R_y, int R_A, double Z_x,
                     double Z_y, POINT &p) {
  double angle = R_A * Pai / 180;
  int _x = p.x, _y = p.y;
  //��ת
  if (R_A != 0) {
    p.x = (long)(_x * cos(angle) - _y * sin(angle) + R_x * (1 - cos(angle)) +
                 R_y * sin(angle));
    p.y = (long)(_x * sin(angle) + _y * cos(angle) + R_y * (1 - cos(angle)) -
                 R_x * sin(angle));
  }
  //����
  if (Z_x != 1.0) p.x = (long)(Z_x * p.x + R_x * (1 - Z_x));
  if (Z_y != 1.0) {
    p.y = (long)(Z_y * p.y + R_y * (1 - Z_y));
  }
  //ƽ��
  if (P_x != 0) p.x = p.x + P_x;
  if (P_y != 0) p.y = p.y + P_y;
}

void DeC_Bezier(const std::vector<POINT> &point, std::vector<POINT> &bezier,
                double u) {
  size_t n = point.size();
  bezier.clear();
  for (auto &v : point) bezier.push_back(v);
  for (size_t k = 1; k < n; k++)
    for (size_t i = 0; i < n - k; i++) {
      bezier[i].x = (1.0 - u) * bezier[i].x + u * bezier[i + 1].x;
      bezier[i].y = (1.0 - u) * bezier[i].y + u * bezier[i + 1].y;
    }
}

bool Inside(const POINT &p, const POINT &v) {
  if (v.x <= p.x + 10 && v.x >= p.x - 10 && v.y <= p.y + 10 && v.y >= p.y - 10)
    return true;
  else
    return false;
}

/*windows��̿�ͨͼ��ӳ�䷽ʽ
void Draw(HDC hdc, int mapMode, SIZE size, int nWindowExt)
{
        HPEN MyPen;
        HBRUSH MyBrush[3];
        MyPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 0));
        MyBrush[0] = CreateSolidBrush(RGB(255, 255, 0));
        MyBrush[1] = CreateSolidBrush(RGB(205, 55, 0));
        MyBrush[2] = CreateSolidBrush(RGB(255, 193, 193));

        SetMapMode(hdc, mapMode);

        if (mapMode == MM_ISOTROPIC)
        {
                SetWindowExtEx(hdc, nWindowExt, nWindowExt, NULL);
                SetViewportExtEx(hdc, size.cx, -size.cy, NULL);

        }
        if (mapMode == MM_ANISOTROPIC)
        {
                SetWindowExtEx(hdc, nWindowExt, nWindowExt, NULL);
                SetViewportExtEx(hdc, size.cx, size.cy, NULL);
                //SetWindowOrgEx(hdc, -size.cx / 2, -size.cy / 2,
NULL);//���ô���ԭ������ SetViewportOrgEx(hdc, size.cx / 2, size.cy / 2, NULL);
        }

        DPtoLP(hdc, (PPOINT)&size, 1); //�豸����ת��Ϊ�߼�����

        SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
        Ellipse(hdc, 15, 10, 445, 440);
        Ellipse(hdc, 15, -10, 445, -440);
        SelectBrush(hdc, MyBrush[0]);
        Ellipse(hdc, 35, 30, 425, 420);
        Ellipse(hdc, 35, -30, 425, -420);
        SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
        POINT mouth[35] = {
                80,260,90,300,100,320,110,340,120,350,130,360,140,370,150,380,165,385,170,390,
                180,392,190,397,200,400,210,400,220,400,230,400,240,398,250,396,260,394,270,392,
                280,387,290,382,300,375,310,370,320,360,330,350,340,340,345,330,350,320,353,310,
                355,300,357,290,356,280,354,270,350,260
        };
        Polygon(hdc, mouth, 35);
        for (int i = 0; i <= 34; i++)
        {
                mouth[i].y = 0 - mouth[i].y;
        }
        Polygon(hdc, mouth, 35);
        SelectBrush(hdc, MyBrush[1]);
        POINT mouth1[17] = {
                95,260,105,300,115,322,130,343,145,357,160,370,180,382,200,388,310,354,315,350,
                330,327,340,313,341,300,343,290,342,280,340,270,339,260
        };
        Polygon(hdc, mouth1, 17);
        for (int i = 0; i <= 16; i++)
        {
                mouth1[i].y = 0 - mouth1[i].y;
        }
        Polygon(hdc, mouth1, 17);
        SelectBrush(hdc, MyBrush[2]);
        POINT mouth2[27] = {
                180,382,190,385,200,388,210,389,220,388,230,387,240,386,250,385,260,382,270,378,
                280,373,290,367,300,361,310,355,300,345,290,337,280,331,270,328,260,325,250,324,
                240,326,230,330,220,333,210,339,200,347,190,362,185,372
        };
        Polygon(hdc, mouth2, 27);
        for (int i = 0; i <= 26; i++)
        {
                mouth2[i].y = 0 - mouth2[i].y;
        }
        Polygon(hdc, mouth2, 27);
        SelectPen(hdc, MyPen);
        POINT x[2];
        MoveToEx(hdc, 348, 255, x);
        LineTo(hdc, 67, 255);
        MoveToEx(hdc, 348, -255, x);
        LineTo(hdc, 67, -255);
        SelectBrush(hdc, GetStockBrush(WHITE_BRUSH));
        POINT eye[18] = {
                67,200,66,190,65,180,66,170,67,160,70,150,76,140,86,130,100,123,105,121,
                110,123,125,126,135,133,145,145,150,160,155,180,154,190,153,200
        };
        Polygon(hdc, eye, 18);
        for (int i = 0; i <= 17; i++)
        {
                eye[i].y = 0 - eye[i].y;
        }
        Polygon(hdc, eye, 18);
        for (int i = 0; i <= 17; i++)
        {
                eye[i].y = 0 - eye[i].y;
        }
        std::for_each(eye, eye + 18, [](POINT& p) {p.x += 180; });
        Polygon(hdc, eye, 18);
        for (int i = 0; i <= 17; i++)
        {
                eye[i].y = 0 - eye[i].y;
        }
        Polygon(hdc, eye, 18);
        SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
        POINT eyeball[12] = {
                125,126,135,133,145,145,150,160,140,165,130,167,120,163,115,160,112,150,114,150,113,140,118,130
        };
        Polygon(hdc, eyeball, 12);
        for (int i = 0; i <= 11; i++)
        {
                eyeball[i].y = 0 - eyeball[i].y;
        }
        Polygon(hdc, eyeball, 12);
        for (int i = 0; i <= 11; i++)
        {
                eyeball[i].y = 0 - eyeball[i].y;
        }
        std::for_each(eyeball, eyeball + 12, [](POINT& p) {p.x += 180; });
        Polygon(hdc, eyeball, 12);
        for (int i = 0; i <= 11; i++)
        {
                eyeball[i].y = 0 - eyeball[i].y;
        }
        Polygon(hdc, eyeball, 12);

        //ɾ��
        DeletePen(MyPen);
        for (int i = 0; i <= 2; i++) {
                DeleteBrush(MyBrush[i]);
        }
};
//λͼ����͸����ʾ
void Draw(HDC hdc, HDC memhdc, HINSTANCE hInst)
{
        HBITMAP BitM1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        HBITMAP BitM2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        HBITMAP BitM3 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        memhdc = CreateCompatibleDC(hdc);
        SelectObject(memhdc, BitM1);
        BitBlt(hdc, 0, 0, 640, 480, memhdc, 0, 0, SRCCOPY);
        SelectObject(memhdc, BitM3);
        BitBlt(hdc, 0, 0, 80, 80, memhdc, 0, 0, SRCPAINT);
        SelectObject(memhdc, BitM2);
        BitBlt(hdc, 0, 0, 80, 80, memhdc, 0, 0, SRCAND);
        DeleteDC(memhdc);
        DeleteBitmap(BitM1);
        DeleteBitmap(BitM2);
        DeleteBitmap(BitM3);
}*/