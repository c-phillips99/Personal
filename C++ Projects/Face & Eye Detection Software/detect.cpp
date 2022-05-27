#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;

void detectAndDraw (Mat& image, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade, double scale);

int main ()
{
    // Class for capturing video feed
    VideoCapture video;
    Mat frame, image;

    CascadeClassifier faceCascade, eyeCascade;
    double scale = 1;

    if (!eyeCascade.load("../haarcascade_eye_tree_eyeglasses.xml"))
    {
        std::cout << "Can't load eyes";
        return -1;
    }
    else
    {
        eyeCascade.load("../haarcascade_eye_tree_eyeglasses.xml");
    }

    if (!faceCascade.load("../haarcascade_frontalface_alt.xml"))
    {
        std::cout << "Can't load face";
        return -1;
    }
    else
    {
        faceCascade.load("../haarcascade_frontalface_alt.xml");
    }

    // 0 for webcam; "path/vid.mp4" for local video
    video.open(0);

    if (video.isOpened())
    {
        while (true)
        {
            // Load next frame
            video >> frame;

            if (frame.empty())
                break;
            else
            {
                Mat frame1 = frame.clone();
                // Draw diagrams
                detectAndDraw(frame1, faceCascade, eyeCascade, scale);
                char c = (char) waitKey(5);

                // Quit if 'q' is pressed
                if (c == 27 || c == 'q' || c == 'Q')
                    break;
            }
        }
    }
    else
    {
        std::cout << "Could not load video";
    }

    return 0;
}

void detectAndDraw (Mat& image, CascadeClassifier& faceCascade, CascadeClassifier& eyeCascade, double scale)
{
    std::vector<Rect> faces;
    Mat grayImage, smallImage;

    // Convert image to black and white
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    double res = 1 / scale;

    // Resize the B/W Image
    resize(grayImage, smallImage, Size(), res, res, INTER_LINEAR);
    // Add contrast for easier detection
    equalizeHist(smallImage, smallImage);

    // Detect faces
    faceCascade.detectMultiScale(smallImage, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size (30, 30));

    for(size_t i = 0; i < faces.size(); i++)
    {
        Rect r = faces[i];
        Mat smallImageROI;
        std::vector<Rect> nestedObjects;
        Point center;
        int radius;
        Scalar color = Scalar (255, 255, 0);
        double aspectRatio = (double) r.width / r.height;
        
        // Draw circles / rectangles around faces
        if (0.75 < aspectRatio && aspectRatio < 1.3)
        {
            center.x = cvRound((r.x + r.width * 0.5) * scale);
            center.y = cvRound((r.y + r.height * 0.5) * scale);
            radius = cvRound((r.width + r.height) * 0.25 * scale);
            circle(image, center, radius, color, 3, 8, 0);
        }
        else
        {
            rectangle(image, Point(cvRound(r.x * scale), cvRound(r.y * scale)),
            Point(cvRound((r.x + r.width-1) * scale), cvRound((r.y + r.height-1) * scale)), color, 3, 8, 0);
        }

        if (eyeCascade.empty())
            continue;
        
        // Focus on face
        smallImageROI = smallImage(r);
        // Detect eyes
        eyeCascade.detectMultiScale(smallImageROI, nestedObjects, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

        // Draw circles around eyes
        for (size_t j = 0; j < nestedObjects.size(); j++)
        {
            Rect nestedR = nestedObjects[j];
            center.x = cvRound((r.x + nestedR.x + nestedR.width * 0.5) * scale);
            center.y = cvRound((r.y + nestedR.y + nestedR.height * 0.5) * scale);
            radius = cvRound((nestedR.width + nestedR.height) * 0.25 * scale);
            circle(image, center, radius, color, 3, 8, 0);
        }
    }

    // Show image with face detection
    imshow("Face Detection - (Press 'Q' to quit)", image);
}
