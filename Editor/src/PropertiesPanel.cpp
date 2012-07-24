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

#include <PropertiesPanel.h>

PropertiesPanel::PropertiesPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style): wxPanel()
{
	wxPanel::Create(parent, id, pos, size, style);

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

	m_PropertyGrid = new wxPropertyGrid(this, ID_PROPERTIES_PANEL_PROPERTY_GRID, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED | wxPG_DESCRIPTION | wxPGMAN_DEFAULT_STYLE);

	bSizer->Add(m_PropertyGrid, 1, wxEXPAND, 0);
	
	SetSizer(bSizer);
	Layout();

	if (m_PropertyGrid != NULL)
	{
		m_PropertyGrid->Connect(wxEVT_ENTER_WINDOW,		wxMouseEventHandler(PropertiesPanel::OnMouseEnter), NULL, this);
	}
}

PropertiesPanel::~PropertiesPanel()
{
	if (m_PropertyGrid != NULL)
	{
		m_PropertyGrid->Disconnect(wxEVT_ENTER_WINDOW,	wxMouseEventHandler(PropertiesPanel::OnMouseEnter), NULL, this);
	}
}

void PropertiesPanel::OnMouseEnter(wxMouseEvent &evt)
{
	SetFocus();
}

