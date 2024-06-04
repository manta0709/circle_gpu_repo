#pragma once
#include <opencv2/opencv.hpp>

struct Data
{
	cv::Mat img;
	std::vector<cv::Point2i> points;
};

class RANSACCircleDetector
{
private:
	std::vector<Data> data;

public:
	RANSACCircleDetector(std::vector<Data>& data) : data(data) {};

	virtual void detectCircle() = 0;

	std::vector<cv::Mat> getImages()
	{
		std::vector<cv::Mat> images;

		for (Data d : data)
		{
			images.push_back(d.img);
		}

		return images;
	}
};