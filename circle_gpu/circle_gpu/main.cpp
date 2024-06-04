#include "RANSACCircleDetector.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "CpuDetector.h"
#include <chrono>
#include <iostream>

constexpr int img_count = 4;

int main()
{
	std::vector<Data> input;

	int u, v;
	for (int i = 1; i <= img_count; i++)
	{
		std::vector<cv::Point2i> tmpPoints;

		std::ifstream f("Assets/points" + std::to_string(i) + ".txt");

		if (!f.is_open())
			return 1;
		
		//Fapados kommentkiszedés fájl elejérõl
		f.ignore(1000, '\n');

		while (!f.eof())
		{
			f >> u >> v;
			tmpPoints.push_back({ u, v });
		}

		f.close();
		input.push_back({ cv::imread("Assets/image" + std::to_string(i) + ".png"), tmpPoints});
	}

	CpuDetector cpuDetector;

	int i = 1;
	for (Data d : input)
	{
		//cv::imshow("image", d.img);
		//cv::waitKey(0);

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		cpuDetector = CpuDetector(d.points, 1000, 2.f);
		cpuDetector.detectCircle();

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "CPU run of image " + std::to_string(i) + " took " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microsecoonds" << std::endl;

		for (cv::Point2i p : cpuDetector.getInliers())
		{
			cv::circle(d.img, p, 4, { 255, 0, 0 }, 2);
		}

		//cv::imshow("Circle", d.img);
		//cv::waitKey(0);

		cv::imwrite("Assets/cpu_circle_" + std::to_string(i) + ".jpg", d.img);

		for (cv::Point2i p : d.points)
		{
			cv::circle(d.img, p, 4, { 0, 255, 0 }, 2);
		}
		cv::imwrite("Assets/cpu_all_points_" + std::to_string(i++) + ".jpg", d.img);
	}

	return 0;
}