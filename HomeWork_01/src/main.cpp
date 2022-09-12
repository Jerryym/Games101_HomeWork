/*
作业要求
本次作业的任务是填写一个旋转矩阵和一个透视投影矩阵。给定三维下三个点
v0(2.0,0.0,−2.0),
v1(0.0,2.0,−2.0),
v2(−2.0,0.0,−2.0),
你需要将这三个点的坐标变换为屏幕坐标并在屏幕上绘制出对应的线框三角形 
(在代码框架中，我们已经提供了 draw_triangle 函数，所以你只需要去构建变换矩阵即可)。
简而言之， 我们需要进行模型、视图、投影、视口等变换来将三角形显示在屏幕上。在提供的代码框架中，我们留下了模型变换和投影变换的部分给你去完成。
提高篇： 在 main.cpp 中构造一个函数，该函数的作用是得到绕任意过原点的轴的旋转变换矩阵。
*/

#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;


Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 
        0, 1, 0, -eye_pos[1], 
        0, 0, 1, -eye_pos[2], 
        0, 0, 0, 1;

    view = translate * view;

    return view;
}

/*
作用：逐个元素地构建模型变换矩阵并返回该矩阵
要求：在此函数中，实现三维绕z轴旋转的变换矩阵

C++中 三角函数传递的数值为弧度
角度 = 180° × 弧度 ÷ π ，弧度 = 角度 × π ÷ 180°
*/
Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    Eigen::Matrix4f Rotation_Mat(4, 4);//创建一个4x4的旋转矩阵
    float rRadian = rotation_angle / 180.0f * MY_PI;
    Rotation_Mat << cos(rRadian), -1.0 * sin(rRadian), 0, 0,
                    sin(rRadian), cos(rRadian), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
    model = Rotation_Mat * model;
    return model;
}

//////////////////////////////////////////////////////////////////////////
/// <summary>
//使用给定的参数逐个元素地构建透视投影并返回该矩阵
//参数:
//eye_fov: 视野角度
//aspect_ratio: 宽高比
//zNear: 近平面
//zFar: 选平面
//对于定义好的视锥，我们定义视野角度为𝛼，宽高比为𝑟𝑎𝑑𝑖𝑜，近平面z值为𝑛，那么投影变换后的长方体
//的中𝑡 = 𝑛 tan 𝛼/2, 𝑏 = −𝑛 tan 𝛼/2, 𝑟 = 𝑟𝑎𝑑𝑖𝑜 ∗ 𝑛 tan 𝛼/2, 𝑙 = −𝑟𝑎𝑑𝑖𝑜 ∗ 𝑛 tan 𝛼/2. 代入正交投影变化公式中即可。
//C++中 三角函数传递的数值为弧度
//角度 = 180° × 弧度 ÷ π ，弧度 = 角度 × π ÷ 180°
/// </summary>
//////////////////////////////////////////////////////////////////////////
Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    //正交投影规范化 先平移再缩放
    Eigen::Matrix4f PerspToOrtho_Mat(4, 4);
    Eigen::Matrix4f OrthoTrans_Mat(4, 4);
    Eigen::Matrix4f OrthoScale_Mat(4, 4);

    float rRadian = eye_fov / 180.0f * MY_PI;
    float rHeight = zNear * tan(rRadian / 2) * 2;
    float rWidth = aspect_ratio * rHeight;

    auto t = zNear * tan(rRadian / 2);
    auto b = -zNear * tan(rRadian / 2);
    auto r = aspect_ratio * zNear * tan(rRadian / 2);
    auto l = -aspect_ratio * zNear * tan(rRadian / 2);

    PerspToOrtho_Mat << zNear, 0, 0, 0,
                        0, zNear, 0, 0,
                        0, 0, zNear + zFar, -zNear * zFar,
                        0, 0, 1, 0;

    OrthoScale_Mat << 2 / (r - l), 0, 0, 0,
        0, 2 / (t - b), 0, 0,
        0, 0, 2 / (zNear - zFar), 0,
        0, 0, 0, 1;

    OrthoTrans_Mat << 1, 0, 0, -(r + l) / 2,
        0, 1, 0, -(r + b) / 2,
        0, 0, 1, -(zNear + zFar) / 2,
        0, 0, 0, 1;

    //正交矩阵
    Eigen::Matrix4f Ortho_Mat = OrthoScale_Mat * OrthoTrans_Mat;
    projection = Ortho_Mat * PerspToOrtho_Mat;
    return projection;
}

/*作用是得到绕任意过原点的轴的旋转变换矩阵。*/
Eigen::Matrix4f get_rotation(Vector3f axis, float angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    return model;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle)); 
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
