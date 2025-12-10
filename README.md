# Galaxy_Wars_in_cpp
👤 Developed by Maaz Hussain & Danyal Aziz | 🎓 OOP Final Project
A robust 2D space survival strategy game built from scratch using C++ and the SFML library. This project serves as a comprehensive implementation of advanced Object-Oriented Programming principles, featuring a custom game engine, memory management, and persistent data systems.


🎮 Project Overview
Galaxy Wars places the player in the cockpit of a starship to defend the galaxy against waves of alien invaders and asteroids. The game was designed to demonstrate architectural patterns in game development, moving beyond basic syntax to create a scalable software system.


✨ Key Features & Happenings

Progressive Difficulty: Features three distinct levels—Easy, Medium, and a Hard mode featuring a final Boss Battle with unique zigzag movement patterns and AI-driven targeting.




Object Pooling: Implemented a template-based ObjectPool<T> to efficiently manage memory for high-frequency objects like projectiles and power-ups, solving performance lag issues.



Binary Save/Load System: A custom serialization system that writes game state (Score, Level, Lives) to binary .dat files, preventing simple text editing cheats.



High Score System: A persistent leaderboard using text file I/O and bubble sort algorithms to rank top commanders.



Power-Up System: Random drops including Shields, Extra Lives, Score Multipliers, and a "Freeze" ability that stops enemies in time.



Dynamic Audio: A SoundManager class that switches background tracks between menus and gameplay and handles spatial sound effects.

🛠️ Technical Concepts & Architecture
This project strictly adheres to OOP principles as detailed in our class diagrams:


Polymorphism & Abstraction: The GameObject abstract base class defines virtual update() and draw() methods overridden by Player, Enemy, and Asteroid.


Templates: Used for the ObjectPool to manage generic game entities efficiently.


Operator Overloading: The + operator is overloaded in the Player class to handle logic when interacting with power-ups.


Exception Handling: Custom GameException class to gracefully handle missing assets (textures/fonts) or file I/O errors without crashing.



Friend Classes: The Player class declares Game as a friend to allow the engine access to private timers and shield states.

📂 File Structure
main.cpp - Entry point.

Game.h / .cpp - The central engine managing the loop and states.

GameObject.h - Abstract parent class for all entities.

ObjectPool.h - Template class for memory management.

save_x.dat - Binary save files.

© 2025 Maaz Hussain & Danyal Aziz. All Rights Reserved.
