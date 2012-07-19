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

#ifndef _TEXT_OVERLAY_H_
#define _TEXT_OVERLAY_H_

#include <core/Config.h>
#include <render/Overlay.h>

#include <string>

namespace render
{

class Font;
	
enum Alignment
{
	A_LEFT,
	A_RIGHT,
	A_CENTER
};

//! Represents a text overlay which is rendered on top of the 'normal' scene contents.
class ENGINE_PUBLIC_EXPORT TextOverlay: public Overlay
{
public:

	TextOverlay();
	TextOverlay(const std::string& name);

	~TextOverlay();

	//! Sets the the Font which this overlay will use.
	void setFont(const std::string& name);
	void setFont(Font* font);

	Material* getMaterial() const;

	void setCaption(const std::string& caption);
	const std::string& getCaption() const;

	void setAlignment(Alignment a);
	
	Alignment getAlignment() const;

	void setCharHeight(float height);
	float getCharHeight() const;

	void setSpaceWidth(float width);
	float getSpaceWidth() const;

protected:

	void initializeImpl();
	void updateTransformImpl();

	Font* mFont;

	std::string mCaption;

	/// The text alignment
	Alignment mAlignment;

	float mCharHeight;
	float mPixelCharHeight;
	float mSpaceWidth;
	float mPixelSpaceWidth;
	unsigned int mAllocSize;
	float mViewportAspectCoef;

	// Internal method to allocate memory, only reallocates when necessary
	void allocateMemory(unsigned int numChars);

	void updatePositionBinding();

	void updateTextureBinding();
};

} //namespace render

#endif