/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

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

#include <render/RenderWindow.h>
#include <render/Viewport.h>
#include <engine/EngineSettings.h>

namespace render
{

RenderWindow::RenderWindow()
{
	mTop = -1;
	mLeft = -1;
}

unsigned int RenderWindow::getTop() const
{
	return mTop;
}

unsigned int RenderWindow::getLeft() const
{
	return mLeft;
}

bool RenderWindow::isFullScreen()
{
	return mIsFullScreen;
}

void RenderWindow::setFullscreen(bool fullScreen, unsigned int width, unsigned int height) {}

bool RenderWindow::isVisible() const
{
	return true;
}

void RenderWindow::setVisible(bool visible) {}

bool RenderWindow::isActive() const
{
	return (mActive && isVisible());
}

void RenderWindow::updateImpl(float elapsedTime)
{
	// call superclass
	RenderTarget::updateImpl(elapsedTime);
}

} // end namespace render
