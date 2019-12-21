#pragma once
#include "matching2D.hpp"

void performaceEval(std::vector<DataFrame>& dataBuffer, bool bVis);

void detectImgKeypoints(std::string detectorType, std::vector<cv::KeyPoint>& keypoints, cv::Mat& imgGray, bool bVis);

/**
 * \brief This function erases the key points outside of the box
 * \param[in] vehicleRect The rectangle that we want to get all the points in
 * \param[in,out] keypoints The original keypoints that we will remove the points outside of the rectangle and keep only points insdie rectangle
 */ 
void eraseKeyPointsOutsideBox(cv::Rect vehicleRect,std::vector<cv::KeyPoint>& keypoints);
