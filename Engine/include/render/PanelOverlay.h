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

#ifndef _PANEL_OVERLAY_H_
#define _PANEL_OVERLAY_H_

#include <core/Config.h>
#include <render/Overlay.h>

#include <string>

namespace render
{

//! Represents a panel overlay which is rendered on top of the 'normal' scene contents.
class ENGINE_PUBLIC_EXPORT PanelOverlay: public Overlay
{
public:

	PanelOverlay();
	PanelOverlay(const std::string& name);

	~PanelOverlay();

	//! Sets the the Material which this overlay will use.
	void setMaterial(const std::string& filename);
	void setMaterial(Material* material);

protected:

	void initializeImpl();

	void updatePositionBinding();

	void updateTextureBinding();
};

} //namespace render

#endif