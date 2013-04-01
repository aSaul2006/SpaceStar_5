#include "InputManager.h"

InputManager* InputManager::instance;

// Constructor
InputManager::InputManager(void)
{
	m_pDIObject = NULL;
	m_pDIKeyboard = NULL;
	m_pDIMouse = NULL;
}

// Destructor
InputManager::~InputManager(void)
{
	Shutdown();
}

InputManager* InputManager::GetInstance(void)
{
	if(instance == 0)
		instance = new InputManager();
	return instance;
}

void InputManager::DestroyInstance(void)
{
	if(instance)
	{
		delete instance;
		instance = NULL;
	}
}

void InputManager::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	// Direct input object
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);

	// Direct input devices
	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	// Set data format
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard); 
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);

	// Direct Input - Set Cooperative level
	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void InputManager::Update(void)
{
	HRESULT result;

	// Clear the keyboard input buffer
	ZeroMemory(buffer, sizeof(buffer));

	// Get the current keyboard input device state
	result = m_pDIKeyboard->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

	if(FAILED(result))
	{
		// Device has probably been lost if failed
		// so keep trying until we find it
		do
		{
			result = m_pDIKeyboard->Acquire();
		}while(result == DIERR_INPUTLOST);

		// If we failed for some other reason, simple
		// return out of the method
		if(FAILED(result))
			return;

		// Read the current keyboard state again
		m_pDIKeyboard->GetDeviceState(sizeof(buffer), &buffer);
	}

	// Clear the mouse input buffer
	ZeroMemory(&mouseState, sizeof(mouseState));

	// Get the mouse input device state
	result = m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);

	if(FAILED(result))
	{
		// Device has probably been lost if failed, so keep
		// trying to get it back until it's found
		do
		{
			result = m_pDIMouse->Acquire();
		}while(result == DIERR_INPUTLOST);

		// If we failed for some other reason, quit the method
		if(FAILED(result))
			return;

		// Read the current mouse state again
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	}
}

void InputManager::Shutdown(void)
{
	if(m_pDIMouse)
	{
		m_pDIMouse->Unacquire();
		RELEASE_COM(m_pDIMouse);
	}
	if(m_pDIMouse)
	{
		m_pDIKeyboard->Unacquire();
		RELEASE_COM(m_pDIKeyboard);
	}
	RELEASE_COM(m_pDIObject);
}

bool InputManager::KeyboardKeyPressed(short key)
{
	// checks when the button is pressed down
	if(buffer[key] & 0x80) 
	{
		// makes sure the action is only performed
		// once. 
		if(!m_bKeyDown[key])
		{
			m_bKeyDown[key] = true;
			return true;
		}
		else
			return false;
	}
	else 
	{
		// checks to see when the button is released or
		// not being pressed (when key is up)
		if(m_bKeyDown[key])
		{ 
			m_bKeyDown[key] = false;
		}
		return false;
	}
}

bool InputManager::KeyboardKeyDown(short key)
{
	// checks when the button is being pressed 
	if(buffer[key] & 0x80)
		return true;
	else
		return false;
}

bool InputManager::MouseButtonDown(short button)
{
	if(mouseState.rgbButtons[button] & 0x80)
	{
		// Mouse button was pressed
		return true;
	}
	else
	{
		// Mouse button was not pressed
		return false;
	}
}

bool InputManager::MouseButtonPressed(short button)
{
	// checks when the button is pressed down
	if(mouseState.rgbButtons[button] & 0x80) 
	{
		// makes sure the action is only performed
		// once. 
		if(!buttonDown[button])
		{
			buttonDown[button] = true;
			return true;
		}
		else
			return false;
	}
	else 
	{
		// checks to see when the button is released or
		// not being pressed (when key is up)
		if(buttonDown[button])
		{ 
			buttonDown[button] = false;
		}
		return false;
	}
}