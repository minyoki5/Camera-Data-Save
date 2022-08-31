#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/CompressedImage.h>
#include <sensor_msgs/Image.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>


class Transfer_Camera{
  public:
    Transfer_Camera(){
      sub_cam_info = nh_.subscribe("/image_raw_1/compressed", 1,
                                 &Transfer_Camera::CallbackCameraInfoParam0, this,
                                 ros::TransportHints().tcpNoDelay());   //CompressedImage 메시지를 subscribe
      sub_pano_info = nh_.subscribe("/panorama", 1,
                                 &Transfer_Camera::CallbackCameraInfoParam1, this,
                                 ros::TransportHints().tcpNoDelay());   //Image 메시지를 subscribe

    }
    ~Transfer_Camera(){}


  private:
    ros::NodeHandle nh_;
    ros::Subscriber sub_cam_info;
    ros::Subscriber sub_pano_info;
    u_int32_t cam0_count_{0};   //compressed image count
    u_int32_t cam1_count_{0};   //image count


  public:
    void CallbackCameraInfoParam0(const sensor_msgs::CompressedImageConstPtr& msg) {

        char file_path[512] = {0};
        time_t e_time = msg->header.stamp.sec;
        struct tm* e_tm;
        e_tm = localtime(&e_time);
        char date[512] = {0};
        char date2[512] = {0};
        std::ofstream write_file;
        //write_file.precision(15);

        cv_bridge::CvImagePtr cv_ptr;

         try {
          cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
          ROS_ERROR("cv_bridge exception: %s", e.what());
        }
        if (cv_ptr->image.empty()) {
          ROS_INFO("Empty");
        }

        cv::Mat image = cv_ptr->image;  //CvMat형식으로 메시지 형식의 이미지를 변환

        std::vector<uchar> buff;  // buffer for coding
        std::vector<int> param(2);
        param[0] = cv::IMWRITE_JPEG_QUALITY;
        param[1] = 95;  // default(95) 0-100
        sprintf(file_path, "/home/minhyukoh");
        sprintf(file_path, "%s/images/camera1/%010d.jpg", file_path, cam0_count_);
       
        cv::imwrite(file_path,image,param); //파일 경로에 지정한 파라미터로 image를 저장
        cam0_count_++;
        
    }

    void CallbackCameraInfoParam1(const sensor_msgs::ImageConstPtr& msg) {

        char file_path[512] = {0};
        time_t e_time = msg->header.stamp.sec;
        struct tm* e_tm;
        e_tm = localtime(&e_time);
        char date[512] = {0};
        char date2[512] = {0};
        std::ofstream write_file;
        //write_file.precision(15);

        cv_bridge::CvImagePtr cv_ptr;

         try {
          cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
          ROS_ERROR("cv_bridge exception: %s", e.what());
        }
        if (cv_ptr->image.empty()) {
          ROS_INFO("%d, jpg files DONE.",cam1_count_);
        }

        cv::Mat image = cv_ptr->image;
        std::vector<uchar> buff;  // buffer for coding
        std::vector<int> param(2);
        param[0] = cv::IMWRITE_JPEG_QUALITY;
        param[1] = 95;  // default(95) 0-100
        sprintf(file_path, "/home/minhyukoh");
        sprintf(file_path, "%s/images/panorama/%010d.jpg", file_path, cam1_count_);
        cv::imwrite(file_path,image,param);

        cam1_count_++;

    }
};


int main(int argc, char** argv){
  ros::init(argc, argv, "raw_to_jpg");

  Transfer_Camera recorder;
  
  ros::spin();
}