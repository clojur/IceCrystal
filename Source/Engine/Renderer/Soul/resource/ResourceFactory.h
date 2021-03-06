/*
 * Soul: a small object-oriented OpenGL Rendering Kernel.
 */
/*
 * Authors: JiangSi.
 */

#ifndef _Soul_RESOURCE_FACTORY_H_
#define _Soul_RESOURCE_FACTORY_H_

#include <exception>
#include <map>

#include "Soul/resource/Resource.h"

namespace Soul
{

/**
 * A Resource factory, creates resources from ResourceDescriptor. There is only
 * one instance of this class, which registers a creation function for each
 * %resource type name.
 *
 * @ingroup resource
 */
class Soul_API ResourceFactory
{
public:
    /**
     * A function that creates a Resource from a ResourceDescriptor.
     *
     * @param manager the manager that will manage the created %resource.
     * @param name the %resource name.
     * @param desc the %resource descriptor.
     * @param e an optional XML element providing contextual information (such
     *      as the XML element in which the %resource descriptor was found).
     * @return the created %resource.
     */
    typedef ptr<Object> (*createFunc) (ptr<ResourceManager> manager, const std::string &name,
        ptr<ResourceDescriptor> desc, const TiXmlElement *e);

    /**
     * Returns the unique instance of this class.
     */
    static ResourceFactory *getInstance();

    /**
     * Registers a new %resource type with this factory.
     *
     * @param type a %resource type, as it appears in the tag of a
     *      ResourceDescriptor (e.g. texture1D, texture2D, shader, program, etc).
     * @param f a function that can create %resources of this type.
     */
    void addType(const std::string &type, createFunc f);

    /**
     * Creates a Resource from the given ResourceDescriptor.
     *
     * @param manager the manager that will manage the created %resource.
     * @param name the %resoure name.
     * @param desc the %resource descriptor.
     * @param e an optional XML element providing contextual information (such
     *      as the XML element in which the %resource descriptor was found).
     * @return the created %resource.
     */
    ptr<Object> create(ptr<ResourceManager> manager, const std::string &name,
        ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL);

    /**
     * Utility function to dump all the types of resources
     * Used for debugging
     * TODO: remove?
     * */
    
    void dumpTypes();

    /**
     * A utility template to automate the registration of new %resource types.
     * @tparam t a %resource type (e.g. texture1D, shader, mesh, etc).
     * @tparam T the corresponding concrete Resource class. This class must be
     *      instantiated for all resources of type t.
     */
    template <const char* t, class T>
    class Type
    {
    public:
        /**
         * Creation function for resources of class T. This function
         * just calls new T(manager, name, desc, e).
         * See ResourceFactory::createFunc
         */
        static ptr<Object> ctor(ptr<ResourceManager> manager, const std::string &name,
                ptr<ResourceDescriptor> desc, const TiXmlElement *e)
        {
            return new T(manager, name, desc, e);
        }

        /**
         * Creates a new Type instance. This constructor registers the creation
         * function #ctor (encapsulating a call to new T) with the %resource
         * type t in the ResourceFactory instance. Hence declaring a static
         * variable of type Type<t, T> automatically registers a new %resource
         * type.
         */
        Type()
        {
            ResourceFactory::getInstance()->addType(std::string(t), ctor);
        }
    };

private:
    /**
     * The registered creation functions. Maps %resource types (such as shader,
     * program, mesh, etc) to %resource creation functions.
     */
    std::map<std::string, createFunc> types;

    /**
     * The unique instance of this class.
     */
    static ResourceFactory *INSTANCE;
};

}

#endif
