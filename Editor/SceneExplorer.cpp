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

#include <SceneExplorer.h>
#include <MainWindow.h>
#include <game/GameObject.h>
#include <game/Transform.h>
#include <game/ComponentDefines.h>
#include <game/GameManager.h>
#include <engine/EngineManager.h>

SceneExplorerWidget::SceneExplorerWidget(QWidget *parent): QWidget(parent)
{
	engine::EngineManager::getInstance().addEngineEventReceiver(this);

	mTreeWidget = new QTreeWidget(this);

	QVBoxLayout* boxlayout = new QVBoxLayout(this);
	boxlayout->setMargin(0);
	boxlayout->addWidget(mTreeWidget);

	//add root node here!!!

	connect(mTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	/*if (mTreeCtrl != NULL)
	{
		mTreeImages = new wxImageList(16, 16, true, 9);
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_root.png"), wxBITMAP_TYPE_PNG));			//Index 0
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_gameobject.png"), wxBITMAP_TYPE_PNG));	//Index 1
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_transform.png"), wxBITMAP_TYPE_PNG));	//Index 2
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_light.png"), wxBITMAP_TYPE_PNG));		//Index 3
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_camera.png"), wxBITMAP_TYPE_PNG));		//Index 4
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_model.png"), wxBITMAP_TYPE_PNG));		//Index 5
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_sound.png"), wxBITMAP_TYPE_PNG));		//Index 6
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_listener.png"), wxBITMAP_TYPE_PNG));		//Index 7
		mTreeImages->Add(wxBitmap(MainFrame::GetDataPath() + wxT("/") + wxT("scene_body.png"), wxBITMAP_TYPE_PNG));			//Index 8

		mTreeCtrl->AssignImageList(mTreeImages);

		//Insert root item
		wxTreeItemId item = mTreeCtrl->AddRoot(wxString("Scene"), 0);
		mTreeCtrl->SetItemBold(item);

		mTreeCtrl->Connect(wxEVT_ENTER_WINDOW,		wxMouseEventHandler(SceneExplorerPanel::OnMouseEnter), NULL, this);
	}*/
}

SceneExplorerWidget::~SceneExplorerWidget()
{
	engine::EngineManager::getInstance().removeEngineEventReceiver(this);

	/*if (mTreeCtrl != NULL)
	{
		mTreeCtrl->Disconnect(wxEVT_ENTER_WINDOW,	wxMouseEventHandler(SceneExplorerPanel::OnMouseEnter), NULL, this);
	}*/
}

void SceneExplorerWidget::engineInitialized()
{	
}

void SceneExplorerWidget::engineUninitialized()
{
	if (mTreeWidget != NULL)
	{
		mTreeWidget->clear();
	}
}

void SceneExplorerWidget::engineStarted()
{
	/*if (mTreeCtrl != NULL)
	{
		std::map<unsigned int, game::GameObject*> gameObjects = game::GameManager::getInstance().getGameObjects();
		std::map<unsigned int, game::GameObject*>::const_iterator i;
		for (i = gameObjects.begin(); i != gameObjects.end(); ++i)
		{
			game::GameObject* pGameObject = i->second;
			if (pGameObject != NULL)
			{
				std::string name = pGameObject->getName();

				wxTreeItemId gameobjectItem = mTreeCtrl->AppendItem(mTreeCtrl->GetRootItem(), wxString(name), 1);

				std::map<unsigned int, game::Component*> components = pGameObject->getComponents();
				std::map<unsigned int, game::Component*>::const_iterator j;
				for (j = components.begin(); j != components.end(); ++j)
				{
					game::Component* pComponent = j->second;
					if (pComponent != NULL)
					{
						std::string name = pComponent->getName();

						int imageID = 2;
						unsigned int type = pComponent->getType();
						switch(type)
						{
						case game::COMPONENT_TYPE_TRANSFORM:
							imageID = 2;
							break;
						case game::COMPONENT_TYPE_LIGHT:
							imageID = 3;
							break;
						case game::COMPONENT_TYPE_CAMERA:
							imageID = 4;
							break;
						case game::COMPONENT_TYPE_MODEL:
							imageID = 5;
							break;
						case game::COMPONENT_TYPE_SOUND:
							imageID = 6;
							break;
						case game::COMPONENT_TYPE_LISTENER:
							imageID = 7;
							break;
						case game::COMPONENT_TYPE_BODY:
							imageID = 8;
							break;
						}

						wxTreeItemId componentItem = mTreeCtrl->AppendItem(gameobjectItem, wxString(name), imageID);
					}
				}
			}
		}
	}*/
}

void SceneExplorerWidget::selectionChanged()
{
	//TODO!!!
}