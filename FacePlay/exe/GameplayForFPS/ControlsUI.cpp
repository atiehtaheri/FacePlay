#include "ControlsUI.h"
#include <map>


//#define ADD_METHOD(map_var, func) map_var["func"] = &func


enum {
	ID_OK = 4,
	ID_RESTORE = 5,
	ID_SAVE = 6
};

float key_intensities[] = { .0f, .0f, .0f, .0f, .0f, .0f };

float AU6_threshold;
float AU12_threshold;
float AU1_threshold;
float AU4_1_threshold;
float AU15_threshold;
float AU9_threshold;
float AU10_threshold;
float AU1_1_threshold;
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

int tracking_modes[] = { 0, 0, 0, 0, 0, 0 };
int swapping_modes[] = { 0, 0, 0, 0, 0, 0 };

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
const int AU1_1_ID_SLIDER = 1400;
const int HN_ID_SLIDER = 1500;

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

const int CB1_ID = 310;
const int CB2_ID = 320;
const int CB3_ID = 330;
const int CB4_ID = 340;
const int CB5_ID = 350;
const int CB6_ID = 360;

const int THRESHOLDS_NUMBER = 15;

const int STATIC_TEXT_NUMBS = 9;
const int STATIC_TEXT_NUMBS1 = 10;
const int STATIC_TEXT_NUMBS2 = 3;
const int STATIC_TEXT_NUMBS3 = 8;

std::array<bool, THRESHOLDS_NUMBER> is_intensity_arr;
int golden_index = -1;

int angles_diff_threshold = 35;

const std::string thresholds_filename = "FPSDefaults.txt";

class wxListViewComboPopup : public wxListView, public wxComboPopup
{
public:
	// Initialize member variables
	virtual void Init()
	{
		m_value = -1;
	}
	// Create popup control
	virtual bool Create(wxWindow* parent)
	{
		return wxListView::Create(parent, 1, wxPoint(0, 0), wxDefaultSize);
	}
	// Return pointer to the created control
	virtual wxWindow *GetControl() { return this; }
	// Translate string into a list selection
	virtual void SetStringValue(const wxString& s)
	{
		int n = wxListView::FindItem(-1, s);
		if (n >= 0 && n < wxListView::GetItemCount())
			wxListView::Select(n);
	}
	// Get list selection as a string
	virtual wxString GetStringValue() const
	{
		if (m_value >= 0)
			return wxListView::GetItemText(m_value);
		return wxEmptyString;
	}
	// Do mouse hot-tracking (which is typical in list popups)
	void OnMouseMove(wxMouseEvent& event)
	{
		// TODO: Move selection to cursor
	}
	// On mouse left up, set the value and close the popup
	void OnMouseClick(wxMouseEvent& WXUNUSED(event))
	{
		m_value = wxListView::GetFirstSelected();
		// TODO: Send event as well
		Dismiss();
	}
protected:
	int m_value; // current item index
private:
	wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(wxListViewComboPopup, wxListView)
EVT_MOTION(wxListViewComboPopup::OnMouseMove)
EVT_LEFT_UP(wxListViewComboPopup::OnMouseClick)
wxEND_EVENT_TABLE()

ControlsUI::ControlsUI(const wxString& title)
	: wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(1400, 850))
{
	
	isClosed = false;
	wxColour col1, col2;
	col1.Set(wxT("#2F1739"));
	
	text_color = new wxColour("#FFFFFF");

	panel = new wxPanel(this, -1);
	panel->SetBackgroundColour(col1);

	wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
	

	wxBoxSizer *left_panel = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *right_panel = new wxBoxSizer(wxVERTICAL);
	
	panel00 = new wxBoxSizer(wxHORIZONTAL);

	panel11 = new wxFlexGridSizer(1, 2, 0, 10);
	panel01 = new wxFlexGridSizer(2, 11, 10, 10);
	
	panel12 = new wxFlexGridSizer(1, 2, 0, 10);
	panel02 = new wxFlexGridSizer(2, 11, 10, 10);

	panel13 = new wxFlexGridSizer(1, 2, 0, 10);
	panel03 = new wxFlexGridSizer(2, 11, 10, 10);

	panel14 = new wxFlexGridSizer(1, 2, 0, 10);
	panel04 = new wxFlexGridSizer(2, 11, 10, 10);

	panel15 = new wxFlexGridSizer(1, 2, 0, 10);
	panel05 = new wxFlexGridSizer(2, 11, 10, 10);

	panel16 = new wxFlexGridSizer(1, 2, 0, 10);
	panel06 = new wxFlexGridSizer(2, 11, 10, 10);


	panel07 = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer *panel21 = new wxFlexGridSizer(2, 1, 20, 10);

	wxStaticText *txt = new wxStaticText(
		panel, -1, wxT("From the drop-down menus, you can choose the tracking mode you want for each key." 
			"Then, you can adjust the sensitivity to your preference."));
	txt->SetForegroundColour("#F6F7DD");
	wxFont font = txt->GetFont();
	font.SetPointSize(13);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	// font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	txt->SetFont(font);
	panel00->Add(txt, 0);

	readThresholds(thresholds_filename);

	// ROW 1
	addRow1();
	switch (golden_index)
	{
	case(0): {
		// ROW 1
		addHNRow11();

		// ROW 2
		addRow2();
		addFERow20();
		addFERow21();

		// ROW 3
		addRow3();
		addFERow30();
		addFERow31();

		// ROW 4
		addRow4();
		addFERow40();
		addFERow41();

		// ROW 5
		addRow5();
		addFERow50();
		addFERow51();

		// ROW 6
		addRow6();
		addFERow60();
		addFERow61();
	}
			 break;
	case(1): {
		// ROW 1
		addFERow10();
		addFERow11();
		
		// ROW 2
		addRow2();
		addHNRow12();

		// ROW 3
		addRow3();
		addFERow30();
		addFERow31();

		// ROW 4
		addRow4();
		addFERow40();
		addFERow41();

		// ROW 5
		addRow5();
		addFERow50();
		addFERow51();

		// ROW 6
		addRow6();
		addFERow60();
		addFERow61();
	}
			 break;
	case(2): {
		// ROW 1
		addFERow10();
		addFERow11();

		// ROW 2
		addRow2();
		addFERow20();
		addFERow21();

		// ROW 3
		addRow3();
		addHNRow13();

		// ROW 4
		addRow4();
		addFERow40();
		addFERow41();

		// ROW 5
		addRow5();
		addFERow50();
		addFERow51();

		// ROW 6
		addRow6();
		addFERow60();
		addFERow61();
	}
			 break;
	case(3): {
		// ROW 1
		addFERow10();
		addFERow11();

		// ROW 2
		addRow2();
		addFERow20();
		addFERow21();

		// ROW 3
		addRow3();
		addFERow30();
		addFERow31();

		// ROW 4
		addRow4();
		addHNRow14();

		// ROW 5
		addRow5();
		addFERow50();
		addFERow51();

		// ROW 6
		addRow6();
		addFERow60();
		addFERow61();
	}
			 break;
	case(4): {
		// ROW 1
		addFERow10();
		addFERow11();

		// ROW 2
		addRow2();
		addFERow20();
		addFERow21();

		// ROW 3
		addRow3();
		addFERow30();
		addFERow31();

		// ROW 4
		addRow4();
		addFERow40();
		addFERow41();

		// ROW 5
		addRow5();
		addHNRow15();
		std::cout << "eeeeeeeeeesssssssssss\n";
		// ROW 6
		addRow6();
		addFERow60();
		addFERow61();
	}
			 break;
	case(5): {
		// ROW 1
		addFERow10();
		addFERow11();

		// ROW 2
		addRow2();
		addFERow20();
		addFERow21();

		// ROW 3
		addRow3();
		addFERow30();
		addFERow31();

		// ROW 4
		addRow4();
		addFERow40();
		addFERow41();

		// ROW 5
		addRow5();
		addFERow50();
		addFERow51();

		// ROW 6
		addRow6();
		addHNRow16();
	}
			 break;
	default: {
		// ROW 1
		addFERow10();
		addFERow11();

		// ROW 2
		addRow2();
		addFERow20();
		addFERow21();

		// ROW 3
		addRow3();
		addFERow30();
		addFERow31();

		// ROW 4
		addRow4();
		addFERow40();
		addFERow41();

		// ROW 5
		addRow5();
		addFERow50();
		addFERow51();

		// ROW 6
		addRow6();
		addFERow60();
		addFERow61();
	}
		break;
	}

	// ROW 7
	addRow7();

	left_panel->Add(panel00, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 10);

	left_panel->Add(panel11, 0, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 10);
	left_panel->Add(panel01, 1, wxALL | wxEXPAND, 15);

	left_panel->Add(panel12, 0, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 10);
	left_panel->Add(panel02, 1, wxALL | wxEXPAND, 15);

	left_panel->Add(panel13, 0, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 10);
	left_panel->Add(panel03, 1, wxALL | wxEXPAND, 15);

	left_panel->Add(panel14, 0, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 10);
	left_panel->Add(panel04, 1, wxALL | wxEXPAND, 15);

	left_panel->Add(panel15, 0, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 10);
	left_panel->Add(panel05, 1, wxALL | wxEXPAND, 15);

	left_panel->Add(panel16, 0, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 10);
	left_panel->Add(panel06, 1, wxALL | wxEXPAND, 15);

	left_panel->Add(panel07, 1, wxALIGN_CENTER);


	//Col 2 
	tester_label = new wxStaticText(panel, -1, wxT("Result Window:"));
	tester_label->SetForegroundColour("#F6F7DD");
	text_tester = new wxTextCtrl(panel, -1, wxT(""),
		wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);
	text_tester->SetFocus();
	font = tester_label->GetFont();
	font.SetPointSize(12);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	tester_label->SetFont(font);

	panel21->Add(tester_label, 1, wxEXPAND);
	panel21->Add(text_tester, 1, wxEXPAND);
	panel21->AddGrowableRow(1, 1);
	panel21->AddGrowableCol(0, 1);


	right_panel->Add(panel21, 1, wxALL | wxEXPAND, 10);

	hbox1->Add(left_panel, 0);

	hbox1->Add(right_panel, 1, wxALL | wxEXPAND, 35);

	panel->SetSizer(hbox1);
	Centre();
}

void ControlsUI::OnOK(wxCommandEvent & event)
{
	isClosed = true;

	text_tester->SetFocus();
}

void ControlsUI::OnSave(wxCommandEvent & event)
{
	writeThresholds(thresholds_filename);

	text_tester->SetFocus();
}

void ControlsUI::OnRestore(wxCommandEvent & event)
{
	removeRow1();
	switch (golden_index)
	{
	case(0): {
		// ROW 1
		removeHN1();

		// ROW 2
		removeRow2();
		removeFERow20();
		removeFERow21();

		// ROW 3
		removeRow3();
		removeFERow30();
		removeFERow31();

		// ROW 4
		removeRow4();
		removeFERow40();
		removeFERow41();

		// ROW 5
		removeRow5();
		removeFERow50();
		removeFERow51();

		// ROW 6
		removeRow6();
		removeFERow60();
		removeFERow61();
	}
			 break;
	case(1): {
		// ROW 1
		removeFERow10();
		removeFERow11();

		// ROW 2
		removeRow2();
		removeHN2();

		// ROW 3
		removeRow3();
		removeFERow30();
		removeFERow31();

		// ROW 4
		removeRow4();
		removeFERow40();
		removeFERow41();

		// ROW 5
		removeRow5();
		removeFERow50();
		removeFERow51();

		// ROW 6
		removeRow6();
		removeFERow60();
		removeFERow61();
	}
			 break;
	case(2): {
		// ROW 1
		removeFERow10();
		removeFERow11();

		// ROW 2
		removeRow2();
		removeFERow20();
		removeFERow21();

		// ROW 3
		removeRow3();
		removeHN3();

		// ROW 4
		removeRow4();
		removeFERow40();
		removeFERow41();

		// ROW 5
		removeRow5();
		removeFERow50();
		removeFERow51();

		// ROW 6
		removeRow6();
		removeFERow60();
		removeFERow61();
	}
			 break;
	case(3): {
		// ROW 1
		removeFERow10();
		removeFERow11();

		// ROW 2
		removeRow2();
		removeFERow20();
		removeFERow21();

		// ROW 3
		removeRow3();
		removeFERow30();
		removeFERow31();

		// ROW 4
		removeRow4();
		removeHN4();

		// ROW 5
		removeRow5();
		removeFERow50();
		removeFERow51();

		// ROW 6
		removeRow6();
		removeFERow60();
		removeFERow61();
	}
			 break;
	case(4): {
		// ROW 1
		removeFERow10();
		removeFERow11();

		// ROW 2
		removeRow2();
		removeFERow20();
		removeFERow21();

		// ROW 3
		removeRow3();
		removeFERow30();
		removeFERow31();

		// ROW 4
		removeRow4();
		removeFERow40();
		removeFERow41();

		// ROW 5
		removeRow5();
		removeHN5();

		// ROW 6
		removeRow6();
		removeFERow60();
		removeFERow61();
	}
			 break;
	case(5): {
		// ROW 1
		removeFERow10();
		removeFERow11();

		// ROW 2
		removeRow2();
		removeFERow20();
		removeFERow21();

		// ROW 3
		removeRow3();
		removeFERow30();
		removeFERow31();

		// ROW 4
		removeRow4();
		removeFERow40();
		removeFERow41();

		// ROW 5
		removeRow5();
		removeFERow50();
		removeFERow51();

		// ROW 6
		removeRow6();
		removeHN6();
	}
			 break;
	default: {
		// ROW 1
		removeFERow10();
		removeFERow11();

		// ROW 2
		removeRow2();
		removeFERow20();
		removeFERow21();

		// ROW 3
		removeRow3();
		removeFERow30();
		removeFERow31();

		// ROW 4
		removeRow4();
		removeFERow40();
		removeFERow41();

		// ROW 5
		removeRow5();
		removeFERow50();
		removeFERow51();

		// ROW 6
		removeRow6();
		removeFERow60();
		removeFERow61();
	}
			 break;
	}
	keynumbs.erase(keynumbs.begin(), keynumbs.end());
	//removeFERow71();
	readThresholds(thresholds_filename);
	// ROW 1
	addRow1();
	panel11->Layout();
	switch (golden_index)
	{
	case(0): {
		// ROW 1
		addHNRow11();
		panel01->Layout();

		// ROW 2
		addRow2();
		panel12->Layout();
		addFERow20();
		addFERow21();
		panel02->Layout();

		// ROW 3
		addRow3();
		panel13->Layout();
		addFERow30();
		addFERow31();
		panel03->Layout();

		// ROW 4
		addRow4();
		panel14->Layout();
		addFERow40();
		addFERow41();
		panel04->Layout();

		// ROW 5
		addRow5();
		panel15->Layout();
		addFERow50();
		addFERow51();
		panel05->Layout();

		// ROW 6
		addRow6();
		panel16->Layout();
		addFERow60();
		addFERow61();
		panel06->Layout();
	}
			 break;
	case(1): {
		// ROW 1
		addFERow10();
		addFERow11();
		panel01->Layout();

		// ROW 2
		addRow2();
		panel12->Layout();
		addHNRow12();
		panel02->Layout();

		// ROW 3
		addRow3();
		panel13->Layout();
		addFERow30();
		addFERow31();
		panel03->Layout();

		// ROW 4
		addRow4();
		panel14->Layout();
		addFERow40();
		addFERow41();
		panel04->Layout();

		// ROW 5
		addRow5();
		panel15->Layout();
		addFERow50();
		addFERow51();
		panel05->Layout();

		// ROW 6
		addRow6();
		panel16->Layout();
		addFERow60();
		addFERow61();
		panel06->Layout();
	}
			 break;
	case(2): {
		// ROW 1
		addFERow10();
		addFERow11();
		panel01->Layout();

		// ROW 2
		addRow2();
		panel12->Layout();
		addFERow20();
		addFERow21();
		panel02->Layout();

		// ROW 3
		addRow3();
		panel13->Layout();
		addHNRow13();
		panel03->Layout();

		// ROW 4
		addRow4();
		panel14->Layout();
		addFERow40();
		addFERow41();
		panel04->Layout();

		// ROW 5
		addRow5();
		panel15->Layout();
		addFERow50();
		addFERow51();
		panel05->Layout();

		// ROW 6
		addRow6();
		panel16->Layout();
		addFERow60();
		addFERow61();
		panel06->Layout();
	}
			 break;
	case(3): {
		// ROW 1
		addFERow10();
		addFERow11();
		panel01->Layout();

		// ROW 2
		addRow2();
		panel12->Layout();
		addFERow20();
		addFERow21();
		panel02->Layout();

		// ROW 3
		addRow3();
		panel13->Layout();
		addFERow30();
		addFERow31();
		panel03->Layout();

		// ROW 4
		addRow4();
		panel14->Layout();
		addHNRow14();
		panel04->Layout();

		// ROW 5
		addRow5();
		panel15->Layout();
		addFERow50();
		addFERow51();
		panel05->Layout();

		// ROW 6
		addRow6();
		panel16->Layout();
		addFERow60();
		addFERow61();
		panel06->Layout();
	}
			 break;
	case(4): {
		// ROW 1
		addFERow10();
		addFERow11();
		panel01->Layout();

		// ROW 2
		addRow2();
		panel12->Layout();
		addFERow20();
		addFERow21();
		panel02->Layout();

		// ROW 3
		addRow3();
		panel13->Layout();
		addFERow30();
		addFERow31();
		panel03->Layout();

		// ROW 4
		addRow4();
		panel14->Layout();
		addFERow40();
		addFERow41();
		panel04->Layout();

		// ROW 5
		addRow5();
		panel15->Layout();
		addHNRow15();
		panel05->Layout();

		// ROW 6
		addRow6();
		panel16->Layout();
		addFERow60();
		addFERow61();
		panel06->Layout();
	}
			 break;
	case(5): {
		// ROW 1
		addFERow10();
		addFERow11();
		panel01->Layout();

		// ROW 2
		addRow2();
		panel12->Layout();
		addFERow20();
		addFERow21();
		panel02->Layout();

		// ROW 3
		addRow3();
		panel13->Layout();
		addFERow30();
		addFERow31();
		panel03->Layout();

		// ROW 4
		addRow4();
		panel14->Layout();
		addFERow40();
		addFERow41();
		panel04->Layout();

		// ROW 5
		addRow5();
		panel15->Layout();
		addFERow50();
		addFERow51();
		panel05->Layout();

		// ROW 6
		addRow6();
		panel16->Layout();
		addHNRow16();
		panel06->Layout();
	}
			 break;
	default: {
		// ROW 1
		addFERow10();
		addFERow11();
		panel01->Layout();

		// ROW 2
		addRow2();
		panel12->Layout();
		addFERow20();
		addFERow21();
		panel02->Layout();

		// ROW 3
		addRow3();
		panel13->Layout();
		addFERow30();
		addFERow31();
		panel03->Layout();

		// ROW 4
		addRow4();
		panel14->Layout();
		addFERow40();
		addFERow41();
		panel04->Layout();

		// ROW 5
		addRow5();
		panel15->Layout();
		addFERow50();
		addFERow51();
		panel05->Layout();

		// ROW 6
		addRow6();
		panel16->Layout();
		addFERow60();
		addFERow61();
		panel06->Layout();
	}
			 break;
	}
	text_tester->SetFocus();
}

void ControlsUI::Exit()
{
	Close(true);
}

void ControlsUI::OnExit(wxCommandEvent & event)
{
}

void ControlsUI::OnScroll6(wxScrollEvent & event)
{
	int au6_val = AU6_slider->GetValue();
	AU6_threshold = au6_val / 10.0f;
	AU6_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU6_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll12(wxScrollEvent & event)
{
	int au12_val = AU12_slider->GetValue();
	AU12_threshold = au12_val / 10.0f;
	AU12_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU12_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll1(wxScrollEvent & event)
{
	int au1_val = AU1_slider->GetValue();
	AU1_threshold = au1_val / 10.0f;
	AU1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU1_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll4_1(wxScrollEvent & event)
{
	int au4_1_val = AU4_1_slider->GetValue();
	AU4_1_threshold = au4_1_val / 10.0f;
	AU4_1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU4_1_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll15(wxScrollEvent & event)
{
	int au15_val = AU15_slider->GetValue();
	AU15_threshold = au15_val / 10.0f;
	AU15_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU15_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll9(wxScrollEvent & event)
{
	int au9_val = AU9_slider->GetValue();
	AU9_threshold = au9_val / 10.0f;
	AU9_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU9_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll10(wxScrollEvent & event)
{
	int au10_val = AU10_slider->GetValue();
	AU10_threshold = au10_val / 10.0f;
	AU10_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU10_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll1_1(wxScrollEvent & event)
{
	int au1_1_val = AU1_1_slider->GetValue();
	AU1_1_threshold = au1_1_val / 10.0f;
	AU1_1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU1_1_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll2(wxScrollEvent & event)
{
	int au2_val = AU2_slider->GetValue();
	AU2_threshold = au2_val / 10.0f;
	AU2_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU2_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll5(wxScrollEvent & event)
{
	int au5_val = AU5_slider->GetValue();
	AU5_threshold = au5_val / 10.0f;
	AU5_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU5_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll7(wxScrollEvent & event)
{
	int au7_val = AU7_slider->GetValue();
	AU7_threshold = au7_val / 10.0f;
	AU7_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU7_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll23(wxScrollEvent & event)
{
	int au23_val = AU23_slider->GetValue();
	AU23_threshold = au23_val / 10.0f;
	AU23_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU23_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll4_2(wxScrollEvent & event)
{
	int au4_2_val = AU4_2_slider->GetValue();
	AU4_2_threshold = au4_2_val / 10.0f;
	AU4_2_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU4_2_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll25(wxScrollEvent & event)
{
	int au25_val = AU25_slider->GetValue();
	AU25_threshold = au25_val / 10.0f;
	AU25_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU25_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnScroll26(wxScrollEvent & event)
{
	int au26_val = AU26_slider->GetValue();
	AU26_threshold = au26_val / 10.0f;
	AU26_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU26_threshold));
	text_tester->SetFocus();
}

void ControlsUI::OnHNScroll(wxScrollEvent & event)
{
	angles_diff_threshold = hn_slider->GetValue();
	hn_slider_value->SetLabel(wxString::Format(wxT("%02d"), angles_diff_threshold));

	text_tester->SetFocus();
}

void ControlsUI::OnRB1Select(wxCommandEvent & event)
{
	if (key1_presence_rb->GetValue()) {
		AU6_slider->Enable(false);
		AU12_slider->Enable(false);
		is_intensity_arr[0] = 0;
		K1_presence = true;
	}
	else {
		AU6_slider->Enable(true);
		AU12_slider->Enable(true);
		is_intensity_arr[0] = 1;
		K1_presence = false;
	}
	text_tester->SetFocus();
}

void ControlsUI::OnRB2Select(wxCommandEvent & event)
{
	if (key2_presence_rb->GetValue()) {
		AU1_slider->Enable(false);
		AU4_1_slider->Enable(false);
		AU15_slider->Enable(false);
		is_intensity_arr[2] = 0;
		K2_presence = true;
	}
	else {
		AU1_slider->Enable(true);
		AU4_1_slider->Enable(true);
		AU15_slider->Enable(true);
		is_intensity_arr[2] = 1;
		K2_presence = false;
	}
	text_tester->SetFocus();
}

void ControlsUI::OnRB3Select(wxCommandEvent & event)
{
	if (key3_presence_rb->GetValue()) {
		AU9_slider->Enable(false);
		AU10_slider->Enable(false);
		is_intensity_arr[5] = 0;
		K3_presence = true;
	}
	else {
		AU9_slider->Enable(true);
		AU10_slider->Enable(true);
		is_intensity_arr[5] = 1;
		K3_presence = false;
	}
	text_tester->SetFocus();
}

void ControlsUI::OnRB4Select(wxCommandEvent & event)
{
	if (key4_presence_rb->GetValue()) {
		AU1_1_slider->Enable(false);
		AU2_slider->Enable(false);
		AU5_slider->Enable(false);
		is_intensity_arr[7] = 0;
		K4_presence = true;
	}
	else {
		AU1_1_slider->Enable(true);
		AU2_slider->Enable(true);
		AU5_slider->Enable(true);
		is_intensity_arr[7] = 1;
		K4_presence = false;
	}
	text_tester->SetFocus();
}

void ControlsUI::OnRB5Select(wxCommandEvent & event)
{
	if (key5_presence_rb->GetValue()) {
		AU7_slider->Enable(false);
		AU23_slider->Enable(false);
		is_intensity_arr[10] = 0;
		K5_presence = true;
	}
	else {
		AU7_slider->Enable(true);
		AU23_slider->Enable(true);
		is_intensity_arr[10] = 1;
		K5_presence = false;
	}
	text_tester->SetFocus();
}

void ControlsUI::OnRB6Select(wxCommandEvent & event)
{
	if (key6_presence_rb->GetValue()) {
		AU4_2_slider->Enable(false);
		AU25_slider->Enable(false);
		AU26_slider->Enable(false);
		is_intensity_arr[12] = 0;
		K6_presence = true;
	}
	else {
		AU4_2_slider->Enable(true);
		AU25_slider->Enable(true);
		AU26_slider->Enable(true);
		is_intensity_arr[12] = 1;
		K6_presence = false;
	}
	text_tester->SetFocus();
}

void ControlsUI::OnCB1Select(wxCommandEvent & event)
{
	if (ComboBox1->GetValue() == "Head Nodding") {
		golden_index = 0;
		if (tracking_modes[0] == 0) {
			// tempSave();
			if (tracking_modes[1] == 1) {
				ComboBox2->SetValue("Facial Expression");
				removeHN2();
				addFERow20();
				addFERow21();
				// openTempSaved();
				panel02->Layout();
			}
			else if (tracking_modes[2] == 1) {
				ComboBox3->SetValue("Facial Expression");
				removeHN3();
				addFERow30();
				addFERow31();
				// openTempSaved();
				panel03->Layout();
			}
			else if (tracking_modes[3] == 1) {
				ComboBox4->SetValue("Facial Expression");
				removeHN4();
				addFERow40();
				addFERow41();
				// openTempSaved();
				panel04->Layout();
			}
			else if (tracking_modes[4] == 1) {
				ComboBox5->SetValue("Facial Expression");
				removeHN5();
				addFERow50();
				addFERow51();
				openTempSaved();
				// panel05->Layout();
			}
			else if (tracking_modes[5] == 1) {
				ComboBox6->SetValue("Facial Expression");
				removeHN6();
				addFERow60();
				addFERow61();
				// openTempSaved();
				panel06->Layout();
			}
			removeFERow10();
			removeFERow11();
			addHNRow11();
			panel01->Layout();
		}
	}
	else {
		golden_index = -1;
		if (tracking_modes[0] == 1) {
			removeHN1();
			addFERow10();
			addFERow11();
			// openTempSaved();
			panel01->Layout();
		}
	}
	Refresh();

	text_tester->SetFocus();
}

void ControlsUI::OnCB2Select(wxCommandEvent & event)
{
	if (ComboBox2->GetValue() == "Head Nodding") {
		golden_index = 1;
		if (tracking_modes[1] == 0) {
			// tempSave();
			if (tracking_modes[0] == 1) {
				std::cout << "I AM HERE, HEEEEEEEEY!!!!!!!\n";
				ComboBox1->SetValue("Facial Expression");
				removeHN1();
				addFERow10();
				addFERow11();
				// openTempSaved();
				// panel11->Layout();
				panel01->Layout();
			}
			else if (tracking_modes[2] == 1) {
				ComboBox3->SetValue("Facial Expression");
				removeHN3();
				addFERow30();
				addFERow31();
				// openTempSaved();
				panel03->Layout();
			}
			else if (tracking_modes[3] == 1) {
				ComboBox4->SetValue("Facial Expression");
				removeHN4();
				addFERow40();
				addFERow41();
				// openTempSaved();
				panel04->Layout();
			}
			else if (tracking_modes[4] == 1) {
				ComboBox5->SetValue("Facial Expression");
				removeHN5();
				addFERow50();
				addFERow51();
				// openTempSaved();
				panel05->Layout();
			}
			else if (tracking_modes[5] == 1) {
				ComboBox6->SetValue("Facial Expression");
				removeHN6();
				addFERow60();
				addFERow61();
				// openTempSaved();
				panel06->Layout();
			}
			removeFERow20();
			removeFERow21();
			addHNRow12();
			panel02->Layout();
		}
	}
	else {
		golden_index = -1;
		if (tracking_modes[1] == 1) {
			removeHN2();
			addFERow20();
			addFERow21();
			// openTempSaved();
			panel02->Layout();
		}
	}
	Refresh();

	text_tester->SetFocus();
}

void ControlsUI::OnCB3Select(wxCommandEvent & event)
{
	if (ComboBox3->GetValue() == "Head Nodding") {
		golden_index = 2;
		if (tracking_modes[2] == 0) {
			// tempSave();
			if (tracking_modes[0] == 1) {
				ComboBox1->SetValue("Facial Expression");
				removeHN1();
				addFERow10();
				addFERow11();
				// openTempSaved();
				panel01->Layout();
			}
			else if (tracking_modes[1] == 1) {
				ComboBox2->SetValue("Facial Expression");
				removeHN2();
				addFERow20();
				addFERow21();
				// openTempSaved();
				panel02->Layout();
			}
			else if (tracking_modes[3] == 1) {
				ComboBox4->SetValue("Facial Expression");
				removeHN4();
				addFERow40();
				addFERow41();
				// openTempSaved();
				panel04->Layout();
			}
			else if (tracking_modes[4] == 1) {
				ComboBox5->SetValue("Facial Expression");
				removeHN5();
				addFERow50();
				addFERow51();
				// openTempSaved();
				panel05->Layout();
			}
			else if (tracking_modes[5] == 1) {
				ComboBox6->SetValue("Facial Expression");
				removeHN6();
				addFERow60();
				addFERow61();
				// openTempSaved();
				panel06->Layout();
			}
			removeFERow30();
			removeFERow31();
			addHNRow13();
			panel03->Layout();
		}
	}
	else {
		golden_index = -1;
		if (tracking_modes[2] == 1) {
			removeHN3();
			addFERow30();
			addFERow31();
			// openTempSaved();
			panel03->Layout();
		}
	}
	Refresh();

	text_tester->SetFocus();
}

void ControlsUI::OnCB4Select(wxCommandEvent & event)
{
	if (ComboBox4->GetValue() == "Head Nodding") {
		golden_index = 3;
		if (tracking_modes[3] == 0) {
			// tempSave();
			if (tracking_modes[0] == 1) {
				ComboBox1->SetValue("Facial Expression");
				removeHN1();
				addFERow10();
				addFERow11();
				// openTempSaved();
				panel01->Layout();
			}
			else if (tracking_modes[1] == 1) {
				ComboBox2->SetValue("Facial Expression");
				removeHN2();
				addFERow20();
				addFERow21();
				// openTempSaved();
				panel02->Layout();
			}
			else if (tracking_modes[2] == 1) {
				ComboBox3->SetValue("Facial Expression");
				removeHN3();
				addFERow30();
				addFERow31();
				// openTempSaved();
				panel03->Layout();
			}
			else if (tracking_modes[4] == 1) {
				ComboBox5->SetValue("Facial Expression");
				removeHN5();
				addFERow50();
				addFERow51();
				// openTempSaved();
				panel05->Layout();
			}
			else if (tracking_modes[5] == 1) {
				ComboBox6->SetValue("Facial Expression");
				removeHN6();
				addFERow60();
				addFERow61();
				// openTempSaved();
				panel06->Layout();
			}
			removeFERow40();
			removeFERow41();
			addHNRow14();
			panel04->Layout();
		}
	}
	else {
		golden_index = -1;
		if (tracking_modes[3] == 1) {
			removeHN4();
			addFERow40();
			addFERow41();
			// openTempSaved();
			panel04->Layout();
		}
	}
	Refresh();

	text_tester->SetFocus();
}

void ControlsUI::OnCB5Select(wxCommandEvent & event)
{
	if (ComboBox5->GetValue() == "Head Nodding") {
		golden_index = 4;
		if (tracking_modes[4] == 0) {
			// tempSave();
			if (tracking_modes[0] == 1) {
				ComboBox1->SetValue("Facial Expression");
				removeHN1();
				addFERow10();
				addFERow11();
				// openTempSaved();
				panel01->Layout();
			}
			else if (tracking_modes[1] == 1) {
				ComboBox2->SetValue("Facial Expression");
				removeHN2();
				addFERow20();
				addFERow21();
				// openTempSaved();
				panel02->Layout();
			}
			else if (tracking_modes[2] == 1) {
				ComboBox3->SetValue("Facial Expression");
				removeHN3();
				addFERow30();
				addFERow31();
				// openTempSaved();
				panel03->Layout();
			}
			else if (tracking_modes[3] == 1) {
				ComboBox4->SetValue("Facial Expression");
				removeHN4();
				addFERow40();
				addFERow41();
				// openTempSaved();
				panel04->Layout();
			}
			else if (tracking_modes[5] == 1) {
				ComboBox6->SetValue("Facial Expression");
				removeHN6();
				addFERow60();
				addFERow61();
				// openTempSaved();
				panel06->Layout();
			}
			removeFERow50();
			removeFERow51();
			addHNRow15();
			panel05->Layout();
		}
	}
	else {
		golden_index = -1;
		if (tracking_modes[4] == 1) {
			removeHN5();
			addFERow50();
			addFERow51();
			// openTempSaved();
			panel05->Layout();
		}
	}
	Refresh();

	text_tester->SetFocus();
}

void ControlsUI::OnCB6Select(wxCommandEvent & event)
{
	if (ComboBox6->GetValue() == "Head Nodding") {
		golden_index = 5;
		if (tracking_modes[5] == 0) {
			// tempSave();
			if (tracking_modes[0] == 1) {
				ComboBox1->SetValue("Facial Expression");
				removeHN1();
				addFERow10();
				addFERow11();
				// openTempSaved();
				panel01->Layout();
			}
			else if (tracking_modes[1] == 1) {
				ComboBox2->SetValue("Facial Expression");
				removeHN2();
				addFERow20();
				addFERow21();
				// openTempSaved();
				panel06->Layout();
			}
			else if (tracking_modes[2] == 1) {
				ComboBox3->SetValue("Facial Expression");
				removeHN3();
				addFERow30();
				addFERow31();
				// openTempSaved();
				panel03->Layout();
			}
			else if (tracking_modes[3] == 1) {
				ComboBox4->SetValue("Facial Expression");
				removeHN4();
				addFERow40();
				addFERow41();
				// openTempSaved();
				panel04->Layout();
			}
			else if (tracking_modes[4] == 1) {
				ComboBox5->SetValue("Facial Expression");
				removeHN5();
				addFERow50();
				addFERow51();
				// openTempSaved();
				panel05->Layout();
			}
			removeFERow60();
			removeFERow61();
			addHNRow16();
			panel06->Layout();
		}
	}
	else {
		golden_index = -1;
		if (tracking_modes[5] == 1) {
			removeHN6();
			addFERow60();
			addFERow61();
			// openTempSaved();
			panel06->Layout();
		}
	}
	Refresh();

	text_tester->SetFocus();
}

void ControlsUI::readThresholds(const std::string filename)
{
	std::string line;
	std::ifstream infile(filename);
	std::array<float, THRESHOLDS_NUMBER> thresholds_arr;
	std::array<int, THRESHOLDS_NUMBER> modes_arr;
	int threshold_cnt = 0;

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		float a;
		bool b;
		int c;
		if (!(iss >> a >> b >> c)) { 
			break; 
		}
		if (threshold_cnt < 15) {
			thresholds_arr[threshold_cnt] = a;
			is_intensity_arr[threshold_cnt] = b;
			modes_arr[threshold_cnt] = c;
		}
		else {
			angles_diff_threshold = (int) a;
		}
		threshold_cnt++;
	}
	size_t indices_arr[] = { 0, 2, 5, 7, 10, 12 };

	for (size_t i = 0; i < (sizeof(tracking_modes) / sizeof(tracking_modes[0])); i++) {
		tracking_modes[i] = modes_arr[indices_arr[i]];
		if (tracking_modes[i]) {
			golden_index = i;
		}
	}
	

	AU6_threshold = thresholds_arr[0];
	AU12_threshold = thresholds_arr[1];
	AU1_threshold = thresholds_arr[2];
	AU4_1_threshold = thresholds_arr[3];
	AU15_threshold = thresholds_arr[4];
	AU9_threshold = thresholds_arr[5];
	AU10_threshold = thresholds_arr[6];
	AU1_1_threshold = thresholds_arr[7];
	AU2_threshold = thresholds_arr[8];
	AU5_threshold = thresholds_arr[9];
	AU7_threshold = thresholds_arr[10];
	AU23_threshold = thresholds_arr[11];
	AU4_2_threshold = thresholds_arr[12];
	AU25_threshold = thresholds_arr[13];
	AU26_threshold = thresholds_arr[14];

	/*AU6_slider->SetValue((int)(AU6_threshold * 10));
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

	AU1_1_slider->SetValue((int)(AU1_1_threshold * 10));
	AU1_1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU1_1_threshold));

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
	AU12_slider->Enable(is_intensity_arr[0]);
	K1_presence = !is_intensity_arr[0];
	key1_presence_rb->SetValue(!is_intensity_arr[0]);
	key1_intensity_rb->SetValue(is_intensity_arr[0]);

	AU1_slider->Enable(is_intensity_arr[2]);
	AU4_1_slider->Enable(is_intensity_arr[2]);
	AU15_slider->Enable(is_intensity_arr[2]);
	K2_presence = !is_intensity_arr[2];
	key2_presence_rb->SetValue(!is_intensity_arr[2]);
	key2_intensity_rb->SetValue(is_intensity_arr[2]);

	AU9_slider->Enable(is_intensity_arr[5]);
	AU10_slider->Enable(is_intensity_arr[5]);
	K3_presence = !is_intensity_arr[5];
	key3_presence_rb->SetValue(!is_intensity_arr[5]);
	key3_intensity_rb->SetValue(is_intensity_arr[5]);

	AU1_1_slider->Enable(is_intensity_arr[7]);
	AU2_slider->Enable(is_intensity_arr[7]);
	AU5_slider->Enable(is_intensity_arr[7]);
	K4_presence = !is_intensity_arr[7];
	key4_presence_rb->SetValue(!is_intensity_arr[7]);
	key4_intensity_rb->SetValue(is_intensity_arr[7]);

	AU7_slider->Enable(is_intensity_arr[10]);
	AU23_slider->Enable(is_intensity_arr[10]);
	K5_presence = !is_intensity_arr[10];
	key5_presence_rb->SetValue(!is_intensity_arr[10]);
	key5_intensity_rb->SetValue(is_intensity_arr[10]);

	AU4_2_slider->Enable(is_intensity_arr[12]);
	AU25_slider->Enable(is_intensity_arr[12]);
	AU26_slider->Enable(is_intensity_arr[12]);
	K6_presence = !is_intensity_arr[12];
	key6_presence_rb->SetValue(!is_intensity_arr[12]);
	key6_intensity_rb->SetValue(is_intensity_arr[12]);
	
	switch (golden_index)
	{
		case(0): {
			removeFERow10();
			removeFERow11();
			addHNRow11();
			ComboBox1->SetValue("Head Nodding");
			panel01->Layout();
		}
			break;
		case(1): {
			removeFERow20();
			removeFERow21();
			addHNRow12();
			ComboBox2->SetValue("Head Nodding");
			panel02->Layout();
		}
			break;
		case(2): {
			removeFERow30();
			removeFERow31();
			addHNRow13();
			ComboBox3->SetValue("Head Nodding");
			panel03->Layout();
		}
			break;
		case(3): {
			removeFERow40();
			removeFERow41();
			addHNRow14();
			ComboBox4->SetValue("Head Nodding");
			panel04->Layout();
		}
			break;
		case(4): {
			removeFERow50();
			removeFERow51();
			addHNRow15();
			ComboBox5->SetValue("Head Nodding");
			panel05->Layout();
		}
			break;
		case(5): {
			removeFERow60();
			removeFERow61();
			addHNRow16();
			ComboBox6->SetValue("Head Nodding");
			panel06->Layout();
		}
			break;
		default:
			break;
		} */
	}

void ControlsUI::writeThresholds(const std::string filename)
{
	std::ofstream outfile(filename);

	outfile << AU6_threshold << " " << key1_intensity_rb->GetValue() << " " << tracking_modes[0] << "\n";
	outfile << AU12_threshold << " " << key1_intensity_rb->GetValue() << " " << tracking_modes[0] << "\n";
	outfile << AU1_threshold << " " << key2_intensity_rb->GetValue() << " " << tracking_modes[1] << "\n";
	outfile << AU4_1_threshold << " " << key2_intensity_rb->GetValue() << " " << tracking_modes[1] << "\n";
	outfile << AU15_threshold << " " << key2_intensity_rb->GetValue() << " " << tracking_modes[1] << "\n";
	outfile << AU9_threshold << " " << key3_intensity_rb->GetValue() << " " << tracking_modes[2] << "\n";
	outfile << AU10_threshold << " " << key3_intensity_rb->GetValue() << " " << tracking_modes[2] << "\n";
	outfile << AU1_1_threshold << " " << key4_intensity_rb->GetValue() << " " << tracking_modes[3] << "\n";
	outfile << AU2_threshold << " " << key4_intensity_rb->GetValue() << " " << tracking_modes[3] << "\n";
	outfile << AU5_threshold << " " << key4_intensity_rb->GetValue() << " " << tracking_modes[3] << "\n";
	outfile << AU7_threshold << " " << key5_intensity_rb->GetValue() << " " << tracking_modes[4] << "\n";
	outfile << AU23_threshold << " " << key5_intensity_rb->GetValue() << " " << tracking_modes[4] << "\n";
	outfile << AU4_2_threshold << " " << key6_intensity_rb->GetValue() << " " << tracking_modes[5] << "\n";
	outfile << AU25_threshold << " " << key6_intensity_rb->GetValue() << " " << tracking_modes[5] << "\n";
	outfile << AU26_threshold << " " << key6_intensity_rb->GetValue() << " " << tracking_modes[5] << "\n";
	outfile << angles_diff_threshold << " " << 0 << " " << 0 << "\n";

	// Close the file
	outfile.close();
}

void ControlsUI::tempSave()
{
	key_intensities[0] = key1_intensity_rb->GetValue();
	key_intensities[1] = key2_intensity_rb->GetValue();
	key_intensities[2] = key3_intensity_rb->GetValue();
	key_intensities[3] = key4_intensity_rb->GetValue();
	key_intensities[4] = key5_intensity_rb->GetValue();
	key_intensities[5] = key6_intensity_rb->GetValue();
}

void ControlsUI::openTempSaved()
{
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

	AU1_1_slider->SetValue((int)(AU1_1_threshold * 10));
	AU1_1_slider_value->SetLabel(wxString::Format(wxT("%.1f"), AU1_1_threshold));

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

	AU6_slider->Enable(key_intensities[0]);
	AU12_slider->Enable(key_intensities[0]);
	K1_presence = !key_intensities[0];
	key1_presence_rb->SetValue(!key_intensities[0]);
	key1_intensity_rb->SetValue(key_intensities[0]);

	AU1_slider->Enable(key_intensities[1]);
	AU4_1_slider->Enable(key_intensities[1]);
	AU15_slider->Enable(key_intensities[1]);
	K2_presence = !key_intensities[1];
	key2_presence_rb->SetValue(!key_intensities[1]);
	key2_intensity_rb->SetValue(key_intensities[1]);

	AU9_slider->Enable(key_intensities[2]);
	AU10_slider->Enable(key_intensities[2]);
	K3_presence = !key_intensities[2];
	key3_presence_rb->SetValue(!key_intensities[2]);
	key3_intensity_rb->SetValue(key_intensities[2]);

	AU1_1_slider->Enable(key_intensities[3]);
	AU2_slider->Enable(key_intensities[3]);
	AU5_slider->Enable(key_intensities[3]);
	K4_presence = !key_intensities[3];
	key4_presence_rb->SetValue(!key_intensities[3]);
	key4_intensity_rb->SetValue(key_intensities[3]);

	AU7_slider->Enable(key_intensities[4]);
	AU23_slider->Enable(key_intensities[4]);
	K5_presence = !key_intensities[4];
	key5_presence_rb->SetValue(!key_intensities[4]);
	key5_intensity_rb->SetValue(key_intensities[4]);

	AU4_2_slider->Enable(key_intensities[5]);
	AU25_slider->Enable(key_intensities[5]);
	AU26_slider->Enable(key_intensities[5]);
	K6_presence = !key_intensities[5];
	key6_presence_rb->SetValue(!key_intensities[5]);
	key6_intensity_rb->SetValue(key_intensities[5]);
}

void ControlsUI::addRow1()
{
	wxStaticText *txt1 = new wxStaticText(panel, -1, wxT("Key 1"));
	txt1->SetForegroundColour("#F6F7DD");
	wxFont font = txt1->GetFont();
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	txt1->SetFont(font);
	keynumbs.push_back(txt1);
	panel11->Add(keynumbs[0], 0);

	m_arrItems1 = new wxArrayString;

	m_arrItems1->Add(wxT("Facial Expression"));
	m_arrItems1->Add(wxT("Head Nodding"));
	
	/*wxString *m_arrItems = new wxString;
	m_arrItems[0] = wxT("Facial Expression");
	m_arrItems[0] = wxT("Facial Expression");
	m_arrItems[size_t(tracking_modes[0])]
	wxString *m_arrItems = new wxString;
	m_arrItems[0] = wxT("Facial Expression");
	m_arrItems[1] = wxT("Head Nodding");*/

	ComboBox1 = new wxComboBox(panel, CB1_ID, m_arrItems1[0][size_t(tracking_modes[0])],
		wxDefaultPosition, wxDefaultSize, *m_arrItems1, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
	//ComboBox1->SetValue(cstr1[0]);

	Connect(CB1_ID, wxEVT_COMBOBOX,	wxCommandEventHandler(ControlsUI::OnCB1Select));
	panel11->Add(ComboBox1, 1);


}

void ControlsUI::addFERow10()
{
	tracking_modes[0] = 0;
	key1 = new wxStaticText(panel, -1, wxT("Happiness:"));
	key1->SetForegroundColour(*text_color);
	wxFont font = key1->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	key1->SetFont(font);

	panel01->Add(key1, 1, wxEXPAND);

	size_t sz = empty_static_texts1.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts1.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS1; i++) {
		panel01->Add(empty_static_texts1[i], 0);
	}

	
}

void ControlsUI::addFERow11()
{
	key1_presence_rb = new wxRadioButton(panel, K1_ID_presence_RB,
		wxT("Presence"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	key1_presence_rb->SetForegroundColour(*text_color);
	wxFont font = key1_presence_rb->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	key1_presence_rb->SetFont(font);
	Connect(K1_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
		wxCommandEventHandler(ControlsUI::OnRB1Select));

	key1_intensity_rb = new wxRadioButton(panel, K1_ID_intensity_RB,
		wxT("Intensity", wxDefaultPosition, wxDefaultSize));
	key1_intensity_rb->SetForegroundColour(*text_color);
	key1_intensity_rb->SetFont(font);
	Connect(K1_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
		wxCommandEventHandler(ControlsUI::OnRB1Select));

	AU6 = new wxStaticText(panel, -1, wxT("AU6 (Cheek Raiser)"));
	AU6->SetForegroundColour(*text_color);
	AU6->SetFont(font);

	AU6_slider = new wxSlider(panel, AU6_ID_SLIDER, (int)(AU6_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	//AU6_slider->SetForegroundColour("#9C56BA");
	Connect(AU6_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll6));

	AU6_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU6_threshold));
	AU6_slider_value->SetForegroundColour(*text_color);
	AU6_slider_value->SetFont(font);

	AU12 = new wxStaticText(panel, -1, wxT("AU12 (Lip Corner Puller)"));
	AU12->SetForegroundColour(*text_color);
	AU12->SetFont(font);

	AU12_slider = new wxSlider(panel, AU12_ID_SLIDER, (int)(AU12_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU12_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll12));

	AU12_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU12_threshold));
	AU12_slider_value->SetForegroundColour(*text_color);
	AU12_slider_value->SetFont(font);
	AU6_slider->Enable(is_intensity_arr[0]);
	AU12_slider->Enable(is_intensity_arr[0]);
	K1_presence = !is_intensity_arr[0];
	key1_presence_rb->SetValue(!is_intensity_arr[0]);
	key1_intensity_rb->SetValue(is_intensity_arr[0]);

	panel01->Add(key1_presence_rb, 1, wxEXPAND);
	panel01->Add(key1_intensity_rb, 1, wxEXPAND);
	panel01->Add(AU6, 0, wxEXPAND);
	panel01->Add(AU6_slider, 1, wxEXPAND);
	panel01->Add(AU6_slider_value, 1, wxEXPAND);
	panel01->Add(AU12, 0, wxEXPAND);
	panel01->Add(AU12_slider, 1, wxEXPAND);
	panel01->Add(AU12_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts1.size();
	for (size_t i = sz; i < sz + STATIC_TEXT_NUMBS2; i++) {
		empty_static_texts1.push_back(new wxStaticText(panel, -1, wxT("")));
	}

	for (size_t i = sz; i < sz + STATIC_TEXT_NUMBS2; i++) {
		panel01->Add(empty_static_texts1[i], 0, wxEXPAND);
	}
}

void ControlsUI::addRow2()
{
	wxStaticText *txt = new wxStaticText(panel, -1, wxT("Key 2"));
	txt->SetForegroundColour("#F6F7DD");
	wxFont font = txt->GetFont();
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	txt->SetFont(font);
	keynumbs.push_back(txt);
	panel12->Add(keynumbs[1], 0);


	wxArrayString m_arrItems;

	m_arrItems.Add(wxT("Facial Expression"));
	m_arrItems.Add(wxT("Head Nodding"));

	ComboBox2 = new wxComboBox(panel, CB2_ID, m_arrItems[size_t(tracking_modes[1])],
		wxDefaultPosition, wxDefaultSize, m_arrItems, 0, wxDefaultValidator, _T("ID_COMBOBOX2"));
	Connect(CB2_ID, wxEVT_COMBOBOX, wxCommandEventHandler(ControlsUI::OnCB2Select));
	panel12->Add(ComboBox2, 1);

}

void ControlsUI::addFERow20()
{
	tracking_modes[1] = 0;
	key2 = new wxStaticText(panel, -1, wxT("Sadness:"));
	key2->SetForegroundColour(*text_color);
	font = key2->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	key2->SetFont(font);

	panel02->Add(key2, 1, wxEXPAND);


	size_t sz = empty_static_texts2.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts2.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS1; i++) {
		panel02->Add(empty_static_texts2[i], 0);
	}

}

void ControlsUI::addFERow21()
{
	key2_presence_rb = new wxRadioButton(panel, K2_ID_presence_RB,
		wxT("Presence"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	key2_presence_rb->SetForegroundColour(*text_color);
	wxFont font = key2_presence_rb->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	key2_presence_rb->SetFont(font);
	Connect(K2_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB2Select));
	key2_intensity_rb = new wxRadioButton(panel, K2_ID_intensity_RB,
		wxT("Intensity"));
	key2_intensity_rb->SetForegroundColour(*text_color);
	key2_intensity_rb->SetFont(font);
	Connect(K2_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB2Select));

	AU1 = new wxStaticText(panel, -1, wxT("AU1 (Inner Brow Raiser)"));
	AU1->SetForegroundColour(*text_color);
	AU1->SetFont(font);
	AU1_slider = new wxSlider(panel, AU1_ID_SLIDER, (int)(AU1_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU1_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll1));
	AU1_slider->Enable(false);

	AU1_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU1_threshold));
	AU1_slider_value->SetForegroundColour(*text_color);
	AU1_slider_value->SetFont(font);

	AU4_1 = new wxStaticText(panel, -1, wxT("AU4 (Brow Lowerer)"));
	AU4_1->SetForegroundColour(*text_color);
	AU4_1->SetFont(font);
	AU4_1_slider = new wxSlider(panel, AU4_1_ID_SLIDER, (int)(AU4_1_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU4_1_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll4_1));
	AU4_1_slider->Enable(false);

	AU4_1_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU4_1_threshold));
	AU4_1_slider_value->SetForegroundColour(*text_color);
	AU4_1_slider_value->SetFont(font);

	AU15 = new wxStaticText(panel, -1, wxT("AU15 (Lip Corner Depressor)"));
	AU15->SetForegroundColour(*text_color);
	AU15->SetFont(font);
	AU15_slider = new wxSlider(panel, AU15_ID_SLIDER, (int)(AU15_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU15_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll15));
	AU15_slider->Enable(false);

	AU15_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU15_threshold));
	AU15_slider_value->SetForegroundColour(*text_color);
	AU15_slider_value->SetFont(font);

	AU1_slider->Enable(is_intensity_arr[2]);
	AU4_1_slider->Enable(is_intensity_arr[2]);
	AU15_slider->Enable(is_intensity_arr[2]);
	K2_presence = !is_intensity_arr[2];
	key2_presence_rb->SetValue(!is_intensity_arr[2]);
	key2_intensity_rb->SetValue(is_intensity_arr[2]);

	panel02->Add(key2_presence_rb, 1, wxEXPAND);
	panel02->Add(key2_intensity_rb, 1, wxEXPAND);
	panel02->Add(AU1, 0, wxEXPAND);
	panel02->Add(AU1_slider, 1, wxEXPAND);
	panel02->Add(AU1_slider_value, 1, wxEXPAND);
	panel02->Add(AU4_1, 0, wxEXPAND);
	panel02->Add(AU4_1_slider, 1, wxEXPAND);
	panel02->Add(AU4_1_slider_value, 1, wxEXPAND);
	panel02->Add(AU15, 0, wxEXPAND);
	panel02->Add(AU15_slider, 1, wxEXPAND);
	panel02->Add(AU15_slider_value, 1, wxEXPAND);
}

void ControlsUI::addRow3()
{
	wxStaticText *txt = new wxStaticText(panel, -1, wxT("Key 3"));
	txt->SetForegroundColour("#F6F7DD");
	keynumbs.push_back(txt);
	panel13->Add(keynumbs[2], 0);
	wxFont font = keynumbs[2]->GetFont();
	// std::cout << font.GetPointSize() << "\n";
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	keynumbs[2]->SetFont(font);

	wxArrayString m_arrItems;

	// Create common wxArrayString array
	m_arrItems.Add(wxT("Facial Expression"));
	m_arrItems.Add(wxT("Head Nodding"));
	//m_arrItems.Add(wxT("Dot"));

	ComboBox3 = new wxComboBox(panel, CB3_ID, m_arrItems[size_t(tracking_modes[2])],
		wxDefaultPosition, wxDefaultSize, m_arrItems, 0, wxDefaultValidator, _T("ID_COMBOBOX3"));
	Connect(CB3_ID, wxEVT_COMBOBOX, wxCommandEventHandler(ControlsUI::OnCB3Select));
	panel13->Add(ComboBox3, 1);

}

void ControlsUI::addFERow30()
{
	tracking_modes[2] = 0;
	key3 = new wxStaticText(panel, -1, wxT("Disgust:"));
	key3->SetForegroundColour(*text_color);
	font = key3->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	key3->SetFont(font);

	panel03->Add(key3, 1, wxEXPAND);
	
	size_t sz = empty_static_texts3.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts3.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS1; i++) {
		panel03->Add(empty_static_texts3[i], 0);
	}


}

void ControlsUI::addFERow31()
{
	key3_presence_rb = new wxRadioButton(panel, K3_ID_presence_RB,
		wxT("Presence"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	key3_presence_rb->SetForegroundColour(*text_color);
	Connect(K3_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB3Select));
	key3_intensity_rb = new wxRadioButton(panel, K3_ID_intensity_RB,
		wxT("Intensity"));
	key3_intensity_rb->SetForegroundColour(*text_color);
	Connect(K3_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB3Select));
	//key3_intensity_rb->SetValue(true);

	AU9 = new wxStaticText(panel, -1, wxT("AU9 (Nose Wrinkler)"));
	AU9->SetForegroundColour(*text_color);
	AU9_slider = new wxSlider(panel, AU9_ID_SLIDER, (int)(AU9_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU9_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll9));

	AU9_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU9_threshold));
	AU9_slider_value->SetForegroundColour(*text_color);

	AU10 = new wxStaticText(panel, -1, wxT("AU10 (Upper Lip Raiser)"));
	AU10->SetForegroundColour(*text_color);
	AU10_slider = new wxSlider(panel, AU10_ID_SLIDER, (int)(AU10_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU10_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll10));

	AU10_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU10_threshold));
	AU10_slider_value->SetForegroundColour(*text_color);

	AU9_slider->Enable(is_intensity_arr[5]);
	AU10_slider->Enable(is_intensity_arr[5]);
	K3_presence = !is_intensity_arr[5];
	key3_presence_rb->SetValue(!is_intensity_arr[5]);
	key3_intensity_rb->SetValue(is_intensity_arr[5]);

	panel03->Add(key3_presence_rb, 1, wxEXPAND);
	panel03->Add(key3_intensity_rb, 1, wxEXPAND);
	panel03->Add(AU9, 0, wxEXPAND);
	panel03->Add(AU9_slider, 1, wxEXPAND);
	panel03->Add(AU9_slider_value, 1, wxEXPAND);
	panel03->Add(AU10, 0, wxEXPAND);
	panel03->Add(AU10_slider, 1, wxEXPAND);
	panel03->Add(AU10_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts3.size();
	for (size_t i = sz; i < sz + STATIC_TEXT_NUMBS2; i++) {
		empty_static_texts3.push_back(new wxStaticText(panel, -1, wxT("")));
	}

	for (size_t i = sz; i < sz + STATIC_TEXT_NUMBS2; i++) {
		panel03->Add(empty_static_texts3[i], 0, wxEXPAND);
	}
}

void ControlsUI::addRow4()
{
	wxStaticText *txt = new wxStaticText(panel, -1, wxT("Key 4"));
	txt->SetForegroundColour("#F6F7DD");

	wxFont font = txt->GetFont();
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	txt->SetFont(font);
	keynumbs.push_back(txt);
	panel14->Add(keynumbs[3], 0);

	
	/*keynumbs.push_back(txt);
	panel14->Add(keynumbs[3], 0);
	wxFont font = keynumbs[3]->GetFont();
	// std::cout << font.GetPointSize() << "\n";
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	keynumbs[3]->SetFont(font);*/

	wxArrayString m_arrItems;

	// Create common wxArrayString array
	m_arrItems.Add(wxT("Facial Expression"));
	m_arrItems.Add(wxT("Head Nodding"));
	//m_arrItems.Add(wxT("Dot"));

	ComboBox4 = new wxComboBox(panel, CB4_ID, m_arrItems[size_t(tracking_modes[3])],
		wxDefaultPosition, wxDefaultSize, m_arrItems, 0, wxDefaultValidator, _T("ID_COMBOBOX4"));
	Connect(CB4_ID, wxEVT_COMBOBOX, wxCommandEventHandler(ControlsUI::OnCB4Select));
	panel14->Add(ComboBox4, 1);

}

void ControlsUI::addFERow40()
{
	tracking_modes[3] = 0;
	key4 = new wxStaticText(panel, -1, wxT("Wide Eyes:"));
	key4->SetForegroundColour(*text_color);
	font = key4->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	key4->SetFont(font);

	
	panel04->Add(key4, 1, wxEXPAND);

	size_t sz = empty_static_texts4.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts4.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS1; i++) {
		panel04->Add(empty_static_texts4[i], 0);
	}

}

void ControlsUI::addFERow41()
{
	key4_presence_rb = new wxRadioButton(panel, K4_ID_presence_RB,
		wxT("Presence"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	key4_presence_rb->SetForegroundColour(*text_color);
	Connect(K4_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB4Select));
	key4_intensity_rb = new wxRadioButton(panel, K4_ID_intensity_RB,
		wxT("Intensity"));
	key4_intensity_rb->SetForegroundColour(*text_color);
	Connect(K4_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB4Select));
	//key4_intensity_rb->SetValue(true);

	AU1_1 = new wxStaticText(panel, -1, wxT("AU1 (Inner Brow Raiser)"));
	AU1_1->SetForegroundColour(*text_color);
	AU1_1_slider = new wxSlider(panel, AU1_1_ID_SLIDER, (int)(AU1_1_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU1_1_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll1_1));

	AU1_1_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU1_1_threshold));
	AU1_1_slider_value->SetForegroundColour(*text_color);

	AU2 = new wxStaticText(panel, -1, wxT("AU2 (Outer Brow Raiser)"));
	AU2->SetForegroundColour(*text_color);
	AU2_slider = new wxSlider(panel, AU2_ID_SLIDER, (int)(AU2_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU2_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll2));

	AU2_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU2_threshold));
	AU2_slider_value->SetForegroundColour(*text_color);

	AU5 = new wxStaticText(panel, -1, wxT("AU5 (Upper Lid Raiser)"));
	AU5->SetForegroundColour(*text_color);
	AU5_slider = new wxSlider(panel, AU5_ID_SLIDER, (int)(AU5_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU5_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll5));

	AU5_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU5_threshold));
	AU5_slider_value->SetForegroundColour(*text_color);

	AU1_1_slider->Enable(is_intensity_arr[7]);
	AU2_slider->Enable(is_intensity_arr[7]);
	AU5_slider->Enable(is_intensity_arr[7]);
	K4_presence = !is_intensity_arr[7];
	key4_presence_rb->SetValue(!is_intensity_arr[7]);
	key4_intensity_rb->SetValue(is_intensity_arr[7]);


	panel04->Add(key4_presence_rb, 1, wxEXPAND);
	panel04->Add(key4_intensity_rb, 1, wxEXPAND);
	panel04->Add(AU1_1, 0, wxEXPAND);
	panel04->Add(AU1_1_slider, 1, wxEXPAND);
	panel04->Add(AU1_1_slider_value, 1, wxEXPAND);
	panel04->Add(AU2, 0, wxEXPAND);
	panel04->Add(AU2_slider, 1, wxEXPAND);
	panel04->Add(AU2_slider_value, 1, wxEXPAND);
	panel04->Add(AU5, 0, wxEXPAND);
	panel04->Add(AU5_slider, 1, wxEXPAND);
	panel04->Add(AU5_slider_value, 1, wxEXPAND);

	/*size_t sz = empty_static_texts4.size();
	for (size_t i = sz; i < sz + 3; i++) {
		empty_static_texts4.push_back(new wxStaticText(panel, -1, wxT("")));
	}

	for (size_t i = sz; i < sz + 3; i++) {
		panel01->Add(empty_static_texts4[i], 0, wxEXPAND);
	}*/
}

void ControlsUI::addRow5()
{
	wxStaticText *txt = new wxStaticText(panel, -1, wxT("Key 5"));
	txt->SetForegroundColour("#F6F7DD");
	wxFont font = txt->GetFont();
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	txt->SetFont(font);
	keynumbs.push_back(txt);
	panel15->Add(keynumbs[4], 0);


	/*keynumbs.push_back(txt);
	panel15->Add(keynumbs[4], 0);
	wxFont font = keynumbs[4]->GetFont();
	// std::cout << font.GetPointSize() << "\n";
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	keynumbs[4]->SetFont(font);*/

	wxArrayString m_arrItems;

	// Create common wxArrayString array
	m_arrItems.Add(wxT("Facial Expression"));
	m_arrItems.Add(wxT("Head Nodding"));
	//m_arrItems.Add(wxT("Dot"));

	ComboBox5 = new wxComboBox(panel, CB5_ID, m_arrItems[size_t(tracking_modes[4])],
		wxDefaultPosition, wxDefaultSize, m_arrItems, 0, wxDefaultValidator, _T("ID_COMBOBOX5"));
	Connect(CB5_ID, wxEVT_COMBOBOX, wxCommandEventHandler(ControlsUI::OnCB5Select));
	panel15->Add(ComboBox5, 1);

}

void ControlsUI::addFERow50()
{
	tracking_modes[4] = 0;
	key5 = new wxStaticText(panel, -1, wxT("Contempt:"), wxDefaultPosition);
	key5->SetForegroundColour(*text_color);
	font = key5->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	key5->SetFont(font);

	
	panel05->Add(key5, 1, wxEXPAND);


	size_t sz = empty_static_texts5.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts5.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS1; i++) {
		panel05->Add(empty_static_texts5[i], 0);
	}

}

void ControlsUI::addFERow51()
{
	key5_presence_rb = new wxRadioButton(panel, K5_ID_presence_RB,
		wxT("Presence"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	key5_presence_rb->SetForegroundColour(*text_color);
	Connect(K5_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB5Select));
	key5_intensity_rb = new wxRadioButton(panel, K5_ID_intensity_RB,
		wxT("Intensity"));
	key5_intensity_rb->SetForegroundColour(*text_color);
	Connect(K5_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB5Select));
	//key5_intensity_rb->SetValue(true);

	AU7 = new wxStaticText(panel, -1, wxT("AU7 (Lid Tightener)"));
	AU7->SetForegroundColour(*text_color);
	AU7_slider = new wxSlider(panel, AU7_ID_SLIDER, (int)(AU7_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU7_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll7));

	AU7_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU7_threshold));
	AU7_slider_value->SetForegroundColour(*text_color);

	AU23 = new wxStaticText(panel, -1, wxT("AU23 (Lip Tightener)"));
	AU23->SetForegroundColour(*text_color);
	AU23_slider = new wxSlider(panel, AU23_ID_SLIDER, (int)(AU23_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU23_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll23));

	AU23_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU23_threshold));
	AU23_slider_value->SetForegroundColour(*text_color);

	AU7_slider->Enable(is_intensity_arr[10]);
	AU23_slider->Enable(is_intensity_arr[10]);
	K5_presence = !is_intensity_arr[10];
	key5_presence_rb->SetValue(!is_intensity_arr[10]);
	key5_intensity_rb->SetValue(is_intensity_arr[10]);

	panel05->Add(key5_presence_rb, 1, wxEXPAND);
	panel05->Add(key5_intensity_rb, 1, wxEXPAND);
	panel05->Add(AU7, 0, wxEXPAND);
	panel05->Add(AU7_slider, 1, wxEXPAND);
	panel05->Add(AU7_slider_value, 1, wxEXPAND);
	panel05->Add(AU23, 0, wxEXPAND);
	panel05->Add(AU23_slider, 1, wxEXPAND);
	panel05->Add(AU23_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts5.size();
	for (size_t i = sz; i < sz + STATIC_TEXT_NUMBS2; i++) {
		empty_static_texts5.push_back(new wxStaticText(panel, -1, wxT("")));
	}

	for (size_t i = sz; i < sz + STATIC_TEXT_NUMBS2; i++) {
		panel05->Add(empty_static_texts5[i], 0, wxEXPAND);
	}
}

void ControlsUI::addRow6()
{
	wxStaticText *txt = new wxStaticText(panel, -1, wxT("Key 6"));
	txt->SetForegroundColour("#F6F7DD");
	keynumbs.push_back(txt);
	panel16->Add(keynumbs[5], 0);
	wxFont font = keynumbs[5]->GetFont();
	// std::cout << font.GetPointSize() << "\n";
	font.SetPointSize(12);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	keynumbs[5]->SetFont(font);

	wxArrayString m_arrItems;

	// Create common wxArrayString array
	m_arrItems.Add(wxT("Facial Expression"));
	m_arrItems.Add(wxT("Head Nodding"));
	//m_arrItems.Add(wxT("Dot"));

	ComboBox6 = new wxComboBox(panel, CB6_ID, m_arrItems[size_t(tracking_modes[5])],
		wxDefaultPosition, wxDefaultSize, m_arrItems, 0, wxDefaultValidator, _T("ID_COMBOBOX6"));
	Connect(CB6_ID, wxEVT_COMBOBOX, wxCommandEventHandler(ControlsUI::OnCB6Select));
	panel16->Add(ComboBox6, 1);

}

void ControlsUI::addFERow60()
{
	tracking_modes[5] = 0;
	key6 = new wxStaticText(panel, -1, wxT("Jaw Drop:"));
	key6->SetForegroundColour(*text_color);
	font = key6->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	key6->SetFont(font);

	
	panel06->Add(key6, 1, wxEXPAND);

	size_t sz = empty_static_texts6.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts6.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS1; i++) {
		panel06->Add(empty_static_texts6[i], 0, wxEXPAND);
	}

}

void ControlsUI::addFERow61()
{
	key6_presence_rb = new wxRadioButton(panel, K6_ID_presence_RB,
		wxT("Presence"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	key6_presence_rb->SetForegroundColour(*text_color);
	Connect(K6_ID_presence_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB6Select));
	key6_intensity_rb = new wxRadioButton(panel, K6_ID_intensity_RB,
		wxT("Intensity"));
	key6_intensity_rb->SetForegroundColour(*text_color);
	Connect(K6_ID_intensity_RB, wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(ControlsUI::OnRB6Select));
	//key3_intensity_rb->SetValue(true);

	AU4_2 = new wxStaticText(panel, -1, wxT("AU4 (Brow Lowerer)"));
	AU4_2->SetForegroundColour(*text_color);
	// wxTextCtrl *tcAU9 = new wxTextCtrl(panel, -1);
	AU4_2_slider = new wxSlider(panel, AU4_2_ID_SLIDER, (int)(AU4_2_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU4_2_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll4_2));
	AU4_2_slider->Enable(false);

	AU4_2_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU4_2_threshold));
	AU4_2_slider_value->SetForegroundColour(*text_color);

	AU25 = new wxStaticText(panel, -1, wxT("AU25 (Lips Part)"));
	AU25->SetForegroundColour(*text_color);
	// wxTextCtrl *tcAU10 = new wxTextCtrl(panel, -1);
	AU25_slider = new wxSlider(panel, AU25_ID_SLIDER, (int)(AU25_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU25_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll25));
	AU25_slider->Enable(false);

	AU25_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU25_threshold));
	AU25_slider_value->SetForegroundColour(*text_color);

	AU26 = new wxStaticText(panel, -1, wxT("AU26 (Jaw Drop)"));
	AU26->SetForegroundColour(*text_color);
	// wxTextCtrl *tcAU9 = new wxTextCtrl(panel, -1);
	AU26_slider = new wxSlider(panel, AU26_ID_SLIDER, (int)(AU26_threshold * 10), 0, 50,
		wxDefaultPosition, wxSize(115, -1));
	Connect(AU26_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnScroll26));
	AU26_slider->Enable(false);

	AU26_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%.1f"), AU26_threshold));
	AU26_slider_value->SetForegroundColour(*text_color);

	AU4_2_slider->Enable(is_intensity_arr[12]);
	AU25_slider->Enable(is_intensity_arr[12]);
	AU26_slider->Enable(is_intensity_arr[12]);
	K6_presence = !is_intensity_arr[12];
	key6_presence_rb->SetValue(!is_intensity_arr[12]);
	key6_intensity_rb->SetValue(is_intensity_arr[12]);

	panel06->Add(key6_presence_rb, 1, wxEXPAND);
	panel06->Add(key6_intensity_rb, 1, wxEXPAND);
	panel06->Add(AU4_2, 0, wxEXPAND);
	panel06->Add(AU4_2_slider, 1, wxEXPAND);
	panel06->Add(AU4_2_slider_value, 1, wxEXPAND);
	panel06->Add(AU25, 0, wxEXPAND);
	panel06->Add(AU25_slider, 1, wxEXPAND);
	panel06->Add(AU25_slider_value, 1, wxEXPAND);
	panel06->Add(AU26, 0, wxEXPAND);
	panel06->Add(AU26_slider, 1, wxEXPAND);
	panel06->Add(AU26_slider_value, 1, wxEXPAND);
}

void ControlsUI::addRow7()
{
	restoreDefaults_btn = new wxButton(panel, ID_RESTORE, wxT("Restore Defaults"),
		wxDefaultPosition, wxSize(125, 35));
	restoreDefaults_btn->SetBackgroundColour("#E69A8DFF");
	wxFont font_btn = restoreDefaults_btn->GetFont();
	font_btn.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font_btn.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	restoreDefaults_btn->SetFont(font_btn);
	Connect(ID_RESTORE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ControlsUI::OnRestore));

	SaveAsDefaults_btn = new wxButton(panel, ID_SAVE, wxT("Save As Defaults"),
		wxDefaultPosition, wxSize(125, 35));
	SaveAsDefaults_btn->SetBackgroundColour("#E69A8DFF");
	SaveAsDefaults_btn->SetFont(font_btn);
	Connect(ID_SAVE, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ControlsUI::OnSave));

	ok_btn = new wxButton(panel, ID_OK, wxT("OK"), wxDefaultPosition, wxSize(125, 35)/*wxSize(82, 30)*/);
	ok_btn->SetBackgroundColour("#E69A8DFF");
	//font_btn.SetPointSize(9);
	ok_btn->SetFont(font_btn);
	Connect(ID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(ControlsUI::OnOK));

	for (int i = 0; i < 8; i++) {
		empty_static_texts7.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (int i = 0; i < 3; i++) {
		//panel07->Add(empty_static_texts7[i], 0, wxEXPAND);
	}

	panel07->Add(restoreDefaults_btn, 0);
	// panel07->Add(empty_static_texts7[3], 0, wxEXPAND);
	//panel07->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	panel07->Add(SaveAsDefaults_btn, 0);
	// panel07->Add(new wxStaticText(panel, -1, wxT("")), 0, wxEXPAND);
	panel07->Add(ok_btn, 0);

	for (int i = 4; i < 8; i++) {
		//panel07->Add(empty_static_texts7[i], 0, wxEXPAND);
	}

}

void ControlsUI::removeRow1()
{
	//panel11->Detach(keynumbs[0]); 

	wxStaticText *ptr = keynumbs[0];
	panel11->Detach(ptr);
	ptr->Destroy();
	panel11->Detach(ComboBox1);
	ComboBox1->Hide();

	/*size_t sz = empty_static_texts.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS; i++) {
		wxStaticText *ptr = empty_static_texts[sz - i - 1];
		panel01->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS; i++) {
		empty_static_texts.pop_back();
	}*/
}

void ControlsUI::removeFERow10()
{
	panel01->Detach(key1);
	key1->Hide();
	size_t sz = empty_static_texts1.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts1[sz - i - 1];
		panel01->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts1.pop_back();
	}
}

void ControlsUI::removeFERow11()
{
	panel01->Detach(key1_presence_rb);
	key1_presence_rb->Hide();
	panel01->Detach(key1_intensity_rb);
	key1_intensity_rb->Hide();
	panel01->Detach(AU6);
	AU6->Hide();
	panel01->Detach(AU6_slider);
	AU6_slider->Hide();
	panel01->Detach(AU6_slider_value);
	AU6_slider_value->Hide();
	panel01->Detach(AU12);
	AU12->Hide();
	panel01->Detach(AU12_slider);
	AU12_slider->Hide();
	panel01->Detach(AU12_slider_value);
	AU12_slider_value->Hide();
	size_t sz = empty_static_texts1.size();
	for (size_t i = 0; i < 3; i++) {
		wxStaticText *ptr = empty_static_texts1[sz - i - 1];
		panel01->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < 3; i++) {
		empty_static_texts1.pop_back();
	}
}

void ControlsUI::removeRow2()
{
	//panel11->Detach(keynumbs[0]);
	wxStaticText *ptr = keynumbs[1];
	panel12->Detach(ptr);
	ptr->Destroy();
	panel12->Detach(ComboBox2);
	ComboBox2->Hide();
}

void ControlsUI::removeRow3()
{
	wxStaticText *ptr = keynumbs[2];
	panel13->Detach(ptr);
	ptr->Destroy();
	panel13->Detach(ComboBox3);
	ComboBox3->Hide();
}

void ControlsUI::removeRow4()
{
	wxStaticText *ptr = keynumbs[3];
	panel14->Detach(ptr);
	ptr->Destroy();
	panel14->Detach(ComboBox4);
	ComboBox4->Hide();
}

void ControlsUI::removeRow5()
{
	wxStaticText *ptr = keynumbs[4];
	panel15->Detach(ptr);
	ptr->Destroy();
	panel15->Detach(ComboBox5);
	ComboBox5->Hide();
}

void ControlsUI::removeRow6()
{
	wxStaticText *ptr = keynumbs[5];
	panel16->Detach(ptr);
	ptr->Destroy();
	panel16->Detach(ComboBox6);
	ComboBox6->Hide();
}

void ControlsUI::removeFERow20()
{
	panel02->Detach(key2);
	key2->Hide();

	for (size_t i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts2[i];
		panel02->Detach(ptr);
		ptr->Destroy();
	}
	
	empty_static_texts2.erase(empty_static_texts2.begin(), empty_static_texts2.begin()+STATIC_TEXT_NUMBS1);	

}

void ControlsUI::removeFERow21()
{
	panel02->Detach(key2_presence_rb);
	//key2_presence_rb->Destroy();
	key2_presence_rb->Hide();
	panel02->Detach(key2_intensity_rb);
	key2_intensity_rb->Hide();
	panel02->Detach(AU1);
	AU1->Hide();
	panel02->Detach(AU1_slider);
	AU1_slider->Hide();
	panel02->Detach(AU1_slider_value);
	AU1_slider_value->Hide();
	panel02->Detach(AU4_1);
	AU4_1->Hide();
	panel02->Detach(AU4_1_slider);
	AU4_1_slider->Hide();
	panel02->Detach(AU4_1_slider_value);
	AU4_1_slider_value->Hide();
	panel02->Detach(AU15);
	AU15->Hide();
	panel02->Detach(AU15_slider);
	AU15_slider->Hide();
	panel02->Detach(AU15_slider_value);
	AU15_slider_value->Hide();
}

void ControlsUI::removeFERow30()
{
	panel03->Detach(key3);
	key3->Hide();

	for (size_t i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts3[i];
		panel03->Detach(ptr);
		ptr->Destroy();
	}

	empty_static_texts3.erase(empty_static_texts3.begin(), empty_static_texts3.begin() + STATIC_TEXT_NUMBS1);
}

void ControlsUI::removeFERow31()
{
	panel03->Detach(key3_presence_rb);
	key3_presence_rb->Hide();
	panel03->Detach(key3_intensity_rb);
	key3_intensity_rb->Hide();
	panel03->Detach(AU9);
	AU9->Hide();
	panel03->Detach(AU9_slider);
	AU9_slider->Hide();
	panel03->Detach(AU9_slider_value);
	AU9_slider_value->Hide();
	panel03->Detach(AU10);
	AU10->Hide();
	panel03->Detach(AU10_slider);
	AU10_slider->Hide();
	panel03->Detach(AU10_slider_value);
	AU10_slider_value->Hide();
	size_t sz = empty_static_texts3.size();
	for (size_t i = 0; i < 3; i++) {
		wxStaticText *ptr = empty_static_texts3[sz - i - 1];
		panel03->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < 3; i++) {
		empty_static_texts3.pop_back();
	}
}

void ControlsUI::removeFERow40()
{
	panel04->Detach(key4);
	key4->Hide();

	for (size_t i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts4[i];
		panel04->Detach(ptr);
		ptr->Destroy();
	}

	empty_static_texts4.erase(empty_static_texts4.begin(), empty_static_texts4.begin() + STATIC_TEXT_NUMBS1);
}

void ControlsUI::removeFERow41()
{
	panel04->Detach(key4_presence_rb);
	key4_presence_rb->Hide();
	panel04->Detach(key4_intensity_rb);
	key4_intensity_rb->Hide();
	panel04->Detach(AU1_1);
	AU1_1->Hide();
	panel04->Detach(AU1_1_slider);
	AU1_1_slider->Hide();
	panel04->Detach(AU1_1_slider_value);
	AU1_1_slider_value->Hide();
	panel04->Detach(AU2);
	AU2->Hide();
	panel04->Detach(AU2_slider);
	AU2_slider->Hide();
	panel04->Detach(AU2_slider_value);
	AU2_slider_value->Hide();
	panel04->Detach(AU5);
	AU5->Hide();
	panel04->Detach(AU5_slider);
	AU5_slider->Hide();
	panel04->Detach(AU5_slider_value);
	AU5_slider_value->Hide();
}

void ControlsUI::removeFERow50()
{
	panel05->Detach(key5);
	key5->Hide();

	for (size_t i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts5[i];
		panel05->Detach(ptr);
		ptr->Destroy();
	}

	empty_static_texts5.erase(empty_static_texts5.begin(), empty_static_texts5.begin() + STATIC_TEXT_NUMBS1);
}

void ControlsUI::removeFERow51()
{
	panel05->Detach(key5_presence_rb);
	key5_presence_rb->Hide();
	panel05->Detach(key5_intensity_rb);
	key5_intensity_rb->Hide();
	panel05->Detach(AU7);
	AU7->Hide();
	panel05->Detach(AU7_slider);
	AU7_slider->Hide();
	panel05->Detach(AU7_slider_value);
	AU7_slider_value->Hide();
	panel05->Detach(AU23);
	AU23->Hide();
	panel05->Detach(AU23_slider);
	AU23_slider->Hide();
	panel05->Detach(AU23_slider_value);
	AU23_slider_value->Hide();
	size_t sz = empty_static_texts5.size();
	for (size_t i = 0; i < 3; i++) {
		wxStaticText *ptr = empty_static_texts5[sz - i - 1];
		panel05->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < 3; i++) {
		empty_static_texts5.pop_back();
	}
}

void ControlsUI::removeFERow60()
{
	panel06->Detach(key6);
	key6->Hide();

	for (size_t i = 0; i < STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts6[i];
		panel06->Detach(ptr);
		ptr->Destroy();
	}

	empty_static_texts6.erase(empty_static_texts6.begin(), empty_static_texts6.begin() + STATIC_TEXT_NUMBS1);
}

void ControlsUI::removeFERow61()
{
	panel06->Detach(key6_presence_rb);
	key6_presence_rb->Hide();
	panel06->Detach(key6_intensity_rb);
	key6_intensity_rb->Hide();
	panel06->Detach(AU4_2);
	AU4_2->Hide();
	panel06->Detach(AU4_2_slider);
	AU4_2_slider->Hide();
	panel06->Detach(AU4_2_slider_value);
	AU4_2_slider_value->Hide();
	panel06->Detach(AU25);
	AU25->Hide();
	panel06->Detach(AU25_slider);
	AU25_slider->Hide();
	panel06->Detach(AU25_slider_value);
	AU25_slider_value->Hide();
	panel06->Detach(AU26);
	AU26->Hide();
	panel06->Detach(AU26_slider);
	AU26_slider->Hide();
	panel06->Detach(AU26_slider_value);
	AU26_slider_value->Hide();
}

void ControlsUI::removeFERow71()
{
	size_t sz = empty_static_texts7.size();
	std::cout << "SIZEEEEEEE 1: " << sz << "\n";
	for (size_t i = 0; i < 3; i++) {
		wxStaticText *ptr = empty_static_texts7[i];
		panel07->Detach(ptr);
		ptr->Destroy();
	}

	empty_static_texts7.erase(empty_static_texts7.begin(), empty_static_texts7.begin() + 3);

	panel07->Detach(restoreDefaults_btn);
	restoreDefaults_btn->Hide();
	panel07->Detach(SaveAsDefaults_btn); 
	SaveAsDefaults_btn->Hide();

	wxStaticText *ptr = empty_static_texts7[0];
	panel07->Detach(ptr);
	ptr->Destroy();

	empty_static_texts7.erase(empty_static_texts7.begin());

	panel07->Detach(ok_btn);
	ok_btn->Hide();

	sz = empty_static_texts7.size();
	std::cout << "SIZEEEEEEE 2: " << sz << "\n";
	for (wxStaticText *ptr : empty_static_texts7) {
		panel07->Detach(ptr);
		ptr->Destroy();
	}

	empty_static_texts7.erase(empty_static_texts7.begin(), empty_static_texts7.begin() + 4);


}

void ControlsUI::addHNRow11()
{
	tracking_modes[0] = 1;
	hn_label = new wxStaticText(panel, -1, wxT("Head Movement Length:"));
	hn_label->SetForegroundColour(*text_color);
	font = hn_label->GetFont();
	font.SetPointSize(10);
	font.SetFamily(wxMODERN);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	hn_label->SetFont(font);

	panel01->Add(hn_label, 0, wxEXPAND);

	hn_slider = new wxSlider(panel, HN_ID_SLIDER, angles_diff_threshold, 20, 60,
		wxDefaultPosition, wxSize(115, -1));
	Connect(HN_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnHNScroll));

	hn_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%02d"), angles_diff_threshold));
	hn_slider_value->SetForegroundColour(*text_color);
	font.SetWeight(wxNORMAL);
	hn_slider_value->SetFont(font);

	panel01->Add(hn_slider, 1, wxEXPAND);
	panel01->Add(hn_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts1.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts1.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		panel01->Add(empty_static_texts1[i], 0);
	}
}

void ControlsUI::addHNRow12()
{
	tracking_modes[1] = 1;
	hn_label = new wxStaticText(panel, -1, wxT("Head Movement Length:"));
	hn_label->SetForegroundColour(*text_color);
	font = hn_label->GetFont();
	font.SetPointSize(10);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	hn_label->SetFont(font);

	panel02->Add(hn_label, 0, wxEXPAND);

	hn_slider = new wxSlider(panel, HN_ID_SLIDER, angles_diff_threshold, 20, 60,
		wxDefaultPosition, wxSize(115, -1));
	Connect(HN_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnHNScroll));

	hn_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%02d"), angles_diff_threshold));
	hn_slider_value->SetForegroundColour(*text_color);
	font.SetWeight(wxNORMAL);
	hn_slider_value->SetFont(font);

	panel02->Add(hn_slider, 1, wxEXPAND);
	panel02->Add(hn_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts2.size();
	std::cout << "SZZZZZZZZZZ:  " << sz << "\n";
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts2.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		panel02->Add(empty_static_texts2[i], 0);
	}
}

void ControlsUI::addHNRow13()
{
	tracking_modes[2] = 1;
	hn_label = new wxStaticText(panel, -1, wxT("Head Movement Length:"));
	hn_label->SetForegroundColour(*text_color);
	font = hn_label->GetFont();
	font.SetPointSize(10);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	hn_label->SetFont(font);

	panel03->Add(hn_label, 0, wxEXPAND);

	hn_slider = new wxSlider(panel, HN_ID_SLIDER, angles_diff_threshold, 20, 60,
		wxDefaultPosition, wxSize(115, -1));
	Connect(HN_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnHNScroll));

	hn_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%02d"), angles_diff_threshold));
	hn_slider_value->SetForegroundColour(*text_color);
	font.SetWeight(wxNORMAL);
	hn_slider_value->SetFont(font);

	panel03->Add(hn_slider, 1, wxEXPAND);
	panel03->Add(hn_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts3.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts3.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		panel03->Add(empty_static_texts3[i], 0);
	}
}

void ControlsUI::addHNRow14()
{
	tracking_modes[3] = 1;
	hn_label = new wxStaticText(panel, -1, wxT("Head Movement Length:"));
	hn_label->SetForegroundColour(*text_color);
	font = hn_label->GetFont();
	font.SetPointSize(10);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	hn_label->SetFont(font);

	panel04->Add(hn_label, 0, wxEXPAND);

	hn_slider = new wxSlider(panel, HN_ID_SLIDER, angles_diff_threshold, 20, 60,
		wxDefaultPosition, wxSize(115, -1));
	Connect(HN_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnHNScroll));

	hn_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%02d"), angles_diff_threshold));
	hn_slider_value->SetForegroundColour(*text_color);
	font.SetWeight(wxNORMAL);
	hn_slider_value->SetFont(font);

	panel04->Add(hn_slider, 1, wxEXPAND);
	panel04->Add(hn_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts4.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts4.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		panel04->Add(empty_static_texts4[i], 0);
	}
}

void ControlsUI::addHNRow15()
{
	tracking_modes[4] = 1;
	hn_label = new wxStaticText(panel, -1, wxT("Head Movement Length:"));
	hn_label->SetForegroundColour(*text_color);
	font = hn_label->GetFont();
	font.SetPointSize(10);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	hn_label->SetFont(font);

	panel05->Add(hn_label, 0, wxEXPAND);

	hn_slider = new wxSlider(panel, HN_ID_SLIDER, angles_diff_threshold, 20, 60,
		wxDefaultPosition, wxSize(115, -1));
	Connect(HN_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnHNScroll));

	hn_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%02d"), angles_diff_threshold));
	hn_slider_value->SetForegroundColour(*text_color);
	font.SetWeight(wxNORMAL);
	hn_slider_value->SetFont(font);

	panel05->Add(hn_slider, 1, wxEXPAND);
	panel05->Add(hn_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts5.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts5.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		panel05->Add(empty_static_texts5[i], 0);
	}
}

void ControlsUI::addHNRow16()
{
	tracking_modes[5] = 1;
	hn_label = new wxStaticText(panel, -1, wxT("Head Movement Length:"));
	hn_label->SetForegroundColour(*text_color);
	font = hn_label->GetFont();
	font.SetPointSize(10);
	font.SetFaceName(wxT("Tahoma"));
	font.SetWeight(wxFONTWEIGHT_BOLD);
	hn_label->SetFont(font);

	panel06->Add(hn_label, 0, wxEXPAND);

	hn_slider = new wxSlider(panel, HN_ID_SLIDER, angles_diff_threshold, 20, 60,
		wxDefaultPosition, wxSize(115, -1));
	Connect(HN_ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(ControlsUI::OnHNScroll));

	hn_slider_value = new wxStaticText(panel, -1, wxString::Format(wxT("%02d"), angles_diff_threshold));
	hn_slider_value->SetForegroundColour(*text_color);
	font.SetWeight(wxNORMAL);
	hn_slider_value->SetFont(font);

	panel06->Add(hn_slider, 1, wxEXPAND);
	panel06->Add(hn_slider_value, 1, wxEXPAND);

	size_t sz = empty_static_texts6.size();
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts6.push_back(new wxStaticText(panel, -1, wxT("")));
	}
	for (size_t i = sz; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		panel06->Add(empty_static_texts6[i], 0);
	}
}

void ControlsUI::removeHN1()
{
	panel01->Detach(hn_label);
	hn_label->Hide();
	panel01->Detach(hn_slider);
	hn_slider->Hide();
	panel01->Detach(hn_slider_value);
	hn_slider_value->Hide();

	size_t sz = empty_static_texts1.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts1[sz - i - 1];
		panel01->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts1.pop_back();
	}
}

void ControlsUI::removeHN2()
{
	panel02->Detach(hn_label);
	hn_label->Hide();
	panel02->Detach(hn_slider);
	hn_slider->Hide();
	panel02->Detach(hn_slider_value);
	hn_slider_value->Hide();

	size_t sz = empty_static_texts2.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts2[sz - i - 1];
		panel01->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts2.pop_back();
	}
}

void ControlsUI::removeHN3()
{
	panel03->Detach(hn_label);
	hn_label->Hide();
	panel03->Detach(hn_slider);
	hn_slider->Hide();
	panel03->Detach(hn_slider_value);
	hn_slider_value->Hide();

	size_t sz = empty_static_texts3.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts3[sz - i - 1];
		panel03->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts3.pop_back();
	}
}

void ControlsUI::removeHN4()
{
	panel04->Detach(hn_label);
	hn_label->Hide();
	panel04->Detach(hn_slider);
	hn_slider->Hide();
	panel04->Detach(hn_slider_value);
	hn_slider_value->Hide();

	size_t sz = empty_static_texts4.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts4[sz - i - 1];
		panel04->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts4.pop_back();
	}
}

void ControlsUI::removeHN5()
{
	panel05->Detach(hn_label);
	hn_label->Hide();
	panel05->Detach(hn_slider);
	hn_slider->Hide();
	panel05->Detach(hn_slider_value);
	hn_slider_value->Hide();

	size_t sz = empty_static_texts5.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts5[sz - i - 1];
		panel05->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts5.pop_back();
	}
}

void ControlsUI::removeHN6()
{
	panel06->Detach(hn_label);
	hn_label->Hide();
	panel06->Detach(hn_slider);
	hn_slider->Hide();
	panel06->Detach(hn_slider_value);
	hn_slider_value->Hide();

	size_t sz = empty_static_texts6.size();
	for (size_t i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		wxStaticText *ptr = empty_static_texts6[sz - i - 1];
		panel06->Detach(ptr);
		ptr->Destroy();
	}
	for (int i = 0; i < STATIC_TEXT_NUMBS + STATIC_TEXT_NUMBS1; i++) {
		empty_static_texts6.pop_back();
	}
}
