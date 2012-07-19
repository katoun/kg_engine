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