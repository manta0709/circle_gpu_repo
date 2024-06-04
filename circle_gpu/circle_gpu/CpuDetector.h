#pragma once
#include "RANSACCircleDetector.h"

class CpuDetector : public RANSACCircleDetector
{
private:
	cv::Point3f getCircleParams(const std::vector<cv::Point2i>& points) const
	{
		cv::Mat A(points.size(), 3, CV_32F);
		cv::Mat B(points.size(), 1, CV_32F);

		for (size_t i = 0; i < points.size(); i++)
		{
			A.at<float>(i, 0) = points[i].x;
			A.at<float>(i, 1) = points[i].y;
			A.at<float>(i, 2) = 1.0f;

			B.at<float>(i, 0) = powf(points[i].x, 2) + powf(points[i].y, 2);
		}

		cv::Mat X(3, 1, CV_32F);
		cv::solve(A, B, X, cv::DECOMP_NORMAL);
		//gives [2xc, 2yc, r^2-xc^2-yc^2]
		float x = X.at<float>(0, 0) / 2.f;
		float y = X.at<float>(1, 0) / 2.f;
		float r = sqrtf(X.at<float>(2, 0) + powf(x, 2) + powf(y, 2));

		return { x, y, r };
	}

	inline float distance(const cv::Point3f& c, const cv::Point2f& p) const
	{
		return abs(sqrtf(powf(p.x - c.x, 2) + powf(p.y - c.y, 2)) - c.z);
	}

	std::vector<cv::Point2i> filterInliers(const cv::Point3f& params) const
	{
		std::vector<cv::Point2i> inlier_points;

		for (cv::Point2i p : data)
		{
			if (distance(params, p) < threshold) {
				inlier_points.push_back(p);
			}
		}

		return inlier_points;
	}

	void ransacIter() {
		int idx1, idx2, idx3;

		do
		{
			idx1 = getRandomIndex();
			idx2 = getRandomIndex();
			idx3 = getRandomIndex();
		} while (idx1 == idx2 || idx1 == idx3 || idx2 == idx3);

		std::vector<cv::Point2i> tmp = filterInliers(getCircleParams({ data[idx1], data[idx2], data[idx3] }));;

		if (tmp.size() > mostInliers)
		{
			mostInliers = tmp.size();
			inliers = tmp;
		}
	}
	 
public:
	CpuDetector() {}

	CpuDetector(const std::vector<cv::Point2i>& data, const int iters, const float threshold) : RANSACCircleDetector(data, iters, threshold) {}
	
	void detectCircle() {
		for (int i = 0; i < iters; i++)
		{
			ransacIter();
		}
	}
};