////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_APPLICATIONCOMMANDS_H__
#define __GUI_APPLICATIONCOMMANDS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class RoutedUICommand;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a standard set of application related commands.
///
/// * *CancelPrintCommand*
/// * *CloseCommand*
/// * *ContextMenuCommand*
/// * *CopyCommand*
/// * *CorrectionListCommand*
/// * *CutCommand*
/// * *DeleteCommand*
/// * *FindCommand*
/// * *HelpCommand*
/// * *NewCommand*
/// * *OpenCommand*
/// * *PasteCommand*
/// * *PrintCommand*
/// * *PrintPreviewCommand*
/// * *PropertiesCommand*
/// * *RedoCommand*
/// * *ReplaceCommand*
/// * *SaveCommand*
/// * *SaveAsCommand*
/// * *SelectAllCommand*
/// * *StopCommand*
/// * *UndoCommand*
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.applicationcommands.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API ApplicationCommands
{
    /// Commands
    //@{
    static const RoutedUICommand* CancelPrintCommand;
    static const RoutedUICommand* CloseCommand;
    static const RoutedUICommand* ContextMenuCommand;
    static const RoutedUICommand* CopyCommand;
    static const RoutedUICommand* CorrectionListCommand;
    static const RoutedUICommand* CutCommand;
    static const RoutedUICommand* DeleteCommand;
    static const RoutedUICommand* FindCommand;
    static const RoutedUICommand* HelpCommand;
    static const RoutedUICommand* NewCommand;
    static const RoutedUICommand* OpenCommand;
    static const RoutedUICommand* PasteCommand;
    static const RoutedUICommand* PrintCommand;
    static const RoutedUICommand* PrintPreviewCommand;
    static const RoutedUICommand* PropertiesCommand;
    static const RoutedUICommand* RedoCommand;
    static const RoutedUICommand* ReplaceCommand;
    static const RoutedUICommand* SaveCommand;
    static const RoutedUICommand* SaveAsCommand;
    static const RoutedUICommand* SelectAllCommand;
    static const RoutedUICommand* StopCommand;
    static const RoutedUICommand* UndoCommand;
    //@}

    NS_DECLARE_REFLECTION(ApplicationCommands, NoParent)
};

}

#endif
