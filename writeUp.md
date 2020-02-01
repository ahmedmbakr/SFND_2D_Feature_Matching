# Camera Based 2D Feature Tracking

Here I will address every single points of the rubric for the Midterm camera project.

## MP.0 Mid-Term Report

Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf.

The writeup info and the description of all rubric points are described in this file.

## MP.1 Data Buffer Optimization

Implement a vector for dataBuffer objects whose size does not exceed a limit (e.g. 2 elements). This can be achieved by pushing in new elements on one end and removing elements on the other end.

The dataBuffer is implemented in MidTermProject_Camera_Student.cpp file with lines 113-120.

## MP.2 Keypoint Detection

Implement detectors HARRIS, FAST, BRISK, ORB, AKAZE, and SIFT and make them selectable by setting a string accordingly.

Implemented in file matching_2D_student.cpp in function `detKeypointsModern`.

## MP.3 Keypoint Removal

Remove all keypoints outside of a pre-defined rectangle and only use the keypoints within the rectangle for further processing.

The removal of the keypoints is implemented in `eraseKeyPointsOutsideBox` function defined in MidTermProject_PerformanceEval.cpp file starting rom line 140.

## MP.4 Keypoint Descriptors

Implement descriptors BRIEF, ORB, FREAK, AKAZE and SIFT and make them selectable by setting a string accordingly.

The descriptors implemenation is inside `descKeypoints` function in matching_2D_student.cpp file starting from line 81.

## MP.5 Descriptor Matching, MP.6 Descriptor Distance Ratio

Implement FLANN matching as well as k-nearest neighbor selection. Both methods must be selectable using the respective strings in the main function.

Use the K-Nearest-Neighbor matching to implement the descriptor distance ratio test, which looks at the ratio of best vs. second-best match to decide whether to keep an associated pair of keypoints.

Both points Mp.5, MP.6 are implemented inside `matchDescriptors` function in matching_2D_student.cpp file starting from line 7.

## MP.7, MP.8, MP.9 Performance Evaluation 1,2,3

Performace evaluation is done in a separate file `MidTermProject_PerformanceEval.cpp` which is that contains the main logic inside function `performaceEval` defined starting from line 23 that contains 6 nested loops to test all possilbe combinations of detectors, descriptors , matchers for all images and checks the number of detected keypoints, matched keypoints, and logs the time for each step. Finally the output is generated inside output.csv file.

The csv file is proceesed, converted to excel sheet, and filters are added to compare between different combinations.
The generated file can be found in the main directory under the name `SFND_2D_Feature_Matching.xlsx`.
Here is the main conclusion when comparing the different combinations:

- For detector BRISK, it is the best for finding the maximum number of keypoints.

- For detector BRISK, and Descriptor BRIEF, this combination is the best regarding Nr. Matched keypoints.

- FAST detector is the fastest detector while BRISK detector is the slowest.

-BRIEF is the fastest descriptor while  AKAZE is the slowest one.
