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

#include <input/KeyEvent.h>

namespace input
{

KeyEvent::KeyEvent()
{
	mKey = KEY_UNKNOWN;
	mModifiers = 0;
	mText = 0;
}

KeyEvent::~KeyEvent() {}

void KeyEvent::set(KeyCode key,  std::bitset<KEY_MODIFIER_COUNT> modifiers, unsigned int text, InputDevice *source)
{
	InputEvent::set(source);
	mKey = key;
	mModifiers = modifiers;
	mText = text;
}

KeyCode KeyEvent::getKey() const
{
	return mKey;
}

bool KeyEvent::isModifier(KeyModifier modifier) const
{
	return mModifiers[modifier];
}

unsigned int KeyEvent::getText() const
{
	return mText;
}

} // end namespace input
