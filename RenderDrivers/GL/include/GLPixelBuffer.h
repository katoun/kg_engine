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

#ifndef _GL_PIXEL_BUFFER_H_
#define _GL_PIXEL_BUFFER_H_

#include <GLConfig.h>
#include <render/PixelBuffer.h>

namespace render
{

class GL_PUBLIC_EXPORT GLPixelBuffer : public PixelBuffer
{
public:

	GLPixelBuffer(unsigned int width, unsigned int height, unsigned int depth, resource::PixelFormat format, resource::BufferUsage usage);

	virtual ~GLPixelBuffer();

	void readData(unsigned int offset, unsigned int length, void* pDest);

	void writeData(unsigned int offset, unsigned int length, const void* pSource, bool discardWholeBuffer = false);

	GLuint getGLBufferId() const;

private:

	GLuint mBufferId;

protected:

	void* lockImpl(unsigned int offset, unsigned int length, resource::BufferLocking options);

	void unlockImpl();
};

}// end namespace render

#endif// _GL_PIXEL_BUFFER_H_