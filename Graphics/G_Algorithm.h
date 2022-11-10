#pragma once
#include <vector>
#include "Graphics.h"
#include "framework.h"
#define Pai 3.1415926

struct line {
  int x0;
  int y0;
  int x1;
  int y1;
  char ll;
};

// DDA��ֱ���㷨���е�ֱ���㷨��Bresenhamֱ���㷨��Բ
void DDALine(HDC hdc, int x0, int y0, int x1, int y1);
void midLine(HDC hdc, int x0, int y0, int x1, int y1);
void BresenhamLine(HDC hdc, int x0, int y0, int x1, int y1);
void circle(HDC hdc, int a, int b, int r);

//�����ɨ������㷨
struct ET {
  int ymax;
  float x,dx;
  ET* next;
};
void PolyScan(const std::vector<POINT> &Poly, const HDC &hdc);
//��������ɨ�����㷨
void ScanLineFill4(int x, int y, COLORREF bordercolor, COLORREF fillcolor,
                   HDC hdc);

// cohen-suntherland�㷨
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
int encode(int x, int y, int L, int R, int T, int B);
void CS_LineClip(int &x1, int &y1, int &x2, int &y2, int L, int R, int T,
                 int B);

// Southerland-Hodgman�㷨
void InitBoundary(line &Clip);
void Intersection(POINT s, POINT p, POINT x, POINT y, POINT &i);
bool Inside(POINT p, int edge, char position);
void SoutherlandHodgmanClip(const std::vector<POINT> &Poly, std::vector<POINT> &out,
                            line Clip);

//ƽ�ƣ���ת������
void Pan_Rotate_Zoom(int P_x, int P_y, int R_x, int R_y, int R_A, double Z_x,
                     double Z_y, POINT &p);

// Bezier����de Casteljau�㷨
void DeC_Bezier(const std::vector<POINT> &point, std::vector<POINT> &bezier,
                double u);
bool Inside(const POINT &p, const POINT &v);
