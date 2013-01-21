/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2013 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#ifndef _MAIN_FRAME_H_
#define _MAIN_FRAME_H_

#include <EditorConfig.h>

class MainFrame: public wxFrame
{
public:

	MainFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("KG Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
		
	~MainFrame();

protected:

	wxTreeCtrl* m_sceneExplorer;
	wxPropertyGrid* m_propertyWindow;
	
	wxStatusBar* m_statusBar;
	wxMenuBar* m_menubar;
	wxMenu* m_menuFile;
	wxMenu* m_menuEdit;

	void OnMenuFileNew(wxCommandEvent& event);
	void OnMenuFileOpen(wxCommandEvent& event);
	void OnMenuFileSave(wxCommandEvent& event);
	void OnMenuFileSaveAs(wxCommandEvent& event);
	void OnMenuFileExit(wxCommandEvent& event);

	void OnMenuEditUndo(wxCommandEvent& event);
	void OnMenuEditRedo(wxCommandEvent& event);
	void OnMenuEditDelete(wxCommandEvent& event);
	void OnMenuEditClone(wxCommandEvent& event);
	void OnMenuEditHide(wxCommandEvent& event);
	void OnMenuEditUnhideAll(wxCommandEvent& event);
	void OnMenuEditSelectAll(wxCommandEvent& event);
	void OnMenuEditSelectNone(wxCommandEvent& event);
	void OnMenuEditSelectInvert(wxCommandEvent& event);

private:

	wxAuiManager mAuiManager;
};


#endif