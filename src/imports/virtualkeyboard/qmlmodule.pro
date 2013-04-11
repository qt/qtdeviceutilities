TARGETPATH = "Qt/labs/virtualkeyboard"

QML_FILES = \
	  images/shift.png \
	  images/space.png \
	  images/backspace.png \
	  images/arrow.png \
	  images/enter.png \
	  images/keypad.png \
	  images/hidekeyboard.png \
	  components/AutoScroller.qml \
	  components/InputEngine.qml \
	  components/HideKeyboardKey.qml \
	  components/NumberKey.qml \
	  components/CharacterPreviewBubble.qml \
	  components/SymbolModeKey.qml \
	  components/ChangeLanguageKey.qml \
	  components/SpaceKey.qml \
	  components/Icon.qml \
	  components/BaseKey.qml \
	  components/Key.qml \
	  components/ShiftKey.qml \
	  components/EnterKey.qml \
	  components/AlternativeKeys.qml \
	  components/PressedKey.qml \
	  components/BackspaceKey.qml \
	  Keyboard.qml \
	  layouts/symbols.qml \
	  layouts/numbers.qml \
	  layouts/ar_AR.qml \
	  layouts/en_GB.qml \
	  layouts/fi_FI.qml \

load(qml_module)
