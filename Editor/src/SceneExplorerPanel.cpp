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

#include <SceneExplorerPanel.h>

SceneExplorerPanel::SceneExplorerPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style): wxPanel()
{
	wxPanel::Create(parent, id, pos, size, style);

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

	m_TreeCtrl = new wxTreeCtrl(this, ID_SCENE_EXPLORER_TREE_CTRL, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
	bSizer->Add(m_TreeCtrl, 0, wxALL | wxEXPAND, 0);

	SetSizer(bSizer);
	Layout();
}

SceneExplorerPanel::~SceneExplorerPanel()
{
}
