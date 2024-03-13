#include <iostream>
#include <cstring>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

// 라이브러리의 이미지 전처리 함수
void preprocess_image_bottom(uint8_t *input, int input_size, float *true_input)
{
  // image size
  int H = 480;
  int W = 640;

  // zero fill
  cv::Mat img(cv::Size(W, H), CV_8UC3, input);
  for(int r = 135; r < H - 135; r++){
    for(int c = 265; c < W - 255; c++){
      img.at<cv::Vec3b>(r,c)=cv::Vec3b(0,0,0);
    }
  }

  // zero fill 결과 확인을 위해 이미지 저장
  cv::imwrite("zerofill.BMP", img);

  // margin_params
  int l = 152;
  int r = 140;
  int u = 60;
  int d = 60;

  // crop
  cv::Rect roi(l, u, W - l - r, H - d - u);
  cv::Mat image_cropped = img(roi);

  // preprocess start
  cv::Mat imageSmall;

  // 0-1로 range 변환
  cv::Mat image_float;
  image_cropped.convertTo(image_float, CV_32FC3, 1 / 255.);
  cv::resize(image_float, imageSmall, cv::Size(256, 256));

  // norm
  cv::Scalar mean = cv::mean(imageSmall);
  float meanValue = (mean[0] + mean[1] + mean[2]) / 3.;
  imageSmall -= cv::Scalar(meanValue, meanValue, meanValue);

  memcpy(true_input, imageSmall.ptr<float>(0), 256 * 256 * 3 * sizeof(float));
}

// 위의 함수 동작을 테스트해보기위한 메인함수
int main(int argc, char** argv){

    char* filename=argv[1]; // 이미지파일명
    cv::Mat img_bottom;
    img_bottom = cv::imread(filename);

    float *buffer = (float *)malloc(sizeof(float) * 256 * 256 * 3); // 전처리 결과를 이 주소에 저장
    preprocess_image_bottom(img_bottom.ptr<uint8_t>(0), 640 * 480 * 3, buffer);
    free(buffer);
    return 0;
}