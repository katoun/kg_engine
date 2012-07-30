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

QString MainWindow::mDataPath = "";

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags): QMainWindow(parent, flags)
{
	mSceneViewWidget = NULL;
	mPropertiesWidget = NULL;
	mSceneExplorerWidget = NULL;

	mDataPath = "";

	///////////////////////////////////////////
	std::string configFile;
#ifdef _DEBUG
	configFile = "Editor_d.xml";
#else
	configFile = "Editor.xml";
#endif

	Poco::AutoPtr<Poco::Util::XMLConfiguration> pConf(new Poco::Util::XMLConfiguration());
	try
	{
		pConf->load(configFile);
	}
	catch(...) {}

	if (pConf->has("DataPath[@value]"))
	{
		std::string path = pConf->getString("DataPath[@value]");
		mDataPath = QString::fromStdString(path);
	}

	///////////////////////////////////////////

	QIcon icon;
    icon.addPixmap(QPixmap(mDataPath + "/Editor.png"), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

	QFile file(mDataPath + "/Editor.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	setStyleSheet(styleSheet);

	///////////////////////////////////////////

	resize(800, 600);
	
	CreateMenus();
	CreateWidgets();
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

SceneViewWidget* MainWindow::GetSceneViewWidget()
{
	return mSceneViewWidget;
}

QString MainWindow::GetDataPath()
{
	return mDataPath;
}

void MainWindow::CreateMenus()
{
	mMenuBar = new QMenuBar(this);
	mMenuBar->setObjectName(QString::fromUtf8("mMenuBar"));
	mMenuBar->setGeometry(QRect(0, 0, 616, 22));

	// Menu File
	menuFile = new QMenu(tr("File"), mMenuBar);
	menuFile->setObjectName(QString::fromUtf8("menuFile"));
	mMenuBar->addAction(menuFile->menuAction());

	actNew = new QAction(tr("New"), this);
	actNew->setShortcuts(QKeySequence::New);
	actNew->setStatusTip(tr("Create a new scene"));
	actNew->setIcon(QIcon(mDataPath + "/filenew.png"));
	menuFile->addAction(actNew);

	actOpen = new QAction(tr("Open"), this);
	actOpen->setShortcuts(QKeySequence::Open);
	actOpen->setStatusTip(tr("Open a scene"));
	actOpen->setIcon(QIcon(mDataPath + "/fileopen.png"));
	menuFile->addAction(actOpen);

	actSave = new QAction(tr("Save"), this);
	actSave->setShortcuts(QKeySequence::Save);
	actSave->setStatusTip(tr("Save scene"));
	actSave->setIcon(QIcon(mDataPath + "/filesave.png"));
	menuFile->addAction(actSave);

	actSaveAs = new QAction(tr("Save As"), this);
	actSaveAs->setShortcuts(QKeySequence::SaveAs);
	actSaveAs->setStatusTip(tr("Save scene As"));
	actSaveAs->setIcon(QIcon(mDataPath + "/filesaveas.png"));
	menuFile->addAction(actSaveAs);

	actExit = new QAction(tr("Exit"), this);
	actExit->setShortcuts(QKeySequence::Quit);
	actExit->setStatusTip(tr("Exit Application"));
	actExit->setIcon(QIcon(mDataPath + "/exit.png"));
	menuFile->addAction(actExit);
	// Menu File

	// Menu Edit
	menuEdit = new QMenu(tr("Edit"), mMenuBar);
	menuEdit->setObjectName(QString::fromUtf8("menuFile"));
	mMenuBar->addAction(menuEdit->menuAction());

	actUndo = new QAction(tr("Undo"), this);
	actUndo->setShortcuts(QKeySequence::Undo);
	actUndo->setStatusTip(tr("Undo"));
	actUndo->setIcon(QIcon(mDataPath + "/undo.png"));
	menuEdit->addAction(actUndo);

	actRedo = new QAction(tr("Redo"), this);
	actRedo->setShortcuts(QKeySequence::Redo);
	actRedo->setStatusTip(tr("Redo"));
	actRedo->setIcon(QIcon(mDataPath + "/redo.png"));
	menuEdit->addAction(actRedo);

	menuEdit->addSeparator();

	actDelete = new QAction(tr("Delete"), this);
	//actDelete->setShortcuts(QKeySequence::Delete);
	actDelete->setStatusTip(tr("Delete"));
	//actDelete->setIcon(QIcon(mDataPath + "/delete.png"));
	menuEdit->addAction(actDelete);

	actClone = new QAction(tr("Clone"), this);
	//actClone->setShortcuts(QKeySequence::Clone);
	actClone->setStatusTip(tr("Clone"));
	//actClone->setIcon(QIcon(mDataPath + "/clone.png"));
	menuEdit->addAction(actClone);

	menuEdit->addSeparator();

	actHide = new QAction(tr("Hide"), this);
	//actHide->setShortcuts(QKeySequence::Hide);
	actHide->setStatusTip(tr("Hide"));
	//actHide->setIcon(QIcon(mDataPath + "/hide.png"));
	menuEdit->addAction(actHide);

	actUnhideAll = new QAction(tr("Unhide All"), this);
	//actUnhideAll->setShortcuts(QKeySequence::UnhideAll);
	actUnhideAll->setStatusTip(tr("Unhide All"));
	//actUnhideAll->setIcon(QIcon(mDataPath + "/unhide_all.png"));
	menuEdit->addAction(actUnhideAll);

	menuEdit->addSeparator();

	actSelectAll = new QAction(tr("Select All"), this);
	//actSelectAll->setShortcuts(QKeySequence::SelectAll);
	actSelectAll->setStatusTip(tr("Select All"));
	//actSelectAll->setIcon(QIcon(mDataPath + "/select_all.png"));
	menuEdit->addAction(actSelectAll);

	actSelectNone = new QAction(tr("Select None"), this);
	//actSelectNone->setShortcuts(QKeySequence::SelectNone);
	actSelectNone->setStatusTip(tr("Select None"));
	//actSelectNone->setIcon(QIcon(mDataPath + "/select_none.png"));
	menuEdit->addAction(actSelectNone);

	actSelectInvert = new QAction(tr("Select Invert"), this);
	//actSelectInvert->setShortcuts(QKeySequence::SelectInvert);
	actSelectInvert->setStatusTip(tr("Select Invert"));
	//actSelectInvert->setIcon(QIcon(mDataPath + "/select_invert.png"));
	menuEdit->addAction(actSelectInvert);
	// Menu Edit

	setMenuBar(mMenuBar);

	connect(actNew, SIGNAL(triggered()), this, SLOT(OnMenuFileNew()));
	connect(actOpen, SIGNAL(triggered()), this, SLOT(OnMenuFileOpen()));
	connect(actSave, SIGNAL(triggered()), this, SLOT(OnMenuFileSave()));
	connect(actSaveAs, SIGNAL(triggered()), this, SLOT(OnMenuFileSaveAs()));
	connect(actExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(actUndo, SIGNAL(triggered()), this, SLOT(OnMenuEditUndo()));
	connect(actRedo, SIGNAL(triggered()), this, SLOT(OnMenuEditRedo()));
	connect(actDelete, SIGNAL(triggered()), this, SLOT(OnMenuEditDelete()));
	connect(actClone, SIGNAL(triggered()), this, SLOT(OnMenuEditClone()));
	connect(actHide, SIGNAL(triggered()), this, SLOT(OnMenuEditHide()));
	connect(actUnhideAll, SIGNAL(triggered()), this, SLOT(OnMenuEditUnhideAll()));
	connect(actSelectAll, SIGNAL(triggered()), this, SLOT(OnMenuEditSelectAll()));
	connect(actSelectNone, SIGNAL(triggered()), this, SLOT(OnMenuEditSelectNone()));
	connect(actSelectInvert, SIGNAL(triggered()), this, SLOT(OnMenuEditSelectInvert()));
}

void MainWindow::CreateWidgets()
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

