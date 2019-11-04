#pragma once

/*
* EdgeDetection.cpp: OpenCV demo algorithm for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "EdgeDetection.h"
#include "OnScreenDebug.h"

#include "Engine.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "GameFramework/Actor.h"
#include "OnscreenDebug.h"

#include <opencv2/video/tracking.hpp>
#include <opencv2/core.hpp>

#include <cstdint>

using namespace cv;


EdgeDetection::EdgeDetection(int width, int height) : VisionAlgorithm(width, height)
{
	_vertexCount = 0;
}

EdgeDetection::~EdgeDetection()
{
}

void EdgeDetection::perform(void)
{

	cv::Mat3b hsv;
	cv::cvtColor(*_bgrimg, hsv, cv::COLOR_BGR2HSV);

	cv::Mat1b mask1, mask2;
	inRange(hsv, Scalar(0, 70, 20), Scalar(10, 255, 255), mask1);
	inRange(hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

	Mat1b mask = mask1 | mask2;

	// Run Canny edge detection algorithm on blurred gray image
	cv::Canny(mask, mask, LOW_THRESHOLD, LOW_THRESHOLD*RATIO, KERNEL_SIZE);

	// Find edges
	cv::Mat nonZeroCoordinates;
	cv::findNonZero(mask, nonZeroCoordinates);

	// Store vertices (edge coordinates)
	_vertexCount = min((int)nonZeroCoordinates.total(), MAX_VERTICES);
	for (int i = 0; i < _vertexCount; i++) {
		cv::Point vertex = nonZeroCoordinates.at<cv::Point>(i);
		_vertices[i].x = vertex.x;
		_vertices[i].y = vertex.y;
	}
}

void EdgeDetection::draw(AHUD* hud, int leftx, int topy) 
{ 
	for (int i = 0; i < _vertexCount; ++i) {
		cv::Point vertex = _vertices[i];
		hud->DrawRect(EDGE_COLOR, leftx + vertex.x, topy + vertex.y, 1, 1);
	}
}


VisionAlgorithm * getInstance(int width, int height)
{
	return new EdgeDetection(width, height);
}

