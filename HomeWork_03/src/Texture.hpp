//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    //����������ɫ�ӿ�
    Eigen::Vector3f getColor(float u, float v)
    {
        //////////////////////////////////////////////////////////////////////////
		// �����޶�
        // phong shading��kd�Ǵ�RGB��ȡ����texture shading��kd���Ǹ���UV�����������ȡ��
        // ��˻���������쳣��
        //      Error: Assertion failed ((unsigned)(i1 * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels())) in cv::Mat
        // ˵��ȡ����ʱҪ�޶�UV����
        //////////////////////////////////////////////////////////////////////////
		if (u < 0)  u = 0;
		if (u > 1)  u = 1;
		if (v < 0)  v = 0;
		if (v > 1)  v = 1;

        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
