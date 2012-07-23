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

#include <MainFrame.h>
#include <SceneExplorerPanel.h>
#include <PropertiesPanel.h>
#include <SceneViewPanel.h>

MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style)
{
	mSceneViewPanel = NULL;
	mPropertiesPanel = NULL;
	mSceneExplorerPanel = NULL;
	
	mAuiManager.SetManagedWindow(this);

	CreateMenus();
	CreatePanels();
	CreateStatusBar();

	// Make editor look pretty
	mAuiManager.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_HORIZONTAL);//wxAUI_GRADIENT_VERTICAL
	wxColour active_caption_colour = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
	wxColour active_caption_gradient_colour = wxSystemSettings::GetColour(wxSYS_COLOUR_GRADIENTACTIVECAPTION);
	wxColour active_caption_text_colour = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
	mAuiManager.GetArtProvider()->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, active_caption_colour);
	mAuiManager.GetArtProvider()->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, active_caption_colour);
	mAuiManager.GetArtProvider()->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR, active_caption_gradient_colour);
	mAuiManager.GetArtProvider()->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, active_caption_gradient_colour);
	mAuiManager.GetArtProvider()->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR, active_caption_text_colour);
	mAuiManager.GetArtProvider()->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, active_caption_text_colour);

	mAuiManager.Update();
	///////////////////////////////////////////

	// Connect Events
	Connect(ID_MENU_FILE_NEW,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileNew));
	Connect(ID_MENU_FILE_OPEN,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileOpen));
	Connect(ID_MENU_FILE_SAVE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSave));
	Connect(ID_MENU_FILE_SAVE_AS,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSaveAs));
	Connect(ID_MENU_FILE_EXIT,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileExit));

	Connect(ID_MENU_EDIT_UNDO,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUndo));
	Connect(ID_MENU_EDIT_REDO,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditRedo));
	Connect(ID_MENU_EDIT_DELETE,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditDelete));
	Connect(ID_MENU_EDIT_CLONE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditClone));
	Connect(ID_MENU_EDIT_HIDE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditHide));
	Connect(ID_MENU_EDIT_UNHIDE_ALL,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUnhideAll));
	Connect(ID_MENU_EDIT_SELECT_ALL,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectAll));
	Connect(ID_MENU_EDIT_SELECT_NONE,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectNone));
	Connect(ID_MENU_EDIT_SELECT_INVERT,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectInvert));
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	Disconnect(ID_MENU_FILE_NEW,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileNew));
	Disconnect(ID_MENU_FILE_OPEN,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileOpen));
	Disconnect(ID_MENU_FILE_SAVE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSave));
	Disconnect(ID_MENU_FILE_SAVE_AS,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSaveAs));
	Disconnect(ID_MENU_FILE_EXIT,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileExit));

	Disconnect(ID_MENU_EDIT_UNDO,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUndo));
	Disconnect(ID_MENU_EDIT_REDO,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditRedo));
	Disconnect(ID_MENU_EDIT_DELETE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditDelete));
	Disconnect(ID_MENU_EDIT_CLONE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditClone));
	Disconnect(ID_MENU_EDIT_HIDE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditHide));
	Disconnect(ID_MENU_EDIT_UNHIDE_ALL,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUnhideAll));
	Disconnect(ID_MENU_EDIT_SELECT_ALL,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectAll));
	Disconnect(ID_MENU_EDIT_SELECT_NONE,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectNone));
	Disconnect(ID_MENU_EDIT_SELECT_INVERT,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectInvert));

	mAuiManager.UnInit();
}

SceneViewPanel* MainFrame::GetSceneViewPanel()
{
	return mSceneViewPanel;
}

void MainFrame::CreateMenus()
{
	//Menu
	mMenubar = new wxMenuBar(0);
	
	//Menu File
	mMenuFile = new wxMenu();

	wxMenuItem* m_menuItemNew = new wxMenuItem(mMenuFile, ID_MENU_FILE_NEW, wxString(wxT("New")) + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL);
	mMenuFile->Append(m_menuItemNew);

	wxMenuItem* m_menuItemOpen = new wxMenuItem(mMenuFile, ID_MENU_FILE_OPEN, wxString(wxT("Open...")) + wxT('\t') + wxT("CTRL+O"), wxEmptyString, wxITEM_NORMAL);
	mMenuFile->Append(m_menuItemOpen);

	wxMenuItem* m_menuItemSave = new wxMenuItem(mMenuFile, ID_MENU_FILE_SAVE, wxString(wxT("Save")) + wxT('\t') + wxT("CTRL+S"), wxEmptyString, wxITEM_NORMAL);
	mMenuFile->Append(m_menuItemSave);

	wxMenuItem* m_menuItemSaveAs = new wxMenuItem(mMenuFile, ID_MENU_FILE_SAVE_AS, wxString(wxT("Save As...")) + wxT('\t') + wxT("CTRL+SHIFT+S"), wxEmptyString, wxITEM_NORMAL);
	mMenuFile->Append(m_menuItemSaveAs);

	mMenuFile->AppendSeparator();

	wxMenuItem* m_menuItemExit = new wxMenuItem(mMenuFile, ID_MENU_FILE_EXIT, wxString(wxT("Exit")) + wxT('\t') + wxT("CTRL+X"), wxEmptyString, wxITEM_NORMAL);
	mMenuFile->Append(m_menuItemExit);

	mMenubar->Append(mMenuFile, wxT("File"));
	//Menu File

	//Menu Edit
	mMenuEdit = new wxMenu();

	wxMenuItem* m_menuItemUndo = new wxMenuItem(mMenuFile, ID_MENU_EDIT_UNDO, wxString(wxT("Undo")) + wxT('\t') + wxT("CTRL+Z"), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemUndo);

	wxMenuItem* m_menuItemRedo = new wxMenuItem(mMenuFile, ID_MENU_EDIT_REDO, wxString(wxT("Redo")) + wxT('\t') + wxT("CTRL+Y"), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemRedo);

	mMenuEdit->AppendSeparator();

	wxMenuItem* m_menuItemDelete = new wxMenuItem(mMenuFile, ID_MENU_EDIT_DELETE, wxString(wxT("Delete")) + wxT('\t') + wxT("Del"), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemDelete);

	wxMenuItem* m_menuItemClone = new wxMenuItem(mMenuFile, ID_MENU_EDIT_CLONE, wxString(wxT("Clone")) + wxT('\t') + wxT("CTRL+C"), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemClone);

	mMenuEdit->AppendSeparator();

	wxMenuItem* m_menuItemHide = new wxMenuItem(mMenuFile, ID_MENU_EDIT_HIDE, wxString(wxT("Hide Selection")) + wxT('\t') + wxT("H"), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemHide);

	wxMenuItem* m_menuItemUnhideAll = new wxMenuItem(mMenuFile, ID_MENU_EDIT_UNHIDE_ALL, wxString(wxT("Unhide All")) + wxT('\t') + wxT("CTRL+H"), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemUnhideAll);

	mMenuEdit->AppendSeparator();

	wxMenuItem* m_menuItemSelectAll = new wxMenuItem(mMenuFile, ID_MENU_EDIT_SELECT_ALL, wxString(wxT("Select All")), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemSelectAll);

	wxMenuItem* m_menuItemSelectNone = new wxMenuItem(mMenuFile, ID_MENU_EDIT_SELECT_NONE, wxString(wxT("Select None")), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemSelectNone);

	wxMenuItem* m_menuItemSelectInvert = new wxMenuItem(mMenuFile, ID_MENU_EDIT_SELECT_INVERT, wxString(wxT("Select Invert")), wxEmptyString, wxITEM_NORMAL);
	mMenuEdit->Append(m_menuItemSelectInvert);

	mMenubar->Append(mMenuEdit, wxT("Edit"));
	//Menu Edit

	SetMenuBar(mMenubar);
	//Menu
}

void MainFrame::CreatePanels()
{
	wxAuiPaneInfo paneInfo;
	///////////////////////////////////////////
	mSceneExplorerPanel = new SceneExplorerPanel(this, ID_SCENE_EXPLORER_PANEL, wxDefaultPosition, wxSize(150,-1), wxTAB_TRAVERSAL);

	paneInfo.Caption(wxT("Scene Explorer"));
	paneInfo.Left();
	paneInfo.PinButton(true);
	paneInfo.Dock();
	paneInfo.Resizable();
	paneInfo.FloatingSize(wxDefaultSize);
	paneInfo.DockFixed(false);
	
	mAuiManager.AddPane(mSceneExplorerPanel, paneInfo);
	///////////////////////////////////////////
	mPropertiesPanel = new PropertiesPanel(this, ID_PROPERTIES_PANEL, wxDefaultPosition, wxSize(150,-1), wxTAB_TRAVERSAL);

	paneInfo.Caption(wxT("Properties"));
	paneInfo.Right();
	paneInfo.PinButton(true);
	paneInfo.Dock();
	paneInfo.Resizable();
	paneInfo.FloatingSize(wxDefaultSize);
	paneInfo.DockFixed(false);
	
	mAuiManager.AddPane(mPropertiesPanel, paneInfo);
	///////////////////////////////////////////
	mSceneViewPanel = new SceneViewPanel(this, ID_SCENE_VIEW_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	paneInfo.Caption(wxT("Scene View"));
	paneInfo.CenterPane();

	mAuiManager.AddPane(mSceneViewPanel, paneInfo);
	///////////////////////////////////////////
}

void MainFrame::CreateStatusbar()
{
	mStatusBar = CreateStatusBar(1, wxST_SIZEGRIP, ID_STATUS_BAR);
}

void MainFrame::OnMenuFileNew(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuFileOpen(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuFileSave(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuFileSaveAs(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuFileExit(wxCommandEvent& event)
{
	Close(true);
}

void MainFrame::OnMenuEditUndo(wxCommandEvent& event)
{
	//TODO!!!
}
void MainFrame::OnMenuEditRedo(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuEditDelete(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuEditClone(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuEditHide(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuEditUnhideAll(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuEditSelectAll(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuEditSelectNone(wxCommandEvent& event)
{
	//TODO!!!
}

void MainFrame::OnMenuEditSelectInvert(wxCommandEvent& event)
{
	//TODO!!!
}

