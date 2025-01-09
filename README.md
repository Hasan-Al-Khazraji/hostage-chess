# Hostage Chess ♟

Welcome to Hostage Chess! A chess variant where captured pieces are not removed from the board but are instead held "hostage" and can be reintroduced by the capturing player on their turn.

## How to Play Hostage Chess 🎮
1. Be sure that the project is running (see instructions below)
2. Begin by entering a username
3. Wait for your opponent to connect
4. Once the game starts make a move and click **Done**
5. Wait for your opponent to make a move
6. First player to capture King or have opponent's time run out wins

### Special Rules of Hostage Chess 📜

1. **Capturing Pieces**: When a piece is captured, it is not removed from the game. Instead, it is placed in the capturing player's "prison".
2. **Reintroducing Pieces**: Captured pieces can be reintroduced to the board by the capturing player on their turn. The piece can be placed on any empty square.
3. **Promotion**: Pawns can be promoted to any piece when they reach the opposite end of the board.
4. **Airfield**: Pieces can be dropped from the airfield onto the board, similar to reintroducing captured pieces.
5. **Hostage Exchange**: Players can exchange pieces between their prison and airfield, allowing for strategic reintroductions.


## How to run the project 💾

1. Start by cloning the project into a local repository
2. Run in the terminal of the project `export LD_LIBRARY_PATH=/path/to/lib:$LD_LIBRARY_PATH`
3. Generate the Python bindings from the C code using `swig -python hclib.i`
4. Run `make all`
5. Run the python server by running `python3 ./server.py your_port`
6. Visit `http://localhost:your_port/`
7. Enjoy!

## Languages and Tools Used 🛠
- Python
- C
- SWIG
- SQLite
- Javascript
- Jquery
- HTML and CSS
- Tailwind

### How the technlogies were used
- Backend: Python
- Frontend: JS and Jquery
- Database: SQLite
- Game Logic: C and SWIG
- Testing: C and Python

## Screenshots 📸
Start Screen
![Start](https://github.com/user-attachments/assets/a4b80e97-1b83-43de-b51a-0bb790525938)
Loading Screen
![Loading Screen](https://github.com/user-attachments/assets/3acdf62b-ef06-4bb3-bd75-3348b2fb4657)
Game Screen (ik its ugly I haven't fixed it yet)
![Game Screen](https://github.com/user-attachments/assets/695fc13e-c454-471e-9e34-a4cc0cd41c22)
Winner Screen
![Winner Screen](https://github.com/user-attachments/assets/3348579c-2d2d-4315-a0f7-f85801147e57)
Game History
![Game History Screen](https://github.com/user-attachments/assets/9a464df6-c001-4558-94ce-2fa55ab206da)
Game Log Image 1
![Game Log Image 1](https://github.com/user-attachments/assets/d1427ca0-82f6-41ce-ba26-45f738ae1f5c)
Game Log Image 2
![Game Log Image 2](https://github.com/user-attachments/assets/93b93c86-baf6-4e56-aca9-ac4952391b13)
