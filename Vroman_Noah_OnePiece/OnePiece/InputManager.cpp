#include "pch.h"
#include "InputManager.h"
#include <iostream>
#include <chrono>
InputManager::InputManager()
    :m_AmountOfClicks{0}
    ,m_IsKeyPressedAlready{ false }
    ,m_Timer{0}
{
    for (size_t i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        prev_state[i] = 0;
    }
}

InputManager::~InputManager()
{

}

bool InputManager::IsKeyDown(SDL_Scancode key) const
{
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    bool pressed = state[key];
    return pressed;
}

bool InputManager::IsKeyPressed(SDL_Scancode key)
{
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    bool pressed = state[key] && !prev_state[key];
    prev_state[key] = state[key];
    return pressed;
}

int InputManager::ClicksInRange(SDL_Scancode key,float timeRange, float elapsedSec)
{
    if (IsKeyDown(key) && !m_IsKeyPressedAlready)
    {
        m_IsKeyPressedAlready = true;
        m_Timer = 0;
        m_AmountOfClicks = 0; 
    }

    if (m_IsKeyPressedAlready)
    {
        m_Timer += elapsedSec;
        if (m_Timer >= timeRange)
        {
            m_IsKeyPressedAlready = false;
        }

        if (IsKeyPressed(key))
        {
            ++m_AmountOfClicks;
        }

        if (!m_IsKeyPressedAlready && m_AmountOfClicks > 0)
        {
            m_Timer = 0;
            m_AmountOfClicks = 0;
        }
    }

    return m_AmountOfClicks;
}





