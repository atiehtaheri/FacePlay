// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
//#include <stdafx.h>
/*
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
*/

#include <thread>
#include <iostream>

// Local includes
#include "ControlsUI.h"

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
// bool isGoingBackward;

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
void runCharacter(char order) {
	if (order == 'W') {
		clickKey('W');
		isGoingForward = !isGoingForward;
	}
	else if (order == 'Q') {
		if (isTurningRight) {
			isTurningRight = false;
			isTurningLeft = true;
		}
		else {
			isTurningLeft = !isTurningLeft;
		}
		clickKey('Q');
	}
	else if (order == 'E') {
		if (isTurningLeft) {
			isTurningLeft = false;
			isTurningRight = true;
		}
		else {
			isTurningRight = !isTurningRight;
		}
		clickKey('E');
	}
}

void runWithPress(char order) {
	if (order == 'W' || order == 'Q' || order == 'E') {
		//pressKey(order);
		runCharacter(order);
	}
	else if (order == 'Y' || order == 0x20 || order == 0x18) {
		clickKey(order);
		//Sleep(500);
	}
	Sleep(500);
	return;
}

static void printErrorAndAbort(const std::string & error)
{
	std::cout << error << std::endl;
}
std::vector<std::string> get_arguments(int argc, char **argv)
{

	std::vector<std::string> arguments;

	// First argument is reserved for the name of the executable
	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(std::string(argv[i]));
	}
	return arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////


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
	// MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
	// frame->Show(true);
	// CustomDialog *custom = new CustomDialog(wxT("CustomDialog"));
	// custom->Show(true);
	ControlsUI *fgs = new ControlsUI(wxT("Controls Setting"));
	fgs->Show(true);

	std::vector<std::string> arguments = get_arguments(argc, argv);

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
	int cnt = 0;
	int smile_count = 0;
	int sad_count = 0;
	int eyesopened_count = 0;
	int frowned_count = 0;
	int angry_count = 0;
	int fear_count = 0;
	int pensive_count = 0;
	while (true) // this is not a for loop as we might also be reading from a webcam
	{

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
			
			if (fgs->isClosed) {
				// std::cout << fgs->isClosed << "\n";
				fgs->Show(false);
			}
			// Converting to grayscale
			cv::Mat_<uchar> grayscale_image = sequence_reader.GetGrayFrame();


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

			// Setting up the recorder output
			open_face_rec.SetObservationHOG(detection_success, hog_descriptor, num_hog_rows, num_hog_cols, 31); // The number of channels in HOG is fixed at the moment, as using FHOG
			open_face_rec.SetObservationVisualization(visualizer.GetVisImage());

			/*if (counter == 0) {
				std::cout << "Reg: " << face_analyser.GetCurrentAUsReg().size() << "\t Class: " << face_analyser.GetCurrentAUsClass().size() << "\n";
			}*/

			open_face_rec.SetObservationActionUnits(face_analyser.GetCurrentAUsReg(), face_analyser.GetCurrentAUsClass());
			/*myfile << visualizer.au_frame_data.present01 << ", " << visualizer.au_frame_data.reg01 << ", " <<
				visualizer.au_frame_data.present02 << ", " << visualizer.au_frame_data.reg02 << ", " <<
				visualizer.au_frame_data.present04 << ", " << visualizer.au_frame_data.reg04 << ", " <<
				visualizer.au_frame_data.present05 << ", " << visualizer.au_frame_data.reg05 << ", " <<
				visualizer.au_frame_data.present06 << ", " << visualizer.au_frame_data.reg06 << ", " <<
				visualizer.au_frame_data.present07 << ", " << visualizer.au_frame_data.reg07 << ", " <<
				visualizer.au_frame_data.present09 << ", " << visualizer.au_frame_data.reg09 << ", " <<
				visualizer.au_frame_data.present10 << ", " << visualizer.au_frame_data.reg10 << ", " <<
				visualizer.au_frame_data.present12 << ", " << visualizer.au_frame_data.reg12 << ", " <<
				visualizer.au_frame_data.present14 << ", " << visualizer.au_frame_data.reg14 << ", " <<
				visualizer.au_frame_data.present15 << ", " << visualizer.au_frame_data.reg15 << ", " <<
				visualizer.au_frame_data.present17 << ", " << visualizer.au_frame_data.reg17 << ", " <<
				visualizer.au_frame_data.present20 << ", " << visualizer.au_frame_data.reg20 << ", " <<
				visualizer.au_frame_data.present23 << ", " << visualizer.au_frame_data.reg23 << ", " <<
				visualizer.au_frame_data.present25 << ", " << visualizer.au_frame_data.reg25 << ", " <<
				visualizer.au_frame_data.present26 << ", " << visualizer.au_frame_data.reg26 << ", " <<
				visualizer.au_frame_data.present28 << ", " << visualizer.au_frame_data.reg28 << ", " <<
				visualizer.au_frame_data.present45 << ", " << visualizer.au_frame_data.reg45 << ",\n";*/
			// std::cout << AU6_threshold << "\n";
			if (K1_presence) {
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
				if (visualizer.au_frame_data.reg06 > AU6_threshold && visualizer.au_frame_data.reg12 > AU12_threshold) { // forward -> be happy
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
			if (K2_presence) {
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
			if (K3_presence) {
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
			if (K4_presence) {
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
			if (K5_presence) {
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
			} else {
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
			if (K6_presence) {
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
						runWithPress(0x1B);
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
						runWithPress(0x1B);
						fear_count = 0;
					}
				}
			}
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
			//Sender(visualizer.au_frame_data, cnt);
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
		if (recording_params.outputAUs())
		{
			INFO_STREAM("Postprocessing the Action Unit predictions");
			face_analyser.PostprocessOutputFile(open_face_rec.GetCSVFile());
		}

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
	wxEntryStart(argc, argv);
	wxTheApp->CallOnInit();
	wxTheApp->OnRun();
	wxTheApp->OnExit();
	wxEntryCleanup();
	

	return 0;
}
