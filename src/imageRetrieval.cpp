/*
  Identify image fils in a directory
*/
#include <dirent.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>

#include "image.hpp"
#include "process.hpp"

using namespace cv;
using namespace std;
using namespace image;

/*
  Given a directory on the command line, scans through the directory for image files.

  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.
 */
int main(int argc, char *argv[]) {
    char dirname[256];
    cv::Mat source;
    vector<cv::Mat> images;
    int feature;
    int num2Show = 6;

    // check for sufficient arguments
    if (argc < 4) {
        cout << "usage: %s <source> <directory path> <mode> & optional <# images to show>\n";
        exit(-1);
    }
    // # of images to show
    if (argc > 4) {
        num2Show = atoi(argv[4]);
    }

    // print descriptions
    process::printModeDescriptions();

    // get the source image
    source = cv::imread(argv[1]);
    printf("\nProcessing source %s\n", argv[1]);
    if (source.data == NULL) {
        printf("Unable to read query image %s\n", argv[1]);
        exit(-1);
    }

    // get the directory path & load images
    strcpy(dirname, argv[2]);
    process::loadImages(images, dirname);
    cout << "number of images: " << images.size() << "\n\n";

    // assign feature mode by input
    feature = atoi(argv[3]);
    switch (feature) {
    case 0:
        MODE = BASELINE;
        break;
    case 1:
        MODE = HISTOGRAM;
        break;
    case 2:
        MODE = MULTI_HISTOGRAM;
        break;
    case 3:
        MODE = SOBEL_COLOR_RGB;
        break;
    case 4:
        MODE = CUSTOM;
        break;
    case 5:
        MODE = RG_HISTOGRAM;
        break;
    case 6:
        MODE = SOBEL_CHROMA_RG;
        break;
    case 7:
        MODE = SHAPE;
        break;
    case 8:
        MODE = GRADIENT_COLOR_HS;
        break;
    }

    // process images
    vector<pair<cv::Mat, float>> imgDists;
    imgDists = image::calculateDistances(source, images, MODE);

    // sort images
    vector<cv::Mat> res;
    res = image::sortByDistances(imgDists);

    // truncate results
    if (res.size() <= num2Show)
        num2Show = res.size();
    res.resize(num2Show);
    cout << "show sorted results: " << res.size() << "\n";

    // display results
    // process::displayResults(res);

    process::displayResultsInOneWindow(res);

    // NOTE: must add waitKey, or the program will terminate, without showing the result images
    waitKey(0);
    printf("Terminating\n");

    return (0);
}