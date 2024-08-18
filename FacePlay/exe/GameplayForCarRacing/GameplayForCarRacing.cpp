///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017, Carnegie Mellon University and University of Cambridge,
// all rights reserved.
//
// ACADEMIC OR NON-PROFIT ORGANIZATION NONCOMMERCIAL RESEARCH USE ONLY
//
// BY USING OR DOWNLOADING THE SOFTWARE, YOU ARE AGREEING TO THE TERMS OF THIS LICENSE AGREEMENT.  
// IF YOU DO NOT AGREE WITH THESE TERMS, YOU MAY NOT USE OR DOWNLOAD THE SOFTWARE.
//
// License can be found in OpenFace-license.txt

//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite at least one of the following works:
//
//       OpenFace 2.0: Facial Behavior Analysis Toolkit
//       Tadas Baltrušaitis, Amir Zadeh, Yao Chong Lim, and Louis-Philippe Morency
//       in IEEE International Conference on Automatic Face and Gesture Recognition, 2018  
//
//       Convolutional experts constrained local model for facial landmark detection.
//       A. Zadeh, T. Baltrušaitis, and Louis-Philippe Morency,
//       in Computer Vision and Pattern Recognition Workshops, 2017.    
//
//       Rendering of Eyes for Eye-Shape Registration and Gaze Estimation
//       Erroll Wood, Tadas Baltrušaitis, Xucong Zhang, Yusuke Sugano, Peter Robinson, and Andreas Bulling 
//       in IEEE International. Conference on Computer Vision (ICCV),  2015 
//
//       Cross-dataset learning and person-specific normalisation for automatic Action Unit detection
//       Tadas Baltrušaitis, Marwa Mahmoud, and Peter Robinson 
//       in Facial Expression Recognition and Analysis Challenge, 
//       IEEE International Conference on Automatic Face and Gesture Recognition, 2015 
//
///////////////////////////////////////////////////////////////////////////////


// FeatureExtraction.cpp : Defines the entry point for the feature extraction console application.
#include <thread>
#include <iostream>
// Local includes
#include "ControlsUI.h"
#include "DeviceEnumerator.h"
#include "LandmarkCoreIncludes.h"

#include <Face_utils.h>
#include <FaceAnalyser.h>
#include <GazeEstimation.h>
#include <RecorderOpenFace.h>
#include <RecorderOpenFaceParameters.h>
#include <SequenceCapture.h>
#include <Visualizer.h>
#include <VisualizationUtils.h>

#ifndef CONFIG_DIR
#define CONFIG_DIR "~"
#endif

#define INFO_STREAM( stream ) \
std::cout << stream << std::endl

#define WARN_STREAM( stream ) \
std::cout << "Warning: " << stream << std::endl

#define ERROR_STREAM( stream ) \
std::cout << "Error: " << stream << std::endl


bool isTurningRight;
bool isTurningLeft;
bool isGoingForward;
bool isGoingBackward;

/*
class Macro
{
	std::vector<CHAR> keys;
	std::vector<DWORD> pause;
	bool loop = false;
	UINT stopKey = VK_NUMPAD5;

public:
	Macro(std::vector<CHAR> keys, std::vector<DWORD> pause, bool loop = false, UINT stopKey = VK_NUMPAD5);
	static void pressKey(CHAR keyParam);
	void run();
};


Macro::Macro(std::vector<CHAR> keys, std::vector<DWORD> pause, bool loop, UINT stopKey) {
	this->keys = keys;
	this->pause = pause;
	this->loop = loop;
	this->stopKey = stopKey;
}*/

void clickKey(CHAR keyParam) {
	SHORT key;
	UINT mappedkey;
	INPUT input = { 0 };
	key = VkKeyScan(keyParam);
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	Sleep(10);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void pressKey(CHAR keyParam) {
	const int KEYEVENT_KEYUP = 0x02;
	if (keyParam == '1') {
		if (GetAsyncKeyState('2') & 0x8000) {
			keybd_event('2', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('3') & 0x8000) {
			keybd_event('3', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('4') & 0x8000) {
			keybd_event('4', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState(keyParam) & 0x8000) {
			keybd_event(keyParam, 0, KEYEVENT_KEYUP, 0);
			Sleep(400);
		}
		else {
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
	}
	else if (keyParam == '2') {
		if (GetAsyncKeyState('1') & 0x8000) {
			keybd_event('1', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('3') & 0x8000) {
			keybd_event('3', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('4') & 0x8000) {
			keybd_event('4', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState(keyParam) & 0x8000) {
			keybd_event(keyParam, 0, KEYEVENT_KEYUP, 0);
			Sleep(400);
		}
		else {
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
	}
	else if (keyParam == '3') {
		if (GetAsyncKeyState('1') & 0x8000) {
			keybd_event('1', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('2') & 0x8000) {
			keybd_event('2', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('4') & 0x8000) {
			keybd_event('4', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(500);
		}
		else if (GetAsyncKeyState(keyParam) & 0x8000) {
			keybd_event(keyParam, 0, KEYEVENT_KEYUP, 0);
			Sleep(500);
		}
		else {
			keybd_event(keyParam, 0, 0, 0);
			Sleep(500);
		}		
	}
	else if (keyParam == '4') {
		if (GetAsyncKeyState('1') & 0x8000) {
			keybd_event('1', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('2') & 0x8000) {
			keybd_event('2', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(400);
		}
		else if (GetAsyncKeyState('3') & 0x8000) {
			keybd_event('3', 0, KEYEVENT_KEYUP, 0);
			keybd_event(keyParam, 0, 0, 0);
			Sleep(500);
		}
		else if (GetAsyncKeyState(keyParam) & 0x8000) {
			keybd_event(keyParam, 0, KEYEVENT_KEYUP, 0);
			Sleep(500);
		}
		else {
			keybd_event(keyParam, 0, 0, 0);
			Sleep(500);
		}
	}

	/*if (GetAsyncKeyState(keyParam) & 0x8000)
	{
		keybd_event(keyParam, 0, KEYEVENT_KEYUP, 0);
	}
	else {
		keybd_event(keyParam, 0, 0, 0);
	}
	if (keyParam == '1' || keyParam == '2') {
		Sleep(1500);
	}
	else {
		Sleep(400);
		keybd_event(keyParam, 0, 0, 0);
	}*/
}
/*void runWithClick(char order) {

}*/
void driveCar1(char order) {
	if (order == '1') { ///// FORWARD
		if (isGoingForward) {
			if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				clickKey('1');
				Sleep(10);
				clickKey('3');
				Sleep(10);
				isGoingForward = false;
			}
			else if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				clickKey('1');
				Sleep(10);
				clickKey('4');
				Sleep(10);
				isGoingForward = false;
			}
			else {
				clickKey('1');
				Sleep(10);
				isGoingForward = false;
			}
		}
		else if (isGoingBackward) {
			if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				clickKey('2');
				Sleep(10);
				clickKey('1');
				Sleep(10);
				clickKey('3');
				Sleep(10);
				isGoingBackward = false;
				isGoingForward = true;
			}
			else if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				clickKey('2');
				Sleep(10);
				clickKey('1');
				Sleep(10);
				clickKey('4');
				Sleep(10);
				isGoingBackward = false;
				isGoingForward = true;
			}
			else {
				clickKey('2');
				Sleep(10);
				clickKey('1');
				Sleep(10);
				isGoingBackward = false;
				isGoingForward = true;
			}
		}
		else if (isTurningLeft) {
			clickKey('3');
			Sleep(10);
			clickKey('1');
			Sleep(10);
			clickKey('3');
			Sleep(10);
			isGoingForward = true;
		}
		else if (isTurningRight) {
			clickKey('4');
			Sleep(10);
			clickKey('1');
			Sleep(10);
			clickKey('4');
			Sleep(10);
			isGoingForward = true;
		}
		else {
			clickKey('1');
			Sleep(10);
			isGoingForward = !isGoingForward;
		}
	}
	if (order == '2') { /////// BACKWARD
		if (isGoingBackward) {
			if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				clickKey('2');
				Sleep(10);
				clickKey('3');
				Sleep(10);
				isGoingBackward = false;
			}
			else if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				clickKey('2');
				Sleep(10);
				clickKey('4');
				Sleep(10);
				isGoingBackward = false;
			}
			else {
				clickKey('2');
				Sleep(10);
				isGoingBackward = false;
			}
		}
		else if (isGoingForward) {
			if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				clickKey('1');
				Sleep(10);
				clickKey('2');
				Sleep(10);
				clickKey('3');
				Sleep(10);
				isGoingForward = false;
				isGoingBackward = true;
			}
			else if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				clickKey('1');
				Sleep(10);
				clickKey('2');
				Sleep(10);
				clickKey('4');
				Sleep(10);
				isGoingForward = false;
				isGoingBackward = true;
			}
			else {
				clickKey('1');
				Sleep(10);
				clickKey('2');
				Sleep(10);
				isGoingForward = false;
				isGoingBackward = true;
			}
		}
		else if (isTurningLeft) {
			clickKey('3');
			Sleep(10);
			clickKey('2');
			Sleep(10);
			clickKey('3');
			Sleep(10);
			isGoingBackward = true;
		}
		else if (isTurningRight) {
			clickKey('4');
			Sleep(10);
			clickKey('2');
			Sleep(10);
			clickKey('4');
			Sleep(10);
			isGoingBackward = true;
		}
		else {
			clickKey('2');
			Sleep(10);
			isGoingBackward = true;
		}
	}
	if (order == '3') { //// LEFT
		if (isGoingForward) {
			if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				isTurningLeft = false;
			}
			else if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				clickKey('3');
				Sleep(10);
				isTurningRight = false;
				isTurningLeft = true;
			}
			else {
				clickKey('3');
				Sleep(10);
				isTurningLeft = true;
			}
		}
		else if (isGoingBackward) {
			if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				isTurningLeft = false;
			}
			else if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				clickKey('3');
				Sleep(10);
				isTurningRight = false;
				isTurningLeft = true;
			}
			else {
				clickKey('3');
				Sleep(10);
				isTurningLeft = true;
			}
		}
		else if (isTurningRight) {
			clickKey('4');
			Sleep(10);
			clickKey('3');
			Sleep(10);
			isTurningRight = false;
			isTurningLeft = true;
		}
		else if (isTurningLeft) {
			clickKey('3');
			Sleep(10);
			isTurningLeft = false;
		}
		else {
			clickKey('3');
			Sleep(10);
			isTurningLeft = true;
		}
	}
	else if (order == '4') { //// RIGHT
		if (isGoingForward) {
			if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				isTurningRight = false;
			}
			else if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				clickKey('4');
				Sleep(10);
				isTurningLeft = false;
				isTurningRight = true;
			}
			else {
				clickKey('4');
				Sleep(10);
				isTurningRight = true;
			}
		}
		else if (isGoingBackward) {
			if (isTurningRight) {
				clickKey('4');
				Sleep(10);
				isTurningRight = false;
			}
			else if (isTurningLeft) {
				clickKey('3');
				Sleep(10);
				clickKey('4');
				Sleep(10);
				isTurningLeft = false;
				isTurningRight = true;
			}
			else {
				clickKey('4');
				Sleep(10);
				isTurningRight = true;
			}
		}
		else if (isTurningLeft) {
			clickKey('3');
			Sleep(10);
			clickKey('4');
			Sleep(10);
			isTurningLeft = false;
			isTurningRight = true;
		}
		else if (isTurningRight) {
			clickKey('4');
			Sleep(10);
			isTurningRight = false;
		}
		else {
			clickKey('4');
			Sleep(10);
			isTurningRight = true;
		}
	}
}

void driveCar(char order) {
	if (order == '1') {
		if (isGoingBackward) {
			clickKey('2');
			isGoingBackward = false;
			isGoingForward = true;
		}
		else if (isTurningLeft) {
			clickKey('3');
			isTurningLeft = false;
			isGoingForward = true;
		}
		else if (isTurningRight) {
			clickKey('4');
			isTurningRight = false;
			isGoingForward = true;
		}
		else {
			isGoingForward = !isGoingForward;
		}
		clickKey('1');
	}
	else if (order == '2') {
		if (isGoingForward) {
			clickKey('1');
			isGoingForward = false;
			isGoingBackward = true;
		}
		else if (isTurningLeft) {
			clickKey('3');
			isTurningLeft = false;
			isGoingBackward = true;
		}
		else if (isTurningRight) {
			clickKey('4');
			isTurningRight = false;
			isGoingBackward = true;
		}
		else {
			isGoingBackward = !isGoingBackward;
		}
		clickKey('2');
	}
	else if (order == '3') {
		if (isTurningRight) {
			clickKey('4');
			isTurningRight = false;
			isTurningLeft = true;
		}
		/*else if (isGoingForward) {
			clickKey('1');
			clickKey('3');
			isGoingForward = false;
			isTurningLeft = true;
		}
		else if (isGoingBackward) {
			clickKey('2');
			clickKey('3');
			isGoingBackward = false;
			isTurningLeft = true;
		}*/
		else {
			isTurningLeft = !isTurningLeft;
		}
		clickKey('3');
	}
	else if (order == '4') {
		if (isTurningLeft) {
			clickKey('3');
			clickKey('4');
			isTurningLeft = false;
			isTurningRight = true;
		}
		/*else if (isGoingForward) {
			clickKey('1');
			clickKey('4');
			isGoingForward = false;
			isTurningRight = true;
		}
		else if (isGoingBackward) {
			clickKey('2');
			clickKey('4');
			isGoingBackward = false;
			isTurningRight = true;
		}*/
		else {
			clickKey('4');
			isTurningRight = !isTurningRight;
		}
	}
}

void runWithPress(char order) {
	/*if (keys.size() != pause.size()) {
		std::cout << "Amount of keys and breaks must be equal!" << std::endl;
		return;
	}
	do {
	*/
	for (int i = 0; i < 1; ++i) {
		driveCar1(order);
		Sleep(300);
		//Sleep(50); //always at least 50ms break time
	}
	// if (GetAsyncKeyState(stopKey))//stop the loop if defined break key gets pressed.
	return;
	//} while (loop);
}

static void printErrorAndAbort(const std::string & error)
{
	std::cout << error << std::endl;
}

#define FATAL_STREAM( stream ) \
printErrorAndAbort( std::string( "Fatal error: " ) + stream )

std::vector<std::string> get_arguments(int argc, char **argv)
{

	std::vector<std::string> arguments;
	// First argument is reserved for the name of the executable
	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(std::string(argv[i]));
		// std::cout << "ATI: " << std::string(argv[i]) << "\n";
	}
	/*arguments.push_back("/exe/GameplayForWAandFPS");
	*/arguments.push_back("-device");
	arguments.push_back("0");

	return arguments;
}

int var = 0;
float y_old = 0;
float x_old = 0;
float flag_x = 0;
float flag_y = 0;
float sign_x = 0;
float sign_y = 0;
float sign_a = 0;
float sign_b = 0;
float del_x = 0;
float del_y = 0;
int yes = 0;
int yes_counter = 0;
int yes_affirmed = 4;
float no = 0;
double angle_converter = 180.0f / 3.14f;

int pose_angle_old = 300;
int pose_angle_oldest = 300;
int temp_diff_sum = 0;
// int angles_diff_threshold = 35;
int temp_diff = 0;
bool look_down = true;
void detect_yes_or_no(cv::Vec6d pose) {
	//pose_angle1 = int(pose[3]);
}

void yes_detect() {
	if (flag_y > 0) {
		flag_y--;
	}

	if (del_y <= 3 && del_y >= -3 && flag_y > 0) {
		yes++;
		flag_y = 0;
	}

	if (del_y < -6) {
		flag_y = 10;
	}
	else {
		flag_y--;
	}
}

void head_gesture_detect(cv::Rect_<float> bb) {
	float bbx = bb.x;
	float bby = bb.y;
	float bbw = bb.width;
	float bbh = bb.height;

	if (del_x > 5 || del_x < -5) {
		sign_x++;
		if (sign_x > 20) {
			x_old = bbx + bbw / 2;
			y_old = bby + bbh / 3;
			sign_a = sign_b = flag_x = flag_y = 0;
		}
	}
	else {
		sign_x = 0;
	}

	if (del_y > 5 || del_y < -5) {
		sign_y++;
		if (sign_y > 20) {
			x_old = bbx + bbw / 2;
			y_old = bby + bbh / 3;
			sign_a = sign_b = flag_x = flag_y = 0;
		}
	}
	else {
		sign_y = 0;
	}

	del_y = ((y_old + (bby + bbh / 3)) / bbh) * 100;
	del_x = ((x_old + (bbx + bbw / 2)) / bbw) * 100;

	yes_detect();
}

void head_motion(cv::Rect_<float> bb) {
	float bbx = bb.x;
	float bby = bb.y;
	float bbw = bb.width;
	float bbh = bb.height;

	if (del_y > 5 || del_y < -5) {

	}

}

extern int tracking_modes[];

extern float AU6_threshold;
extern float AU12_threshold;
extern float AU1_threshold;
extern float AU4_1_threshold;
extern float AU15_threshold;
extern float AU9_threshold;
extern float AU10_threshold;
extern float AU2_threshold;
extern float AU5_threshold;
extern float AU7_threshold;
extern float AU23_threshold;
extern float AU4_2_threshold;
extern float AU25_threshold;
extern float AU26_threshold;

extern bool K1_presence;
extern bool K2_presence;
extern bool K3_presence;
extern bool K4_presence;
extern bool K5_presence;
extern bool K6_presence;

extern int angles_diff_threshold;

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

enum
{
	ID_Hello = 1

};


bool MyApp::OnInit()
{
	/*while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		float a;
		if (!(iss >> a) { break; } // error
		thresholds[threshold_cnt] = a;
		// process pair (a,b)
	}*/
	ControlsUI *fgs = new ControlsUI(wxT("Control Settings"));
	fgs->Show(true);
	cv::VideoCapture camera;
	DeviceEnumerator deviceEnum;
	int deviceId = -1;
	std::map<int, Device> videoDevices = deviceEnum.getVideoDevicesMap();
	std::map<int, Device>::iterator it;
	for (it = videoDevices.begin(); it != videoDevices.end(); it++) {
		//std::cout << it->first << " SSSSDDDDFFFF " << it->second.id << "  " << it->second.deviceName <<
			// " mmmm " << it->second.devicePath << std::endl;
		if (camera.open(it->second.id)) {
			deviceId = it->second.id;
			break;
		}
	}
	camera.release();
	if (deviceId == -1) {
		std::cout << "No CAMERA WAS FOUND!!!\n";
	}
	std::string deviceIdStr = std::to_string(deviceId);
	std::cout << "A Camera with ID of " << deviceIdStr << " is found and is being used.\n";

	std::vector<std::string> arguments; // = get_arguments(argc, argv);
	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(std::string(argv[i]));
		// std::cout << "ATI: " << std::string(argv[i]) << "\n";
	}
	arguments.push_back("-device");
	arguments.push_back(deviceIdStr);
	// no arguments: output usage
	if (arguments.size() == 1)
	{
		std::cout << "For command line arguments see:" << std::endl;
		std::cout << " https://github.com/TadasBaltrusaitis/OpenFace/wiki/Command-line-arguments";
		return 0;
	}

	isGoingForward = false;
	// isGoingBackward = false;
	isTurningLeft = false;
	isTurningRight = false;

	// Load the modules that are being used for tracking and face analysis
	// Load face landmark detector
	LandmarkDetector::FaceModelParameters det_parameters(arguments);
	// Always track gaze in feature extraction
	LandmarkDetector::CLNF face_model(det_parameters.model_location);

	if (!face_model.loaded_successfully)
	{
		std::cout << "ERROR: Could not load the landmark detector" << std::endl;
		return 1;
	}

	// Load facial feature extractor and AU analyser
	FaceAnalysis::FaceAnalyserParameters face_analysis_params(arguments);
	FaceAnalysis::FaceAnalyser face_analyser(face_analysis_params);

	if (!face_model.eye_model)
	{
		std::cout << "WARNING: no eye model found" << std::endl;
	}

	if (face_analyser.GetAUClassNames().size() == 0 && face_analyser.GetAUClassNames().size() == 0)
	{
		std::cout << "WARNING: no Action Unit models found" << std::endl;
	}

	Utilities::SequenceCapture sequence_reader;

	// A utility for visualizing the results
	Utilities::Visualizer visualizer(arguments);

	// Tracking FPS for visualization
	Utilities::FpsTracker fps_tracker;
	fps_tracker.AddFrame();
	int smile_count = 0;
	int sad_count = 0;
	int eyesopened_count = 0;
	int frowned_count = 0;
	int angry_count = 0;
	int fear_count = 0;
	int pensive_count = 0;
	while (true) // this is not a for loop as we might also be reading from a webcam
	{
		std::cout << "I AM ATIEH!!!!\n";
		// The sequence reader chooses what to open based on command line arguments provided
		if (!sequence_reader.Open(arguments))
			break;

		INFO_STREAM("Device or file opened");

		if (sequence_reader.IsWebcam())
		{
			INFO_STREAM("WARNING: using a webcam in feature extraction, Action Unit predictions will not be as accurate in real-time webcam mode");
			INFO_STREAM("WARNING: using a webcam in feature extraction, forcing visualization of tracking to allow quitting the application (press q)");
			visualizer.vis_track = true;
		}

		cv::Mat captured_image;

		Utilities::RecorderOpenFaceParameters recording_params(arguments, true, sequence_reader.IsWebcam(),
			sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, sequence_reader.fps);
		if (!face_model.eye_model)
		{
			recording_params.setOutputGaze(false);
		}
		Utilities::RecorderOpenFace open_face_rec(sequence_reader.name, recording_params, arguments);

		if (recording_params.outputGaze() && !face_model.eye_model)
			std::cout << "WARNING: no eye model defined, but outputting gaze" << std::endl;

		captured_image = sequence_reader.GetNextFrame();

		// For reporting progress
		double reported_completion = 0;

		INFO_STREAM("Starting tracking");

		int counter = 0;
		int intensities_size = 0;
		int occurences_size = 0;

		while (!captured_image.empty())
		{
			// std::cout << "CAPTURED IMAGE IS NOT EMPTY!!!!\n";

			if (fgs->isClosed) {
				// std::cout << fgs->isClosed << "\n";
				fgs->Show(false);
			}
			// Converting to grayscale
			cv::Mat_<uchar> grayscale_image = sequence_reader.GetGrayFrame();

			// std::cout << "ABCDEFGHIJK  " << bounding_box1.width << " x:  " << bounding_box1.x
			//	<< " y:  " << bounding_box1.y << "  H:  " << bounding_box1.height << "\n";
			// The actual facial landmark detection / tracking
			bool detection_success = LandmarkDetector::DetectLandmarksInVideo(captured_image, face_model, det_parameters, grayscale_image);

			// Gaze tracking, absolute gaze direction
			cv::Point3f gazeDirection0(0, 0, 0); cv::Point3f gazeDirection1(0, 0, 0); cv::Vec2d gazeAngle(0, 0);

			if (detection_success && face_model.eye_model)
			{

				GazeAnalysis::EstimateGaze(face_model, gazeDirection0, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, true);
				GazeAnalysis::EstimateGaze(face_model, gazeDirection1, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, false);
				gazeAngle = GazeAnalysis::GetGazeAngle(gazeDirection0, gazeDirection1);
			}

			//head_gesture_detect(bounding_box1);
			//std::cout << "YES:\t" << yes << "\n";

			// Do face alignment
			cv::Mat sim_warped_img;
			cv::Mat_<double> hog_descriptor; int num_hog_rows = 0, num_hog_cols = 0;

			// Perform AU detection and HOG feature extraction, as this can be expensive only compute it if needed by output or visualization
			if (recording_params.outputAlignedFaces() || recording_params.outputHOG() || recording_params.outputAUs() || visualizer.vis_align || visualizer.vis_hog || visualizer.vis_aus)
			{
				face_analyser.AddNextFrame(captured_image, face_model.detected_landmarks, face_model.detection_success, sequence_reader.time_stamp, sequence_reader.IsWebcam());
				face_analyser.GetLatestAlignedFace(sim_warped_img);
				face_analyser.GetLatestHOG(hog_descriptor, num_hog_rows, num_hog_cols);
			}

			// Work out the pose of the head from the tracked model

			cv::Vec6d pose_estimate = LandmarkDetector::GetPose(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);
			/*if (var % 2 == 0) {
				// std::cout << "ANGLE3:\t" << int(pose_estimate[3] * 100) << "\tOLD:\t" << pose_angle_old << "\n";
				if (int(pose_estimate[3] * 100) > pose_angle_old) {
					temp_diff = int(pose_estimate[3] * 100) - pose_angle_old;
					if (temp_diff > angles_diff_threshold) {
						yes++;
						//look_down = true;
						temp_diff = 0;
						pose_angle_old = int(pose_estimate[3] * 100);
					}
				}
				else {
					pose_angle_old = int(pose_estimate[3] * 100);
				}
				//std::cout << /*pose_estimate[0] << "   " << pose_estimate[1] << "   " << pose_estimate[2] << "   " <<
					//int(pose_estimate[3] * 100) << "   " << int(pose_estimate[4] * 100) << "   " << int(pose_estimate[5] * 100) << "\n";
					//yes << "\n";
			}*/
			if (var % 2 == 0) {
				temp_diff = (int(pose_estimate[3] * 100) - pose_angle_old);
				////// std::cout << int(pose_estimate[3] * 100) << "\t";
				if (temp_diff > 0) {
					temp_diff_sum += temp_diff;
					if (temp_diff_sum > angles_diff_threshold) {
						yes = 1;
						temp_diff_sum = 0;
						pose_angle_old = int(pose_estimate[3] * 100);
					}
				}
				else {
					temp_diff_sum = 0;
					pose_angle_oldest = (int(pose_estimate[3] * 100) - pose_angle_oldest);
				}
				pose_angle_old = int(pose_estimate[3] * 100);
				temp_diff = 0;
				//std::cout << /*pose_estimate[0] << "   " << pose_estimate[1] << "   " << pose_estimate[2] << "   " <<
					//int(pose_estimate[3] * 100) << "   " << int(pose_estimate[4] * 100) << "   " << int(pose_estimate[5] * 100) << "\n";
					//yes << "\n";
				////// std::cout << "YES: " << yes << "\n";
			}
			var++;
			// Keeping track of FPS
			fps_tracker.AddFrame();

			// Displaying the tracking visualizations
			visualizer.SetImage(captured_image, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);
			visualizer.SetObservationFaceAlign(sim_warped_img);
			visualizer.SetObservationHOG(hog_descriptor, num_hog_rows, num_hog_cols);
			visualizer.SetObservationLandmarks(face_model.detected_landmarks, face_model.detection_certainty, face_model.GetVisibilities());
			visualizer.SetObservationPose(pose_estimate, face_model.detection_certainty);
			visualizer.SetObservationGaze(gazeDirection0, gazeDirection1, LandmarkDetector::CalculateAllEyeLandmarks(face_model), LandmarkDetector::Calculate3DEyeLandmarks(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy), face_model.detection_certainty);
			visualizer.SetObservationActionUnits(face_analyser.GetCurrentAUsReg(), face_analyser.GetCurrentAUsClass());
			visualizer.SetFps(fps_tracker.GetFPS());

			// detect key presses
			char character_press = visualizer.ShowObservation();

			// quit processing the current sequence (useful when in Webcam mode)
			if (character_press == 'q')
			{
				fgs->Exit();
				break;
			}
			if (character_press == 's')
			{
				fgs->Show(true);
				fgs->isClosed = false;
			}

			// Setting up the recorder output
			open_face_rec.SetObservationHOG(detection_success, hog_descriptor, num_hog_rows, num_hog_cols, 31); // The number of channels in HOG is fixed at the moment, as using FHOG
			open_face_rec.SetObservationVisualization(visualizer.GetVisImage());

			/*if (counter == 0) {
				std::cout << "Reg: " << face_analyser.GetCurrentAUsReg().size() << "\t Class: " << face_analyser.GetCurrentAUsClass().size() << "\n";
			}*/

			open_face_rec.SetObservationActionUnits(face_analyser.GetCurrentAUsReg(), face_analyser.GetCurrentAUsClass());
			if (tracking_modes[0]) {
				if (yes) {
					runWithPress('1');
				}
			}
			else if (K1_presence) {
				// std::cout << "PRESENCE\n";
				if (visualizer.au_frame_data.present06 && visualizer.au_frame_data.present12) {
					frowned_count = 0;
					eyesopened_count = 0;
					sad_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					smile_count++;
					if (smile_count == 5) {
						runWithPress('1');
						smile_count = 0;
					}
				}
			}
			else {
				// std::cout << "INTENSITY\n";
				if (visualizer.au_frame_data.reg06 > AU6_threshold &&
					visualizer.au_frame_data.reg12 > AU12_threshold) { // forward -> be happy
					frowned_count = 0;
					eyesopened_count = 0;
					sad_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					smile_count++;
					if (smile_count == 5) {
						runWithPress('1');
						smile_count = 0;
					}
				}
			}
			if (tracking_modes[1]) {
				if (yes) {
					runWithPress('2');
				}
			}
			else if (K2_presence) {
				if (visualizer.au_frame_data.present01 && visualizer.au_frame_data.present04 &&
					visualizer.au_frame_data.present15) { // backward -> be sad
					smile_count = 0;
					frowned_count = 0;
					eyesopened_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					sad_count++;
					if (sad_count == 5) {
						runWithPress('2');
						sad_count = 0;
					}
				}
			}
			else {
				if (visualizer.au_frame_data.reg01 > AU1_threshold && visualizer.au_frame_data.reg04 > AU4_1_threshold &&
					visualizer.au_frame_data.reg15 > AU15_threshold) {
					// visualizer.au_frame_data.present15) { // backward -> be sad
					smile_count = 0;
					frowned_count = 0;
					eyesopened_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					sad_count++;
					if (sad_count == 5) {
						runWithPress('2');
						sad_count = 0;
					}
				}
			}
			if (tracking_modes[2]) {
				if (yes) {
					runWithPress('3');
				}
			}
			else if (K3_presence) {
				if (visualizer.au_frame_data.present09 && visualizer.au_frame_data.present10) {
					smile_count = 0;
					sad_count = 0;
					eyesopened_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					frowned_count++;
					if (frowned_count == 4) {
						runWithPress('3');
						frowned_count = 0;
					}
				}
			}
			else {
				if (visualizer.au_frame_data.reg09 > AU9_threshold && visualizer.au_frame_data.reg10 > AU10_threshold) { // turn left -> do frowning
					smile_count = 0;
					sad_count = 0;
					eyesopened_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					frowned_count++;
					if (frowned_count == 4) {
						runWithPress('3');
						frowned_count = 0;
					}
				}
			}
			if (tracking_modes[3]) {
				if (yes) {
					runWithPress('4');
				}
			}
			else if (K4_presence) {
				if (visualizer.au_frame_data.present02 && visualizer.au_frame_data.present05) {
					smile_count = 0;
					sad_count = 0;
					frowned_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					eyesopened_count++;
					if (eyesopened_count == 10) {
						runWithPress('4');
						eyesopened_count = 0;
					}
				}
			}
			else {
				if (visualizer.au_frame_data.reg02 > AU2_threshold && visualizer.au_frame_data.reg05 > AU5_threshold) { // turn right -> open eye
					smile_count = 0;
					sad_count = 0;
					frowned_count = 0;
					angry_count = 0;
					fear_count = 0;
					pensive_count = 0;
					eyesopened_count++;
					if (eyesopened_count == 10) {
						runWithPress('4');
						eyesopened_count = 0;
					}
				}
			}
			if (tracking_modes[4]) {
				if (yes) {
					runWithPress('5');
				}
			}
			else if (K5_presence) {
				if (visualizer.au_frame_data.present07 && visualizer.au_frame_data.present23) {
					smile_count = 0;
					sad_count = 0;
					frowned_count = 0;
					eyesopened_count = 0;
					fear_count = 0;
					pensive_count = 0;
					angry_count++;
					if (angry_count == 5) {
						runWithPress('5');
						angry_count = 0;
					}
				}
			}
			else {
				if (visualizer.au_frame_data.reg07 > AU7_threshold &&
					visualizer.au_frame_data.reg23 > AU23_threshold) { // jump -> lip and lid tightner
					smile_count = 0;
					sad_count = 0;
					frowned_count = 0;
					eyesopened_count = 0;
					fear_count = 0;
					pensive_count = 0;
					angry_count++;
					if (angry_count == 5) {
						runWithPress('5');
						angry_count = 0;
					}
				}
			}
			if (tracking_modes[5]) {
				if (yes) {
					runWithPress('6');
				}
			}
			else if (K6_presence) {
				if (visualizer.au_frame_data.present04 && visualizer.au_frame_data.present25 &&
					visualizer.au_frame_data.present26) {
					smile_count = 0;
					sad_count = 0;
					frowned_count = 0;
					eyesopened_count = 0;
					angry_count = 0;
					pensive_count = 0;
					fear_count++;
					if (fear_count == 4) {
						runWithPress('6');
						fear_count = 0;
					}
				}
			}
			else {
				if (visualizer.au_frame_data.reg04 > AU4_2_threshold && visualizer.au_frame_data.reg25 > AU25_threshold &&
					visualizer.au_frame_data.reg26 > AU26_threshold) { // pause -> brow lowerer and jaw drop
				//if (visualizer.au_frame_data.reg04 > 1.5f && visualizer.au_frame_data.reg25 > 1.7f &&
				//	visualizer.au_frame_data.reg26 > 1.7f) { // pause -> brow lowerer and jaw drop
					smile_count = 0;
					sad_count = 0;
					frowned_count = 0;
					eyesopened_count = 0;
					angry_count = 0;
					pensive_count = 0;
					fear_count++;
					if (fear_count == 4) {
						runWithPress('6');
						fear_count = 0;
					}
				}
			}
			yes = 0;
			//if (visualizer.au_frame_data.reg06 > 2.0f && visualizer.au_frame_data.reg12 > 2.0f
			//	//visualizer.au_frame_data.present06 && visualizer.au_frame_data.present12 //&& 
			//	/*!visualizer.au_frame_data.present04 && !visualizer.au_frame_data.present09 && !visualizer.au_frame_data.present10*/) {
			//	frowned_count = 0;
			//	eyesopened_count = 0;
			//	sad_count = 0;
			//	angry_count = 0;
			//	fear_count = 0;
			//	pensive_count = 0;
			//	smile_count++;
			//	if (smile_count == 5) {
			//		run('1');
			//		smile_count = 0;
			//	}
			//}
			//if (visualizer.au_frame_data.present01 && visualizer.au_frame_data.present04 &&
			//	visualizer.au_frame_data.present15
			//	//visualizer.au_frame_data.present09 && visualizer.au_frame_data.present10 &&
			//	/*!visualizer.au_frame_data.present12*/) {
			//	smile_count = 0;
			//	frowned_count = 0;
			//	eyesopened_count = 0;
			//	angry_count = 0;
			//	fear_count = 0;
			//	pensive_count = 0;
			//	sad_count++;
			//	if (sad_count == 5) {
			//		run('2');
			//		sad_count = 0;
			//	}
			//}
			//if (visualizer.au_frame_data.reg09 > 1.4f && visualizer.au_frame_data.reg10 > 2.0f) {
			//	smile_count = 0;
			//	sad_count = 0;
			//	eyesopened_count = 0;
			//	angry_count = 0;
			//	fear_count = 0;
			//	pensive_count = 0;
			//	frowned_count++;
			//	if (frowned_count == 4) {
			//		run('3');
			//		frowned_count = 0;
			//	}
			//}
			//if (/*visualizer.au_frame_data.reg05 > 0.7f && */visualizer.au_frame_data.reg07 > 1.2f &&
			//	visualizer.au_frame_data.reg23 > 1.0f) {
			//	smile_count = 0;
			//	sad_count = 0;
			//	frowned_count = 0;
			//	eyesopened_count = 0;
			//	fear_count = 0;
			//	pensive_count = 0;
			//	angry_count++;
			//	if (angry_count == 10) {
			//		run('4');
			//		angry_count = 0;
			//	}
			//}
			//if (visualizer.au_frame_data.reg02 > 0.5f && visualizer.au_frame_data.reg05 > 1.5f) {
			//	smile_count = 0;
			//	sad_count = 0;
			//	frowned_count = 0;
			//	angry_count = 0;
			//	fear_count = 0;
			//	pensive_count = 0;
			//	eyesopened_count++;
			//	if (eyesopened_count == 10) {
			//		run('Y');
			//		eyesopened_count = 0;
			//	}
			//}
			//if (visualizer.au_frame_data.reg04 > 1.5f && visualizer.au_frame_data.reg25 > 1.7f &&
			//	visualizer.au_frame_data.reg26 > 1.7f) {
			//	smile_count = 0;
			//	sad_count = 0;
			//	frowned_count = 0;
			//	eyesopened_count = 0;
			//	angry_count = 0;
			//	pensive_count = 0;
			//	fear_count++;
			//	if (fear_count == 4) {
			//		run('N');
			//		fear_count = 0;
			//	}
			//}
			//if (visualizer.au_frame_data.reg02 > 0.9f && visualizer.au_frame_data.reg17 > 1.0f &&
			//	visualizer.au_frame_data.reg05 < 0.3f) {
			//	smile_count = 0;
			//	sad_count = 0;
			//	frowned_count = 0;
			//	eyesopened_count = 0;
			//	angry_count = 0;
			//	fear_count = 0;
			//	pensive_count++;
			//	if (pensive_count == 5) {
			//		run('D');
			//		pensive_count = 0;
			//	}
			//}
			//std::cout << visualizer.au_frame_data.present1 << "\n";
			///std::this_thread::sleep_for(std::chrono::milliseconds(200));
			/*if (visualizer.au_frame_data.present06 && visualizer.au_frame_data.present12) { // happy means start
				run('1');
			}
			if (visualizer.au_frame_data.present01 && visualizer.au_frame_data.present04 && visualizer.au_frame_data.present15) { // sad means stop
				run('2');
			}
			if (visualizer.au_frame_data.present26 && visualizer.au_frame_data.reg26 > 3.0f) { // jaw drop means yes
				run('Y');
			}
			else if (visualizer.au_frame_data.present09 && visualizer.au_frame_data.present10) { // nose wrinkling and upper lip raising means no
				run('N');
			}
			//if (visualizer.au_frame_data.reg18 > .8f && visualizer.au_frame_data.reg14 > .6f && visualizer.au_frame_data.present14) { // blinking means pick up
			if (visualizer.au_frame_data.present04 && visualizer.au_frame_data.present05 && visualizer.au_frame_data.present07 && visualizer.au_frame_data.present14) {
				run('3');
			}
			else if (visualizer.au_frame_data.reg02 > 3.0f && visualizer.au_frame_data.reg05 > 3.0f) { // Openning eyes means put down
				run('4');
			}*/
			//else if (visualizer.au_frame_data.present1)
			//Sender(visualizer.au_frame_data, threshold_cnt);
			//cnt++;
			open_face_rec.SetObservationLandmarks(face_model.detected_landmarks, face_model.GetShape(sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy),
				face_model.params_global, face_model.params_local, face_model.detection_certainty, detection_success);
			open_face_rec.SetObservationPose(pose_estimate);
			open_face_rec.SetObservationGaze(gazeDirection0, gazeDirection1, gazeAngle, LandmarkDetector::CalculateAllEyeLandmarks(face_model), LandmarkDetector::Calculate3DEyeLandmarks(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy));
			open_face_rec.SetObservationTimestamp(sequence_reader.time_stamp);
			open_face_rec.SetObservationFaceID(0);
			open_face_rec.SetObservationFrameNumber(sequence_reader.GetFrameNumber());
			open_face_rec.SetObservationFaceAlign(sim_warped_img);
			open_face_rec.WriteObservation();
			open_face_rec.WriteObservationTracked();

			// Reporting progress
			if (sequence_reader.GetProgress() >= reported_completion / 10.0)
			{
				std::cout << reported_completion * 10 << "% ";
				if (reported_completion == 10)
				{
					std::cout << std::endl;
				}
				reported_completion = reported_completion + 1;
			}

			// Grabbing the next frame in the sequence
			captured_image = sequence_reader.GetNextFrame();

		}

		INFO_STREAM("Closing output recorder");
		open_face_rec.Close();
		INFO_STREAM("Closing input reader");
		sequence_reader.Close();
		INFO_STREAM("Closed successfully");
		/*if (recording_params.outputAUs())
		{
			INFO_STREAM("Postprocessing the Action Unit predictions");
			face_analyser.PostprocessOutputFile(open_face_rec.GetCSVFile());
		}*/

		// Reset the models for the next video
		face_analyser.Reset();
		face_model.Reset();

	}
	return true;
}

IMPLEMENT_APP_NO_MAIN(MyApp);
IMPLEMENT_WX_THEME_SUPPORT;

int main(int argc, char **argv)
{
	/*std::filesystem::path p = fs::current_path();

	std::cout << "The current path " << p << " decomposes into:\n"
		<< "root-path " << p.root_path() << '\n'
		<< "relative path " << p.relative_path() << '\n';*/
	wxEntryStart(argc, argv);
	wxTheApp->CallOnInit();
	wxTheApp->OnRun();
	wxTheApp->OnExit();
	wxEntryCleanup();


	return 0;
}