#pragma once
#include <unordered_map>

namespace th
{
    class InputManager
    {
    public:
        static InputManager& getInstance();
        
        // 更新所有按键状态（每帧调用）
        void update();
        
        // 检查按键是否刚刚按下（需要释放后重新按下）
        bool isKeyJustPressed(int key);
        
        // 检查按键是否按下（持续检测）
        bool isKeyDown(int key) const;
        
        // 检查按键是否释放
        bool isKeyReleased(int key) const;
        
    private:
        InputManager() = default;
        ~InputManager() = default;

        struct KeyState {
            bool current = false;   // 当前帧状态
            bool previous = false;  // 上一帧状态
        };
        
        std::unordered_map<int, KeyState> keyStates;
    };
}