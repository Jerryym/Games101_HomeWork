//////////////////////////////////////////////////////////////////////////
//给定一个点 P = (2, 1), 将该点绕原点先逆时针旋转 45◦，再平移(1, 2), 计算出
//变换后点的坐标（要求用齐次坐标进行计算）。
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>
#include <eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>

int main()
{
	Eigen::Vector3d P(2.0f, 1.0f, 1.0f);//点p坐标向量
	Eigen::Matrix3d mat_Rotation;//旋转矩阵
	Eigen::Matrix3d mat_Translation;//平移矩阵

	//矩阵初始化
	double rRadian = 45.0 / 180.0 * EIGEN_PI;
	mat_Rotation << cos(rRadian), -1.0 * sin(rRadian), 0, 
					sin(rRadian), cos(rRadian),		   0, 
					0,			  0,				   1;
					
	mat_Translation << 1, 0, 1,
					   0, 1, 2,
					   0, 0, 1;
	
	std::cout << "变换前点的坐标为: (";
	std::cout << P[0] << ", " << P[1] << ", " << P[2] << ")" << std::endl;

	//先旋转，在平移
	P = mat_Translation * mat_Rotation * P;

	//输出结果
	std::cout << "变换后点的坐标为: (";
	std::cout << P[0] << ", " << P[1] << ", " << P[2] << ")" << std::endl;

	return 0;
}
