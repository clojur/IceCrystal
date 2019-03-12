
////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{
namespace GUI
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> Ptr<T> LoadXaml(const char* filename)
{
    Ptr<T> root;

    Ptr<BaseComponent> xaml = LoadXaml(filename);
    if (xaml != 0)
    {
        root = NsDynamicCast< Ptr<T> >(xaml);
        if (root == 0)
        {
            NS_ERROR("LoadXaml('%s'): invalid requested root type", filename);
        }
    }

    return root;
}

}
}
