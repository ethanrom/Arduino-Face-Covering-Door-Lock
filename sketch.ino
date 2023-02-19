#include <Servo.h>
#include <opencv2/opencv.hpp>

// Define the pins for the servo motor
#define SERVO_PIN 9

// Create a Servo object
Servo doorLock;

// Define the OpenCV cascade classifiers for face and nose detection
cv::CascadeClassifier faceCascade, noseCascade;

// Define the OpenCV video capture object
cv::VideoCapture camera(0);

// Set the minimum face size for detection (in pixels)
const int minFaceSize = 80;

void setup() {
  // Attach the servo to the pin
  doorLock.attach(SERVO_PIN);

  // Load the OpenCV cascade classifiers
  faceCascade.load("haarcascade_frontalface_default.xml");
  noseCascade.load("haarcascade_mcs_nose.xml");

  // Open the camera
  if (!camera.isOpened()) {
    Serial.println("Could not open camera");
    while (1);
  }
}

void loop() {
  // Capture a frame from the camera
  cv::Mat frame;
  camera >> frame;

  // Convert the frame to grayscale
  cv::Mat gray;
  cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

  // Detect faces in the grayscale image
  std::vector<cv::Rect> faces;
  faceCascade.detectMultiScale(gray, faces, 1.3, 5);

  // Check if any faces were detected
  if (!faces.empty()) {
    // Get the largest face in the image
    cv::Rect largestFace;
    int maxArea = 0;
    for (const auto& face : faces) {
      if (face.area() > maxArea) {
        largestFace = face;
        maxArea = face.area();
      }
    }

    // Check if the face is large enough
    if (largestFace.width > minFaceSize && largestFace.height > minFaceSize) {
      // Detect the nose within the face region
      cv::Mat faceROI = gray(largestFace);
      std::vector<cv::Rect> noses;
      noseCascade.detectMultiScale(faceROI, noses, 1.3, 5);

      // Check if the nose was detected
      if (!noses.empty()) {
        // Rotate the servo to unlock the door
        doorLock.write(90);

        // Wait for a second
        delay(1000);

        // Rotate the servo back to lock the door
        doorLock.write(0);
      }
    }
  }
}
