/**
* @Author	DJ Coombes
* @date		04th November 2016
* @brief	The input forwarder will forward any SFLM input events to the web renderer.
*/

#include "inputForwarder.h"

InputForwarder::InputForwarder() {
	m_keyIdentifierMap[sf::Keyboard::Insert]	= Awesomium::KeyCodes::AK_INSERT;
	m_keyIdentifierMap[sf::Keyboard::Home]		= Awesomium::KeyCodes::AK_HOME;
	m_keyIdentifierMap[sf::Keyboard::Delete]	= Awesomium::KeyCodes::AK_DELETE;
	m_keyIdentifierMap[sf::Keyboard::End]		= Awesomium::KeyCodes::AK_END;
	m_keyIdentifierMap[sf::Keyboard::PageDown]	= Awesomium::KeyCodes::AK_NEXT;
	m_keyIdentifierMap[sf::Keyboard::Right]		= Awesomium::KeyCodes::AK_RIGHT;
	m_keyIdentifierMap[sf::Keyboard::Left]		= Awesomium::KeyCodes::AK_LEFT;
	m_keyIdentifierMap[sf::Keyboard::Down]		= Awesomium::KeyCodes::AK_DOWN;
	m_keyIdentifierMap[sf::Keyboard::Up]		= Awesomium::KeyCodes::AK_UP;
	m_keyIdentifierMap[sf::Keyboard::LShift]	= Awesomium::KeyCodes::AK_LSHIFT;
	m_keyIdentifierMap[sf::Keyboard::RShift]	= Awesomium::KeyCodes::AK_RSHIFT;
	m_keyIdentifierMap[sf::Keyboard::LControl]	= Awesomium::KeyCodes::AK_LCONTROL;
	m_keyIdentifierMap[sf::Keyboard::RControl]	= Awesomium::KeyCodes::AK_RCONTROL;
	m_keyIdentifierMap[sf::Keyboard::LAlt]		= Awesomium::KeyCodes::AK_LMENU;
	m_keyIdentifierMap[sf::Keyboard::RAlt]		= Awesomium::KeyCodes::AK_RMENU;
	m_keyIdentifierMap[sf::Keyboard::F1]		= Awesomium::KeyCodes::AK_F1;
	m_keyIdentifierMap[sf::Keyboard::F2]		= Awesomium::KeyCodes::AK_F2;
	m_keyIdentifierMap[sf::Keyboard::F3]		= Awesomium::KeyCodes::AK_F3;
	m_keyIdentifierMap[sf::Keyboard::F4]		= Awesomium::KeyCodes::AK_F4;
	m_keyIdentifierMap[sf::Keyboard::F5]		= Awesomium::KeyCodes::AK_F5;
	m_keyIdentifierMap[sf::Keyboard::F6]		= Awesomium::KeyCodes::AK_F6;
	m_keyIdentifierMap[sf::Keyboard::F7]		= Awesomium::KeyCodes::AK_F7;
	m_keyIdentifierMap[sf::Keyboard::F8]		= Awesomium::KeyCodes::AK_F8;
	m_keyIdentifierMap[sf::Keyboard::F9]		= Awesomium::KeyCodes::AK_F9;
	m_keyIdentifierMap[sf::Keyboard::F10]		= Awesomium::KeyCodes::AK_F10;
	m_keyIdentifierMap[sf::Keyboard::F11]		= Awesomium::KeyCodes::AK_F11;
	m_keyIdentifierMap[sf::Keyboard::F12]		= Awesomium::KeyCodes::AK_F12;
	m_keyIdentifierMap[sf::Keyboard::Return]	= Awesomium::KeyCodes::AK_RETURN;
	m_keyIdentifierMap[sf::Keyboard::BackSpace] = Awesomium::KeyCodes::AK_BACK;
	m_keyIdentifierMap[sf::Keyboard::A]			= Awesomium::KeyCodes::AK_A;
	m_keyIdentifierMap[sf::Keyboard::B]			= Awesomium::KeyCodes::AK_B;
	m_keyIdentifierMap[sf::Keyboard::C]			= Awesomium::KeyCodes::AK_C;
	m_keyIdentifierMap[sf::Keyboard::D]			= Awesomium::KeyCodes::AK_D;
	m_keyIdentifierMap[sf::Keyboard::E]			= Awesomium::KeyCodes::AK_E;
	m_keyIdentifierMap[sf::Keyboard::F]			= Awesomium::KeyCodes::AK_F;
	m_keyIdentifierMap[sf::Keyboard::G]			= Awesomium::KeyCodes::AK_G;
	m_keyIdentifierMap[sf::Keyboard::H]			= Awesomium::KeyCodes::AK_H;
	m_keyIdentifierMap[sf::Keyboard::I]			= Awesomium::KeyCodes::AK_I;
	m_keyIdentifierMap[sf::Keyboard::J]			= Awesomium::KeyCodes::AK_J;
	m_keyIdentifierMap[sf::Keyboard::K]			= Awesomium::KeyCodes::AK_K;
	m_keyIdentifierMap[sf::Keyboard::L]			= Awesomium::KeyCodes::AK_L;
	m_keyIdentifierMap[sf::Keyboard::M]			= Awesomium::KeyCodes::AK_M;
	m_keyIdentifierMap[sf::Keyboard::N]			= Awesomium::KeyCodes::AK_N;
	m_keyIdentifierMap[sf::Keyboard::O]			= Awesomium::KeyCodes::AK_O;
	m_keyIdentifierMap[sf::Keyboard::P]			= Awesomium::KeyCodes::AK_P;
	m_keyIdentifierMap[sf::Keyboard::Q]			= Awesomium::KeyCodes::AK_Q;
	m_keyIdentifierMap[sf::Keyboard::R]			= Awesomium::KeyCodes::AK_R;
	m_keyIdentifierMap[sf::Keyboard::S]			= Awesomium::KeyCodes::AK_S;
	m_keyIdentifierMap[sf::Keyboard::T]			= Awesomium::KeyCodes::AK_T;
	m_keyIdentifierMap[sf::Keyboard::U]			= Awesomium::KeyCodes::AK_U;
	m_keyIdentifierMap[sf::Keyboard::V]			= Awesomium::KeyCodes::AK_V;
	m_keyIdentifierMap[sf::Keyboard::W]			= Awesomium::KeyCodes::AK_W;
	m_keyIdentifierMap[sf::Keyboard::X]			= Awesomium::KeyCodes::AK_X;
	m_keyIdentifierMap[sf::Keyboard::Y]			= Awesomium::KeyCodes::AK_Y;
	m_keyIdentifierMap[sf::Keyboard::Z]			= Awesomium::KeyCodes::AK_Z;
	m_keyIdentifierMap[sf::Keyboard::Num1]		= Awesomium::KeyCodes::AK_1;
	m_keyIdentifierMap[sf::Keyboard::Num2]		= Awesomium::KeyCodes::AK_2;
	m_keyIdentifierMap[sf::Keyboard::Num3]		= Awesomium::KeyCodes::AK_3;
	m_keyIdentifierMap[sf::Keyboard::Num4]		= Awesomium::KeyCodes::AK_4;
	m_keyIdentifierMap[sf::Keyboard::Num5]		= Awesomium::KeyCodes::AK_5;
	m_keyIdentifierMap[sf::Keyboard::Num6]		= Awesomium::KeyCodes::AK_6;
	m_keyIdentifierMap[sf::Keyboard::Num7]		= Awesomium::KeyCodes::AK_7;
	m_keyIdentifierMap[sf::Keyboard::Num8]		= Awesomium::KeyCodes::AK_8;
	m_keyIdentifierMap[sf::Keyboard::Num9]		= Awesomium::KeyCodes::AK_9;
	m_keyIdentifierMap[sf::Keyboard::Num0]		= Awesomium::KeyCodes::AK_0;
	m_keyIdentifierMap[sf::Keyboard::Numpad1]	= Awesomium::KeyCodes::AK_NUMPAD1;
	m_keyIdentifierMap[sf::Keyboard::Numpad2]	= Awesomium::KeyCodes::AK_NUMPAD2;
	m_keyIdentifierMap[sf::Keyboard::Numpad3]	= Awesomium::KeyCodes::AK_NUMPAD3;
	m_keyIdentifierMap[sf::Keyboard::Numpad4]	= Awesomium::KeyCodes::AK_NUMPAD4;
	m_keyIdentifierMap[sf::Keyboard::Numpad5]	= Awesomium::KeyCodes::AK_NUMPAD5;
	m_keyIdentifierMap[sf::Keyboard::Numpad6]	= Awesomium::KeyCodes::AK_NUMPAD6;
	m_keyIdentifierMap[sf::Keyboard::Numpad7]	= Awesomium::KeyCodes::AK_NUMPAD7;
	m_keyIdentifierMap[sf::Keyboard::Numpad8]	= Awesomium::KeyCodes::AK_NUMPAD8;
	m_keyIdentifierMap[sf::Keyboard::Numpad9]	= Awesomium::KeyCodes::AK_NUMPAD9;
	m_keyIdentifierMap[sf::Keyboard::Numpad0]	= Awesomium::KeyCodes::AK_NUMPAD0;
	m_keyIdentifierMap[sf::Keyboard::Comma]		= Awesomium::KeyCodes::AK_OEM_COMMA;
	m_keyIdentifierMap[sf::Keyboard::Add]		= Awesomium::KeyCodes::AK_ADD;
	m_keyIdentifierMap[sf::Keyboard::Subtract]	= Awesomium::KeyCodes::AK_SUBTRACT;
	m_keyIdentifierMap[sf::Keyboard::Multiply]	= Awesomium::KeyCodes::AK_MULTIPLY;
	m_keyIdentifierMap[sf::Keyboard::Divide]	= Awesomium::KeyCodes::AK_DIVIDE;
	m_keyIdentifierMap[sf::Keyboard::Slash]		= Awesomium::KeyCodes::AK_OEM_2;
	m_keyIdentifierMap[sf::Keyboard::BackSlash] = Awesomium::KeyCodes::AK_OEM_5;
	m_keyIdentifierMap[sf::Keyboard::Slash]		= Awesomium::KeyCodes::AK_OEM_5;
	m_keyIdentifierMap[sf::Keyboard::Space]		= Awesomium::KeyCodes::AK_SPACE;
	m_keyIdentifierMap[sf::Keyboard::SemiColon] = Awesomium::KeyCodes::AK_OEM_1;
	m_keyIdentifierMap[sf::Keyboard::Equal]		= Awesomium::KeyCodes::AK_OEM_PLUS;
	m_keyIdentifierMap[sf::Keyboard::LBracket]	= Awesomium::KeyCodes::AK_OEM_4;
	m_keyIdentifierMap[sf::Keyboard::RBracket]	= Awesomium::KeyCodes::AK_OEM_6;
	m_keyIdentifierMap[sf::Keyboard::Quote]		= Awesomium::KeyCodes::AK_OEM_7;
	m_keyIdentifierMap[sf::Keyboard::Tab]		= Awesomium::KeyCodes::AK_TAB;
	m_keyIdentifierMap[sf::Keyboard::Tilde]		= Awesomium::KeyCodes::AK_OEM_3;
	m_keyIdentifierMap[sf::Keyboard::Escape]	= Awesomium::KeyCodes::AK_ESCAPE;

	m_keyAsciiMap[sf::Keyboard::Insert]			= 0x2D;
	m_keyAsciiMap[sf::Keyboard::Home]			= 0x24;
	m_keyAsciiMap[sf::Keyboard::Delete]			= 0x2E;
	m_keyAsciiMap[sf::Keyboard::End]			= 0x23;
	m_keyAsciiMap[sf::Keyboard::PageDown]		= 0x22;
	m_keyAsciiMap[sf::Keyboard::Right]			= 0x27;
	m_keyAsciiMap[sf::Keyboard::Left]			= 0x25;
	m_keyAsciiMap[sf::Keyboard::Down]			= 0x28;
	m_keyAsciiMap[sf::Keyboard::Up]				= 0x26;
	m_keyAsciiMap[sf::Keyboard::LShift]			= 0xA0;
	m_keyAsciiMap[sf::Keyboard::RShift]			= 0x10;
	m_keyAsciiMap[sf::Keyboard::LControl]		= 0x11;
	m_keyAsciiMap[sf::Keyboard::RControl]		= 0x11;
	m_keyAsciiMap[sf::Keyboard::LAlt]			= 0x12;
	m_keyAsciiMap[sf::Keyboard::RAlt]			= 0x12;
	m_keyAsciiMap[sf::Keyboard::F1]				= 0x70;
	m_keyAsciiMap[sf::Keyboard::F2]				= 0x71;
	m_keyAsciiMap[sf::Keyboard::F3]				= 0x72;
	m_keyAsciiMap[sf::Keyboard::F4]				= 0x73;
	m_keyAsciiMap[sf::Keyboard::F5]				= 0x74;
	m_keyAsciiMap[sf::Keyboard::F6]				= 0x75;
	m_keyAsciiMap[sf::Keyboard::F7]				= 0x76;
	m_keyAsciiMap[sf::Keyboard::F8]				= 0x77;
	m_keyAsciiMap[sf::Keyboard::F9]				= 0x78;
	m_keyAsciiMap[sf::Keyboard::F10]			= 0x79;
	m_keyAsciiMap[sf::Keyboard::F11]			= 0x7A;
	m_keyAsciiMap[sf::Keyboard::F12]			= 0x7B;
	m_keyAsciiMap[sf::Keyboard::Return]			= 0x0D;
	m_keyAsciiMap[sf::Keyboard::BackSpace]		= 0x08;
	m_keyAsciiMap[sf::Keyboard::A]				= 'A';
	m_keyAsciiMap[sf::Keyboard::B]				= 'B';
	m_keyAsciiMap[sf::Keyboard::C]				= 'C';
	m_keyAsciiMap[sf::Keyboard::D]				= 'D';
	m_keyAsciiMap[sf::Keyboard::E]				= 'E';
	m_keyAsciiMap[sf::Keyboard::F]				= 'F';
	m_keyAsciiMap[sf::Keyboard::G]				= 'G';
	m_keyAsciiMap[sf::Keyboard::H]				= 'H';
	m_keyAsciiMap[sf::Keyboard::I]				= 'I';
	m_keyAsciiMap[sf::Keyboard::J]				= 'J';
	m_keyAsciiMap[sf::Keyboard::K]				= 'K';
	m_keyAsciiMap[sf::Keyboard::L]				= 'L';
	m_keyAsciiMap[sf::Keyboard::M]				= 'M';
	m_keyAsciiMap[sf::Keyboard::N]				= 'N';
	m_keyAsciiMap[sf::Keyboard::O]				= 'O';
	m_keyAsciiMap[sf::Keyboard::P]				= 'P';
	m_keyAsciiMap[sf::Keyboard::Q]				= 'Q';
	m_keyAsciiMap[sf::Keyboard::R]				= 'R';
	m_keyAsciiMap[sf::Keyboard::S]				= 'S';
	m_keyAsciiMap[sf::Keyboard::T]				= 'T';
	m_keyAsciiMap[sf::Keyboard::U]				= 'U';
	m_keyAsciiMap[sf::Keyboard::V]				= 'V';
	m_keyAsciiMap[sf::Keyboard::W]				= 'W';
	m_keyAsciiMap[sf::Keyboard::X]				= 'X';
	m_keyAsciiMap[sf::Keyboard::Y]				= 'Y';
	m_keyAsciiMap[sf::Keyboard::Z]				= 'Z';
	m_keyAsciiMap[sf::Keyboard::Num1]			= '1';
	m_keyAsciiMap[sf::Keyboard::Num2]			= '2';
	m_keyAsciiMap[sf::Keyboard::Num3]			= '3';
	m_keyAsciiMap[sf::Keyboard::Num4]			= '4';
	m_keyAsciiMap[sf::Keyboard::Num5]			= '5';
	m_keyAsciiMap[sf::Keyboard::Num6]			= '6';
	m_keyAsciiMap[sf::Keyboard::Num7]			= '7';
	m_keyAsciiMap[sf::Keyboard::Num8]			= '8';
	m_keyAsciiMap[sf::Keyboard::Num9]			= '9';
	m_keyAsciiMap[sf::Keyboard::Num0]			= '0';
	m_keyAsciiMap[sf::Keyboard::Numpad1]		= 0x61;
	m_keyAsciiMap[sf::Keyboard::Numpad2]		= 0x62;
	m_keyAsciiMap[sf::Keyboard::Numpad3]		= 0x63;
	m_keyAsciiMap[sf::Keyboard::Numpad4]		= 0x64;
	m_keyAsciiMap[sf::Keyboard::Numpad5]		= 0x65;
	m_keyAsciiMap[sf::Keyboard::Numpad6]		= 0x66;
	m_keyAsciiMap[sf::Keyboard::Numpad7]		= 0x67;
	m_keyAsciiMap[sf::Keyboard::Numpad8]		= 0x68;
	m_keyAsciiMap[sf::Keyboard::Numpad9]		= 0x69;
	m_keyAsciiMap[sf::Keyboard::Numpad0]		= 0x60;
	m_keyAsciiMap[sf::Keyboard::Comma]			= 0x2C;
	m_keyAsciiMap[sf::Keyboard::Add]			= 0x2B;
	m_keyAsciiMap[sf::Keyboard::Subtract]		= 0x2D;
	m_keyAsciiMap[sf::Keyboard::Multiply]		= 0x2A;
	m_keyAsciiMap[sf::Keyboard::Divide]			= 0x2F;
	m_keyAsciiMap[sf::Keyboard::Slash]			= 0x2F;
	m_keyAsciiMap[sf::Keyboard::BackSlash]		= 0x5C;
	m_keyAsciiMap[sf::Keyboard::Space]			= 0x20;
	m_keyAsciiMap[sf::Keyboard::SemiColon]		= 0x3B;
	m_keyAsciiMap[sf::Keyboard::Equal]			= 0x3D;
	m_keyAsciiMap[sf::Keyboard::LBracket]		= 0x5B;
	m_keyAsciiMap[sf::Keyboard::RBracket]		= 0x5D;
	m_keyAsciiMap[sf::Keyboard::Quote]			= 0x27;
	m_keyAsciiMap[sf::Keyboard::Tab]			= 0x09;
	m_keyAsciiMap[sf::Keyboard::Tilde]			= 0x7E;
	m_keyAsciiMap[sf::Keyboard::Escape]			= 0x1B;
}

void InputForwarder::HandleEvent(Awesomium::WebView* webView, const sf::Event& event) {
	if (!webView) {
		return;
	}

	switch (event.type) {
	case sf::Event::MouseMoved:
		webView->InjectMouseMove(event.mouseMove.x, event.mouseMove.y);
		break;
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left)
			webView->InjectMouseDown(Awesomium::kMouseButton_Left);
		else if (event.type == sf::Event::MouseWheelMoved) {
			if (event.mouseWheel.delta > 0)
				webView->InjectMouseWheel(25, 0);
			else
				webView->InjectMouseWheel(-25, 0);
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left)
			webView->InjectMouseUp(Awesomium::kMouseButton_Left);
		break;
	case sf::Event::KeyPressed:
	case sf::Event::TextEntered:
		if (event.type == sf::Event::TextEntered) {
			Awesomium::WebKeyboardEvent keyEvent;
			keyEvent.native_key_code = event.text.unicode;
			webView->InjectKeyboardEvent(keyEvent);
		}
		else {
			Awesomium::WebKeyboardEvent keyEvent;

			keyEvent.type = event.type == sf::Event::KeyPressed ?
				Awesomium::WebKeyboardEvent::kTypeKeyDown :
				Awesomium::WebKeyboardEvent::kTypeKeyUp;

			char* buf = new char[20];

			keyEvent.virtual_key_code = m_keyIdentifierMap[event.key.code];

			Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
			strcpy_s(keyEvent.key_identifier, buf);
			delete[] buf;

			keyEvent.modifiers = 0;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
				keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModControlKey;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
				keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModShiftKey;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
				keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModAltKey;

			keyEvent.native_key_code = event.key.code;

			if (event.type == sf::Event::KeyReleased) {
				webView->InjectKeyboardEvent(keyEvent);
			}
			else {
				unsigned int chr;
				if ((m_keyAsciiMap[event.key.code] & 0xFF80) == 0)
					chr = m_keyAsciiMap[event.key.code] & 0x7F;
				else
					chr = m_keyAsciiMap[event.key.code];

				keyEvent.text[0] = chr;
				keyEvent.unmodified_text[0] = chr;

				webView->InjectKeyboardEvent(keyEvent);

				if (chr) {
					keyEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;
					keyEvent.virtual_key_code = chr;
					keyEvent.native_key_code = chr;
					webView->InjectKeyboardEvent(keyEvent);
				}
			}
		}
		break;
	}
}
