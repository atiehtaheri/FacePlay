#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <map>
#include <wx/wxprec.h>
#include <wx/combo.h>
#include <wx/listctrl.h>
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

extern int angles_diff_threshold;

extern int tracking_modes[];

class ControlsUI : public wxFrame
{
public:
	ControlsUI(const wxString& title);
	void OnOK(wxCommandEvent & event);
	void OnSave(wxCommandEvent & event);
	void OnRestore(wxCommandEvent & event);
	void Exit();
	void OnExit(wxCommandEvent & event);
	
	void OnScroll6(wxScrollEvent & event);
	void OnScroll12(wxScrollEvent & event);
	void OnScroll1(wxScrollEvent & event);
	void OnScroll4_1(wxScrollEvent & event);
	void OnScroll15(wxScrollEvent & event);
	void OnScroll9(wxScrollEvent & event);
	void OnScroll10(wxScrollEvent & event);
	void OnScroll1_1(wxScrollEvent & event);
	void OnScroll2(wxScrollEvent & event);
	void OnScroll5(wxScrollEvent & event);
	void OnScroll7(wxScrollEvent & event);
	void OnScroll23(wxScrollEvent & event);
	void OnScroll4_2(wxScrollEvent & event);
	void OnScroll25(wxScrollEvent & event);
	void OnScroll26(wxScrollEvent & event);

	void OnHNScroll(wxScrollEvent & event);
	
	void OnRB1Select(wxCommandEvent& event);
	void OnRB2Select(wxCommandEvent& event);
	void OnRB3Select(wxCommandEvent& event);
	void OnRB4Select(wxCommandEvent& event);
	void OnRB5Select(wxCommandEvent& event);
	void OnRB6Select(wxCommandEvent& event);

	void OnCB1Select(wxCommandEvent& event);
	void OnCB2Select(wxCommandEvent & event);
	void OnCB3Select(wxCommandEvent & event);
	void OnCB4Select(wxCommandEvent & event);
	void OnCB5Select(wxCommandEvent & event);
	void OnCB6Select(wxCommandEvent & event);

	wxPanel *panel;
	wxBoxSizer *panel00;
	wxFlexGridSizer *panel01;
	wxFlexGridSizer *panel11;
	wxFlexGridSizer *panel02;
	wxFlexGridSizer *panel12;
	wxFlexGridSizer *panel03;
	wxFlexGridSizer *panel13;
	wxFlexGridSizer *panel04;
	wxFlexGridSizer *panel14;
	wxFlexGridSizer *panel05;
	wxFlexGridSizer *panel15;
	wxFlexGridSizer *panel06;
	wxFlexGridSizer *panel16;
	wxFlexGridSizer *panelhn;
	wxBoxSizer *panel07;
	wxFlexGridSizer *panel17;

	wxColour *text_color;
	wxArrayString *m_arrItems1;
	wxArrayString *m_arrItems2;
	wxArrayString *m_arrItems3;
	wxArrayString *m_arrItems4;
	wxArrayString *m_arrItems5;
	wxArrayString *m_arrItems6;



	/*wxStaticText *txt1;
	wxStaticText *txt2;
	wxStaticText *txt3;
	wxStaticText *txt4;
	wxStaticText *txt5;
	wxStaticText *txt6;
	*/
	wxStaticText *key1;
	wxStaticText *key2;
	wxStaticText *key3;
	wxStaticText *key4;
	wxStaticText *key5;
	wxStaticText *key6;

	wxTextCtrl *tcAU6;
	wxTextCtrl *tcAU12;

	wxStaticText *AU6;
	wxStaticText *AU12;
	wxStaticText *AU1;
	wxStaticText *AU4_1;
	wxStaticText *AU15;
	wxStaticText *AU9;
	wxStaticText *AU10;
	wxStaticText *AU1_1;
	wxStaticText *AU2;
	wxStaticText *AU5;
	wxStaticText *AU7;
	wxStaticText *AU23;
	wxStaticText *AU4_2;
	wxStaticText *AU25;
	wxStaticText *AU26;

	wxSlider *AU6_slider;
	wxSlider *AU12_slider;
	wxSlider *AU1_slider;
	wxSlider *AU4_1_slider;
	wxSlider *AU15_slider;
	wxSlider *AU9_slider;
	wxSlider *AU10_slider;
	wxSlider *AU1_1_slider;
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
	wxStaticText *AU1_1_slider_value;
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

	std::vector<wxStaticText *> keynumbs;
	std::vector<wxStaticText *> empty_static_texts;
	std::vector<wxStaticText *> empty_static_texts1;
	std::vector<wxStaticText *> empty_static_texts2;
	std::vector<wxStaticText *> empty_static_texts3;
	std::vector<wxStaticText *> empty_static_texts4;
	std::vector<wxStaticText *> empty_static_texts5;
	std::vector<wxStaticText *> empty_static_texts6;
	std::vector<wxStaticText *> empty_static_texts7;

	wxComboBox* ComboBox1;
	wxComboBox* ComboBox2;
	wxComboBox* ComboBox3;
	wxComboBox* ComboBox4;
	wxComboBox* ComboBox5;
	wxComboBox* ComboBox6;

	wxStaticText *testl;

	wxStaticText *hn_label;
	wxStaticText *hn_slider_value;
	wxSlider *hn_slider;
	/*wxStaticText *hnlabel2;
	wxStaticText *hnlabel3;
	wxStaticText *hnlabel4;
	wxStaticText *hnlabel5;
	wxStaticText *hnlabel6;*/

	wxButton *restoreDefaults_btn;
	wxButton *SaveAsDefaults_btn;
	wxButton *ok_btn;

	wxStaticText *tester_label;
	wxTextCtrl *text_tester;

	wxFont font;

	bool isClosed;
private:
	
	//std::map<std::string, void(*)()> removeHNs = { {"0", &removeHN1} };
	void readThresholds(const std::string filename);
	void writeThresholds(const std::string filename);

	void tempSave();
	void openTempSaved();

	void addRow1();
	void addFERow10();
	void addFERow11();
	void addRow2();
	void addFERow20();
	void addFERow21();
	void addRow3();
	void addFERow30();
	void addFERow31();
	void addRow4();
	void addFERow40();
	void addFERow41();
	void addRow5();
	void addFERow50();
	void addFERow51();
	void addRow6();
	void addFERow60();
	void addFERow61();
	void addRow7();

	void removeRow1();
	void removeRow2();
	void removeRow3();
	void removeRow4();
	void removeRow5();
	void removeRow6();


	void removeFERow1();
	void removeFERow10();
	void removeFERow11();
	void removeFERow20();
	void removeFERow21();
	void removeFERow30();
	void removeFERow31();
	void removeFERow40();
	void removeFERow41();
	void removeFERow50();
	void removeFERow51();
	void removeFERow60();
	void removeFERow61();
	void removeFERow71();

	void addHNRow11();
	void addHNRow12();
	void addHNRow13();
	void addHNRow14();
	void addHNRow15();
	void addHNRow16();

	void removeHN1();
	void removeHN2();
	void removeHN3();
	void removeHN4();
	void removeHN5();
	void removeHN6();

};
