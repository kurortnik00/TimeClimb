#include "stdafx.h"
#include "GameObjectManager.h"
#include "Game.h"

//Класс отвечающий за удаление, добавление и хранение объектов а так же за их Общую отрисовку и обновление 

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	std::for_each(_gameObjects.begin(), _gameObjects.end(), GameObjectDeallocator());
}

void GameObjectManager::Add(std::string name, VisibleGameObject* gameObject)
{
	_gameObjects.insert(std::pair<std::string, VisibleGameObject*>(name, gameObject));
}

void GameObjectManager::Remove(std::string name)
{
	std::map<std::string, VisibleGameObject*>::iterator results = _gameObjects.find(name);
	if (results != _gameObjects.end())
	{
		delete results->second;
		_gameObjects.erase(results);
	}
}

VisibleGameObject* GameObjectManager::Get(std::string name) const
{
	std::map<std::string, VisibleGameObject*>::const_iterator results = _gameObjects.find(name);
	if (results == _gameObjects.end())
		return NULL;
	return results->second;

}

int GameObjectManager::GetObjectCount() const
{
	return _gameObjects.size();
}


void GameObjectManager::DrawAll(sf::RenderWindow& renderWindow)
{
	std::map<std::string, VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	while (itr != _gameObjects.end())
	{
		if (itr->second->getPlased()) itr->second->Draw(renderWindow);
		itr++;
	}
}

void GameObjectManager::UpdateAll(sf::Event& event)
{
	std::map<std::string, VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	
	while (itr != _gameObjects.end())
	{

		if (itr->first == "0") {
			itr->second->setUnbreakable(false);
		}


		itr->second->Update(event);

		if (itr->second->getClickState()) {				//Была ли кликнута цель
			itr++;
			if (itr != _gameObjects.end()) itr->second->setUnbreakable(false);		//Сделать кликаемым следующий таргет
		}
		else itr++;
	}
}

void GameObjectManager::setAllPlased()
{
	std::map<std::string, VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	while (itr != _gameObjects.end())
	{
		itr->second->setPlased(true);
		itr++;
	}
}