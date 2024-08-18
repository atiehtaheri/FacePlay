#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
class ControlsUI : public wxFrame
{
public:
	ControlsUI(const wxString& title);
	void OnOK(wxCommandEvent & event);
	void OnSave(wxCommandEvent & event);
	void OnRestore(wxCommandEvent & event);
	void Exit();
	void OnExit(wxCommandEvent & event);
	void OnScroll(wxScrollEvent & event);
	void OnSelect(wxCommandEvent& event);
	wxTextCtrl *tcAU6;
	wxTextCtrl *tcAU12;

	wxSlider *AU6_slider;
	wxSlider *AU12_slider;
	wxSlider *AU1_slider;
	wxSlider *AU4_1_slider;
	wxSlider *AU15_slider;
	wxSlider *AU9_slider;
	wxSlider *AU10_slider;
	wxSlider *AU2_slider;
	wxSlider *AU5_slider;
	wxSlider *AU7_slider;
	wxSlider *AU23_slider;
	wxSlider *AU4_2_slider;
	wxSlider *AU25_slider;
	wxSlider *AU26_slider;

	wxStaticText *AU6_slider_value;
	wxStaticText *AU12_slider_value;
	wxStaticText *AU1_slider_value;
	wxStaticText *AU4_1_slider_value;
	wxStaticText *AU15_slider_value;
	wxStaticText *AU9_slider_value;
	wxStaticText *AU10_slider_value;
	wxStaticText *AU2_slider_value;
	wxStaticText *AU5_slider_value;
	wxStaticText *AU7_slider_value;
	wxStaticText *AU23_slider_value;
	wxStaticText *AU4_2_slider_value;
	wxStaticText *AU25_slider_value;
	wxStaticText *AU26_slider_value;

	wxRadioButton *key1_presence_rb;
	wxRadioButton *key2_presence_rb;
	wxRadioButton *key3_presence_rb;
	wxRadioButton *key4_presence_rb;
	wxRadioButton *key5_presence_rb;
	wxRadioButton *key6_presence_rb;

	wxRadioButton *key1_intensity_rb;
	wxRadioButton *key2_intensity_rb;
	wxRadioButton *key3_intensity_rb;
	wxRadioButton *key4_intensity_rb;
	wxRadioButton *key5_intensity_rb;
	wxRadioButton *key6_intensity_rb;

	bool isClosed;
private:
	void readThresholds(const std::string filename);
	void writeThresholds(const std::string filename);
};
