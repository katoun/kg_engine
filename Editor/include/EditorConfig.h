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