#include "ControlsUI.h"


enum {
	ID_OK = 4,
	ID_RESTORE = 5,
	ID_SAVE = 6
};

float AU6_threshold;
float AU12_threshold;
float AU1_threshold;
float AU4_1_threshold;
float AU15_threshold;
float AU9_threshold;
float AU10_threshold;
float AU2_threshold;
float AU5_threshold;
float AU7_threshold;
float AU23_threshold;
float AU4_2_threshold;
float AU25_threshold;
float AU26_threshold;

bool K1_presence = false;
bool K2_presence = true;
bool K3_presence = false;
bool K4_presence = false;
bool K5_presence = false;
bool K6_presence = true;

const int AU6_ID_SLIDER = 10;
const int AU12_ID_SLIDER = 20;
const int AU1_ID_SLIDER = 30;
const int AU4_1_ID_SLIDER = 40;
const int AU15_ID_SLIDER = 50;
const int AU9_ID_SLIDER = 60;
const int AU10_ID_SLIDER = 70;
const int AU2_ID_SLIDER = 80;
const int AU5_ID_SLIDER = 90;
const int AU7_ID_SLIDER = 100;
const int AU23_ID_SLIDER = 110;
const int AU4_2_ID_SLIDER = 120;
const int AU25_ID_SLIDER = 130;
const int AU26_ID_SLIDER = 140;

const int K1_ID_presence_RB = 150;
const int K2_ID_presence_RB = 160;
const int K3_ID_presence_RB = 170;
const int K4_ID_presence_RB = 180;
const int K5_ID_presence_RB = 190;
const int K6_ID_presence_RB = 200;

const int K1_ID_intensity_RB = 250;
const int K2_ID_intensity_RB = 260;
const int K3_ID_intensity_RB = 270;
const int K4_ID_intensity_RB = 280;
const int K5_ID_intensity_RB = 290;
const int K6_ID_intensity_RB = 300;

const int THRESHOLDS_NUMBER = 14;

const std::string thresholds_filename = "WADefaults.txt";

ControlsUI::ControlsUI(const wxString& title)
	: wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(1430, 488))
{
	isClosed = false;

	wxColour col1, col2;
	col1.Set(wxT("#d1bd8a"));

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer *fgs = new wxFlexGridSizer(13, 11, 10, 20/*, 9, 20*/);

	wxFlexGridSizer *fgs2 = new wxFlexGridSizer(1, 1, 10, 20);

	// ROW 1
	wxStaticText *key1 = new wxStaticText(panel, -1, wxT("Happiness [K1]:"));
	wxFont font = key1->GetFont();
	// std::cout << font.GetPointSize() << "\n";
	font.SetPointSize(10);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	key1->SetFont(font);

	key1_presence_rb = new wxRadioButton(panel, K1_ID_presence_RB,
		wxT("Presense"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	Connect(K1_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
		wxCommandEventHandler(ControlsUI::OnSelect));

	key1_intensity_rb = new wxRadioButton(panel, K1_ID_intensity_RB,
		wxT("Intensity", wxDefaultPosition, wxDefaultSize));
	Connect(K1_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
		wxCommandEventHandler(ControlsUI::OnSelect));

	//key1_intensity_rb->SetValue(true);

	wxStaticText *AU6 = new wxStaticText(panel, -1, wxT("AU6 (Cheek Raiser)"));

	AU6_slider = new wxSlider(panel, AU6_ID_SLIDER, (int)(AU6_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU6_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU6_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU6_threshold));


	wxStaticText *AU12 = new wxStaticText(panel, -1, wxT("AU12 (Lip Corner Puller)"));
	AU12_slider = new wxSlider(panel, AU12_ID_SLIDER, (int)(AU12_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU12_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU12_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU12_threshold));

	// ROW 2
	wxStaticText *key2 = new wxStaticText(panel, -1, wxT("Sadness [K2]:"));
	key2->SetFont(font);

	key2_presence_rb = new wxRadioButton(panel, K2_ID_presence_RB,
		wxT("Presense"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	Connect(K2_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	key2_intensity_rb = new wxRadioButton(panel, K2_ID_intensity_RB,
		wxT("Intensity"));
	Connect(K2_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));

	wxStaticText *AU1 = new wxStaticText(panel, -1, wxT("AU1 (Inner Brow Raiser)"));
	AU1_slider = new wxSlider(panel, AU1_ID_SLIDER, (int)(AU1_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU1_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));
	AU1_slider->Enable(false);

	AU1_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU1_threshold));

	wxStaticText *AU4_1 = new wxStaticText(panel, -1, wxT("AU4 (Brow Lowerer)"));
	AU4_1_slider = new wxSlider(panel, AU4_1_ID_SLIDER, (int)(AU4_1_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU4_1_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));
	AU4_1_slider->Enable(false);

	AU4_1_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU4_1_threshold));

	wxStaticText *AU15 = new wxStaticText(panel, -1, wxT("AU15 (Lip Corner Puller)"));
	AU15_slider = new wxSlider(panel, AU15_ID_SLIDER, (int)(AU15_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU15_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));
	AU15_slider->Enable(false);

	AU15_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU15_threshold));

	// ROW 3
	wxStaticText *key3 = new wxStaticText(panel, -1, wxT("Disgust [K3]:"));
	key3->SetFont(font);

	key3_presence_rb = new wxRadioButton(panel, K3_ID_presence_RB,
		wxT("Presense"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	Connect(K3_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	key3_intensity_rb = new wxRadioButton(panel, K3_ID_intensity_RB,
		wxT("Intensity"));
	Connect(K3_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	//key3_intensity_rb->SetValue(true);

	wxStaticText *AU9 = new wxStaticText(panel, -1, wxT("AU9 (Nose Wrinkler)"));
	AU9_slider = new wxSlider(panel, AU9_ID_SLIDER, (int)(AU9_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU9_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU9_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU9_threshold));

	wxStaticText *AU10 = new wxStaticText(panel, -1, wxT("AU10 (Upper Lip Raiser)"));
	AU10_slider = new wxSlider(panel, AU10_ID_SLIDER, (int)(AU10_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU10_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU10_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU10_threshold));

	// ROW 4
	wxStaticText *key4 = new wxStaticText(panel, -1, wxT("Wide Eyes [K4]:"));
	key4->SetFont(font);

	key4_presence_rb = new wxRadioButton(panel, K4_ID_presence_RB,
		wxT("Presense"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	Connect(K4_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	key4_intensity_rb = new wxRadioButton(panel, K4_ID_intensity_RB,
		wxT("Intensity"));
	Connect(K4_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	//key4_intensity_rb->SetValue(true);

	wxStaticText *AU2 = new wxStaticText(panel, -1, wxT("AU2 (Outer Brow Raiser)"));
	AU2_slider = new wxSlider(panel, AU2_ID_SLIDER, (int)(AU2_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU2_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU2_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU2_threshold));

	wxStaticText *AU5 = new wxStaticText(panel, -1, wxT("AU5 (Upper Lid Raiser)"));
	AU5_slider = new wxSlider(panel, AU5_ID_SLIDER, (int)(AU5_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU5_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU5_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU5_threshold));

	// ROW 5
	wxStaticText *key5 = new wxStaticText(panel, -1, wxT("Lip && Lid Tightener [K5]:"), wxDefaultPosition, wxSize(160, 0));
	key5->SetFont(font);

	key5_presence_rb = new wxRadioButton(panel, K5_ID_presence_RB,
		wxT("Presense"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	Connect(K5_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	key5_intensity_rb = new wxRadioButton(panel, K5_ID_intensity_RB,
		wxT("Intensity"));
	Connect(K5_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	//key5_intensity_rb->SetValue(true);

	wxStaticText *AU7 = new wxStaticText(panel, -1, wxT("AU7 (Lid Tightener)"));
	AU7_slider = new wxSlider(panel, AU7_ID_SLIDER, (int)(AU7_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU7_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU7_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU7_threshold));

	wxStaticText *AU23 = new wxStaticText(panel, -1, wxT("AU23 (Lip Tightener)"));
	AU23_slider = new wxSlider(panel, AU23_ID_SLIDER, (int)(AU23_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU23_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));

	AU23_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU23_threshold));

	// ROW 6
	wxStaticText *key6 = new wxStaticText(panel, -1, wxT("Jaw Drop [K6]:"));
	key6->SetFont(font);

	key6_presence_rb = new wxRadioButton(panel, K6_ID_presence_RB,
		wxT("Presense"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	Connect(K6_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	key6_intensity_rb = new wxRadioButton(panel, K6_ID_intensity_RB,
		wxT("Intensity"));
	Connect(K6_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnSelect));
	//key3_intensity_rb->SetValue(true);

	wxStaticText *AU4_2 = new wxStaticText(panel, -1, wxT("AU4 (Brow Lowerer)"));
	// wxTextCtrl *tcAU9 = new wxTextCtrl(panel, -1);
	AU4_2_slider = new wxSlider(panel, AU4_2_ID_SLIDER, (int)(AU4_2_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU4_2_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));
	AU4_2_slider->Enable(false);

	AU4_2_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU4_2_threshold));

	wxStaticText *AU25 = new wxStaticText(panel, -1, wxT("AU25 (Lips Part)"));
	// wxTextCtrl *tcAU10 = new wxTextCtrl(panel, -1);
	AU25_slider = new wxSlider(panel, AU25_ID_SLIDER, (int)(AU25_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU25_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));
	AU25_slider->Enable(false);

	AU25_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU25_threshold));

	wxStaticText *AU26 = new wxStaticText(panel, -1, wxT("AU26 (Jaw Drop)"));
	// wxTextCtrl *tcAU9 = new wxTextCtrl(panel, -1);
	AU26_slider = new wxSlider(panel, AU26_ID_SLIDER, (int)(AU26_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU26_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll));
	AU26_slider->Enable(false);

	AU26_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU26_threshold));

	// ROW 7
	wxButton *restoreDefaults_btn = new wxButton(panel, ID_RESTORE, wxT("Restore Defaults"),
		wxDefaultPosition, wxSize(112, 30));
	wxFont font_btn = restoreDefaults_btn->GetFont();
	font_btn.SetPointSize(9);
	font_btn.SetWeight(wxFONTWEIGHT_BOLD);
	restoreDefaults_btn->SetFont(font_btn);
	Connect(ID_RESTORE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ControlsUI::OnRestore));

	wxButton *SaveAsDefaults_btn = new wxButton(panel, ID_SAVE, wxT("Save As Defaults"),
		wxDefaultPosition, wxSize(112, 30));
	SaveAsDefaults_btn->SetFont(font_btn);
	Connect(ID_SAVE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ControlsUI::OnSave));

	wxButton *ok_btn = new wxButton(panel, ID_OK, wxT("OK"), wxDefaultPosition, wxSize(82, 30));
	font_btn.SetPointSize(9);
	font_btn.SetWeight(wxFONTWEIGHT_BOLD);
	ok_btn->SetFont(font_btn);
	Connect(ID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ControlsUI::OnOK));
	readThresholds(thresholds_filename);
	// ROW 1
	fgs->Add(key1, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	fgs->Add(key1_presence_rb, 1, wxEXPAND);
	fgs->Add(key1_intensity_rb, 1, wxEXPAND);
	fgs->Add(AU6, 0, wxEXPAND);
	fgs->Add(AU6_slider, 1, wxEXPAND);
	fgs->Add(AU6_slider_value, 1, wxEXPAND);
	fgs->Add(AU12, 0, wxEXPAND);
	fgs->Add(AU12_slider, 1, wxEXPAND);
	fgs->Add(AU12_slider_value, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	// ROW 2
	fgs->Add(key2, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	fgs->Add(key2_presence_rb, 1, wxEXPAND);
	fgs->Add(key2_intensity_rb, 1, wxEXPAND);
	fgs->Add(AU1, 0, wxEXPAND);
	fgs->Add(AU1_slider, 1, wxEXPAND);
	fgs->Add(AU1_slider_value, 1, wxEXPAND);
	fgs->Add(AU4_1, 0, wxEXPAND);
	fgs->Add(AU4_1_slider, 1, wxEXPAND);
	fgs->Add(AU4_1_slider_value, 1, wxEXPAND);
	fgs->Add(AU15, 0, wxEXPAND);
	fgs->Add(AU15_slider, 1, wxEXPAND);
	fgs->Add(AU15_slider_value, 1, wxEXPAND);

	// ROW 3
	fgs->Add(key3, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	fgs->Add(key3_presence_rb, 1, wxEXPAND);
	fgs->Add(key3_intensity_rb, 1, wxEXPAND);
	fgs->Add(AU9, 0, wxEXPAND);
	fgs->Add(AU9_slider, 1, wxEXPAND);
	fgs->Add(AU9_slider_value, 1, wxEXPAND);
	fgs->Add(AU10, 0, wxEXPAND);
	fgs->Add(AU10_slider, 1, wxEXPAND);
	fgs->Add(AU10_slider_value, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	// ROW 4
	fgs->Add(key4, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	fgs->Add(key4_presence_rb, 1, wxEXPAND);
	fgs->Add(key4_intensity_rb, 1, wxEXPAND);
	fgs->Add(AU2, 0, wxEXPAND);
	fgs->Add(AU2_slider, 1, wxEXPAND);
	fgs->Add(AU2_slider_value, 1, wxEXPAND);
	fgs->Add(AU5, 0, wxEXPAND);
	fgs->Add(AU5_slider, 1, wxEXPAND);
	fgs->Add(AU5_slider_value, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	// ROW 5

	fgs->Add(key5, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	fgs->Add(key5_presence_rb, 1, wxEXPAND);
	fgs->Add(key5_intensity_rb, 1, wxEXPAND);
	fgs->Add(AU7, 0, wxEXPAND);
	fgs->Add(AU7_slider, 1, wxEXPAND);
	fgs->Add(AU7_slider_value, 1, wxEXPAND);
	fgs->Add(AU23, 0, wxEXPAND);
	fgs->Add(AU23_slider, 1, wxEXPAND);
	fgs->Add(AU23_slider_value, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	// ROW 6
	fgs->Add(key6, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	fgs->Add(key6_presence_rb, 1, wxEXPAND);
	fgs->Add(key6_intensity_rb, 1, wxEXPAND);
	fgs->Add(AU4_2, 0, wxEXPAND);
	fgs->Add(AU4_2_slider, 1, wxEXPAND);
	fgs->Add(AU4_2_slider_value, 1, wxEXPAND);
	fgs->Add(AU25, 0, wxEXPAND);
	fgs->Add(AU25_slider, 1, wxEXPAND);
	fgs->Add(AU25_slider_value, 1, wxEXPAND);
	fgs->Add(AU26, 0, wxEXPAND);
	fgs->Add(AU26_slider, 1, wxEXPAND);
	fgs->Add(AU26_slider_value, 1, wxEXPAND);

	// ROW 7
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(restoreDefaults_btn, 1, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(SaveAsDefaults_btn, 1, wxEXPAND);
	// fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(ok_btn, 1, wxEXPAND | wxALIGN_RIGHT);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	fgs->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);

	//Col 2 
	wxTextCtrl *textTester = new wxTextCtrl(panel, -1, wxT(""),
		wxPoint(-1, -1), wxSize(170, 400), wxTE_MULTILINE);

	fgs2->Add(textTester, 1, wxEXPAND);


	fgs2->AddGrowableRow(0, 1);
	fgs2->AddGrowableCol(0, 1);

	hbox1->Add(fgs, 1, wxALL | wxEXPAND, 15);
	hbox1->Add(fgs2, 0, 15);
	vbox->Add(hbox1, 0, wxALL);
	panel->SetSizer(vbox);
	Centre();
	//ShowModal();

	// Destroy();
}

void ControlsUI::OnOK(wxCommandEvent & event)
{
	isClosed = true;
}

void ControlsUI::OnSave(wxCommandEvent & event)
{
	writeThresholds(thresholds_filename);
}

void ControlsUI::OnRestore(wxCommandEvent & event)
{
	readThresholds(thresholds_filename);
}

void ControlsUI::Exit()
{
	Close(true);
}

void ControlsUI::OnExit(wxCommandEvent & event)
{
}

void ControlsUI::OnScroll(wxScrollEvent & event)
{
	int au6_val = AU6_slider->GetValue();
	AU6_threshold = au6_val / 10.0f;
	AU6_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU6_threshold));

	int au12_val = AU12_slider->GetValue();
	AU12_threshold = au12_val / 10.0f;
	AU12_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU12_threshold));

	int au1_val = AU1_slider->GetValue();
	AU1_threshold = au1_val / 10.0f;
	AU1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU1_threshold));

	int au4_1_val = AU4_1_slider->GetValue();
	AU4_1_threshold = au4_1_val / 10.0f;
	AU4_1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU4_1_threshold));

	int au15_val = AU15_slider->GetValue();
	AU15_threshold = au15_val / 10.0f;
	AU15_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU15_threshold));

	int au9_val = AU9_slider->GetValue();
	AU9_threshold = au9_val / 10.0f;
	AU9_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU9_threshold));

	int au10_val = AU10_slider->GetValue();
	AU10_threshold = au10_val / 10.0f;
	AU10_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU10_threshold));

	int au2_val = AU2_slider->GetValue();
	AU2_threshold = au2_val / 10.0f;
	AU2_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU2_threshold));

	int au5_val = AU5_slider->GetValue();
	AU5_threshold = au5_val / 10.0f;
	AU5_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU5_threshold));

	int au7_val = AU7_slider->GetValue();
	AU7_threshold = au7_val / 10.0f;
	AU7_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU7_threshold));

	int au23_val = AU23_slider->GetValue();
	AU23_threshold = au23_val / 10.0f;
	AU23_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU23_threshold));

	int au4_2_val = AU4_2_slider->GetValue();
	AU4_2_threshold = au4_2_val / 10.0f;
	AU4_2_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU4_2_threshold));

	int au25_val = AU25_slider->GetValue();
	AU25_threshold = au25_val / 10.0f;
	AU25_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU25_threshold));

	int au26_val = AU26_slider->GetValue();
	AU26_threshold = au26_val / 10.0f;
	AU26_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU26_threshold));
}

void ControlsUI::OnSelect(wxCommandEvent & event)
{
	if (key1_presence_rb->GetValue()) {
		AU6_slider->Enable(false);
		AU12_slider->Enable(false);
		K1_presence = true;
	}
	else {
		AU6_slider->Enable(true);
		AU12_slider->Enable(true);
		K1_presence = false;
	}

	if (key2_presence_rb->GetValue()) {
		AU1_slider->Enable(false);
		AU4_1_slider->Enable(false);
		AU15_slider->Enable(false);
		K2_presence = true;
	}
	else {
		AU1_slider->Enable(true);
		AU4_1_slider->Enable(true);
		AU15_slider->Enable(true);
		K2_presence = false;
	}

	if (key3_presence_rb->GetValue()) {
		AU9_slider->Enable(false);
		AU10_slider->Enable(false);
		K3_presence = true;
	}
	else {
		AU9_slider->Enable(true);
		AU10_slider->Enable(true);
		K3_presence = false;
	}

	if (key4_presence_rb->GetValue()) {
		AU2_slider->Enable(false);
		AU5_slider->Enable(false);
		K4_presence = true;
	}
	else {
		AU2_slider->Enable(true);
		AU5_slider->Enable(true);
		K4_presence = false;
	}

	if (key5_presence_rb->GetValue()) {
		AU7_slider->Enable(false);
		AU23_slider->Enable(false);
		K5_presence = true;
	}
	else {
		AU7_slider->Enable(true);
		AU23_slider->Enable(true);
		K5_presence = false;
	}

	if (key6_presence_rb->GetValue()) {
		AU4_2_slider->Enable(false);
		AU25_slider->Enable(false);
		AU26_slider->Enable(false);
		K6_presence = true;
	}
	else {
		AU4_2_slider->Enable(true);
		AU25_slider->Enable(true);
		AU26_slider->Enable(true);
		K6_presence = false;
	}
}

void ControlsUI::readThresholds(const std::string filename)
{
	std::string line;
	std::ifstream infile(filename);
	std::array<float, THRESHOLDS_NUMBER> thresholds_arr;
	std::array<bool, THRESHOLDS_NUMBER> is_intensity_arr;
	int threshold_cnt = 0;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		float a;
		bool b;
		if (!(iss >> a >> b)) {
			break;
		}
		// std::cout << a << "  GG  " << b << "\n";
		thresholds_arr[threshold_cnt] = a;
		is_intensity_arr[threshold_cnt] = b;
		threshold_cnt++;
	}
	/*for (size_t i = 0; i < thresholds_arr.size(); i++)
	{
		std::cout << thresholds_arr[i] << "\t" << thresholds_arr.size() << "\t" << i << "\t" << threshold_cnt << "\n";
	}*/
	AU6_threshold = thresholds_arr[0];
	AU12_threshold = thresholds_arr[1];
	AU1_threshold = thresholds_arr[2];
	AU4_1_threshold = thresholds_arr[3];
	AU15_threshold = thresholds_arr[4];
	AU9_threshold = thresholds_arr[5];
	AU10_threshold = thresholds_arr[6];
	AU2_threshold = thresholds_arr[7];
	AU5_threshold = thresholds_arr[8];
	AU7_threshold = thresholds_arr[9];
	AU23_threshold = thresholds_arr[10];
	AU4_2_threshold = thresholds_arr[11];
	AU25_threshold = thresholds_arr[12];
	AU26_threshold = thresholds_arr[13];

	AU6_slider->SetValue((int)(AU6_threshold * 10));
	AU6_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU6_threshold));

	AU12_slider->SetValue((int)(AU12_threshold * 10));
	AU12_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU12_threshold));

	AU1_slider->SetValue((int)(AU1_threshold * 10));
	AU1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU1_threshold));

	AU4_1_slider->SetValue(AU4_1_threshold * 10);
	AU4_1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU4_1_threshold));

	AU15_slider->SetValue(AU15_threshold * 10);
	AU15_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU15_threshold));

	AU9_slider->SetValue(AU9_threshold * 10);
	AU9_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU9_threshold));

	AU10_slider->SetValue(AU10_threshold * 10);
	AU10_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU10_threshold));

	AU2_slider->SetValue(AU1_threshold * 10);
	AU2_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU2_threshold));

	AU5_slider->SetValue(AU5_threshold * 10);
	AU5_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU5_threshold));

	AU7_slider->SetValue(AU7_threshold * 10);
	AU7_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU7_threshold));

	AU23_slider->SetValue(AU23_threshold * 10);
	AU23_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU23_threshold));

	AU4_2_slider->SetValue(AU4_2_threshold * 10);
	AU4_2_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU4_2_threshold));

	AU25_slider->SetValue(AU25_threshold * 10);
	AU25_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU25_threshold));

	AU26_slider->SetValue(AU26_threshold * 10);
	AU26_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU26_threshold));

	AU6_slider->Enable(is_intensity_arr[0]);
	AU12_slider->Enable(is_intensity_arr[1]);
	K1_presence = !is_intensity_arr[0];
	key1_presence_rb->SetValue(!is_intensity_arr[0]);
	key1_intensity_rb->SetValue(is_intensity_arr[0]);

	AU1_slider->Enable(is_intensity_arr[2]);
	AU4_1_slider->Enable(is_intensity_arr[3]);
	AU15_slider->Enable(is_intensity_arr[4]);
	K2_presence = !is_intensity_arr[2];
	key2_presence_rb->SetValue(!is_intensity_arr[2]);
	key2_intensity_rb->SetValue(is_intensity_arr[2]);

	AU9_slider->Enable(is_intensity_arr[5]);
	AU10_slider->Enable(is_intensity_arr[6]);
	K3_presence = !is_intensity_arr[5];
	key3_presence_rb->SetValue(!is_intensity_arr[5]);
	key3_intensity_rb->SetValue(is_intensity_arr[5]);

	AU2_slider->Enable(is_intensity_arr[7]);
	AU5_slider->Enable(is_intensity_arr[8]);
	K4_presence = !is_intensity_arr[7];
	key4_presence_rb->SetValue(!is_intensity_arr[7]);
	key4_intensity_rb->SetValue(is_intensity_arr[7]);

	AU7_slider->Enable(is_intensity_arr[9]);
	AU23_slider->Enable(is_intensity_arr[10]);
	K5_presence = !is_intensity_arr[9];
	key5_presence_rb->SetValue(!is_intensity_arr[9]);
	key5_intensity_rb->SetValue(is_intensity_arr[9]);

	AU4_2_slider->Enable(is_intensity_arr[11]);
	AU25_slider->Enable(is_intensity_arr[12]);
	AU26_slider->Enable(is_intensity_arr[13]);
	K6_presence = !is_intensity_arr[11];
	key6_presence_rb->SetValue(!is_intensity_arr[11]);
	key6_intensity_rb->SetValue(is_intensity_arr[11]);
}

void ControlsUI::writeThresholds(const std::string filename)
{
	std::ofstream outfile(filename);

	// Write to the file
	outfile << AU6_threshold << " " << key1_intensity_rb->GetValue() << "\n";
	outfile << AU12_threshold << " " << key1_intensity_rb->GetValue() << "\n";
	outfile << AU1_threshold << " " << key2_intensity_rb->GetValue() << "\n";
	outfile << AU4_1_threshold << " " << key2_intensity_rb->GetValue() << "\n";
	outfile << AU15_threshold << " " << key2_intensity_rb->GetValue() << "\n";
	outfile << AU9_threshold << " " << key3_intensity_rb->GetValue() << "\n";
	outfile << AU10_threshold << " " << key3_intensity_rb->GetValue() << "\n";
	outfile << AU2_threshold << " " << key4_intensity_rb->GetValue() << "\n";
	outfile << AU5_threshold << " " << key4_intensity_rb->GetValue() << "\n";
	outfile << AU7_threshold << " " << key5_intensity_rb->GetValue() << "\n";
	outfile << AU23_threshold << " " << key5_intensity_rb->GetValue() << "\n";
	outfile << AU4_2_threshold << " " << key6_intensity_rb->GetValue() << "\n";
	outfile << AU25_threshold << " " << key6_intensity_rb->GetValue() << "\n";
	outfile << AU26_threshold << " " << key6_intensity_rb->GetValue() << "\n";

	// Close the file
	outfile.close();
}
