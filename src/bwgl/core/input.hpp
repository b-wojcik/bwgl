#pragma once

#include <array>

namespace bwgl {
    // Transforms GLFW key constants into an enum.
    enum class KeyCode : int {
        // https://www.glfw.org/docs/3.3/group__keys.html
        // https://www.glfw.org/docs/latest/group__buttons.html

        // Mouse
        Mouse1 = GLFW_MOUSE_BUTTON_1,
        Mouse2 = GLFW_MOUSE_BUTTON_2,
        Mouse3 = GLFW_MOUSE_BUTTON_3,
        Mouse4 = GLFW_MOUSE_BUTTON_4,
        Mouse5 = GLFW_MOUSE_BUTTON_5,
        Mouse6 = GLFW_MOUSE_BUTTON_6,
        Mouse7 = GLFW_MOUSE_BUTTON_7,
        Mouse8 = GLFW_MOUSE_BUTTON_8,
        MouseLeft = Mouse1,
        MouseRight = Mouse2,
        MouseMiddle = Mouse3,

        // Printable 
        Space = GLFW_KEY_SPACE,
        Apostrophe = GLFW_KEY_APOSTROPHE,
        Comma = GLFW_KEY_COMMA,
        Minus = GLFW_KEY_MINUS,
        Period = GLFW_KEY_PERIOD,
        Slash = GLFW_KEY_SLASH,

        Digit0 = GLFW_KEY_0,
        Digit1 = GLFW_KEY_1,
        Digit2 = GLFW_KEY_2,
        Digit3 = GLFW_KEY_3,
        Digit4 = GLFW_KEY_4,
        Digit5 = GLFW_KEY_5,
        Digit6 = GLFW_KEY_6,
        Digit7 = GLFW_KEY_7,
        Digit8 = GLFW_KEY_8,
        Digit9 = GLFW_KEY_9,

        Semicolon = GLFW_KEY_SEMICOLON,
        Equal = GLFW_KEY_EQUAL,

        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,

        LeftBracket = GLFW_KEY_LEFT_BRACKET,
        Backslash = GLFW_KEY_BACKSLASH,
        RightBracket = GLFW_KEY_RIGHT_BRACKET,
        GraveAccent = GLFW_KEY_GRAVE_ACCENT,

        // Navigation
        Escape = GLFW_KEY_ESCAPE,
        Enter = GLFW_KEY_ENTER,
        Tab = GLFW_KEY_TAB,
        Backspace = GLFW_KEY_BACKSPACE,
        Insert = GLFW_KEY_INSERT,
        Delete = GLFW_KEY_DELETE,

        RightArrow = GLFW_KEY_RIGHT,
        LeftArrow = GLFW_KEY_LEFT,
        DownArrow = GLFW_KEY_DOWN,
        UpArrow = GLFW_KEY_UP,

        PageUp = GLFW_KEY_PAGE_UP,
        PageDown = GLFW_KEY_PAGE_DOWN,
        Home = GLFW_KEY_HOME,
        End = GLFW_KEY_END,

        // Locks and system
        CapsLock = GLFW_KEY_CAPS_LOCK,
        ScrollLock = GLFW_KEY_SCROLL_LOCK,
        NumLock = GLFW_KEY_NUM_LOCK,
        PrintScreen = GLFW_KEY_PRINT_SCREEN,
        Pause = GLFW_KEY_PAUSE,

        // Function
        F1 = GLFW_KEY_F1,
        F2 = GLFW_KEY_F2,
        F3 = GLFW_KEY_F3,
        F4 = GLFW_KEY_F4,
        F5 = GLFW_KEY_F5,
        F6 = GLFW_KEY_F6,
        F7 = GLFW_KEY_F7,
        F8 = GLFW_KEY_F8,
        F9 = GLFW_KEY_F9,
        F10 = GLFW_KEY_F10,
        F11 = GLFW_KEY_F11,
        F12 = GLFW_KEY_F12,
        F13 = GLFW_KEY_F13,
        F14 = GLFW_KEY_F14,
        F15 = GLFW_KEY_F15,
        F16 = GLFW_KEY_F16,
        F17 = GLFW_KEY_F17,
        F18 = GLFW_KEY_F18,
        F19 = GLFW_KEY_F19,
        F20 = GLFW_KEY_F20,
        F21 = GLFW_KEY_F21,
        F22 = GLFW_KEY_F22,
        F23 = GLFW_KEY_F23,
        F24 = GLFW_KEY_F24,
        F25 = GLFW_KEY_F25,

        // Numpad
        Numpad0 = GLFW_KEY_KP_0,
        Numpad1 = GLFW_KEY_KP_1,
        Numpad2 = GLFW_KEY_KP_2,
        Numpad3 = GLFW_KEY_KP_3,
        Numpad4 = GLFW_KEY_KP_4,
        Numpad5 = GLFW_KEY_KP_5,
        Numpad6 = GLFW_KEY_KP_6,
        Numpad7 = GLFW_KEY_KP_7,
        Numpad8 = GLFW_KEY_KP_8,
        Numpad9 = GLFW_KEY_KP_9,

        NumpadDecimal = GLFW_KEY_KP_DECIMAL,
        NumpadDivide = GLFW_KEY_KP_DIVIDE,
        NumpadMultiply = GLFW_KEY_KP_MULTIPLY,
        NumpadSubtract = GLFW_KEY_KP_SUBTRACT,
        NumpadAdd = GLFW_KEY_KP_ADD,
        NumpadEnter = GLFW_KEY_KP_ENTER,
        NumpadEqual = GLFW_KEY_KP_EQUAL,

        // Modifiers
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        LeftControl = GLFW_KEY_LEFT_CONTROL,
        LeftAlt = GLFW_KEY_LEFT_ALT,
        LeftSuper = GLFW_KEY_LEFT_SUPER,

        RightShift = GLFW_KEY_RIGHT_SHIFT,
        RightControl = GLFW_KEY_RIGHT_CONTROL,
        RightAlt = GLFW_KEY_RIGHT_ALT,
        RightSuper = GLFW_KEY_RIGHT_SUPER,

        Menu = GLFW_KEY_MENU,
    };

    enum class KeyState : int {
        Release = 0,
        Click = 1,
        Hold = 2
    };

    // Singleton class for handling GLFW keyboard and mouse callbacks.
    class Input {
    public:
        static Input& get() {
            static Input instance;
            return instance;
        }

        // Returns true only once, on its first call, during the time that a key is being held down.
        // Used both for keyboard presses and mouse button inputs.
        bool didKeyClick(KeyCode key) {
            if (keys[int(key)] == KeyState::Click) {
                keys[int(key)] = KeyState::Hold;
                
                return true;
            }

            return false;
        }

        // Returns true if a key has been clicked or is being held down.
        // Used both for keyboard presses and mouse button inputs.
        bool isKeyDown(KeyCode key) {
            KeyState state = keys[int(key)];
            return state == KeyState::Click || state == KeyState::Hold;
        }

        // Cursors X position.
        float cursorX() const {
            return m_cursorX;
        }

        // Cursors Y position.
        float cursorY() const {
            return m_cursorY;
        }

        // !!! Should be called only by bwgl::Window !!!
        void setWindowDimensions(float width, float height) {
            m_windowWidth = width;
            m_windowHeight = height;
        }

        // !!! Should be called only by bwgl::Window !!!
        void setKeyState(int key, KeyState state) {
            keys[key] = state;
        }

        // !!! Should be called only by bwgl::Window !!!
        void setMouseButtonState(int button, KeyState state) {
            keys[button] = state;
        }

        // !!! Should be called only by bwgl::Window !!!
        void setCursorPos(float x, float y) {
            m_cursorX = x;
            m_cursorY = m_windowHeight - y;
        }
    private:
        // GLFW constans for mouse buttons and keyboard keys don't overlap
        std::array<KeyState, GLFW_KEY_LAST + 1> keys;

        // Window Size
        float m_windowWidth = 0.0f;
        float m_windowHeight = 0.0f;
        
        // Cursor Position
        float m_cursorX = 0.0f;
        float m_cursorY = 0.0f;

        Input() {
            keys.fill(KeyState::Release);
        }

        ~Input() = default;
    };
}