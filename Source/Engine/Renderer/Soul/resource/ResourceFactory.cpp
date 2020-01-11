/*
 * Soul: a small object-oriented OpenGL Rendering Kernel.
/*
 * Main authors: JiangSi.
 */

#include "Soul/resource/ResourceFactory.h"

#include "Soul/resource/ResourceManager.h"

using namespace std;

namespace Soul
{

ResourceFactory *ResourceFactory::INSTANCE = NULL;

ResourceFactory *ResourceFactory::getInstance()
{
    if (INSTANCE == NULL) {
        INSTANCE = new ResourceFactory();
    }
    return INSTANCE;
}

void ResourceFactory::addType(const string &type, createFunc f)
{
    types[type] = f;
}

void ResourceFactory::dumpTypes()
{
	std::map<std::string, createFunc>::iterator i;
	for(i = types.begin(); i != types.end(); ++i)
		cout << i->first << endl;


}


ptr<Object> ResourceFactory::create(ptr<ResourceManager> manager, const string &name,
        ptr<ResourceDescriptor> desc, const TiXmlElement *e)
{
    e = e == NULL ? desc->descriptor : e;
    map<string, createFunc>::iterator i = types.find(e->ValueStr());
    if (i != types.end()) {
        return i->second(manager, name, desc, e);
    } else {
        if (Logger::ERROR_LOGGER != NULL) {
            Resource::log(Logger::ERROR_LOGGER, desc, e, "Unknown resource type '" + e->ValueStr() + "'");
        }
        throw exception();
    }
}

}
