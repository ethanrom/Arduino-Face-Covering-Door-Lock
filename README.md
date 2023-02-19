# Arduino-Face-Covering-Door-Lock

<p>The setup function is called when the Arduino is powered on, and attaches the servo to the designated pin, loads the OpenCV cascade classifiers for face and nose detection, and opens the camera for video capture.</p>
<p>The loop function runs continuously, capturing a frame from the camera, converting it to grayscale, and detecting faces in the image using the OpenCV face cascade classifier.</p>
<p>If a face is detected, the largest face in the image is selected, and the nose cascade classifier is used to detect the nose within the face region.</p>
<p>If the nose is detected, the servo rotates 90 degrees to unlock the door, waits for a second, and then rotates back to its original position to lock the door again.</p>
