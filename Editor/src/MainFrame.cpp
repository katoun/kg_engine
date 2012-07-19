/*
-----------------------------------------------------------------------------
KG game engine (https://github.com/katoun/kg_engine) is made available under the MIT License.

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

MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	mAuiManager.SetManagedWindow(this);

	///////////////////////////////////////////
	m_sceneExplorer = new wxTreeCtrl(this, ID_SCENE_EXPLORER, wxDefaultPosition, wxSize(150,-1), wxTR_DEFAULT_STYLE);

	m_propertyWindow = new wxPropertyGrid(this, ID_PROPERTY_WINDOW, wxDefaultPosition, wxSize(150,-1), wxTR_DEFAULT_STYLE);

	m_statusBar = this->CreateStatusBar(1, wxST_SIZEGRIP, ID_STATUS_BAR);

	//Menu
	m_menubar = new wxMenuBar(0);
	
	//Menu File
	m_menuFile = new wxMenu();

	wxMenuItem* m_menuItemNew = new wxMenuItem(m_menuFile, ID_MENU_FILE_NEW, wxString(wxT("New")) + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL);
	m_menuFile->Append(m_menuItemNew);

	wxMenuItem* m_menuItemOpen = new wxMenuItem(m_menuFile, ID_MENU_FILE_OPEN, wxString(wxT("Open...")) + wxT('\t') + wxT("CTRL+O"), wxEmptyString, wxITEM_NORMAL);
	m_menuFile->Append(m_menuItemOpen);

	wxMenuItem* m_menuItemSave = new wxMenuItem(m_menuFile, ID_MENU_FILE_SAVE, wxString(wxT("Save")) + wxT('\t') + wxT("CTRL+S"), wxEmptyString, wxITEM_NORMAL);
	m_menuFile->Append(m_menuItemSave);

	wxMenuItem* m_menuItemSaveAs = new wxMenuItem(m_menuFile, ID_MENU_FILE_SAVE_AS, wxString(wxT("Save As...")) + wxT('\t') + wxT("CTRL+SHIFT+S"), wxEmptyString, wxITEM_NORMAL);
	m_menuFile->Append(m_menuItemSaveAs);

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuItemExit = new wxMenuItem(m_menuFile, ID_MENU_FILE_EXIT, wxString(wxT("Exit")) + wxT('\t') + wxT("CTRL+X"), wxEmptyString, wxITEM_NORMAL);
	m_menuFile->Append(m_menuItemExit);

	m_menubar->Append(m_menuFile, wxT("File"));
	//Menu File

	//Menu Edit
	m_menuEdit = new wxMenu();

	wxMenuItem* m_menuItemUndo = new wxMenuItem(m_menuFile, ID_MENU_EDIT_UNDO, wxString(wxT("Undo")) + wxT('\t') + wxT("CTRL+Z"), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemUndo);

	wxMenuItem* m_menuItemRedo = new wxMenuItem(m_menuFile, ID_MENU_EDIT_REDO, wxString(wxT("Redo")) + wxT('\t') + wxT("CTRL+Y"), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemRedo);

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuItemDelete = new wxMenuItem(m_menuFile, ID_MENU_EDIT_DELETE, wxString(wxT("Delete")) + wxT('\t') + wxT("Del"), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemDelete);

	wxMenuItem* m_menuItemClone = new wxMenuItem(m_menuFile, ID_MENU_EDIT_CLONE, wxString(wxT("Clone")) + wxT('\t') + wxT("CTRL+C"), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemClone);

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuItemHide = new wxMenuItem(m_menuFile, ID_MENU_EDIT_HIDE, wxString(wxT("Hide Selection")) + wxT('\t') + wxT("H"), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemHide);

	wxMenuItem* m_menuItemUnhideAll = new wxMenuItem(m_menuFile, ID_MENU_EDIT_UNHIDE_ALL, wxString(wxT("Unhide All")) + wxT('\t') + wxT("CTRL+H"), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemUnhideAll);

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuItemSelectAll = new wxMenuItem(m_menuFile, ID_MENU_EDIT_SELECT_ALL, wxString(wxT("Select All")), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemSelectAll);

	wxMenuItem* m_menuItemSelectNone = new wxMenuItem(m_menuFile, ID_MENU_EDIT_SELECT_NONE, wxString(wxT("Select None")), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemSelectNone);

	wxMenuItem* m_menuItemSelectInvert = new wxMenuItem(m_menuFile, ID_MENU_EDIT_SELECT_INVERT, wxString(wxT("Select Invert")), wxEmptyString, wxITEM_NORMAL);
	m_menuEdit->Append(m_menuItemSelectInvert);

	m_menubar->Append(m_menuEdit, wxT("Edit"));
	//Menu Edit

	this->SetMenuBar(m_menubar);
	//Menu
	///////////////////////////////////////////

	// add the panes to the manager
	wxAuiPaneInfo paneInfo;
	paneInfo.Caption(wxT("Scene Explorer"));
	paneInfo.Left();
	paneInfo.PinButton(true);
	paneInfo.Dock();
	paneInfo.Resizable();
	paneInfo.FloatingSize(wxDefaultSize);
	paneInfo.DockFixed(false);
	
	mAuiManager.AddPane(m_sceneExplorer, paneInfo);

	paneInfo.Caption(wxT("Properties"));
	paneInfo.Right();
	paneInfo.PinButton(true);
	paneInfo.Dock();
	paneInfo.Resizable();
	paneInfo.FloatingSize(wxDefaultSize);
	paneInfo.DockFixed(false);

	mAuiManager.AddPane(m_propertyWindow, paneInfo);

	// tell the manager to "commit" all the changes just made
	mAuiManager.Update();

	////////////////////////////////////////////

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(ID_MENU_FILE_NEW,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileNew));
	this->Connect(ID_MENU_FILE_OPEN,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileOpen));
	this->Connect(ID_MENU_FILE_SAVE,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSave));
	this->Connect(ID_MENU_FILE_SAVE_AS,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSaveAs));
	this->Connect(ID_MENU_FILE_EXIT,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileExit));

	this->Connect(ID_MENU_EDIT_UNDO,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUndo));
	this->Connect(ID_MENU_EDIT_REDO,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditRedo));
	this->Connect(ID_MENU_EDIT_DELETE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditDelete));
	this->Connect(ID_MENU_EDIT_CLONE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditClone));
	this->Connect(ID_MENU_EDIT_HIDE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditHide));
	this->Connect(ID_MENU_EDIT_UNHIDE_ALL,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUnhideAll));
	this->Connect(ID_MENU_EDIT_SELECT_ALL,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectAll));
	this->Connect(ID_MENU_EDIT_SELECT_NONE,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectNone));
	this->Connect(ID_MENU_EDIT_SELECT_INVERT,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectInvert));
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect(ID_MENU_FILE_NEW,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileNew));
	this->Disconnect(ID_MENU_FILE_OPEN,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileOpen));
	this->Disconnect(ID_MENU_FILE_SAVE,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSave));
	this->Disconnect(ID_MENU_FILE_SAVE_AS,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileSaveAs));
	this->Disconnect(ID_MENU_FILE_EXIT,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuFileExit));

	this->Disconnect(ID_MENU_EDIT_UNDO,				wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUndo));
	this->Disconnect(ID_MENU_EDIT_REDO,				wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditRedo));
	this->Disconnect(ID_MENU_EDIT_DELETE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditDelete));
	this->Disconnect(ID_MENU_EDIT_CLONE,			wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditClone));
	this->Disconnect(ID_MENU_EDIT_HIDE,				wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditHide));
	this->Disconnect(ID_MENU_EDIT_UNHIDE_ALL,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditUnhideAll));
	this->Disconnect(ID_MENU_EDIT_SELECT_ALL,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectAll));
	this->Disconnect(ID_MENU_EDIT_SELECT_NONE,		wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectNone));
	this->Disconnect(ID_MENU_EDIT_SELECT_INVERT,	wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMenuEditSelectInvert));

	mAuiManager.UnInit();
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

