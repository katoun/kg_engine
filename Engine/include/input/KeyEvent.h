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

#ifndef _KEY_EVENT_H_
#define _KEY_EVENT_H_

#include <core/Config.h>
#include <input/InputEvent.h>
#include <input/KeyboardDefines.h>

#include <bitset>

namespace input
{

//! KeyEvent class - DOCUMENTATION NEEDED
class ENGINE_PUBLIC_EXPORT KeyEvent: public InputEvent
{
public:

	//! Contructor
	KeyEvent();

	//! Destructor
	~KeyEvent();

	//! Set the key event
	//! \param key: KeyCode enumeration type, specifying the key.
	//! \param modifiers: specifying the modifiers.
	//! \param text: DOCUMENTATION NEEDED
	//! \param source: The input device which this event originates from.
	void set(KeyCode key, std::bitset<KEY_MODIFIER_COUNT> modifiers, unsigned int text, InputDevice *source);

	//! getKey - DOCUMENTATION NEEDED
	KeyCode getKey() const;

	//! isModifier - DOCUMENTATION NEEDED
	bool isModifier(KeyModifier modifier) const;

	//! getText - DOCUMENTATION NEEDED
	unsigned int getText() const;

protected:

	KeyCode mKey;
	std::bitset<KEY_MODIFIER_COUNT> mModifiers;
	unsigned int mText;
};

} // end namespace input

#endif