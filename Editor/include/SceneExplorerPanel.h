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

#ifndef _SCENE_EXPLORER_PANEL_H_
#define _SCENE_EXPLORER_PANEL_H_

#include <EditorConfig.h>
#include <engine/EngineEvent.h>
#include <engine/EngineEventReceiver.h>

class SceneExplorerPanel : public wxPanel, public engine::EngineEventReceiver
{
public:

	SceneExplorerPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSUNKEN_BORDER | wxTAB_TRAVERSAL);

	~SceneExplorerPanel();

	void engineInitialized();

	void engineUninitialized();

	void engineStarted();

protected:

	void OnMouseEnter(wxMouseEvent &evt);

	wxImageList* mTreeImages;
	wxTreeCtrl* mTreeCtrl;
};

#endif