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

#include <MainWindow.h>
//#include <SceneExplorerPanel.h>
//#include <PropertiesPanel.h>
//#include <SceneViewPanel.h>

#include <string>

#include <Poco/XML/XML.h>
#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>

std::string MainWindow::mDataPath = "";

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags): QMainWindow(parent, flags)
{
	mSceneViewPanel = NULL;
	mPropertiesPanel = NULL;
	mSceneExplorerPanel = NULL;

	QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/Editor.png")), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

	///////////////////////////////////////////
	std::string configFile;
#ifdef _DEBUG
	configFile = "Editor_d.xml";
#else
	configFile = "Editor.xml";
#endif

	mDataPath = "";

	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(configFile);
	}
	catch(...) {}

	if (pConf->has("DataPath[@value]"))
		mDataPath = pConf->getString("DataPath[@value]");

	///////////////////////////////////////////
	
	CreateMenus();
	CreatePanels();
	CreateStatusbar();

	repaint();

	// Set editor default theme
	//TODO!!!
	///////////////////////////////////////////

	// Connect Events
}

MainWindow::~MainWindow()
{
	// Disconnect Events
}

SceneViewPanel* MainWindow::GetSceneViewPanel()
{
	return mSceneViewPanel;
}

std::string MainWindow::GetDataPath()
{
	return mDataPath;
}

void MainWindow::CreateMenus()
{
	mMenuBar = new QMenuBar(this);
	mMenuBar->setObjectName(QString::fromUtf8("mMenuBar"));
	mMenuBar->setGeometry(QRect(0, 0, 616, 22));

	menuFile = new QMenu(tr("File"), mMenuBar);
	menuFile->setObjectName(QString::fromUtf8("menuFile"));
	mMenuBar->addAction(menuFile->menuAction());

	//TODO!!

	setMenuBar(mMenuBar);
}

void MainWindow::CreatePanels()
{
	//TODO!!!
}

void MainWindow::CreateStatusbar()
{
	mStatusBar = new QStatusBar(this);
    mStatusBar->setObjectName(QString::fromUtf8("mStatusBar"));

    setStatusBar(mStatusBar);
}

void MainWindow::OnMenuFileNew()
{
	//TODO!!!
}

void MainWindow::OnMenuFileOpen()
{
	//TODO!!!
}

void MainWindow::OnMenuFileSave()
{
	//TODO!!!
}

void MainWindow::OnMenuFileSaveAs()
{
	//TODO!!!
}

void MainWindow::OnMenuFileExit()
{
	//TODO!!!
}

void MainWindow::OnMenuEditUndo()
{
	//TODO!!!
}
void MainWindow::OnMenuEditRedo()
{
	//TODO!!!
}

void MainWindow::OnMenuEditDelete()
{
	//TODO!!!
}

void MainWindow::OnMenuEditClone()
{
	//TODO!!!
}

void MainWindow::OnMenuEditHide()
{
	//TODO!!!
}

void MainWindow::OnMenuEditUnhideAll()
{
	//TODO!!!
}

void MainWindow::OnMenuEditSelectAll()
{
	//TODO!!!
}

void MainWindow::OnMenuEditSelectNone()
{
	//TODO!!!
}

void MainWindow::OnMenuEditSelectInvert()
{
	//TODO!!!
}

