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

#include <Editor.h>
#include <MainFrame.h>

IMPLEMENT_APP(EditorApp);

bool EditorApp::OnInit()
{
	wxApp::OnInit();

	MainFrame* frame = new MainFrame(NULL);
	frame->SetIcon(wxICON(amain));
	frame->Show();

	mEngineManager = new engine::EngineManager();

	if (mEngineManager != nullptr)
	{
		//mEngineManager->setMainWindowID((HWND)frame->GetHandle());//TODO!!!

		mEngineManager->initialize();
		mEngineManager->start();
	}

	return true;
}

int EditorApp::OnRun()
{
	if (mEngineManager != nullptr && mEngineManager->isRunning())
		mEngineManager->update(0);

	return wxApp::OnRun();
}

int EditorApp::OnExit()
{
	if (mEngineManager != nullptr)
		mEngineManager->uninitialize();

	return wxApp::OnExit();
}