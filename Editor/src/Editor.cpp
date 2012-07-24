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

#include <Editor.h>
#include <MainFrame.h>
#include <SceneViewPanel.h>
#include <engine/EngineManager.h>
#include <engine/EngineSettings.h>
#include <render/RenderManager.h>

IMPLEMENT_APP(EditorApp);

bool EditorApp::OnInit()
{
	wxApp::OnInit();

	wxImage::AddHandler(new wxPNGHandler);

	m_MainFrame = new MainFrame(NULL);
	m_MainFrame->SetIcon(wxICON(amain));
	m_MainFrame->Show();
	m_MainFrame->UpdateWindowUI(wxUPDATE_UI_FROMIDLE);

	Connect(wxEVT_IDLE, wxIdleEventHandler(EditorApp::OnIdle));

	SetTopWindow(m_MainFrame);

	SceneViewPanel* pSceneViewPanel = m_MainFrame->GetSceneViewPanel();
	if (pSceneViewPanel != NULL)
	{
		engine::EngineSettings::getInstance().setMainWindowID((HWND)pSceneViewPanel->GetHandle());
		engine::EngineManager::getInstance().initialize();
		pSceneViewPanel->SetRenderWindow(render::RenderManager::getInstance().getMainWindow());

		engine::EngineManager::getInstance().start();
	}

	return true;
}

void EditorApp::OnIdle(wxIdleEvent& event)
{
	if (engine::EngineManager::getInstance().isRunning())
		engine::EngineManager::getInstance().update(0);
}

int EditorApp::OnExit()
{
	engine::EngineManager::getInstance().uninitialize();

	Disconnect(wxEVT_IDLE, wxIdleEventHandler(EditorApp::OnIdle));

	return wxApp::OnExit();
}