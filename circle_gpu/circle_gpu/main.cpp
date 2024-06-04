#include "RANSACCircleDetector.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>

int img_count = 4;

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

	for (Data d : input)
	{
		cv::imshow("image", d.img);
		cv::waitKey(0);
	}

	RANSACCircleDetector detector(input);

	detector.detectCircle();

	int i = 0;
	for (cv::Mat d : detector.getImages())
	{
		cv::imshow("image", d);
		cv::imwrite("Assets/imageDetected_" + std::to_string(i++) + ".jpg", d);
	}

	return 0;
}