#pragma once
#include <opencv2/opencv.hpp>
#include <random>

struct Data
{
	cv::Mat img;
	std::vector<cv::Point2i> points;
};

class RANSACCircleDetector
{
private:
	std::mt19937 rng;
	std::uniform_real_distribution<float> udist;

protected:
	int iters;
	float threshold;
	std::vector<cv::Point2i> data;
	int mostInliers = 0;
	std::vector<cv::Point2i> inliers;

	inline int getRandomIndex()
	{
		return udist(rng) * data.size() - 1;
	}

public:
	RANSACCircleDetector() {}

	RANSACCircleDetector(const std::vector<cv::Point2i>& data, const int iters, const float threshold) : data(data), iters(iters), threshold(threshold) {
		//Init random
		udist = std::uniform_real_distribution<float>(0.f, 1.f);
		rng.seed(static_cast<long unsigned int>(time(0)));
	};

	virtual void detectCircle() = 0;

	std::vector<cv::Point2i> getInliers() const
	{
		return inliers;
	}
};