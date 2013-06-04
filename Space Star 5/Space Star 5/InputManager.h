#pragma once

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define RELEASE_COM(x) { if(x) { x->Release(); x = 0; } }

class InputManager
{
public:
	~InputManager(void);	// Destructor

	// Singleton access methods
	static InputManager* GetInstance(void);
	static void DestroyInstance(void);

	void Initialize(HWND hWnd, HINSTANCE hInstance);
	void Update(void);
	void Shutdown(void);

	// Input Function (button is pressed)
	// Use this for commands that you want to happen
	// only once on every press of the button
	bool KeyboardKeyPressed(short key);

	// Input Function (button is currently down)
	// Use this for commands that you want to happen
	// as long as the button is being hold down
	bool KeyboardKeyDown(short key);

	// Returns true if the button is currently down
	// False otherwise
	bool MouseButtonDown(short button);

	// Use this for commands that you want to happen
	// only once on every press of the mouse button
	bool MouseButtonPressed(short button);

	//int scan2ascii(DWORD scanCode,unsigned short* result);

	// Detect changes in mouse position and scroll
	int MouseDeltaX(void)
		{ return mouseState.lX; }

	int MouseDeltaY(void)
		{ return mouseState.lY; }

	int MouseDeltaScroll(void)
		{ return mouseState.lZ; }

	// Set the mouse position
	void SetMousePos(int x, int y)
		{ SetCursorPos(x, y); }

private:
	// Singleton instance and constructor
	static InputManager* instance;
	InputManager(void);

	// Direct Input Variables
	IDirectInput8*		m_pDIObject;	// Direct Input Object
	IDirectInputDevice8* m_pDIKeyboard;	// Keyboard device
	IDirectInputDevice8* m_pDIMouse;	// mouse device
	char				buffer[256];	// input variable for keyboard
	bool				m_bKeyDown[256];// checks which key is down
	bool				m_bLockedKeys[256];
	DIMOUSESTATE2		mouseState;		// input variable for mouse
	bool				buttonDown[8];		// checks which mouse button is down
	BYTE				m_cKeyState[256];
};

