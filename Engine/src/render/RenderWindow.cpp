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

#include <render/RenderWindow.h>
#include <render/Viewport.h>
#include <engine/EngineSettings.h>

namespace render
{

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

void RenderWindow::windowMovedOrResized() {}

bool RenderWindow::isFullScreen()
{
	return mIsFullScreen;
}

void RenderWindow::getMetrics(unsigned int& width, unsigned int& height, unsigned int& colorDepth, signed int& left, signed int& top)
{
	width = mWidth;
	height = mHeight;
	colorDepth = mColorDepth;
	left = mLeft;
	top = mTop;
}

void RenderWindow::updateImpl(float elapsedTime)
{
	// call superclass
	RenderTarget::updateImpl(elapsedTime);

	// Swap buffers
	swapBuffers(engine::EngineSettings::getInstance().getVSync());
}

} // end namespace render
