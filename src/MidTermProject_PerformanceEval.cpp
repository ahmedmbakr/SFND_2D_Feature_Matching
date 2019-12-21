#include "MidTermProject_PerformanceEval.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

using namespace std;

void performaceEval(std::vector<DataFrame>& dataBuffer, bool bVis)
{
    #if (OUTPUT_TO_CSV_ENABLE == STD_ON)
    cout<<"Detector, Descriptor, Matcher, Matcher Descriptor, Nr. Keypoints, Nr. Matched keypoints, Logtime for keypoint detection, Logtime for descriptor extract, Logtime for matcher\n";
    #endif
    auto imgGray = (dataBuffer.end() - 1)->cameraImg;
    /* DETECT IMAGE KEYPOINTS */
    vector<string> detectorTypes = {/*"SHITOMASI",*/ "HARRIS", "FAST", "BRISK", "ORB", "AKAZE"/*, "SIFT"*/};
    for(string detectorType : detectorTypes)
    {
        #if (OUTPUT_TO_CSV_ENABLE != STD_ON)
        cout << "detectorType: " << detectorType << endl;
        #endif
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image
        double detectorTime;
        detectImgKeypoints(detectorType, keypoints, imgGray, bVis, &detectorTime);

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150);
        if (bFocusOnVehicle)
        {
            eraseKeyPointsOutsideBox(vehicleRect, keypoints);
        }

        // push keypoints and descriptor for current frame to end of data buffer
        (dataBuffer.end() - 1)->keypoints = keypoints;
        #if (OUTPUT_TO_CSV_ENABLE != STD_ON)
        cout << "Number of keypoints on the car:" << keypoints.size() << endl;
        #endif
        //TODO: take note of the distribution of their neighborhood size in MP.7

        /* EXTRACT KEYPOINT DESCRIPTORS */
        vector<string> descriptorTypes = {"BRISK", "BRIEF", "ORB", "FREAK", "AKAZE"/*, "SIFT"*/};
        for(string descriptorType : descriptorTypes)
        {
            #if (OUTPUT_TO_CSV_ENABLE != STD_ON)
            cout << "descriptorType: " << descriptorType << endl;
            #endif
            cv::Mat descriptors;
            double descriptorTime;
            try
            {
                descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType, &descriptorTime);
            }
            catch(cv::Exception e)
            {
                //cout << "This combination {detector=" << detectorType << ", descriptor=" << descriptorType << "} is invalid\n";
                continue;
            }

            // push descriptors for current frame to end of data buffer
            (dataBuffer.end() - 1)->descriptors = descriptors;

            if (dataBuffer.size() > 1) // wait until at least two images have been processed
            {
                vector<string> matcherTypes = {"MAT_BF"/*, "MAT_FLANN"*/};
                vector<string> matcherDescriptorTypes = {"DES_BINARY", "DES_HOG"};
                vector<string>selectorTypes = {"SEL_NN", "SEL_KNN"};
                for(string matcherType : matcherTypes)
                {
                    for(string matcherDescriptorType : matcherDescriptorTypes)
                    {
                        /*if(descriptorType.compare("SIFT") == 0 && matcherDescriptorType.compare("DES_BINARY") == 0)
                        {
                            //SIFT works only with HOG matcher descriptor
                            continue;
                        }
                        if(descriptorType.compare("SIFT") != 0 && matcherDescriptorType.compare("DES_HOG") == 0)
                        {
                            //ALL OTHER DESCRIPTORS ARE BINARY DESCRIPTOR
                            continue;
                        }*/
                        for(string selectorType : selectorTypes)
                        {
                            vector<cv::DMatch> matches;
                            double matcherTime;
                            try
                            {
                                matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                                    (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                                    matches, matcherDescriptorType, matcherType, selectorType, &matcherTime);
                                #if (OUTPUT_TO_CSV_ENABLE != STD_ON)
                                cout << "Number of matched keypoints for combination ";
                                cout << "{detector=" << detectorType << ", descriptor=" << descriptorType <<  ", matcherType=" << matcherType
                                     << ", matcherDescriptorType="<< matcherDescriptorType << ", selector" << selectorType << "} is " 
                                     << matches.size() << endl;
                                #endif
                            }
                            catch(const cv::Exception& e)
                            {
                                //cout << "This combination {detector=" << detectorType << ", descriptor=" << descriptorType <<  ", matcherType=" << matcherType
                                //     << ", matcherDescriptorType="<< matcherDescriptorType << ", selector" << selectorType << "} is invalid\n";
                                //std::cerr << e.what() << '\n';
                                continue;
                            }
                            #if (OUTPUT_TO_CSV_ENABLE == STD_ON)
                            cout<< detectorType << "," << descriptorType << "," << matcherType << "," << matcherDescriptorType << "," <<
                                   keypoints.size() << "," <<  matches.size() << "," << 
                                   1000.0 * detectorTime << "," << 1000.0 * descriptorTime << "," << 1000.0 * matcherTime << "\n";
                            #endif
                            // store matches in current data frame
                            (dataBuffer.end() - 1)->kptMatches = matches;
                        }
                    }
                }
                
            }
        }
    }
}

void detectImgKeypoints(std::string detectorType, std::vector<cv::KeyPoint>& keypoints, cv::Mat& imgGray, bool bVis, double* time)
{
    // extract 2D keypoints from current image
    detKeypointsModern(keypoints, imgGray, detectorType, false, time);
}

void eraseKeyPointsOutsideBox(cv::Rect vehicleRect,vector<cv::KeyPoint>& keypoints)
{
    //Added this comment so that I do not forget
    /*TODO: there will be keypoints within the box that are e.g. on the road surface or on other vehicles.
     Please keep an eye on their number in relation to keypoints on the actual vehicle and discuss
    this later in the evaluation part of the mid-term project.
    */
    for(auto it = keypoints.end(); it != keypoints.begin(); --it)
    {
        if(vehicleRect.contains(it->pt) == false)
        {
            keypoints.erase(it, it + 1);
        }
    }
}



