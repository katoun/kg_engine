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

#include <render/BlendMode.h>

namespace render
{

bool LayerBlendMode::operator==(const LayerBlendMode& rhs) const
{
	if (blendType != rhs.blendType) 
		return false;

	if (blendType == LBT_COLOR)
	{

		if (operation == rhs.operation &&
		source1 == rhs.source1 &&
			source2 == rhs.source2 &&
			colorArg1 == rhs.colorArg1 &&
			colorArg2 == rhs.colorArg2 &&
			factor == rhs.factor)
		{
			return true;
		}
	}
	else // if (blendType == LBT_ALPHA)
	{
		if (operation == rhs.operation &&
			source1 == rhs.source1 &&
			source2 == rhs.source2 &&
			alphaArg1 == rhs.alphaArg1 &&
			alphaArg2 == rhs.alphaArg2 &&
			factor == rhs.factor)
		{
			return true;
		}
	}
	return false;
}

bool LayerBlendMode::operator!=(const LayerBlendMode& rhs) const
{
	return !(*this == rhs);
}

} //namespace render
