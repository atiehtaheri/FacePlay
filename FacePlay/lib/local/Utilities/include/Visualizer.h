///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017, Tadas Baltrusaitis all rights reserved.
//
// ACADEMIC OR NON-PROFIT ORGANIZATION NONCOMMERCIAL RESEARCH USE ONLY
//
// BY USING OR DOWNLOADING THE SOFTWARE, YOU ARE AGREEING TO THE TERMS OF THIS LICENSE AGREEMENT.  
// IF YOU DO NOT AGREE WITH THESE TERMS, YOU MAY NOT USE OR DOWNLOAD THE SOFTWARE.
//
// License can be found in OpenFace-license.txt
//
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

#ifndef VISUALIZER_H
#define VISUALIZER_H

// System includes
#include <vector>

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Utilities
{
	struct au_data {
		// int name1 = 0;
		float reg01 = 0.0f;
		bool present01 = false;
		// int name2 = 0;
		float reg02 = 0.0f;
		bool present02 = false;
		// int name3 = 0;
		float reg04 = 0.0f;
		bool present04 = false;
		// int name4 = 0;
		float reg05 = 0.0f;
		bool present05 = false;
		// int name5 = 0;
		float reg06 = 0.0f;
		bool present06 = false;
		// int name6 = 0;
		float reg07 = 0.0f;
		bool present07 = false;
		// int name7 = 0;
		float reg09 = 0.0f;
		bool present09 = false;
		// int name8 = 0;
		float reg10 = 0.0f;
		bool present10 = false;
		// int name9 = 0;
		float reg12 = 0.0f;
		bool present12 = false;
		// int name10 = 0;
		float reg14 = 0.0f;
		bool present14 = false;
		// int name11 = 0;
		float reg15 = 0.0f;
		bool present15 = false;
		// int name12 = 0;
		float reg17 = 0.0f;
		bool present17 = false;
		// int name13 = 0;
		float reg20 = 0.0f;
		bool present20 = false;
		// int name14 = 0;
		float reg23 = 0.0f;
		bool present23 = false;
		// int name15 = 0;
		float reg25 = 0.0f;
		bool present25 = false;
		// int name16 = 0;
		float reg26 = 0.0f;
		bool present26 = false;
		// int name17 = 0;
		float reg28 = 0.0f;
		bool present28 = false;
		// int name18 = 0;
		float reg45 = 0.0f;
		bool present45 = false;
	};

	//===========================================================================
	/**
	A class for recording data processed by OpenFace (facial landmarks, head pose, facial action units, aligned face, HOG features, and tracked video
	*/
	class Visualizer {

	public:

		// The constructor for the visualizer that specifies what to visualize
		Visualizer(std::vector<std::string> arguments);
		Visualizer(bool vis_track, bool vis_hog, bool vis_align, bool vis_aus);

		// Adding observations to the visualizer
		
		// Pose related observations
		void SetImage(const cv::Mat& canvas, float fx, float fy, float cx, float cy);

		// All observations relevant to facial landmarks (optional visibilities parameter to not display all landmarks)
		void SetObservationLandmarks(const cv::Mat_<float>& landmarks_2D, double confidence, const cv::Mat_<int>& visibilities = cv::Mat_<int>());

		// Pose related observations
		void SetObservationPose(const cv::Vec6f& pose, double confidence);
		
		void SetObservationActionUnits(const std::vector<std::pair<std::string, double> >& au_intensities, const std::vector<std::pair<std::string, double> >& au_occurences);

		// Gaze related observations
		void SetObservationGaze(const cv::Point3f& gazeDirection0, const cv::Point3f& gazeDirection1, const std::vector<cv::Point2f>& eye_landmarks, const std::vector<cv::Point3f>& eye_landmarks3d, double confidence);

		// Face alignment related observations
		void SetObservationFaceAlign(const cv::Mat& aligned_face);

		// HOG feature related observations
		void SetObservationHOG(const cv::Mat_<double>& hog_descriptor, int num_cols, int num_rows);

		void SetFps(double fps);

		// Return key-press that could have resulted in the open windows
		char ShowObservation();

		cv::Mat GetVisImage();
		cv::Mat GetHOGVis();

		//void Sender(au_data au_i);
		// Keeping track of what we're visualizing
		bool vis_track;
		bool vis_hog;
		bool vis_align;
		bool vis_aus;

		// Can be adjusted to show less confident frames
		double visualisation_boundary = 0.4;

		std::vector<int> au_names_to_numbers;

		au_data au_frame_data;
		

	private:

		// Temporary variables for visualization
		cv::Mat captured_image; // out canvas
		cv::Mat tracked_image;
		cv::Mat hog_image;
		cv::Mat aligned_face_image;
		cv::Mat action_units_image;

		// Useful for drawing 3d
		float fx, fy, cx, cy;

	};
}
#endif