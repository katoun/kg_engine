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

#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <EditorConfig.h>

#include <string>

class PropertiesWidget;
class SceneExplorerWidget;
class SceneViewWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget *parent = NULL, Qt::WFlags flags = 0);
	~MainWindow();

	SceneViewWidget* GetSceneViewWidget();

	static QString GetDataPath();

protected:

	PropertiesWidget* mPropertiesWidget;
	SceneExplorerWidget* mSceneExplorerWidget;
	SceneViewWidget* mSceneViewWidget;
	
	//TODO!!!
	QMenuBar* mMenuBar;
	QStatusBar* mStatusBar;

	QMenu* menuFile;
	QMenu* menuEdit;

	QAction* actNew;
	QAction* actOpen;
	QAction* actSave;
	QAction* actSaveAs;
	QAction* actExit;

	QAction* actUndo;
	QAction* actRedo;
	QAction* actDelete;
	QAction* actClone;
	QAction* actHide;
	QAction* actUnhideAll;
	QAction* actSelectAll;
	QAction* actSelectNone;
	QAction* actSelectInvert;
	
	void CreateMenus();
	void CreateWidgets();
	void CreateStatusbar();

public Q_SLOTS:
	void OnMenuFileNew();
	void OnMenuFileOpen();
	void OnMenuFileSave();
	void OnMenuFileSaveAs();

	void OnMenuEditUndo();
	void OnMenuEditRedo();
	void OnMenuEditDelete();
	void OnMenuEditClone();
	void OnMenuEditHide();
	void OnMenuEditUnhideAll();
	void OnMenuEditSelectAll();
	void OnMenuEditSelectNone();
	void OnMenuEditSelectInvert();

private:

	static QString mDataPath;
};

#endif