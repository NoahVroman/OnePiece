#pragma once
class InputManager final
{
public:
    InputManager();
    ~InputManager();

    InputManager(const InputManager& other) = delete;
    InputManager& operator=(const InputManager& rhs) = delete;
    InputManager(InputManager&& other) = delete;
    InputManager& operator=(InputManager&& rhs) = delete;


    bool IsKeyDown(SDL_Scancode key) const;
    bool IsKeyPressed(SDL_Scancode key);
    int ClicksInRange(SDL_Scancode key, float timeRange,float elapsedSec);

private:
    Uint8 prev_state[SDL_NUM_SCANCODES];
    int m_AmountOfClicks;
    bool m_IsKeyPressedAlready;
    float m_Timer;
    
};

