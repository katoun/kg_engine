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

	QString dataPath = MainWindow::GetDataPath();

	mIconList.push_back(dataPath + "/scene_root.png");			//Index 0
	mIconList.push_back(dataPath + "/scene_gameobject.png");	//Index 1
	mIconList.push_back(dataPath + "/scene_transform.png");		//Index 2
	mIconList.push_back(dataPath + "/scene_light.png");			//Index 3
	mIconList.push_back(dataPath + "/scene_camera.png");		//Index 4
	mIconList.push_back(dataPath + "/scene_model.png");			//Index 5
	mIconList.push_back(dataPath + "/scene_sound.png");			//Index 6
	mIconList.push_back(dataPath + "/scene_listener.png");		//Index 7
	mIconList.push_back(dataPath + "/scene_body.png");			//Index 8

	QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)NULL, QStringList(QString("Scene")));
	item->setIcon(0, QIcon( mIconList[0]));
    item->font(0).setBold(true);
    mTreeWidget->insertTopLevelItem(0, item);

	connect(mTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
}

SceneExplorerWidget::~SceneExplorerWidget()
{
	engine::EngineManager::getInstance().removeEngineEventReceiver(this);
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
	if (mTreeWidget != NULL)
	{
		std::map<unsigned int, game::GameObject*> gameObjects = game::GameManager::getInstance().getGameObjects();
		std::map<unsigned int, game::GameObject*>::const_iterator i;
		for (i = gameObjects.begin(); i != gameObjects.end(); ++i)
		{
			game::GameObject* pGameObject = i->second;
			if (pGameObject != NULL)
			{
				std::string name = pGameObject->getName();

				QTreeWidgetItem* gameobjectItem = new QTreeWidgetItem((QTreeWidget*)NULL, QStringList(QString::fromStdString(name)));
				gameobjectItem->setIcon(0, QIcon( mIconList[1]));
				mTreeWidget->topLevelItem(0)->addChild(gameobjectItem);

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

						QTreeWidgetItem* componentItem = new QTreeWidgetItem((QTreeWidget*)NULL, QStringList(QString::fromStdString(name)));
						componentItem->setIcon(0, QIcon( mIconList[imageID]));
						gameobjectItem->addChild(componentItem);
					}
				}
			}
		}
	}
}

void SceneExplorerWidget::selectionChanged()
{
	//TODO!!!
}