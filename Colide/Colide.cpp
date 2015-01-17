#include <iostream>
using namespace std;

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Separate Axis Theorem implemented via C99
/*
POINT为矢量或者向量，也可表示点；
dot为矢量点投影运算；
normalize为求模运算；
perp计算法线向量；
segment表示线段；
POLYGON为多边形，包括顶点vertices和边edges，所有点的顺序必须按顺时针或者逆时针。
*/
typedef struct 
{
	float x; 
	float y;
} POINT;
typedef struct 
{
	POINT p0;
	POINT dir;
} RAY;

typedef struct 
{
	POINT p0;
	POINT p1;
	POINT dir;
}SEG;

typedef struct 
{
	int n; 
	POINT *vertices; 
	SEG *edges;
} POLYGON; // Assumption: Simply connected => chain vertices together

POLYGON new_polygon(int nvertices, POINT *vertices){
	SEG *edges = (SEG*)malloc(sizeof(SEG)*(nvertices));
	int i;
	for (i = 0; i < nvertices-1; i++)
	{
		POINT dir = {vertices[i+1].x-vertices[i].x, vertices[i+1].y-vertices[i].y};
		SEG cur = {vertices[i], vertices[i+1], dir};
		edges[i] = cur;
	}
	POINT dir = {vertices[0].x-vertices[nvertices-1].x, vertices[0].y-vertices[nvertices-1].y};
	SEG cur = {vertices[nvertices-1], vertices[0], dir};
	edges[nvertices-1] = cur;
	POLYGON shape = {nvertices, vertices, edges};
	return shape;
}

POINT v(float x, float y)
{
	POINT a = {x, y};
	return a;
}

POINT perp(POINT v)
{
	POINT b = {v.y, -v.x};
	return b;
}

SEG segment(POINT p0, POINT p1)
{
	POINT dir = {p1.x-p0.x, p1.y-p0.y};
	SEG s = {p0, p1, dir};
	return s;
}

POLYGON Polygon(int nvertices, ...)
{
	va_list args;
	va_start(args, nvertices);
	POINT *vertices = (POINT*)malloc(sizeof(POINT)*nvertices);
	int i;
	for (i = 0; i < nvertices; i++)
	{
		vertices[i] = va_arg(args, POINT);
	}
	va_end(args);
	return new_polygon(nvertices, vertices);
}
//project为计算投影函数，先计算所有边长的投影，然后算出投影的最大和最小点即起始点；overlap函数判断两条线段是否重合。
POINT normalize(POINT v)
{
	float mag = sqrt(v.x*v.x + v.y*v.y);
	POINT b = {v.x/mag, v.y/mag};
	return b;
}

float dot(POINT a, POINT b)
{
	return a.x*b.x+a.y*b.y;
}

float* project(POLYGON a, POINT axis)
{
	axis = normalize(axis);
	int i;
	float min = dot(a.vertices[0], axis); 
	float max = min;
	//cout << axis.x << " " << axis.y << endl;
	for (i=0;i<a.n;i++)
	{
		float proj = dot(a.vertices[i], axis);
		//cout << a.vertices[i].x << " " << a.vertices[i].y  << " " << proj << endl;
		if (proj < min) min = proj; 
		if (proj > max) max = proj;
	}
	float* arr = (float*)malloc(2*sizeof(float));
	arr[0] = min; 
	arr[1] = max;
	//cout << endl;
	return arr;
}

int contains(float n, float* range)
{
	float a = range[0];
	float b = range[1];
	if (b<a) {a = b; b = range[0];}
	return (n >= a && n <= b);
}

int overlap(float* a_, float* b_)
{
	if (contains(a_[0],b_)) return 1;
	if (contains(a_[1],b_)) return 1;
	if (contains(b_[0],a_)) return 1;
	if (contains(b_[1],a_)) return 1;
	return 0;
}

int sat(POLYGON a, POLYGON b)
{
	int i;
	for (i=0;i<a.n;i++)
	{
		POINT axis = a.edges[i].dir;
		axis = perp(axis);	// 法线向量
		float *a_ = project(a, axis);
		float *b_ = project(b, axis);
		//cout << a_[0] << " " << a_[1] << endl;
		//cout << b_[0] << " " << b_[1] << endl << endl;
		if (!overlap(a_, b_)) 
			return 0;
	}

	for (i=0;i<b.n;i++)
	{
		POINT axis = b.edges[i].dir;
		axis = perp(axis);
		float *a_ = project(a,axis);
		float *b_ = project(b,axis);
		if (!overlap(a_, b_)) 
			return 0;
	}

	return 1;
}

int main()
{
	POLYGON a;
	POLYGON b;

	a = Polygon(4, v(0,0),v(0,5),v(5,4),v(3,0));  
	b = Polygon(4, v(4,4),v(4,6),v(6,6),v(6,4));
	printf("result = %d\n", sat(a,b)); // true

	a = Polygon(4, v(0,0),v(0,3),v(3,3),v(3,0));
	b = Polygon(4, v(4,4),v(4,6),v(6,6),v(6,4));
	printf("result = %d\n", sat(a,b)); // false



	return 0;
}