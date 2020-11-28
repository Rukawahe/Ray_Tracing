#include<windows.h>
#include<gl/glut.h>
#include<iostream>
#include<math.h>
const int left = 1;//0001
const int right = 2;//0010
const int bottom = 4;//0100
const int top = 8;//1000
float xmin, xmax, ymin, ymax;
float x1, x2, i_y1, i_y2;//裁剪直线的两端点坐标
float new_x1, new_x2, new_y1, new_y2;//裁剪后新的坐标
int compute_encode(int x, int y) {
	int ans = 0;
	if (x < xmin)	ans |= left;
	if (x > xmax)	ans |= right;
	if (y < ymin)	ans |= bottom;
	if (y > ymax)	ans |= top;
	return ans;
}
void line_crop1() {//Cohen－Sutherland裁剪算法
	int code1, code2,code3;
	code1 = compute_encode(x1, i_y1);
	code2 = compute_encode(x2, i_y2);
	while (true) {
		if (!(code1 | code2)) {//相或为0，简取,接受并退出循环
			break;
		}
		else if (code1 & code2)//相与为1，简弃，拒绝并推出循环
			break;
		else{
				int code = code1 ? code1 : code2;//在边界外的点
				float x, y;
				if (code & left) {//左边界
					x = xmin;
					y = i_y1 + (i_y2 - i_y1) / (x2 - x1)*(x - x1);
				}
				if(code & right){
					x = xmax;
					y = i_y1 + (i_y2 - i_y1) / (x2 - x1)*(x - x1);
				}
				if (code & bottom) {
					y = ymin;
					x = (y - i_y1)*(x2 - x1) / (i_y2 - i_y1) + x1;
				}
				if (code & top) {
					y = ymax;
					x = (y - i_y1)*(x2 - x1) / (i_y2 - i_y1) + x1;
				}
				//会出现直线两端都在区域外，所以循环进行，将两端点都裁剪
				if (code == code1) {
					x1 = x;
					i_y1 = y;
					code1 = compute_encode(x1, i_y1);
				}
				else {
					x2 = x;
					i_y2 = y;
					code2 = compute_encode(x2, i_y2);
				}
		}
	}
	
}
void line_crop2() {//liang-Barsky裁剪算法
	float p1, p2, p3, p4;
	float q1, q2, q3, q4;
	float u1, u2, u3, u4;
	p1 = -(x2-x1),p2 = x2 - x1,p3 = -(i_y2 - i_y1),p4 = (i_y2 - i_y1);
	q1 = x1 - xmin, q2 = xmax - x1, q3 = i_y1 - ymin, q4 = ymax - i_y1;
	u1 = q1 / p1, u2 = q2 / p2, u3 = q3 / p3, u4 = q4 / p4;
	float umax = 0, umin = 1;
	umax = max(0, max(u1, u3));
	umin = min(1, min(u2, u4));
	new_x1 = x1 + umax * (x2 - x1);
	new_y1 = i_y1 + umax * (i_y2 - i_y1);
	new_x2 = x1 + umin * (x2 - x1);
	new_y2 = i_y1 + umin * (i_y2 - i_y1);
	x1 = new_x1, i_y1 = new_y1;
	x2 = new_x2, i_y2 = new_y2;
}
void init(int argc, char** argv)
{
	glutInit(&argc, argv);  //I初始化 GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  //设置显示模式：单个缓存和使用RGB模型
	glutInitWindowPosition(50, 100);  //设置窗口的顶部和左边位置
	glutInitWindowSize(400, 300);  //设置窗口的高度和宽度
	glutCreateWindow("多边形直线裁剪");

	glClearColor(1.0, 1.0,1.0, 0.0); //窗口背景颜色为白色
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	gluOrtho2D(0.0, 600.0, 0.0, 550.0); // 设置场景的大小
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::cout << "请输入四边形的左右下上区域： ";
	std::cin >> xmin >> xmax >> ymin >> ymax;

	glColor4f(1.0, 0.6, 0.0, 0.75);
	glBegin(GL_POLYGON);
	glVertex2f(xmin, ymin);
	glVertex2f(xmax, ymin);	
	glVertex2f(xmax, ymax);
	glVertex2f(xmin, ymax);
	glEnd();
	glFlush();

	std::cout << "请输入裁剪直线两端点坐标:";
	std::cin >> x1 >> i_y1 >> x2 >> i_y2;
	//x1 = 80, is_y = 100, x2 = 400, _y2 = 500;
	glColor4f(0.0, 0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(x1, i_y1);
	glVertex2f(x2, i_y2);
	glEnd();
	glFlush();

	std::cout << "选择哪个算法:" << std::endl;
	std::cout << "----------1、Cohen-Sutherland算法 " << std::endl;
	std::cout << "----------2、Liang-Barsky算法" << std::endl;
	int num;
	std::cin >> num;
	if (num == 1) {
		line_crop1();
	}
	else
	{
		line_crop2();
	}
	
	std::cout << "hello" << std::endl;
	glColor4f(1.0, 1.0,1.0,0.0);
	glBegin(GL_LINES);
	glVertex2f(x1, i_y1);
	glVertex2f(x2, i_y2);
	glEnd();
	glFlush();
}

int main(int argc, char** argv)
{
	init(argc, argv);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}
