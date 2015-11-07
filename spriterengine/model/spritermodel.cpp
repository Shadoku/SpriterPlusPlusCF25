#include "spritermodel.h"

#include "../entity/entityinstance.h"

#include "../override/imagefile.h"
#include "../override/soundfile.h"
#include "../override/filefactory.h"
#include "../override/objectfactory.h"

#include "../file/filereference.h"

namespace SpriterEngine
{

	SpriterModel::SpriterModel(const std::string & fileName, FileFactory * newFileFactory, ObjectFactory * newObjectFactory) :
		SpriterModel(newFileFactory, newObjectFactory)
	{
		loadFile(fileName);
	}

	SpriterModel::SpriterModel(FileFactory * newFileFactory, ObjectFactory * newObjectFactory) :
		fileFactory(newFileFactory),
		objectFactory(newObjectFactory),
		loader(newFileFactory->newScmlDocumentWrapper(), newFileFactory->newSconDocumentWrapper())
	{
	}

	SpriterModel::~SpriterModel()
	{
		for (auto& it : files)
		{
			delete it;
		}
		for (auto& it : entities)
		{
			delete it;
		}

		if (fileFactory)
		{
			delete fileFactory;
		}

		if (objectFactory)
		{
			delete objectFactory;
		}
	}

	EntityInstance *SpriterModel::getNewEntityInstance(int entityId)
	{
		if (entityId < entities.size())
		{
			return entities.at(entityId)->getNewEntityInstance(this, objectFactory);
		}
		else
		{
			return 0;
		}
	}

	EntityInstance * SpriterModel::getNewEntityInstance(EntityIdVector * entityIds)
	{
		EntityInstance *newEntityInstance = new EntityInstance();
		for (auto& it : *entityIds)
		{
			Entity *entity = getEntity(it);
			if (entity)
			{
				newEntityInstance->appendEntity(this, entity, objectFactory);
			}
			else
			{
				// error
			}
		}
		return newEntityInstance;
	}

	EntityInstance * SpriterModel::getNewEntityInstance(std::string entityName)
	{
		for (auto& it : entities)
		{
			if (it->getName() == entityName)
			{
				return it->getNewEntityInstance(this, objectFactory);
			}
		}
		// error
		return 0;
	}

	void SpriterModel::setupFileReferences(FileReferenceVector *fileReferences)
	{
		for (auto& it : files)
		{
			fileReferences->push_back(new FileReference(it));
		}
	}

	Entity *SpriterModel::pushBackEntity(std::string entityName)
	{
		entities.push_back(new Entity(entityName, entities.size(), &files));
		return entities.back();
	}

	void SpriterModel::pushBackImageFile(std::string initialFilePath, point initialDefaultPivot)
	{
		files.push_back(fileFactory->newImageFile(initialFilePath, initialDefaultPivot));
	}

	void SpriterModel::pushBackSoundFile(std::string initialFilePath)
	{
		files.push_back(fileFactory->newSoundFile(initialFilePath));
	}

	void SpriterModel::pushBackTag(std::string newTag)
	{
		tags.push_back(newTag);
	}

	int SpriterModel::fileCount()
	{
		return files.size();
	}

	File * SpriterModel::getFileAtIndex(int fileIndex)
	{
		if (fileIndex >= 0 && fileIndex < files.size())
		{
			return files.at(fileIndex);
		}
		else
		{
			return 0;
		}
	}

	const std::string * SpriterModel::getTag(int tagIndex)
	{
		if (tagIndex < tags.size())
		{
			return &tags.at(tagIndex);
		}
		else
		{
			// error
			return 0;
		}
	}

	void SpriterModel::loadFile(const std::string & fileName)
	{
		loader.loadFile(this, fileName);
	}

	Entity * SpriterModel::getEntity(int entityId)
	{
		if (entityId < entities.size())
		{
			return entities.at(entityId);
		}
		else
		{
			// error
			return 0;
		}
	}

}
