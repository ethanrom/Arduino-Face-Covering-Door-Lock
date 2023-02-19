#include <Servo.h>
#include <Wire.h>
#include <OV7670.h>
#include <Adafruit_MLX90614.h>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

Servo myServo;  // Create a servo object
OV7670 mycam;  // Create a camera object
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // Create a temperature sensor object
int servoPin = 9;  // Define the pin for the servo signal
int angle = 0;  // Initialize the angle variable to 0
int camPinSCL = A5;  // Define the pin for the camera SCL
int camPinSDA = A4;  // Define the pin for the camera SDA

// Initialize the Haar cascade classifiers for face and nose detection
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier nose_cascade;

void setup() {
  myServo.attach(servoPin);  // Attach the servo to the designated pin
  mycam.begin(camPinSCL, camPinSDA);  // Begin the camera object
  mlx.begin(); // Begin the temperature sensor object
  Serial.begin(9600);  // Begin serial communication
  
  // Load the face and nose detection Haar cascade classifiers
  face_cascade.load("haarcascade_frontalface_default.xml");
  nose_cascade.load("haarcascade_mcs_nose.xml");
}

void loop() {
  uint16_t pixels[120 * 160];  // Create an array to store the camera pixels
  int covered_nose = 0;  // Initialize the covered_nose variable to 0
  mycam.takeRGBPicture(pixels);  // Take a picture with the camera

  // Convert the camera image to grayscale and detect faces and noses using the Haar cascade classifiers
  cv::Mat img(120, 160, CV_8UC2, pixels);
  cv::Mat gray;
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
  std::vector<cv::Rect> faces, noses;
  face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
  nose_cascade.detectMultiScale(gray, noses, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

  // Check if a nose is detected and if it is covered
  if (noses.size() > 0) {
    for (size_t i = 0; i < noses.size(); i++) {
      cv::Rect nose = noses[i];
      cv::Point nose_center(nose.x + nose.width/2, nose.y + nose.height/2);
      for (size_t j = 0; j < faces.size(); j++) {
        cv::Rect face = faces[j];
        if (nose_center.x >= face.x && nose_center.x <= face.x + face.width &&
            nose_center.y >= face.y && nose_center.y <= face.y + face.height) {
          covered_nose = 1;
          break;
        }
      }
    }
  }

  if (covered_nose) {  // If a covered nose is detected
    angle = 90;
    myservo.write(angle);  // Lock the door by rotating the servo 90 degrees
    delay(1000);  // Wait for 1 second
  } else {  // If a covered nose is not detected
    angle = 0;
    myservo.write(angle);  // Unlock the door by rotating the servo back to 0 degrees
    delay(1000);  // Wait for 1 second
  }
}

   
