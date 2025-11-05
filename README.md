# 🎮 Dune: Sand World

A 2D game developed using **C++** and **Cocos2d-x**.  
This project was created to learn new technologies and practice game development skills.

---

## 🚀 Main Features
- ⚔️ **Gameplay:** Use the arrow keys to move the player, press **Z** to shoot, and **Y** to jump  
- 🎨 **Graphics:** 2D rendering using the Cocos2d-x Sprite system  
- 🔊 **Audio:** Supports sound effects and background music  
- 🧠 **Logic:** The player loses health when hit by a bullet; bullets that hit enemies will defeat them  

---

## 🛠️ Technologies Used
| Component | Description |
|------------|-------------|
| **Language** | C++ |
| **Game Engine / Framework** | Cocos2d-x |
| **IDE** | Visual Studio |
| **Operating System** | Windows |

---

## 🎮 Demo
-- **Image**
<img width="1277" height="717" alt="image" src="https://github.com/user-attachments/assets/c135eb2c-68f5-4fd3-b55e-00416deeffc3" />

<img width="1277" height="720" alt="image" src="https://github.com/user-attachments/assets/336f357c-d7a0-43f1-b5d4-119c11575758" />

<img width="1272" height="713" alt="image" src="https://github.com/user-attachments/assets/aba0b47f-4a04-48c3-907b-88be1fadc642" />

<img width="1273" height="713" alt="image" src="https://github.com/user-attachments/assets/699d075b-f53b-4131-a30d-5a9447c90586" />

<img width="1277" height="718" alt="image" src="https://github.com/user-attachments/assets/586e6cf8-bc28-49fa-babe-259b9817340d" />

-- **Video**
![20251105-1305-44 5191162](https://github.com/user-attachments/assets/88ef9f3e-c70f-4e54-b06a-ec2ebb06d587)

![20251105-1308-44 7605858](https://github.com/user-attachments/assets/9dc69b76-bd90-43f7-bd12-c94d42fdaa30)

---

## 📦 Build & Run

### 🧩 Build the Game

```bash
# 1. Create build directory
mkdir build

# 2. Enter the build directory
cd build

# 3. Generate Visual Studio project files
cmake .. -G "Visual Studio 17 2022" -A Win32
```

### 🧩 Run the Game

```bash
# 1. Enter build directory
cd build

# 2. Open the generated Visual Studio solution
btl3_game.sln

# 3. Run the project using "Local Windows Debugger"

# 4. Alternatively, after building, you can run the game directly:
cd bin/btl3_game/debug
btl3_game.exe


