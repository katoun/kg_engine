/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#ifndef _EDITOR_CONFIG_H_
#define _EDITOR_CONFIG_H_

#include <core/Config.h>

#ifdef _UNICODE
#undef _UNICODE
#endif

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/string.h>
#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>

enum
{
	ID_SCENE_EXPLORER,
	ID_PROPERTY_WINDOW,
	ID_RIBBON_CONTROL,
	ID_STATUS_BAR,
	ID_MENU_FILE_NEW,
	ID_MENU_FILE_OPEN,
	ID_MENU_FILE_SAVE,
	ID_MENU_FILE_SAVE_AS,
	ID_MENU_FILE_EXIT,
	ID_MENU_EDIT_UNDO,
	ID_MENU_EDIT_REDO,
	ID_MENU_EDIT_DELETE,
	ID_MENU_EDIT_CLONE,
	ID_MENU_EDIT_HIDE,
	ID_MENU_EDIT_UNHIDE_ALL,
	ID_MENU_EDIT_SELECT_ALL,
	ID_MENU_EDIT_SELECT_NONE,
	ID_MENU_EDIT_SELECT_INVERT,

	ID_COUNT
};

#endif