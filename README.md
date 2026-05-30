# 🐍 SnakeBrandi
<img width="475" height="260" alt="image" src="https://github.com/user-attachments/assets/c5c951fb-e20c-4189-a390-039156d43fb7" />


<img width="513" height="565" alt="image" src="https://github.com/user-attachments/assets/7f070b1e-2381-4f28-bea8-39c57309ac8a" />


<img width="487" height="270" alt="image" src="https://github.com/user-attachments/assets/7bb2dedd-8754-4d0b-8faf-df97a7637970" />




![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows-blue)
![License](https://img.shields.io/badge/License-MIT-green.svg)

> A two-player Snake game written in C — local multiplayer, adaptive speed, sound effects, and a persistent high score. Runs entirely in the Windows terminal.

<img width="500" height="282" alt="2026-05-3002-15-57-ezgif com-video-to-gif-converter" src="https://github.com/user-attachments/assets/94658b8c-ce21-4a9f-a5be-00348dcbef9f" />

---

## Features

- **Single-player and local two-player modes** on the same keyboard
- **Player 1** (green `O`, `WASD`) vs **Player 2** (red `X`, arrow keys)
- **Adaptive speed** — the snake accelerates as your score climbs
- **Sound effects** via the PC speaker — a beep on every fruit, plus a game-over jingle
- **Persistent high score** saved to `highscore.txt` between runs
- **Color-shifting board** that changes color each time a fruit is eaten
- **Pause/resume** (`P`) and an in-game menu with instructions

---

## Controls

| Action     | Player 1 | Player 2     |
| ---------- | -------- | ------------ |
| Move Up    | `W`      | `↑`          |
| Move Down  | `S`      | `↓`          |
| Move Left  | `A`      | `←`          |
| Move Right | `D`      | `→`          |
| Pause      | `P`      | —            |
| Quit       | `X`      | —            |

Eat the fruit (`F`) to grow and score **50 points**. Avoid the walls, your own tail, and the other snake.

---

## Getting Started

### Requirements

- **Windows** — the game uses the Win32 console API (`windows.h`) and `conio.h`
- A C compiler — **[MinGW-w64](https://www.mingw-w64.org/) (gcc)** recommended

### Build & Run

```bash
gcc snake2.c -o snake.exe
snake.exe
```

No extra flags are needed — `Beep` and the console-color functions live in `kernel32`, which gcc links by default.

---

## How It Works

The game runs on a fixed **20×20** board inside a classic loop: `draw()` renders the frame, `input()` reads the keyboard without blocking (`_kbhit`/`_getch`), and `logic()` advances both snakes, checks collisions, and handles fruit. Movement speed is recalculated each tick from the current score, so the game naturally gets harder the better you play. The high score is loaded at startup and rewritten to disk whenever it's beaten.

---

## Roadmap

- [ ] **Cross-platform port** — replace the Win32/`conio.h` calls with `ncurses` so it runs on Linux and macOS
- [ ] **Unify the game loop** — single-player and multiplayer currently duplicate the same loop; extract one `startGame()` function
- [ ] **Dynamic tail growth** — the tail arrays are fixed at 100; switch to dynamic allocation so a long snake can't overflow
- [ ] **Fruit never spawns on a snake** — re-roll the fruit position if it lands on a body segment

---

## Author

**Lucas Daniel** — [GitHub](https://github.com/NoodleLDS) · [LinkedIn](https://www.linkedin.com/in/enf-lucas-daniel/)

One of my first projects in C — built to learn pointers, structs, file I/O, and game loops from the ground up.

---

## License

This project is licensed under the MIT License.
