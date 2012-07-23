/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2012 Catalin Alexandru Nastase

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef _MAIN_FRAME_H_
#define _MAIN_FRAME_H_

#include <EditorConfig.h>

class PropertiesPanel;
class SceneExplorerPanel;
class SceneViewPanel;

class MainFrame : public wxFrame
{
public:

	MainFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("KG Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
		
	~MainFrame();

	SceneViewPanel* GetSceneViewPanel();

protected:

	PropertiesPanel* mPropertiesPanel;
	SceneExplorerPanel* mSceneExplorerPanel;
	SceneViewPanel* mSceneViewPanel;
	
	wxStatusBar* mStatusBar;
	wxMenuBar* mMenubar;
	wxMenu* mMenuFile;
	wxMenu* mMenuEdit;

	void CreateMenus();
	void CreatePanels();
	void CreateStatusbar();

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