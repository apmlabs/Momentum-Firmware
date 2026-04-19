<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset=".github/assets/logo_dark.png">
    <source media="(prefers-color-scheme: light)" srcset=".github/assets/logo_light.png">
    <img
        alt="Momentum Firmware"
        src=".github/assets/logo_dark.png">
  </picture>
</p>

<h2 align="center">
  <a href="#Install">Install</a> · <a href="#list-of-changes">Features</a> · <a href="https://discord.gg/momentum">Discord</a> · <a href="#%EF%B8%8F-support">Donate</a>
</h2>

This custom firmware is based on the [Official Firmware](https://github.com/flipperdevices/flipperzero-firmware) for [Flipper Zero](https://flipperzero.one/), and includes most of the awesome features from [Unleashed](https://github.com/DarkFlippers/unleashed-firmware). It is a direct continuation of the Xtreme firmware, built by the same (and only) developers who made that project special.

<br>
<h2 align="center">Modus Operandi</h2>

The goal of this firmware is to constantly push the bounds of what is possible with Flipper Zero, driving the innovation of many new groundbreaking features, while maintaining the easiest and most customizable user experience of any firmware. Fixing bugs promptly and ensuring a stable and compatible system is also of our utmost importance.

- <h4>Feature-rich: We include all third-party features and apps as long as they fulfill a useful purpose and they work correctly, aswell as implement ourselves many new exciting functionalities.</h4>

- <h4>Stable: We ensure the most stable experience possible by having an actual understanding of what's going on, and proactively making all tweaks and additions backwards-, and inter-, compatible.</h4>

- <h4>Customizable: You can tweak just about everything you see: add/remove apps from the menu, change the animations, replace icon graphics, change your Flipper's name, change how the main menu looks, setup different keybinds like never before, and so much more. All on-device, with no complicated configuration.</h4>

<br>

Note that mentioned below are only a few of our staple additions to the firmware. For a full list check [down here](https://github.com/Next-Flip/Momentum-Firmware#List-of-Changes).

<br>
<h2 align="center">Momentum Settings</h2>

We offer a powerful and easy-to-use application tailor-made for our firmware, that lets you configure everything you could dream of, and more:

<img src=".github/assets/settings.png" align="left" height="160vh"/>
<img align="left" height="180vh" width="10" src="https://upload.wikimedia.org/wikipedia/commons/3/3d/1_120_transparent.png">

- <ins><b>Interface:</b></ins> Tweak every part of your Flipper, from the desktop animations, to the main menu, lockscreen behavior, file browser, etc.

- <ins><b>Protocols:</b></ins> Configure SubGhz settings, add/remove custom frequencies, extend SubGhz frequencies to 281-361, 378-481, 749-962 MHz and setup which GPIO pins are used by different external modules.

- <ins><b>Misc:</b></ins> Everything else that doesn't fit the previous categories. Change your Flipper's name, XP level, screen options, and configure the <a href="https://github.com/Z3BRO/Flipper-Zero-RGB-Backlight">RGB backlight</a>.

<br>

<br>

<h2 align="center">Animations / Asset Packs</h2>

We created our own improved Animation / Asset system that lets you create and cycle through your own `Asset Packs` with only a few button presses, allowing you to easily load custom Animations, Icons and Fonts like never before. Think of it as a Theme system that's never been easier.

<img src=".github/assets/packs-folder.png" align="left" width="200px"/>
You can easily create your own pack, or find some community-made ones on <b><a href="https://momentum-fw.dev/asset-packs">our website</a> or on Discord</b>. Check <a href="https://github.com/Next-Flip/Momentum-Firmware/blob/dev/documentation/file_formats/AssetPacks.md">here</a> for a tutorial on creating your own. Essentially, each <code>Asset Pack</code> can configure its own <code>Anims</code>, <code>Icons</code> & <code>Fonts</code>.

<br clear="left"/>

<br>

<img src=".github/assets/packs-select.png" align="left" width="200px"/>
Once you have some asset packs, upload them to your Flipper in <code>SD/asset_packs</code> (if you did this right you should see <code>SD/asset_packs/PackName/Anims</code> and/or <code>SD/asset_packs/PackName/Icons</code>). Alternatively, install directly using the website.


<br clear="left"/>

<br>

<img src=".github/assets/packs-done.png" align="left" width="200px"/>
After installing the packs to Flipper, hit the <code>Arrow Up</code> button on the main menu and go to <code>Momentum Settings > Interface > Graphics</code>. Here choose which asset pack you want and tweak the other settings how you prefer, then exit the app to reboot and enjoy your fully customized Flipper!

<br clear="left"/>

<br>

<h2 align="center">Bad Keyboard</h2>

<img src=".github/assets/badkb.png" align="left" width="250px"/>
BadUSB is a great app, but it lacks a lot of options. Bad-KB allows you to customize all USB and Bluetooth parameters for your attacks.

In Bluetooth mode it allows you to spoof the display name and MAC address of the device to whatever you want. Showing up as a portable speaker or a wireless keyboard is easily doable, allowing you to get the attention of your target without needing a cable at hand.

In USB mode it also enables additional functionality to spoof the manufacturer and product names, as well as vendor and product IDs (VID/PID).

<br>

<h2 align="center">List of changes</h2>

There are too many to name them all, this is a **non-comprehensive** list of the **most notable from an end-user perspective**. For a more detailed list, you can read through the [**changelogs**](https://github.com/Next-Flip/Momentum-Firmware/releases) and commits/code. Also, you can find a **feature comparison with other firmwares** on [our website](https://momentum-fw.dev/).

Note that this repo is always updated with the great work from our friends at [Unleashed](https://github.com/DarkFlippers/unleashed-firmware) and the latest changes from [OFW](https://github.com/flipperdevices/flipperzero-firmware). Below are mentioned only **our** changes that we can actually be credited for, so make sure to check their fantastic additions aswell. And a huge thank you to both teams!

```txt
[Added]

- Momentum App (Easy configuration of features and behavior of the firmware)
- Asset Packs (Unparalleled theming and customization)
- More UI customization, redesigns and optimizations
- Bad-Keyboard App
- BLE Spam App
- FindMy Flipper App
- NFC Maker App
- Wardriver App
- File Search across SD Card
- Additional NFC parsers and protocols
- NFC Type 4 protocol and NTAG4xx support
- Subdriving (saving GPS coordinates for Sub-GHz)
- Easy spoofing (Name, MAC address, Serial number)
- Video Game Module color configuration right from Flipper
- Enhanced RGB Backlight modes (Full customization & Rainbow mode)
- File management on device (Cut, Copy, Paste, Show, New Dir, etc.)
- Remember Infrared GPIO settings and add IR Blaster support in apps
- Advanced Security measures (Lock on Boot, reset on false pins, etc.)
- Disk Image management (Mount and view image contents, open in Mass Storage)
- Extended JavaScript API (Support for UsbDisk/Mass Storage, File operations)
```
```txt
[Updated]

- Enhanced WiFi support for easiest setup ever
- Extended keyboard with cursor movement and symbols
- File Browser with Sorting, More supported File Types
- Advanced and optimized Level System (Up to 30 levels)
- Desktop Keybind system for full key and press/hold remapping
- Storage backend with instant rename and virtual mounting for disk images
- Expanded Sub-GHz App (Duplicate detection & Ignore, Autosave, History improvements)
- Improved Error Messages (Showing source file paths)
```
```txt
[Removed]

- Unused Dummy Mode
- Broken or Superfluous apps
```

<br>


<br>

<h2 align="center">External Applications Catalog</h2>


This firmware includes **667** external applications across multiple categories.


### Games (145 apps)

| App | Description |
|-----|-------------|
| **1D Pacman** | This repository contains an implementation of 1D Pacman using the Flipper Zero Game Engine as boilerplate. Make sure you clone the source code with submodules: |
| **2048** | - play up to 65K - progress is saved on exit |
| **2048 (Original)** | *No description available* |
| **4 in a Row** | *No description available* |
| **99 Bottles** | *No description available* |
| **Air Arkanoid** | *No description available* |
| **Air Labyrinth** | This is a simple game where you have to navigate a ball through a maze. The game is controlled by tilting the Flipper Zero with a Video Game Module attached.  You can also play the game using the Flipper Zero's d-pad. 1. Attach the video game modu... |
| **Apple Grabber** | Apple Grabber Game for Flipper Zero Flip the your device on vertical mode and use LEFT/RIGHT to move, the objective is to grab all the apples. |
| **Ardudrivin** | 'Ard Drivin' is an Arduboy racing game Coding: Rem |
| **ArduGolf** | **ArduGolf** is a 3D minigolf game with solid ball physics, ported to the **Flipper Zero** handheld device. Play through **18 holes**, plan your shots, bounce off walls, and use slopes to finish each course in as few strokes as possible. |
| **Arduventure** | This project is a port of the original Arduventure project. The main goal of this fork is to port the game from the Arduboy console to **Flipper Zero**. A lightweight retro RPG/adventure for Flipper Zero, based on the classic Arduventure for Ardub... |
| **Arkanoid** | *No description available* |
| **Asteroids** | This is an implementation of the classic Asteroids game for the Flipper Zero. Inside you will find a simple 2D engine that can be reused to implement other games. Note: This one is SimplyMinimal's fork of Antirez's version with several modificatio... |
| **Avocado Zero** | A tiny **care sim** for Flipper Zero: you suspend an avocado pit over a glass of water (toothpicks and all) and try to keep it alive long enough to grow a full root system. Time passes, the water gets murky, and if you neglect it for too long—game... |
| **Banana** | Welcome to the Banana App! 🍌 This app is all about bananas. Everyone loves bananas and want to share that love with you. Whether you're looking to track your banana consumption, find new banana recipes, or learn fun facts about bananas, this app h... |
| **BlackJack** | > The app should be compatible with the official and custom flipper firmwares. If not, follow these steps to build it * Download your firmware's source code |
| **Blackjack By RocketGod** | Blackjack! for your Flipper Zero - Full Blackjack gameplay: |
| **Bomber Fox** | Bomberman for the FlipperZero utilising the SubGHz radio for multiplayer functionality with other nearby flipper devices! For more information about the SubGHz protocol, please see protocol.md |
| **Bomberduck** | Bomberman clone on flipper zero! |
| **BzzBzz** | Haptic rhythm-matching game focusing on tactile feedback rather than visual cues. - The app generates a 5-step vibration sequence. |
| **CarJacker** | Flipper Zero Carjacker App - Private Unleashed V2 Follow the white rabbit. He will lead you through the passage to the land of wiggly roosters. |
| **Catacombs Of The Damned** | **Catacombs of the Damned!** is a first-person shooter / dungeon crawler for the Arduboy miniature game system. Explore **10 floors** of a procedurally generated dungeon, destroy monsters with magical fireballs, and collect as much loot as you can. |
| **Cells Lab** | Это приложение для Flipper Zero, представляющее собой простой клеточный автомат с базовым генетическим алгоритмом. Программа симулирует поведение клеток, которые перемещаются по сетке, потребляют энергию и, при наличии достаточного количества энер... |
| **Checkers** | A classic checkers (draughts) game for the Flipper Zero. This was inspired and based off of Flipper Chess by xtruan |
| **Chess** | `HOW ABOUT A NICE GAME OF CHESS?` - Built against `1.1.2` Flipper Zero firmware release |
| **Chess Clock** | *No description available* |
| **Cigarette** | Finally a way to smoke a cigarette on the Flipper Zero Download the **flipper_cigarette.fap** from the latest release and copy it to the **apps/Games** directory on your **Flipper Zero** |
| **City Bloxx** | City Bloxx for the Flipper Zero! |
| **Cognizant Flipper** | This mysterious app reveals the hidden consciousness of your Flipper Zero. With each press of the OK button, the Flipper shares a word from its digital mind, slowly unveiling its thoughts and desires. Are these random words, or is your Flipper try... |
| **Color Guess** | As a web developer I enjoy guessing colours by HEX Code. This game is targeted at other Devs and graphic designers that also enjoy this. |
| **Connect Wires** | - famous puzzle concept - play on 3 different complexity levels |
| **CountDown** | A game inspired by the "Countdown" numbers round. Select 6 numbers, choose a 3-digit target, |
| **Crossy Road** | Crossy Road de Mikael Fortier [Technologie Émergente] |
| **DeadZone** | Fight through tough challenges, dodge enemy fire, and avoid the falling obstacles to survive! Use the **Left**, **Right**, and **Up** arrow keys to move the player.\ |
| **Decision Maker** | A Flipper Zero app that lets you type your own choices and let a roulette wheel decide — powered by the device's hardware true-random-number generator. - Add up to **20 custom decisions** (up to 20 characters each) |
| **Devillier's Platformer** | This is a simple platformer game developed for the Flipper Zero. The game allows players to control a character as they navigate through a level filled with obstacles and platforms, aiming to reach the finish line. - Building & Running The Applica... |
| **Dice [RM]** | - Displays roll date/time on each roll - Allows 1 to 5 dice to be rolled at once |
| **Digital Kaleidoscope** | Digital Kaleidoscope is a simple, animated visualizer for Flipper Zero. It displays four different patterns that shift and change, turning your Flipper into a miniature kaleidoscope. - **Four Animated Styles** |
| **DnD Dice** | Version: 1.3 (Changelog) **DnD Dice** is a dice rolling application for your **Flipper Zero**. |
| **DOOM** | As tradition goes, Doom is being ported to almost every possible embedded electronic device. Therefore I did an attempt to come up with something close to Doom and still compatible on the Flipper Zero's hardware. This is not the actual Doom game b... |
| **Drifter** | Drifter is a boat game for the Flipper Zero Use left and right keys to affect the drift. The longer the keypress, the bigger the impact. The score multiplier keeps increasing when no key is pressed. |
| **DVD Bouncer** | **simple dvd-bounce application for flipper** Y'know how dvd players got that thing that bounces around? |
| **Executor Keychain** | **Flipper Keyller** is an app for the Flipper Zero that emulates the iconic sounds of the classic 80s keychain: the **Echo Keyller** (also known as the **Executor Keychain**). This vintage gadget produced laser-like sounds when pressing its button... |
| **Fighter Jet** | Simple game about flying a fighter jet. Fly around, shoot the enemies, and dont loose your healt. This is just a demo, might make more finalized version later. |
| **Five Nights at Flipper's** | FNAF game for Flipper Zero. - up - door button |
| **Flappy Bird** | *No description available* |
| **Flight Assault** | Flight Assault is a dynamic action game for flipperzero where players control a spacecraft engaged in combat against enemy ships approaching from different directions. 1. Launch the game on your device. |
| **Flipper Hero** | Welcome to **Flipper Hero**, an engaging typing practice game ported for the Flipper Zero device! **Flipper Hero** is inspired by the beloved minigame "Stratagem Hero, " found in the "Helldivers 2" universe. Situated on your Ship, this game become... |
| **Flippy Road** | A simple arcade game for the Flipper Zero After plugging in your Flipper Zero, either |
| **Fortune Cookie** | The Fortune Cookie app delivers a random motivational quote or fortune each time you open it, providing a small dose of inspiration to brighten your day. |
| **Furious Birds** | Well-known birds and pigs game. The bird is really furious! Pigs are under attack! Next level has even more pigs to eliminate. And you have just 3 attempts! |
| **Game 15** | Original link Logic game Wikipedia |
| **Game of Life** | *No description available* |
| **Geometry Dash** | Geometry Dash Demake with level system, basic physics engine, coyote time physics and basic UI. I am making real ship, ball and ufo physics! |
| **Geometry Dash** | Geometry Dash Demake with level system, basic physics engine, coyote time physics and basic UI. I am making real ship, ball and ufo physics! |
| **Groks Adventure** | **Grok's Adventure v3.3** is the smallest infinite liminal space souls-like game for the Flipper Zero, built with AI code assistance from Grok 3 (xAI). Set in the eerie Backrooms, players control Grok to navigate a procedurally generated world, ba... |
| **Guess The Number** | > An enhanced number guessing game with progressive feedback, score tracking, and immersive multi-sensory experience. - **Progressive Hint System** - Distance-based feedback (very close, close, far) |
| **Hangman** | Первая русскоязычная программа для Flipper Zero. Правила очень просты — «Флиппер» загадывает слово, вам надо его отгадать, выбирая буквы. Если буква встречается, она будет нарисована на той позиции или позициях, |
| **Hanoi Towers** | - famous puzzle concept - complexity level increases each time you solve the puzzle |
| **Heap Defence** | *No description available* |
| **Hirn - Crack Color Codes** | A simple Flipper Zero combination cracking game. A one-player logic game where Flipper Zero creates a secret color code with 4 sybmols of 6 different colors. The user has to guess it in the fewest attempts. She/He receives clues: Black pegs for co... |
| **Hold 'em** | Native single-player Texas Hold'em built specifically for Flipper Zero. Play a full table of compact, readable Hold 'em against up to four bots with real betting rounds, side-pot-aware showdowns, save/load, and a UI tuned for the actual device scr... |
| **Hunter Killer** | A submarine warfare simulation game ported from Pico-8 to Flipper Zero using the Flipper Zero Game Engine. - **Realistic submarine physics** - Navigate using heading and velocity controls |
| **Impostor** | **Flipper Zero** external application (**FAP**). Install it on the **microSD** of your Flipper and run it from **Apps → Games → Impostor Game**. It is a **device-only** party helper: you do not need a phone or PC during the game. Inspired by **Und... |
| **Infinite Tic Tac Toe** | This is an extension of the favorite childhood game of Tic-Tac-Toe but with pieces only lasting 3 turns. This allows for the game to go till there is a winner. |
| **Insults** | *No description available* |
| **Jetpack Joyride** | `JETPACKS, ROCKETS, AND ADVENTURE AWAITS!` - Recreated based on the classic Jetpack Joyride game for Flipper Zero. |
| **Jumping Pawns** | Jumping Pawns is a strategy game for the Flipper Zero. Play locally against a friend or challenge a minimax algorithm. From the main menu: |
| **KC Line** | A one line/row version of the game where a character eats little bits and gets chased around the screen by a ghost. It's pretty obvious where the |
| **Laser Tag** | Use Flipper Zero as your laser blaster, RFID scan for power-ups, and automatic detection of add-on weapons to GPIO such as the Rabbit Labs Masta-Blasta for arena style play. - **Team Battles**: Choose your team and face off in epic Red vs. Blue la... |
| **Laser Tag** | Use Flipper Zero as your laser blaster, RFID scan for power-ups, and automatic detection of add-on weapons to GPIO such as the Rabbit Labs Masta-Blasta for arena style play. - **Team Battles**: Choose your team and face off in epic Red vs. Blue la... |
| **Lifecounter** | A Flipper Zero life tracker for collectible card games such as Flesh and Blood, Magic the Gathering, Sorcery, Lorcana etc. - Buy a Flipper Zero device |
| **Lights Out Flipper Zero** | **Lights Out Flipper Zero (LOFZ)** is a Star Wars-inspired puzzle game for the Flipper Zero, based on the classic "Lights Out" game. As the last Jedi, you must toggle a 17-cell grid of lights to turn them all off, battling the evil Sith Lord Flipp... |
| **Magic 8-ball** | A magic 8 ball game for the Flipper Zero. Press the center button to get a random answer to your question. |
| **Mancala** | * **Left / Right-Button.** Moves cursor between the 6 user pits * **OK** picks up stones from selected pit and perform the distributions of the stones according to the rules |
| **Mandelbrot Set** | *No description available* |
| **Matagotchi** | Tamagotchi(R) like game for Flipper Zero The pet grows from an egg to an adult and then dies of old age. |
| **Mind Probe** | This is a simple version of the online game Akinator/20 Questions I first saw reddit user EscanorSensi asking if a 20 question style game could be implemented on the flipper and it got me thinking... |
| **Mine Sweeper** | -	Enable board verifier for board generation to ensure unambiguous boards! -	Set board width and height |
| **Minesweeper** | This is a Minesweeper implementation for the Flipper Zero device. - Arrow buttons to move |
| **Mode 7 Demo** | A simple proof-of-concept of pseudo 3D rendering, modelled after a famous Mode 7 from SNES. * **Up/Down/Left/Right** - Move the camera |
| **Monster Slayer** | This repository contains an example project showcasing the usage of the Flipper Zero Game Engine. Make sure you clone the source code with submodules: |
| **Monty Hall** | *No description available* |
| **Morse Master** | A comprehensive Morse code learning and practice app for Flipper Zero. Final release of version 1.0. The Morse Master is an interactive application for Flipper Zero that helps you learn and practice Morse code. The app offers two main modes: a Lea... |
| **Multi Counter** | This is a simple external application for the Flipper Zero modified from VS Counter version. The app give you access to up to 4 counters to count your scores for tabletop games for example. |
| **Multi-Dice** | *No description available* |
| **Mystic Balloon** | **Mystic Balloon** is an atmospheric 2D platformer featuring **39 levels**, each requiring precision, careful planning, and attention to detail. The player controls a character who navigates the levels using **balloons**. The hero can **slowly des... |
| **Nah 2 da Nah Nah Nah** | **Nah2-Nah3** is a fun and engaging mini-game application for the Flipper Zero, featuring rhythm and action-based gameplay optimized for the device's 128x64 display and input system. - **Multiple Game Modes**: Choose from six games (Zero Hero and ... |
| **Network Defender** | A cybersecurity-themed game for the Flipper Zero where you defend computer networks from attacks. You play as a network administrator protecting four computer systems from cyber attacks. Your goal is to prevent systems from being hacked and keep p... |
| **Nu pogodi** | *No description available* |
| **P1X Adventure** | *No description available* |
| **P1X Your Own Adventure** | *No description available* |
| **PacMan WIP** | *No description available* |
| **Panis - A Grumpy Bread** | Just a grumpy bread walking and jumping around. - **Left/Right:** Move Panis left or right |
| **Paper Plane** | Port of this game to the Flipper Zero. Your goal is to fly a paper plane through obstacles and get to the bottom of the map. |
| **Pinball0** | Play pinball on your Flipperzero! Get the latest version: |
| **Pocket Battle+** | An enhanced Pokemon battle game for the Flipper Zero, featuring authentic Pokemon data integrated from the pokeyellow disassembly project. - **151 Original Pokemon** with accurate base stats from Pokemon Yellow |
| **Pong** | A Pong game for the Flipper Zero Assuming the toolchain is already installed, copy **flipper_pong** directory to **applications_user** |
| **Puck Girl** | A simple Flipper Zero arcade chase game. The player controls a pie-shaped character ("Puck girl") through a maze, collecting dots while avoiding ghosts. Power pills temporarily make ghosts vulnerable. Like in the classic game, each ghost has disti... |
| **Quadrastic** | **Quadrastic** is a simple addicting game inspired by the Arduboy game of the same name. This, in turn, was based on a popular PSP homebrew game called Squarez. Squarez itself drew inspiration from Ball, a game created by Nicolay Edin and Axel Ham... |
| **Questions** | Flipper app designed to foster meaningful connections by helping you discover more about each other. Perfect for getting in touch with new people or strengthening existing bonds. |
| **Race** | Race game inspired by Race game in BrickGame 9999 in 1. Source is heavily base on tetris game from jeffplang's repo and this article. |
| **Race Game** | A fast-paced 3-lane vertical scrolling car racing game for Flipper Zero. - **3 Lane Racing** — Dodge incoming vehicles on a scrolling road |
| **Reaction Test** | A simple reaction test for the flipper zero. |
| **Reaction Time** | This is a simple reaction game for the Flipper Zero device. The game is designed to test your reaction times, giving you immediate feedback on how quickly you respond to visual prompts. - The game begins with a "Press to start" screen. Press any b... |
| **Reversi** | 1. Clone firmware 2. Go to the `applications_user` directory |
| **Rock Paper Scissors** | This multi-player application relies on a SubGHz API that was recently changed.  Older firmware will get a build error, due to not supporting the new API definitions. It should build successfully on the following versions: |
| **Rock Paper Scissors 2** | A fun rock paper scissor game for the Flipper Zero device. ./fbt fap_rps |
| **Roots of Life** | A zen-puzzle game for FlipperZero puzzle made on GlobalGameJam'23 (theme: "Roots") Nothing interesting in the code, as you would expect from a jam-rushed development. Was just curious about making a custom app for the device |
| **Rubik's Cube Scrambler** | Install the .fap file and put it in your apps folder The On/Off button toggles the vibration notification on and off. The "New" button generates a new scramble. The scramble letters correspond with the following moves: R = Right, L = Left, U = Up,... |
| **Sand Simulation** | Simulation is running at a blazing 10FPS by design. (Can be changed in code) Simulation is running on 128x64 (F0 screen size) |
| **Scorched Tanks** | A flipper zero game inspired by scorched earth. Current state is shown below: |
| **Secret Toggle** | A game for the Flipper Zero device. - Arrow buttons move around the board. |
| **Simon Says** | - LED indicator turns red when Simon is speaking - Use DPAD to match Simon's sequence |
| **Slot Machine** | Simple slots with highscores for flipper zero. *version 1.0.1* |
| **Slots** | A high-fidelity slot machine for your Flipper Zero. When you're bored and feeling lucky, jump into this "all-ways" simulator and see if you can hit the jackpot! 81-Way Slots brings the excitement of the casino floor to your pocket. Unlike traditio... |
| **Snake 2.0** | *No description available* |
| **Snake Game** | *No description available* |
| **Sokoban** | Sokoban is a classic puzzle game. Your objective is to push all the boxes onto the targets. Rules are simple: |
| **Solitaire** | * **Auto-Solve:** Ability to automatically solve the game when all cards are flipped. * **Animated Card Movements:** Animated transitions during solve and deal. |
| **Space Impact** | Gameplay video on Nokia phone: |
| **Space Impact II** | Pixel-perfect clone of Nokia's Space Impact, one of the biggest classics in mobile gaming. All credit for the game goes to Bence Sgánetz, original creator of the clone. Port was made with effort to preserve as much of the |
| **Space Invaders** | Space Invaders adaptaion for Flipper Zero. Check github actions to download or build your own with `ufbt`. |
| **Stratagem Zero** | Stratagem hero clone for the Flipper Zero Designed to be as close to the original as possible |
| **Sudoku** | - ok - increment number - back - clear number |
| **SUPER-CHIP** | A SUPER-CHIP emulator for the Flipper Zero Just click the FAP badge in the title. |
| **Swimmy Fish** | Flappy, er, Swimmy Fish FlipperZero: |
| **T-Rex Runner** | Flipper Zero port of Chrome's running T-rex game - Added command to move DINO |
| **Tamagotchi** | Tama P1 Emulator for Flipper Zero ======================================= |
| **Tanks** | *No description available* |
| **Tarot** | Tarot spread for Flipper Zero. This app can give you a linear spread of the major arcana cards (minor arcana is planned to be added). - Official app store |
| **Tetris** | *No description available* |
| **Tetris Modern** | *No description available* |
| **Tic Tac Toe** | *No description available* |
| **Tic-Tac-Toe Multi** | *No description available* |
| **Ultimate Tic-Tac-Toe** | Ultimate Tic-Tac-Toe is a strategic variant of Tic-Tac-Toe where you have 9 Tic-Tac-Toe boards arranged in a 3x3 grid. The aim of the game is to win more small boards (3x3) to ultimately win the large board (3x3 of Boards). 1. The first player can... |
| **Umpire Indicator** | A baseball umpire indicator app for the Flipper Zero device! This app simulates a traditional baseball umpire's counter, tracking balls, strikes, and outs with a classic scoreboard-style interface. 🧢📋 - ⚾ Tracks balls (0-4), strikes (0-3), and out... |
| **Vexed** | Vexed is puzzle game, where your goal is to clear level from bricks in smallest possible count of moves. Easy to learn, **hard to master** Originally developed for Palm.OS by James McCombe, and later extended by The Vexed Project, was abandonware ... |
| **Video Poker** | *No description available* |
| **Wave** | This game is almost entirely AI-generated through all circles of hell. The logic is simple: the arrow falls down, and when you hold down the **OK** or **UP** button, you start flying up. Avoid obstacles on the way.There is also a pause in the game... |
| **Wolfenduino** | Wolfenduino is a demake of id software's Wolfenstein 3D for the Arduboy FX. The game recreates the classic shareware episode "Escape from Wolfenstein" with original-style gameplay adapted for portable hardware. |
| **Yappy Invaders** | Yappy Invaders is a space invaders clone designed specifically for the Flipper Zero device. It combines the classic gameplay of space invaders with the unique hardware capabilities of Flipper Zero, creating an engaging experience for users. Classi... |
| **Yatzee** | Yahtzee game for flipperzero Its not beautiful, but it works and now I can play Yahtzee on my flipper while I shit at work. |
| **ZERO!** | ZERO! is a card game where the aim is to get rid of all your cards before your opponents! The game implements the rules of the classic card game UNO, with adaptations to make it playable on the Flipper Zero. 1. Players must match the card in the d... |
| **Zombiez** | *No description available* |

### Tools (107 apps)

| App | Description |
|-----|-------------|
| **[WIP] Skeleton Sample App** | You can use this application as a starting point for creating your own application.  It contains the skeleton framework to get an application running on the Flipper Zero. Please let me know any feedback! |
| **Access Audit** | A Flipper Zero app for **defensive auditing of NFC and RFID access-control credentials**. Tap a card, get an instant risk score and plain-English advice. Save a named session report to SD. |
| **Air Level** | This is a simple level that can use the motion-tracking sensor of the Video Game Module. - Clone your firmware (`git clone --recursive https://github.com/flipperdevices/flipperzero-firmware.git`) |
| **Analog Clock** | *No description available* |
| **Authenticator** | *No description available* |
| **Barcode** | A barcode generator for the Flipper Zero that supports **UPC-A**, **EAN-8**, **EAN-13**, **Code-39**, **Codabar**, and **Code-128**[1] Note: Barcode save locations have been moved from `/barcodes` to `/apps_data/barcodes` |
| **BG Loader** | This is a plugin for the Flipper Zero that allows compatible apps to be loaded and to remain running in the background. |
| **Big Clock** | > Full-screen digital bedside/tableside clock with adjustable brightness **⬇️ Download Latest Release** - Get the `.fap` file and copy to `/ext/apps/Tools/` on your Flipper. |
| **BlackJack Counter** | A BlackJack Counter Fap for Flipper Zero embedded Device. |
| **Brainfuck** | Brainfuck interpreter and editor for the F0. Supports text inputs and outputs. |
| **Caesar Cipher** | A caesar cipher encoder for the Flipper Zero device. Start app, painfully input your ciphertext with the onscreen keyboard. Replace spaces with underscores. Hit "Save", scroll output. |
| **Calculator** | *No description available* |
| **Calendar** | - Flipper Zero Official Website. A simple way to explain to your friends what Flipper Zero can do. - Flipper Zero Firmware Update. Improvements for your dolphin: latest firmware releases, upgrade tools for PC and mobile devices. |
| **Calendar of weeks** | This **Calendar of week** is an application for the Flipper Zero that displays a weekly view of dates &mdash;  the current week along with the previous and next week. Today's date is highlighted with inverted colors. Monday is the first day of the... |
| **Calendar TECHART** | This repository contains the source code for a simple calendar application designed for the Flipper Zero device. The app provides an intuitive display of the days of the month and supports basic navigation to explore different months and years. - ... |
| **CAN Tools** | CAN Tools is a Flipper Zero external app for creating simple DBC-style signal definitions and decoding CAN frames with them. - Create and save DBC signal entries (name, CAN ID, bit layout, scaling, unit, min/max). |
| **Chronometer** | ⏱️⏱️ A chronometer application for the Flipper Zero ⏱️⏱️ This chronometer is accurate to the millisecond. **TIM2** internal timer of the **STM32** MCU is used to generate a 64 MHz clock signal. This signal is used to count elapsed time. |
| **CLI-GUI Bridge** | Allows CLI control from GUI, giving untethered access to sub-ghz chat, system diagnostics, and more. Swing by the releases page, and download a pre-built .fap file for the latest flipperzero firmware. Use qFlipper to copy the .fap file into SD Car... |
| **Clock** | *No description available* |
| **Combo Cracker** | **Combo Cracker** is an on-the-go combination lock cracking tool for the **Flipper Zero**, inspired by security researcher Samy Kamkar’s work on the mechanical vulnerabilities in *Master Lock* combination padlocks. Using a clever approach/exploit ... |
| **Combo Cracker** | **Combo Cracker** is an on-the-go combination lock cracking tool for the **Flipper Zero**, inspired by security researcher Samy Kamkar’s work on the mechanical vulnerabilities in *Master Lock* combination padlocks. Using a clever approach/exploit ... |
| **Count Down Timer** | `up/down`: set second/minute/hour value. `ok`: start/stop counting. |
| **Counter** | This is a simple plugin for the Flipper Zero. It gives you access to a counter which you can increment and decrement using the up and down buttons respectively. |
| **Device Info** | *No description available* |
| **Doc Viewer Lite** | A lightweight text document viewer application for Flipper Zero that allows you to read text files directly on your device. - View text files (`.txt`) stored on your Flipper Zero's SD card |
| **DTMF Dolphin** | Original Link DTMF (Dual-Tone Multi-Frequency) dialer, Bluebox, and Redbox. |
| **Enigma** | - Built against `0.104.0` Flipper Zero firmware release - Uses enigma-machine lib |
| **Eye Saver** | This app is designed to prevent eye strain when looking at a screen for extended periods of time. 69 percent of Americans have reported eye strain at least once a week, and this app is designed to minimize eye fatigue and discomfort. * Repeating 2... |
| **F0 Forth** | This is a plugin for the Flipper Zero that ports Once started the plugin will add a `forth` command to the CLI that can be used |
| **Financial Calculator** | **Very basic TVM calculator for Flipper Zero** Have you ever thought to yourself "Man, I wish I could figure out the Future Value of a $12,000 investment, over the span of 3 years, with a 7.5% interest rate, in which I pay another $3,500 into each... |
| **FlipBIP Crypto Wallet** | - Last built against `1.0.1` Flipper Zero firmware release - Using Trezor crypto libs from `core/v2.5.3` release |
| **FlipCrypt** | FlipCrypt is a Flipper Zero app that provides a collection of classic cipher algorithms, cryptographic hash functions, and some text encoding methods to explore and learn about. You can emulate the result using NFC, generate a QR code, or save it ... |
| **FlipCrypt** | FlipCrypt is a Flipper Zero app that provides a collection of classic cipher algorithms, cryptographic hash functions, and some text encoding methods to explore and learn about. You can emulate the result using NFC, generate a QR code, or save it ... |
| **Flipfetch** | Sistema de información estilo fastfetch para Flipper Zero. - Firmware version |
| **FlipNote** | **A real text editor for Flipper Zero** — bc 5 buttons and a 128×64 screen are more than enough. > Made to prove the Flipper forum wrong. They said it couldn't be usable. |
| **Flipp Pomodoro** | *No description available* |
| **FlipPar** | FlipPar is a Flipper Zero external app for tracking golf or disc golf rounds on-device. It lets you set the number of holes and players, rename players, record par and score values hole-by-hole, and export a plain-text score sheet to the SD card. ... |
| **Flipper Aid** | Flipper application for emergency response with pulse timer and CPR timer |
| **Flipper Printer** | A Flipper Zero external application that combines a coin flip game with thermal printer functionality using the T7-US thermal printer module. *The T7-US thermal printer in action with the Flipper Zero coin flip game and custom text printing.* |
| **Flipper Wedge** | A Flipper Zero application that transforms your device into a contactless tag-to-keyboard interface. Read RFID and NFC tags, then automatically type their UIDs and NDEF data as HID keyboard input via USB or Bluetooth. - **RFID (125 kHz)**: EM4100,... |
| **Flipper95** | Application to stress test the Flipper by finding Mersenne primes using the Lucas-Lehmer Test, much like Prime95 does. |
| **FlipperPwn** | _____ _ _                 ____ \|  ___\| (_)_ __  _ __   _\|  _ \__      ___ __ |
| **FlipperZero Clock** | Displays customizable clock in full-screen or split-view with date and weekday. Customizations are saved in a configuration file and retrieved on next application start. Use the ok key to switch between split-view and full-screen mode. |
| **Ford Radio Codes** | Ford Radio 'M' &amp; 'V' Unlock Code Generator For Flipper Zero! Instantly generate unlock codes for for 'M' and 'V' series radios directly on your Flipper Zero! |
| **GnomishTool** | Gnomish army knife for Flipper Zero owners. Civilian market version! Another gift from the brothers from Gnomeregan. |
| **HEX Editor** | inspired by QtRoS/flipper-zero-hex-viewer Read any file line by line, and by Ok allow change char. Useful for NFC file "Edit Dump" feature with out smartphone. |
| **HEX Viewer** | Hex Viewer application for Flipper Zero! The app allows you to view various files as HEX |
| **HyperFocus Calc** | External Flipper Zero app that computes **hyperfocal distance** from focal length (mm), aperture (full-stop sequence), and each sensor’s **circle of confusion** (CoC). CoC defaults to diagonal/1500 from sensor width and height (mm); you can overri... |
| **IconEdit** | Create images and animated icons up to 128x64 in size directly on the Flipper! Easily preview how your images, icons, and animations will look directly on the device. Send your images to your PC without swapping the SDCard! Significantly improves ... |
| **ID Card** | This program represents a simple application for managing an ID card for Flipper Zero. The "ID Card v2" application allows users to easily manage personal data such as name, phone number, address, and notes. These data are stored in a text file (i... |
| **Key Copier** | A Flipper Zero app for measuring key bitting patterns. The best way to install the latest build would be download it from Flipper Zero's Official App Store: |
| **Key Copier** | A Flipper Zero app for measuring key bitting patterns. To measure your key: |
| **LED Blinker** | A Flipper Zero application that blinks LEDs with a decreasing frequency over time. Unlike traditional Pomodoro timers, it provides visual feedback through LED blinks that gradually slow down. This application can be downloaded from Flipper Lab. |
| **LED Pulsator** | This app is a proof-of-concept to show abilities of the LED light integrated in the Flipper Zero device. By default the Flipper Zero firmware framework offers various controls to the LED, including |
| **Letter Beacon** | A letter/number/symbol Morse beacon on **RFID**/**NFC** interfaces of the **Flipper Zero** (banner has been made with **Dall.E 2**) |
| **Lightning Distance** | An app that calculates the distance of a lightning strike based on time between seeing the flash and hearing the thunderclap. I am not, nor do I claim to be a programmer in any sense. I had an idea I thought was cool, utilised AI to make it, then ... |
| **Lishi** | An app for saving values obtained from the **Lishi tool**, which is used for lock decoding. This app allows users to input, view, and store values from the decoding process for easy reference and organization. The Lishi tool is used to decode lock... |
| **ListEM** | **ListEM** is a Flipper Zero application that generates large, customizable UID dictionaries for **RFID**, **NFC**, and **iButton** protocols **directly on your Flipper!**. I decided to build it as a native Flipper app for **flexibility, portabili... |
| **Math Wiz** | Polynomial-Solver is an application for the Flipper Zero that enables users to solve and manipulate polynomials up to the sixth degree. With this app, you can evaluate polynomials, calculate derivatives and integrals, and visualize results directl... |
| **Mitzi Type Aid Demo** | My take on a Flipper Zero type-aid. You can compare my version with out-complete with the standard keyboard. From the main screen, you have three options: |
| **Moon Phases** | A simple application to display the current phase of the moon along with additional lunar information. For Flipper Zero device. |
| **Multi Converter** | *No description available* |
| **Multi Timer** | *A feature-rich multi-timer application for Flipper Zero with a charming dolphin mascot!* Features • Installation • Usage • Development • Screenshots |
| **NFC/RFID Detector** | *No description available* |
| **Nickname Generator** | Nickname Generator in flipper zero Easily create unique names using this app for flipper zero. |
| **Nightstand Clock** | Fork of the standard clock app, for use as a bedside clock at night -Date and AM/PM have their places swapped |
| **Notes for FZ (WIP)** | *No description available* |
| **Orgasmotron** | This app is a proof-of-concept to show abilities of the vibration motor in the Flipper Zero device. I'm also using this to learn how to code for Flipper Zero |
| **Paranoia** | Paranoia Mode is an anti-surveillance tool for Flipper Zero that scans for hidden wireless cameras, RFID skimmers, and infrared monitoring devices. Perfect for travelers, privacy enthusiasts, or anyone concerned about electronic eavesdropping. - *... |
| **Password Generator** | This is a simple Password Generator plugin (**fap**) for the Flipper Zero. |
| **Password Manager** | Use you flipper zero as a password manager and authenticator! The flipper stores your usernames and password and can write them on your PC acting as a keyboard! |
| **Pet a Dolphin** | Flipper app to make your dolphin a little bit happier (Reset mood) 1. Put directory into applications_user directory of flipperzero firmware sdk |
| **Pet Your Dolphin** | An app for the Flipper Zero that allows you to pet your Dolphin companion and soothe it while earning XP. |
| **Pomodoro Timer** | The Pomodoro Technique is a time management method developed by Francesco Cirillo in the late 1980s.[1] It uses a kitchen timer to break work into intervals, typically 25 minutes in length, separated by short breaks. Each interval is known as a po... |
| **Programmer Calculator** | This repository contains a calculator application designed to run on Flipper Zero. - `Decimal to Binary` |
| **Programmer Calculator** | This repository contains a calculator application designed to run on Flipper Zero. - `Decimal to Binary` |
| **QR Code** | Display qrcodes on the [Flipper Zero] Grab the latest `qrcode.fap` from [Releases]. |
| **QRCode Generator** | Generates and displays QRCodes on the flipper zero. \| ! \| ! \| !  \| |
| **Quac!** | Get the latest version: * Flipper Lab - recommended |
| **Quadratic Solver** | This app takes a, b, and c values from a quadratic equation in form "ax^2 + bx + c = 0" Select a number when hovering over a, b, or c with **LEFT** to decrease number, and **RIGHT** to increase. Press **OK** when selected values to solve the equat... |
| **Reality Clock** | > Dimensional stability monitoring using real CC1101 multi-band RSSI analysis **⬇️ Download Latest Release** - Get the `.fap` file and copy to `/ext/apps/Tools/` on your Flipper. |
| **Resistance Calculator** | A simple app to calculate resistance from the coloured bands. See: * Pick from 3, 4, 5, or 6 band resistor. |
| **Resistance Calculator** | A simple app to calculate resistance from the coloured bands, based on original work by Lewis Westbury. * Pick from 3, 4, 5, or 6 band resistor. |
| **Roman decoder** | An Flipper Zero app that converts Roman numerals to decimal values. Roman Decoder is an application that allows users to easily convert Roman numerals to decimals. The app provides an interactive user interface that allows users to select Roman ch... |
| **ROT13** | I moved the source code to github because the flipper zero application catalog only accepts repos from github Actual repo: https://github.com/nothingbutlucas/flipperzero_rot13 |
| **Rubiks Cube Timer** | *No description available* |
| **SD Info** | The application shows information about the SD card and you can also perform a test that will show the card status. |
| **Segment Clock** | Simple segment clock. Press the up button to toggle between 12 and 24 hour mode. |
| **Sig Diary** | > A passive background scanning application for Flipper Zero that logs and annotates signals (IR/RF/NFC) based on fingerprinting. Perfect for hobbyist recon or daily security audits. - **Multi-protocol Scanning**: Simultaneously monitors IR, RF, a... |
| **SKIDcity** | *"Your flipper can't do that. and if it could, you'd be in federal prison."* So you got a Flipper Zero. Maybe you saw it on TikTok. Maybe some guy on Discord told you it could hack traffic lights, clone any card, crash wifi networks, and steal car... |
| **Smartra VIN2PIN** | Flipper Zero app calculator for SMARTRA2 immobilizer pins for **Hyundai** and **KIA** vehicles - Supports most Hyundai/KIA models using SMARTRA2. |
| **Space Travel Calculator** | A minimalist trajectory calculator for the Flipper Zero. Plan real interplanetary missions with KSP-style visual feedback. Answer the critical mission planning questions: |
| **Spindle Calculator** | A trade tool for carpenters and joiners to calculate spindle counts and spacings for balustrades, right on site — no phone or calculator needed. Given a length and spindle size, it calculates: |
| **Tasks** | A TODO list application for the Flipper Zero using the UntitledFlipperZero framework. 1. Install ufbt |
| **Tasks** | A TODO list application for the Flipper Zero using the UntitledFlipperZero framework. 1. Install ufbt |
| **Text Viewer** | *No description available* |
| **Time Dilation Calculator** | A pair of minimalist space and physics calculators for the Flipper Zero. No games, no splash screens -- just the numbers. Interplanetary trajectory planner with real-time orbital visualization. Select a destination (Mars, Venus, Jupiter, Europa, T... |
| **Time Tracker** | Flipper Time Tracker is an application for flipperzero device that allows the user to track multiple tasks at the same time. Application uses csv file to store and handle task data that can be easily exported to excel or similar for further analys... |
| **Timer** | *No description available* |
| **Tone Generator** | *No description available* |
| **Tree Identification** | **Goal:** Identify a tree by answering questions on your Flipper Zero. In case there is only one question to answer, the user flow would roughly be the following |
| **uPython** | Allows you to use the power of Python natively on your Flipper Zero. The application is available on the official Flipper Lab. |
| **uPython** | Allows you to use the power of Python natively on your Flipper Zero. The application is available on the official Flipper Lab. |
| **VIN Decoder** | VIN decoder for Flipper Zero VIN Decoder is a mini flipper zero tool that allows you to decode VIN (Vehicle Identification Number) numbers. This project provides users with the ability to enter their VIN and obtain information about the vehicle, i... |
| **Voltage Calculator** | VoltCalc is a simple voltage calculator to calculate values using Ohms Law V = R * I. Yes, a calculator or your phone could probably do this much faster, but it seemed like a good first app |
| **Voltage Calculator** | VoltCalc is a simple voltage calculator to calculate values using Ohms Law V = R * I. Yes, a calculator or your phone could probably do this much faster, but it seemed like a good first app |
| **Voyah Password** | This program allows you to generate the engineering menu password for Voyah vehicles. Since the password depends on the current date and is calculated according to the Chinese time zone, you will need to set your current time zone after launching ... |

### Nfc (51 apps)

| App | Description |
|-----|-------------|
| **AmiTool** | An Flipper Zero toolkit to work with NTAG215 tags with gaming consoles. This project is usable but still in development. Ensure you back up all important data and avoid using it with another experimental setup to prevent damage or data loss. |
| **AmiTool** | An Flipper Zero toolkit to work with NTAG215 tags with gaming consoles. This project is usable but still in development. Ensure you back up all important data and avoid using it with another experimental setup to prevent damage or data loss. |
| **Amusement IC** | * "next page" in display scene to show overflowing info (full vendor name, PMm) * emulate url card (see nfcurl) |
| **COGS MyKey** | This is a Flipper application for editing and writing COGES Mykey Dongles. I’m releasing this because idiots are selling similar apps for profit and violating the MIKAI license. This release is for educational purposes only and comes with zero sup... |
| **Cyborg Detector** | App generates a continous NFC field to make body mod implant LEDs glow. Tested on a Dangerous Things xSIID. - Feedback showing when a cyborg is detected. |
| **GhostBook** | **Encrypted NFC Contact Sharing for Flipper Zero** Share your contact info with a tap. Protected by passcode. Auto-wipes on failed attempts. |
| **HID iClass (PicoPass)** | See readme: https://lab.flipper.net/apps/picopass/ git clone https://github.com/bettse/picopass.git |
| **ISO 15693-3 NFC Writer** | An application for the Flipper Zero device that provides advanced handling of ISO 15693-3 compliant NFC tags (commonly found as ICODE SLIX). The application allows reading and writing memory data, managing block locks, and modifying special AFI an... |
| **Metroflip** | Metroflip is a multi-protocol metro card reader app for the Flipper Zero, inspired by the Metrodroid project. It enables the parsing and analysis of metro cards from transit systems around the world, providing a proof-of-concept for exploring tran... |
| **MFDesfire Auth** | *No description available* |
| **MFKey** | *No description available* |
| **MFP Reader** | A standalone Flipper Zero application for reading, dumping and emulating **MIFARE Plus SL3** smart cards. Implements the full MFP SL3 protocol over |
| **Mi Band NFC** | A comprehensive Flipper Zero application for managing NFC data on Xiaomi Mi Band devices with advanced features including automatic backups, detailed logging, and progress tracking. This application provides a complete toolkit for reading, writing... |
| **MIFARE Classic Editor** | *No description available* |
| **Mifare Fuzzer** | This app allows you to fake Mifare UIDs. It can emulate either UIDs or full cards from your Flipper's storage. |
| **MiZip Balance Editor** | This app main goal is to make it easy to modify a MiZip tag balance. If you want to contribute to make this app better, feel free to submit PR! |
| **Networking NFC + QR** | Esta aplicación para Flipper Zero permite emular una tarjeta NFC y mostrar un código QR simultáneamente en la pantalla. Es ideal para situaciones de networking, permitiendo compartir tu contacto o información de dos formas distintas al mismo tiemp... |
| **NFC APDU Runner** | *No description available* |
| **NFC Comparator** | This is a simple tool for checking NFC cards using a Flipper Zero device. It allows you to compare a stored NFC card against a physical card, checking for UID, UID length, and protocol. The tool is designed to help users identify and manage NFC ca... |
| **NFC Dict Manager** | A powerful application for your Flipper Zero that lets you manage, optimize, and organize your NFC dictionaries with ease. Handle mf_classic_dict.nfc and mf_classic_dict_user.nfc files, without connecting to your PC. The app provides a main menu w... |
| **NFC Fuzzer** | _   _ _____ ____   _____ \| \ \| \|  ___/ ___\| \|  ___\|   _ _______  _ __ |
| **NFC Keyboard** | Paste NFC card ids to keyboard input |
| **NFC Login** | A Flipper Zero app that uses NFC cards to automatically type passwords on your computer via USB HID or BLE HID. Perfect for quick desktop logins using NFC cards, tags or implants! Special thanks to **Equip**, **tac0s** and **WillyJL** for the help... |
| **NFC Magic** | *No description available* |
| **NFC Maker** | *No description available* |
| **NFC Playlist** | The idea behind this app is to allow for you to test multiple copies of NFC's at once as a bulk test When starting the app you are greeted by a select file option where you choose the playlist you wanna run. |
| **NFC Relay** | NFC Relay app for flipper zero, relay NFCA APDU and response between two flipper. * configurable UART pin and baudrate (Right now is hardcored using `15/16 aka. C1/C0` and 38400) |
| **NFC Sniffer** | This is a simple Flipper app that listens for the commands being sent by a NFC reader, and displays them to the Flipper's screen. The commands are also logged to the nfc_sniffer_logs folder in the root of the ext directory of the Flipper. Note: Th... |
| **NFC URL** | A Flipper Zero application that generates and emulates NFC tags that point to URLs. - Generate http(s), (s)ftp, mailto, etc. URLs |
| **NFC-Eink** | - This app allows emulation and writing of NFC Eink tags from Waveshare and Goodisplay - Image saved as one screen type can be then loaded to screen of another type |
| **OpenPrintTag** | A Flipper Zero application to read and write OpenPrintTag NFC tags used for 3D printing filament spools. OpenPrintTag is an open-source NFC tag standard for 3D printing materials, developed by Prusa Research. It stores material information like br... |
| **Passport Reader** | **Passy** is a tool for reading electronic passports (ePassports/eMRTDs) using the Flipper Zero device. It extracts and decodes data securely stored in modern passports, designed for hobbyists, researchers, and security professionals. - Reads and ... |
| **SaFlip** | Application to communicate with Saflok credentials and readers. Currently only works on Momentum Firmware, due to missing Date/Time screen support until OFW PR#4261 is merged. |
| **Seader** | A Flipper Zero application (aka "fap") that read credential from HID: iClass, iClass SE, Desfire EV1/EV2, and Seos using a HID SAM and UART adapter.  Latest release on the App Catalog. |
| **Seos Compatible** | Flipper app for reading and emulating Seos®-compatible cards/fobs/mobile credentials. 🎬 Demo Video |
| **Seos compatible** | Flipper app for reading and emulating Seos®-compatible cards/fobs/mobile credentials. 🎬 Demo Video |
| **SLI Writer** | I made a simple Flipper app to write **magic ISO15693 tags with changeable UID** using `.nfc` files. I also developed an **Android version** of the app, available as an `.apk`. |
| **Sonicare Head ID** | Sonicare Brush Head ID ====================== |
| **T-Union Master (China)** | T-Union Master（交通卡大师）是基于 flipper zero 平台用以查询交通联合卡综合信息的工具。 查询内容包括卡号、卡名、卡种、到期日期、余额等基础信息，充值、交易记录（10 条），交通工具、线路、站台等行程信息（30 条），查询方式为离线查询，无需蓝牙 wifi 等。 |
| **TonUINO Writer** | NFC card writer application for TonUINO audio boxes. Create and manage NFC cards for the TonUINO DIY music player system. - ✅ **Write TonUINO Cards** - Configure NFC cards with folder, mode, and special settings |
| **TuLlave** | This is a project to add the capability of reading TuLlave cards to the Flipper Zero device. TuLlave is a contactless smart card used to pay for public transport services in Bogotá, Colombia. It operates on the MIFARE Classic protocol, which is su... |
| **UDECard** | Flipper Zero application to analyse student ID cards from the University of Duisburg-Essen (Intercard) > Download UDECard directly from the Flipper Application Catalog |
| **UID Brute Smarter** | > An advanced NFC UID analysis and testing tool for the Flipper Zero. UID Brute Smarter is a security research tool for authorized penetration testing and access control system auditing. It provides advanced NFC key management and intelligent patt... |
| **UL-C Bruteforce** | *No description available* |
| **UL-C Bruteforce Optimized** | *No description available* |
| **UL-C Relay** | *No description available* |
| **ULCFKey** | *No description available* |
| **ULCFKey** | *No description available* |
| **VB Migration Assistant** | VB Lab Migration Assistant for Flipper Zero =========================================== |
| **VK Thermo** | *No description available* |
| **Weebo** | A powerful Flipper Zero NTAG215 parser, writer, emulator, remixer, and duplicator, designed for enthusiasts who want to unlock the full potential of their NFC experience. google "key_retail.bin" and you should be able to find kt. |

### Sub-Ghz (37 apps)

| App | Description |
|-----|-------------|
| **Chief Cooker** | Your ultimate Flipper Zero restaurant pager tool. Be a _real chief_ of all the restaurants on the food court! This app supports receiving, decoding, editing and sending restaurant pager signals. |
| **Chief Cooker** | Your ultimate Flipper Zero restaurant pager tool. Be a _real chief_ of all the restaurants on the food court! This app supports receiving, decoding, editing and sending restaurant pager signals. |
| **ClayLoop** | **Clay pigeon / skeet shooting controller for Flipper Zero Sub-GHz signals** Queue up to 4 `.sub` files and transmit them in sequence with configurable delay, duration, interval, and repeat count. Features LED/beep countdown, vibration feedback, m... |
| **Enhanced Sub-Ghz Chat** | This is a plugin for the Flipper Zero that reimplements the Sub-GHz chat feature that is available on the CLI. In addition it allows for basic |
| **Flipper Share** | Flipper Share is a wireless-enabled file sharing application for Flipper Zero. It allows to send any file over a Sub-GHz via internal transmitter directly from one Flipper Zero to another without any additional hardware, cables, smartphones, compu... |
| **Frequency Analyzer w External** | *No description available* |
| **FRSScan** | Scans common FRS and GMRS frequencies and plays them over the speaker so you can hear them. Press the center button to lock. |
| **Genie Door Recorder** | Version 3.x no longer requires changes to the firmware! The application includes the Genie protocol encoder/decoder, thanks to @MMX for the suggestion. - Connecting to remote |
| **HAM Scanner** | A lightweight SubGHz radio scanner, designed to behave like a scanner for common radio bands. - RSSI live view during scanning |
| **Marmalade** | This **RF Marmalade App** for the Flipper Zero, made by **RocketGod**, is a powerful tool for marmalade across multiple radio frequencies and modulation schemes. Below is an in-depth look at each mode, from its technical details to the real-world ... |
| **Music to Sub-GHz Radio** | The `Music to Sub-GHz Radio` application converts Flipper Music Files (.FMF) into a RAW .SUB file format that can be transmitted over the Sub-GHz radio! The Flipper Zero can receive the music and play it back. There are large collections of songs ... |
| **OpenShock** | A Flipper Zero application for controlling OpenShock compatible shockers over 433 MHz. - **Transmit** — send commands to any supported shocker by configuring model, ID, channel, command type, and intensity. Hold OK to transmit continuously. |
| **POCSAG Pager** | Plugin based on Weather Station from OFW Icons by @Svaarich |
| **ProtoPirate** | Read message by following link below: https://protopirate.net/ProtoPirate |
| **ProtoPirate** | Read message by following link below: https://protopirate.net/ProtoPirate |
| **ProtoView** | ProtoView is a digital signal detection, visualization, editing and reply tool for the Flipper Zero. The Flipper default application, called Subghz, is able to identify certain RF protocols, but when the exact protocol is not implemented (and ther... |
| **Radio Scanner** | Scans frequencies available to the CC1101 and plays them over the speaker so you can hear them. - Does NOT play "FM radio stations" since those frequencies are not available. |
| **Remote for CaiXianlin** | A Flipper Zero application to control CaiXianlin shock collar. **WARNING:** This application is intended for **educational and research purposes only**. |
| **Restaurant Pager** | This app triggers restaurant pagers in a brute force manner, useful to test if devices are still functional. - Retekess T119 |
| **Rolling Flaws** | Rolling Flaws (version 1.5) by @CodeAllNight. YouTube demo of using Rolling Flaws application.  The video shows how to use the application to simulate a receiver that has a Replay attack flaw, Pairing FZ to a receiver, Cloning sequence attack, Fut... |
| **RS41 Tracker** | Receives and decodes **Vaisala RS41** radiosondes using the Flipper Zero's built-in CC1101 sub-GHz radio — or an **external CC1101 module** on the GPIO header. |
| **ShapShup** | A simple application for viewing SubGhz RAW files in the form of a signal level as it is shows in SubGHz RAW read mode. First you need to select RAW file. |
| **Spectrum Analyzer** | This application allows you to plot a chart showing the relationship between amplitude and frequency, detecting nearby signal sources. If there is a nearby source broadcasting a signal at the observed frequency, the graph will go up sharply. The a... |
| **Sub Analyzer** | Extract as much signal data from a Flipper Zero .sub file as possible |
| **Sub Duplicate Finder** | An application for Flipper Zero to identify, manage, and clean up duplicate `*.sub` files from Sub-GHz storage. \| Main Menu \| Groups View \| File Management \| |
| **Sub-GHz Bruteforcer** | SubGhz BruteForcer from Unleashed Firmware This software is for experimental purposes only and is not meant for any illegal activity/purposes. |
| **Sub-GHz Jammer Detect** | *No description available* |
| **Sub-GHz Playlist** | *No description available* |
| **Sub-GHz Playlist Creator** | A Flipper Zero application for creating and managing playlists of .sub files. - Create new playlists |
| **Sub-GHz Playlist Creator** | A Flipper Zero application for creating and managing playlists of .sub files. - Create new playlists |
| **Sub-GHz Remote** | *No description available* |
| **Sub-GHz Scheduler** | A Flipper Zero app to send SubGHz signals at a given interval. Individual `*.sub` or playlist `*.txt` files can be used. Schedule intervals selectable: |
| **Sub-GHz Spectrum** | ____        _      ____  _ / ___\| _   _\| \|__  / ___\|\| \|__  ____ |
| **SubGhz Toolkit** | Reverse engineer Flipper Zero SubGHz protocols and Keeloq Manufacturer Codes |
| **TPMS Reader** | *No description available* |
| **TPMS Reader** | *No description available* |
| **Weather Station** | *No description available* |

### Gpio (87 apps)

| App | Description |
|-----|-------------|
| **24Cxx Programmer** | Menu główne z operacjami (Odczyt, Zapis, Kasowanie) Menu konfiguracji (Adres I2C, Rozmiar pamięci) |
| **[A33] Flipper Blackhat** | The Flipper Blackhat app is used to control the blackhat. ufbt launch |
| **[BMI/LSM] Air Mouse** | > "You can turn anything into an air mouse if you're brave enough" — Piper, a.k.a. Pez |
| **[CAN] CAN Commander** | *No description available* |
| **[DIAG] GPIO Badge** | This is an app for the GPIO DIAGNOSTICS BOARD FOR FLIPPER ZERO by MakeItHackin. - Left/Right buttons to change speed of the effect. |
| **[GB] Pokemon Trading** | The video below trades a Bulbasaur from the Flipper to a Game Boy Color with Pokemon Silver. The Game Boy trades its Cyndaquil. The Flipper is then used to modify the Cyndaquil to infect it with Pokerus, modify its EVs and IVs, and have the Cyndaq... |
| **[GPIO] 7-Segment Output** | This is a GPIO push-pull output demo application for driving a 7-segment display on the Flipper Zero.  The goal of this project is to show application developers how GPIO works for push-pull output.  This project was derived from the \plugins\basi... |
| **[GPIO] Controller** | A visual tool to control the general purpose pins of the Flipper Zero |
| **[GPIO] Explorer** | - Digital rgb led, for which you must have digital rgb led and at least 3 100 Ohm resisters or greater or the led might brown out. - Digital led, for which you must have led diod and at least 100 Ohm resistor or the led might brown out. |
| **[GPIO] Longwave Clock** | This is a Flipper Zero app to receive and decode, or simulate, multiple time signal broadcasts with different protocols and time formats. For receiving via GPIO, an inexpensive receiver connected to a receiving pin is required. DCF77 is broadcaste... |
| **[GPIO] Oscilloscope** | To compile and install cd flipperzero-firmware/applications_user |
| **[GPIO] Reader (aureli1c)** | Plugin to read the GPIOs on the Flipper Zero. |
| **[GPIO] Reader (biotinker)** | This is a fork of the `gpio` app built into the flipper, with added functionality to read GPIO inputs. Supports pulling high or low. |
| **[GPIO] RGB LED** | *No description available* |
| **[GPIO] Sentry Safe** | Plugin exploiting vulnerability to open any **Sentry Safe** or **Master Lock** electronic safes. 🔓 Based on the vulnerability described here |
| **[GPIO] Signal Generator** | *No description available* |
| **[GPIO] Timelapse** | english version below Blog post: theageoflove.ru |
| **[GPIO] Wire Tester** | This is a simple Flipper Zero app that beeps if a wire is continuous. Copy the `wire_tester` directory to the `applications_user` directory of your |
| **[KT0803] FM Transmitter** | Use KT0803-type chips to transmit signal from module AUX port FM Transmitters are illegal if they are too powerful. Use in well-shielded environment or do not use it for too long. |
| **[LED] Flashlight** | Simple Flashlight special for @Svaarich by @xMasterX Icon by @Svaarich |
| **[MAG] MagSpoof** | MagSpoof-ing, as coined / invented by Samy Kamkar, is the use of an electromagnet to emulate the data on a traditional magstripe card —  wirelessly! This app ports the original MagSpoof project to the Flipper Zero, adding file management and emula... |
| **[Mx2125] Step Counter** | This is a simple StepCounter/Pedometer for FlipperZero using a Memsic2125 module. > Thanks to @jamisonderek for the tutorial on how to implement Memsic2125 module on Flipper Zero, and for the support. |
| **[RC2014] ColecoVision** | A Flipper Zero application and [RC2014] module allowing the Flipper to be used as a controller for ColecoVision games on the [RC2014]. |
| **[SPI] Terminal** | *No description available* |
| **[TEA5767] FM Radio** | Application for flipperzero device to control radio boards. - VCC = 3V3, Pin 9 |
| **[UART] UART Terminal** | Flipper Zero app to control various devices via UART interface. \| **FW Official** \| **FW Unleashed** \| |
| **[WII] EC Analyser** | This Protocol Analyser offers a full Test and Calibrate system for Wii Extension Controllers. __Disclaimer:__ *Use of this plugin, and notably connecting an Extension Controller to the FlipperZero is performed entirely at your own risk.* |
| **Air Stats** | CO2 + climate monitor for Flipper Zero. Reads CO2 (MH-Z19B/C) and temperature/humidity/pressure (BME280, DHT22, etc.) through GPIO. Two sensors at once. \| PWM + BME280 (good air) \| PWM + BME280 (bad air) \| UART \| |
| **Analog Output** | *No description available* |
| **Atari SIO Emulator** | sio2flip is a Flipper Zero application that emulates SIO peripherals for Atari 8-bit computers. The project is still in its early stages, but it already features: - **Floppy drive emulation** |
| **Battery Checker** | ADC for Flipper with a reference voltage of 2.5V Suitable for measuring battery voltage up to 2.5V |
| **BunnyConnect** | A Flipper Zero application named after the desire to connct to a BashBunny, that bridges serial communication with USB HID keyboard functionality, enabling seamless text input and command execution across connected devices. BunnyConnect transforms... |
| **C5 Lab** | The Flipper Zero companion app (`Lab_C5.fap`) mirrors the ESP32-C5 CLI workflows (scan, results, attacks, wardrive, Sniffer Dog, portal control) so you can steer the board from the handheld UI. If you need an alternate firmware SDK that keeps the ... |
| **CAN Transceiver [MCP2515]** | This application allows a Flipper Zero to use an MCP2515 CAN transceiver to send and receive messages on a standard CAN bus. That includes those found in vehicles. Using this app, you can: |
| **CANBUS** | This app allows you to interact with CAN Bus networks. Now you can interact with CAN Bus transmissions using the Flipper Zero. Basic tasks such as sniffing and injection are available, making it easy to perform activities such as analysis, error d... |
| **CANBUS ATTACK** | This repository contains a custom Flipper Zero application for performing basic security testing on CAN Bus networks. It was developed as part of a university cybersecurity research project and is intended for educational and ethical use only. |
| **Canon Intervalometer** | + 2 LTV4N35 octocopuler + 2 220 Ohm resistor |
| **Coffee EEPROM** | - Vendors Still Allow You to Hack Their Vending Machine - Read credit on load |
| **Continuity Tester** | A collection of applications I made for Flipper Zero |
| **DelfiRTL** | Based on: https://github.com/tesa-klebeband/RTL8720dn-Deauther also the FAP was made based on UART example from https://github.com/jamisonderek/flipper-zero-tutorials This project utilizes the **RTL8720DN** WiFi module in combination with the **Fl... |
| **Digimon F-Com** | A Flipper Zero app that allows the flipper to communicate with Digimon V-Pets. Currently tested devices: |
| **E220 LoRa Configurator** | E220-900T22S(JP), a configurator for LoRa modules used only in Japan. I hope to support other modules in due course. |
| **Encoder Reader** | This is a simple app for flipper that can read incremental quadrature encoders and show absolute and relative coordinate. - 5V - supply |
| **Evil BW16 Controller** | A Flipper Zero application for controlling the Evil-BW16 WiFi deauther module via UART. **Author:** dag nazty |
| **Fencing Test Box** | This is a simple emulator of a fencing testbox for the Flipper Zero. The main menu is shown below: |
| **Flipagotchi** | This program will interface the pwnagotchi with the flipper. This will be accomplished using custom code on the pwnagotchi's end to give the flipper simple rendering instructions over UART. This project is broken down into two parts: flipagotchi a... |
| **FlipGemini** | Chat with Google's Gemini AI on your Flipper Zero! Original idea from https://github.com/d4rks1d33/Gemini-Flipper - Wi-Fi Developer Board, BW16, Raspberry Pi, or ESP32 device flashed with FlipperHTTP v2.1.4 or higher: https://github.com/jblanked/F... |
| **Flippy Temp** | flippy sensor is a WIP multi-sensor board for the flipper zero. It includes a SHT30 for temperature and humidity, a LSM6DS3TR-C for gyro and accelerometer and a PA1010D for GPS. A first batch is currently in production at JLCPCB. It comes with an ... |
| **FlipTelegram** | Flipper Zero Telegram Client - WiFi Developer Board, Raspberry Pi, BW16, or ESP32 device flashed with FlipperHTTP v2.1 or higher: https://github.com/jblanked/FlipperHTTP |
| **FM Radio w Vol [TEA+PT]** | Flipper Zero external app for TEA5767 FM reception, PT2257/PT2259-S audio control, and RDS decoding from the TEA5767 MPXO path. - Quick Start |
| **FM Transmitter KT0803** | Use KT0803-type chips to transmit signal from module AUX port FM Transmitters are illegal if they are too powerful. Use in well-shielded environment or do not use it for too long. |
| **GPIO with I2C** | *No description available* |
| **GS1 Parser [UHF]** | This is a GS1 UHF payload parser based on version 2.2 of the Tag Data Standard. This currently only supports parsing SGTIN formatted EPCs. |
| **HC-11 Modem** | * Installation Flipper Zero app send data to / from a HC-11 wireless RF UART communication module. |
| **I2C Explorer** | This is a simple tool for interactive exploration of an I2C bus topology and visualization and modification of device register |
| **INA Meter** | INA Meter is an application for Flipper Zero that allows you to read I2C-connected current/power monitors from Texas Instruments. - **INA219** – 0–26V, 16-bit resolution (±0.5% accuracy) |
| **Lab401/DigiLab** | Lab401's DigiLab by tixlegeek is a Flipper Zero® accessory that introduces a new way to explore electronics. Designed to be the first tool you reach for when you're developing, debugging, tinkering or exploring digital electronics. - **Discover si... |
| **Lab401/Light Messenger** | **LAB401**'s **LightMessenger** by **tixlegeek** is an additional hardware module designed for the **Flipper Zero**. It allows users to display messages and images in the air using POV on an array yof RGB Leds. With a resolution of 16 pixels in he... |
| **LAN Tester** | > **Русская версия ниже / Russian version below** Turn your **Flipper Zero + W5500 Lite** module into a professional-grade portable LAN tester. Analyze Ethernet links, discover network neighbors, scan subnets, fingerprint DHCP servers --- all from... |
| **LaserCat** | Привет народ! Рад представить вам мое первое приложение для **Flipper Zero**. Все с чего-то начинают, и я решил начать с простого, но забавного проекта — лазера для кошки. Это приложение включает лазер, чтобы ваш питомец мог весело провести время.... |
| **LD2410 Human Detector** | An application that turns your Flipper Zero into a sophisticated human presence detector using the **HLK-LD2410 mmWave sensor**. *   **Real-time Detection**: Displays the current detection status (No Target, Moving, Static, or Both). |
| **Logic Analyzer** | Source: https://github.com/g3gg0/flipper-logic_analyzer I'm in the process of bringing this in line with the latest flipper firmwares. Right now it loads on my flipper, next I need to test its functionality for bugs. Here are the steps I used to u... |
| **LoRa Relay** | *No description available* |
| **LoRa Sample** | Work with LoRa radio communication signals. Now you can interact with LoRa transmissions using the Flipper Zero. Basic tasks such as sniffing and injection are available, making it easy to perform activities such as analysis, error detection and c... |
| **LoRA_Terminal** | Flipper Zero app to control Lora Breakout board. - Read log and command output by uart |
| **LoRadar** | LoRadar is an app that allows users to localize stratospheric balloons using their Flipper Zero and LoRa modules. The Flipper receives LoRa APRS-IS frames directly from the balloons and forwards it to a companion mobile app via Bluetooth. |
| **MODBUS** | A Flipper plugin with helpful Modbus RTU features, you will be able to read and write data into a Mobus RTU network. Requires the Electronic Cats Flipper Addon "Modbus". |
| **Moisture Sensor** | A Flipper Zero application that reads values from a Capacitive Moisture Sensor v1.2. \| Sensor View \| Calibration Menu \| |
| **Noptel LRF Sampler** | * Installation Flipper Zero app to get range measurements from a Noptel LRF rangefinder. |
| **PIR HC-SR501 Alarm** | Simple proximity alarm app for Flipper Zero using HC-SR501 PIR sensor. Triggers vibration, LED alerts, and screen notifications. Features 3-second alarm duration with blinking feedback. |
| **Pokemon Trading** | The video below trades a Bulbasaur from the Flipper to a Game Boy Color with Pokemon Silver. The Game Boy trades its Cyndaquil. The Flipper is then used to modify the Cyndaquil to infect it with Pokerus, modify its EVs and IVs, and have the Cyndaq... |
| **SD SPI** | Flipper Zero FAP for Lock and Unlock SD card / Micro SD card through SPI protocol (CMD42). Without Flipper Zero SDBoard the SD card it must be connected as in the table below |
| **SERMA CAN-FD-HS** | This software implements an USB to CAN bridge compatible with **Linux can-utils and slcan driver.** This software application is designed to run on flipper zero device and needs SERMA CAN FD board to be plugged in GPIOs ports. |
| **Servo Tester 2** | https://user-images.githubusercontent.com/8887221/228034391-aa9df44c-dcf7-4999-9d22-590cc7ac0392.mp4 This app aims to replicate the behavior of a standard RC servo) tester, by replicating a PWM RC servo signal. You can read more about it in this b... |
| **Servotester** | Application uses A7 pin for servo PWM output. Has different modes: Manual, Center, Auto. Go to the releases, download apps.zip and extract it in your SD card. |
| **SI4713 Tuner** | *No description available* |
| **SPI Flash Dump** | ____  ____ ___   _____ _           _ / ___\|\|  _ \_ _\| \|  ___\| \| __ _ ___\| \|__ |
| **Spotify Remote** | Thank you to Derek Jamison for debugging help! https://github.com/benjamindrussell/flipper-spotify-remote/assets/46113261/9d0cc41b-262d-4086-8f4b-a16d3f9a65fe |
| **StroboMeter** | Flipper StroboMeter is an application designed to measure the RPM (revolutions per minute) of a rotating object using the Flipper Zero device. By connecting an LED to Pin 4(A4) and GND and setting the RPM, you can emit a strobe light at that speci... |
| **Tesla Mod** | English \| 繁體中文 \| 简体中文 > **Tesla FSD region-gate bypass for Flipper Zero.** Enables the FSD UI toggle for users who **already have an active FSD subscription or purchase** but are in a region where the "Traffic Light and Stop Sign Control" option... |
| **u-blox GPS** | Flipper Zero app to read from a u-blox GPS over I2C. This app can display data, log a path to a KML file, and sync the Flipper's time to |
| **UART Sniff** | *No description available* |
| **WAV Recorder** | *No description available* |
| **WHC SWIO Flasher** | *No description available* |
| **Wiegand Reader** | This application supports W4, W8, W24, W26, W32, W34, W37, W40 and W48 formats. This application can be used to test Wiegand readers and keypads. It can save the data to a file, and can load and replay the data. Timings are measured and displayed;... |
| **WS2812B LED Tester** | This application is used to test WS2812B LEDs.  You can connect the WS2812B LEDs to any available GPIO pin.  If you are powering the LEDs using the Flipper Zero, be sure to consider the power requirements of the LEDs.  The 3V3 pin has a 1200mA max... |
| **YuriCable Pro Max** | Implementation to use a Flipper-Zero as SWD/DCSD-Cable for iPhones just like known Kanzi/Kong/Tamarin Cables \| iPhone      \| DCSD \| Reset \| DFU \| JTAG \| |

### Infrared (29 apps)

| App | Description |
|-----|-------------|
| **Cross Remote** | - Infrared working - SubGhz working |
| **Fire String** | Generate truly random strings from the Flipper Zero using its IR sensor. Capture the infrared bursts from sparking a flame as an unpredictable entropy source. The app also allows saving and loading strings from internal storage or transferring the... |
| **Flame RNG** | Inspired by this repo I have implemented a similar functionality entirely in the flipper zero It uses the IR sensor on the Flipper Zero to gather entropy from IR sources (such as a lighter) to seed a random number generator. The idea is that fire ... |
| **FlipIRFreq** | FlipIRFreq is a Flipper Zero external app for sending either a high-frequency IR carrier or a low-frequency IR pulse train with a user-selected frequency, duty cycle, burst length, and output pin. - Two signal paths in one app: |
| **Flipper Tag** | Use this code as a foundation for developing Flipper Zero Applications, change the contents of this file when developing a new project - Start Screen |
| **Hard Hat Brigade IR** | Flipper Zero app. This application sends infrared messages to the hardhat that will be available at DEF CON 33. Launch app on Flipper Zero, type your message, point the device at the receiver, and press Save. After a brief pause and a vibration, t... |
| **Hitachi AC Remote** | Hitachi AC remote controller for Flipper Zero. Emulates the PC-LHxx series remote controller, specifically the PC-LH3 series, and also the PC-LH6Q variant that can be found on AliExpress and similar places. |
| **HTW AC Remote** | Flipper Zero application for controlling HTW air conditioners via infrared. - **Full IR Protocol Support** - Complete implementation of HTW AC IR protocol |
| **InfraFi** | Transmit WiFi credentials from a **Flipper Zero** to a **Linux server** using infrared. Point, press Send, connected. Built for headless servers (NAS boxes, Intel NUCs, etc.) where typing WiFi passwords is painful or impossible. |
| **IR Blaster** | IR Blaster for Flipper Zero This application allows you to guess the IR remote control codes for devices. |
| **IR Decoder** | My Flipper Zero stuff. (Not to be confused with any of my projects pertaining to my cat, Flipper). Shows the same address and command values as the standard IR app, but without any of that silly remote nonsense! |
| **IR Intervalometer** | This is a simple configurable valometer app for Sony cameras. It provides a configurable shutter timer and uses the IR blaster to trigger the camera |
| **IR Remote** | It is a plugin like UniversalRF Remix but for infrared files. I do this plugin for convenience, because the main IR app need to navigate for different button abit troublesome (buttons like up,down,left,right,back). I found it useful for TV and TV ... |
| **IR Scope** | *No description available* |
| **IR Signal Generator** | The Flipper Zero IR Signal Generator allows you to create and emit infrared signals, making it a valuable tool for various applications. This is particularly useful for security assessments and hardware hacking tasks. With a simple setup, you can ... |
| **IR Transfer** | File transfer over infrared between two Flipper Zero devices using the **NEC** protocol for both control and data. [WARNING] This repo is public for testing purpose only atm, It is not working properly as intented |
| **Lasko 2519 Timer** | An automated timer application for the Lasko 2519 Tower Fan (and compatible models). This app uses the Flipper Zero's IR blaster to cycle the fan power ON and OFF at configurable intervals. -   **Automated Cycling**: Automatically toggles the fan ... |
| **LIDAR Emulator** | This app can be used to emulate infrared signals of different LIDARs. Enter the submenu and a LIDAR gun then press and hold OK button to transmit. |
| **Midea AC Remote** | Midea Electric AC remote control for Flipper Zero. Compatible with `RG10B(B)/BGEF` remote type and probably others similar: `RG10B(B1)/BGEFU1`, `RG10B1(B)/BGEF`, `RG10B2(B)/BGCEF`, |
| **Miele Scout RX2** | Flipper Zero app for controlling the Miele Scout RX2 robot vacuum over IR. Grab the pre-built `dist/miele_scout.fap` from this repo and copy it to your Flipper Zero SD card: |
| **Mitsubishi AC Remote** | Mitsubishi Electric AC remote control for Flipper Zero. Available to download on the Flipper App Catalog here |
| **Netflix TV Remote** | Infrared remote control app for Netflix on Sony BRAVIA 4K VH2 TV. Death Note-themed graphical interface. |
| **Pause Timer** | Flipper app to ensure that you don't miss parts of your show when using the bathroom during an ad break! This app will play a pre-recorded IR signal back on a countdown, there are a few use cases but it was developed specifically for the ad thing.... |
| **R.O.B. Control** | Do you have one of these? Wikipedia - Nintendo R.O.B. |
| **TagTinker** | Infrared ESL Research Toolkit for Flipper Zero Protocol study • Signal analysis • Controlled display experiments on authorized hardware |
| **Timed Remote** | A Flipper Zero application that sends IR (infrared) commands after a configurable time delay. - Browse and select IR signals from existing `.ir` files |
| **Xbox Controller** | - IR codes for XBOX: Lucaslhm/Flipper-IRDB/Consoles/Microsoft |
| **Xbox Controller** | - IR codes for XBOX: Lucaslhm/Flipper-IRDB/Consoles/Microsoft |
| **XRemote** | Version 1.3 - Changelog Navigation to the menu to press each button individually can be often uncomfortable because it requires scrolling to the desired button and selecting it. The idea behind `XRemote` is that all physical buttons are pre-mapped... |

### Bluetooth (12 apps)

| App | Description |
|-----|-------------|
| **Anki Remote** | It lets you assign any keyboard key to each button on your Flipper Zero and use it as a BLE (Bluetooth Low Energy) remote. Key mappings are saved to the SD card inside the apps_data folder. To use the app, simply open it and select **Manage Preset... |
| **BLE Clicker** | BLE HID keyboard for voice-driven Claude Code workflow on iTerm2. Turns Flipper Zero into a wireless remote — dictate, submit, navigate panes and tabs without touching the keyboard. Designed to be held **vertically**. |
| **BLE Spam** | *No description available* |
| **BT Trigger** | Control your smartphone camera via your Flipper Zero With the BT Trigger app, you can remotely take pictures with the help of your Flipper Zero via Bluetooth Low Energy. |
| **BTHome** | This application turns the Flipper Zero into a BTHome beacon, and can be used to integrate the Flipper with home automation systems |
| **Cookie Clicker** | The goal of this application is use the Flipper Zero's bluetooth connectivity to send mouse clicks to your phone at a given frequency.  If you are playing a game, such as Cookie Clicker 2, this will result in a 'screen tap' being simulated.  Only ... |
| **FindMy Flipper** | This app extends the functionality of the FlipperZero's bluetooth capabilities, enabling it to act as an Apple AirTag, Samsung SmartTag, or a Tile Tracker. It utilizes the FlipperZero's BLE beacon to broadcast a SmartTag signal to be picked up by ... |
| **Flight Monitor** | ﻿# Flight Monitor - War Thunder Dashboard for Flipper Zero This is a comprehensive flight monitoring application designed for the Flipper Zero that interfaces with War Thunder flight simulator via Bluetooth Low Energy (BLE). The application provid... |
| **Govee Control** | Control Govee H6006 Smart LED Bulbs directly from your Flipper Zero! - BLE device discovery |
| **MagicBand Plus Lights (Unofficial)** | **Unofficial project.** Not affiliated with, endorsed, or sponsored by The Walt Disney Company. MagicBand and MagicBand+ are trademarks of Disney Enterprises, Inc. Names are used only to |
| **PC Monitor** | Flipper Application for monitoring PC resources **A backend running on your PC is required** |
| **Remote for Kodi** | *No description available* |

### Media (29 apps)

| App | Description |
|-----|-------------|
| **ATM player** | `ATM` — это плеер музыки для Flipper Zero на базе ATM-синтезатора (движок из Arduventure/Arduboy). Приложение читает текстовые файлы `*.atm` из `assets/`, компилирует их во внутренний бинарный ATM-формат и воспроизводит через `ATM.play()`. |
| **BPM Tapper** | A BPM Tapper for the Flipper Zero. 1. Navigate to the FAP Build |
| **DVD Screensaver** | A DVD player inspired screensaver for the Flipper Zero. |
| **Etch a Sketch** | Turn the Flipper Zero into an Etch A Sketch This is a modification of the original paint app. |
| **Fart Sound Generator** | Flipper Zero App to generate interesting noises. This a bit juvenile sound app allows your Flipper to break winds with adjustable parameters 💨 1. **Navigate**: Use Up/Down buttons to select a parameter |
| **Flizzer Tracker** | A Flipper Zero chiptune tracker. Supports 4 channels, external (through PA6 pin) and internal (built-in buzzer) audio output. Each channel has a functionality akin to MOS Technology SID sound chip channel. Telegram channel |
| **Fmatrix** | fmatrix is a simple Flipper Zero app that displays the famous "matrix rain" effect. (suggestions appreciated and welcome!) |
| **FMF2USBMIDI** | Convert Flipper Zero Music Player file to MIDI Signal and transfer it via USB Port. this code was tested in Flipper Zero SDK v50.1 to v54.0 |
| **Guido Music Score Reader** | A Guido musical notation reader for Flipper Zero * **OK** opens file browser if no file is loaded. Otherwise starts/stops playback |
| **Image Scroller** | A simple Flipper Zero to view annotated, tiled images. The user can scroll around a large images (consisting of many small 128x64px-tiles saved in the `assets/`-folder). There is a CSV file which specifies the arrangement of the tiles. Another CSV... |
| **Image Viewer** | Image viewer for flipper zero! 1. Create image with img2fbm |
| **Karl Eido** | A simple Flipper Zero kaleidoscope app. **Karl Eido** displays triangular grid of equilateral triangles. The up/down arrows control the size of the triangles in steps of 2px, starting at side-length a=5px up to a=63px. The base side of the leftmos... |
| **Metronome** | A metronome for the Flipper Zero. 1. Navigate to the FAP Build |
| **MIDI Ocarina** | push button to play midi note. I made this to check if the MIDI library TX works. |
| **MIDI RX** | receive midi and play song |
| **Morse Code** | Simple Morse Code parser Flipper Application Package. - Up/Down buttons to set volume |
| **Music Beeper** | *No description available* |
| **Music Player** | *No description available* |
| **Music Tracker** | -=-=- MVP Stage: minimum viable player -=-=- >Get latest build< |
| **Ocarina** | A basic Ocarina (of Time) for the Flipper Zero. Controls are the same as the N64 version of the Ocarina of Time, the Ok button takes the place of the A button |
| **Paint** | *No description available* |
| **SAM AYBABTU** | *No description available* |
| **Snowflake** | A simple Flipper Zero app to create nice-looking 60° or 120° graphics. Watch the iterative growth of a snowflake on 64x64 pixels. |
| **Space Playground** | Link to the app: https://lab.flipper.net/apps/fzspground This is a "screensaver" that simulates a space environment with planets and asteroids. |
| **Text to SAM** | - Type in a message and press the back button (or select save and press back at the text preview) - SAM will say the message and the app will exit. |
| **Tuning Fork** | Tuning fork for tuning musical instruments and more - 6 strings: Standard, Drop D, DADGAD (Dsus4), Standard D, C#G#C#F#G#C# (C#sus4), Drop C, CGCFGC |
| **USB-MIDI** | This is a simple Flipper app, that turns it into a monophonic MIDI instrument. Launch the app, connect to computer via USB, use in music software. |
| **Video Player** | A Flipper Zero application to play video files (with sound). Uses custom video file format. Is proven to work at 30 FPS and 44100 Hz audio sample rate with full 128 by 64 pixels resolution. Maximum video length is limited only by your SD card capa... |
| **WAV Player** | A Flipper Zero application for playing wav files. My fork adds support for correct playback speed (for files with different sample rates) and for mono files (original wav player only plays stereo). ~~You still need to convert your file to unsigned... |

### Usb (27 apps)

| App | Description |
|-----|-------------|
| **Agentic Remote USB** | A one-handed Flipper Zero remote for Claude Code and AI coding agents. Five buttons. No keyboard required. Built by Kasen Sansonetti & Wetware Labs. |
| **Bad Duck3** | The first DuckyScript 3.0 implementation for Flipper Zero with USB and Bluetooth HID support. Full implementation of DuckyScript 3.0 control flow: |
| **BadUSB Pro** | ____            _  _   _ ____  ____    ____ \| __ )  __ _  __\| \|\| \| \| / ___\|\| __ )  \|  _ \ _ __ ___ |
| **BarCode ScannerE** | * Speed: 19200 (Bit Rate) * VID: 1155 \| PID: 22336 |
| **CCID Emulator** | ____ ____ ___ ____  _____                 _       _ / ___/ ___\|_ _\|  _ \\| ____\|_ __ ___  _   _\| \| __ _\| \|_ ___  _ __ |
| **Chameleon Ultra** | Complete Flipper Zero application (.fap) to control and manage your Chameleon Ultra device via USB or Bluetooth. **🎬 Features fun bar animation when devices connect!** Watch Chameleon and Dolphin meet at a bar! 🍺 |
| **Click Recorder** | Turn your Flipper Zero into a powerful mouse automation tool. Record, replay, randomize, or spam mouse clicks over USB or Bluetooth. No software needed on the target computer just connect your flipper zero. Capture the exact timing of your mouse c... |
| **Clippy** | Has ever happened to you when you are setting up your new smartphone and you are asked for the wifi password to feel the extreme urge to start cursing every known divinity? Clippy solves this problem by turning your Flipper Zero into a cross-devic... |
| **FlipDeck** | **Turn your Flipper Zero into a USB macro pad.** FlipDeck transforms your Flipper Zero into a programmable USB macro pad with media controls, custom shortcuts, and an extensible plugin system. Media keys work **instantly** on any OS — no drivers, ... |
| **FlipPass** | FlipPass lets Flipper Zero open a KeePass vault and send credentials to another device through USB HID or Bluetooth HID. - Browse KeePass KDBX 4 vaults stored on the SD card. |
| **FlipperTrack** | USB HID activity simulator for Flipper Zero. Generates periodic mouse movement, keystroke bursts, and window-cycling inputs over USB HID — useful for keeping systems active during presentations, testing HID automation pipelines, or verifying endpo... |
| **Gatekeeper** | **BadUSB Integration:** Injects passwords instantly as a keyboard device. **Master Combo Security:** Protects your data with a mandatory 4-click directional sequence (D-pad) required upon every launch. |
| **HID Exfil** | _   _ ___ ____  _____       __ _ _ \| \| \| \|_ _\|  _ \\| ____\|_  __/ _(_) \| |
| **HID File Transfer** | This is an application for the Flipper Zero that allows to exchange files using the HID protocol with around 260kB/s. This approach is especially useful when access to mass storage devices is blocked on a PC. |
| **LEGO Dimensions Toy Pad** | Use your Flipper Zero as a Lego Dimensions ToyPad This project emulates the behavior of the original USB ToyPad, allowing you to interact with Lego Dimensions using your Flipper Zero. this app can place characters, vehicles directly from your Flip... |
| **Mass Storage** | *No description available* |
| **Mouse Jiggler** | *No description available* |
| **MTP** | See this in action on YouTube! - **Latest Release**: Download |
| **PC Monitor USB** | Flipper Application for monitoring PC resources via USB (Virtual COM Port). **A backend running on your PC is required.** |
| **Portal Of Flipper** | USB Emulator Original App by bettse |
| **SD Card Mounter** | This is a small Flipper Zero app that lets you connect the internal micro SD card reader directly to your computer. Since the raw contents of the card are passed directly to the computer, it means that filesystems not normally supported by the Fli... |
| **Switch Controller** | A Flipper Zero application that emulates a Nintendo Switch Pro Controller over USB with macro recording and playback capabilities. **✅ Compatible with Official Firmware and Momentum Firmware** |
| **USB Consumer Control** | This is a Flipper Zero application for sending Consumer Control Button (CCB) presses as a USB HID device. This is useful for researching how various devices handle USB CCBs, for example in the context of kiosk breakouts. A more detailed writeup of... |
| **USB Game Controller** | This is an app for the Flipper Zero that emulates a game controller (specifically the Xbox 360 controller) and lets the user interact with the emulated joystick, A and B buttons. It also contains a full implementation of XInput for the Flipper Zer... |
| **USB HID Autofire** | Latest release \| Changelog \| |
| **USB Midi Capturing** | Towards a capturing MIDI signals via USB using Flipper Zero. The goal is to allow real-time monitoring of MIDI events including Note On/Off, Control Changes, Program Changes, Pitch Bend, and more. In order to get it all working, we would need to a... |
| **USB Remote** | *No description available* |

### Misc (2 apps)

| App | Description |
|-----|-------------|
| **hc11_modem** | * Installation Flipper Zero app send data to / from a HC-11 wireless RF UART communication module. |
| **Prince Of Arabia** | Inspired by a timeless classic, Press-Play-On-Tape presents 'Prince of Arabia'! Escape the dungeons and free the princess! |

### Gpio/Sensors (22 apps)

| App | Description |
|-----|-------------|
| **[AS7331] UV Meter** | A Flipper Zero application designed to measure ultraviolet (UV) radiation levels using the AS7331 sensor. It supports individual measurements for UV-A, UV-B, and UV-C wavelengths. The easiest way to hook everything up is to use a breakout board, s... |
| **[BH1750] Lightmeter** | An application that suggests settings for your manual camera based on the reading of the ambient light sensor. Can also be used in a pure lux meter mode. \| Sensor \| Flipper Zero \| |
| **[GPIO] Unitemp** | Flipper Zero application for reading temperature, humidity, CO2 and pressure sensors like a DHT11/22, DS18B20, BMP280, HTU21, and more. \| Model            \| Tested on \| Interface        \| Temp Range  \| Temp Acc \| Temp Res \| Hum Range  \| Hu... |
| **[HC-SR] Dist. Sensor** | *No description available* |
| **[J305] Atomic Dice Roller** | 🎲☢ An atomic dice roller for the Flipper Zero ☢🎲 **Geiger Counter** application must work on your **Flipper Zero** to be able to use this application. You need the same **geiger board** and this board must be connected in the same way as described... |
| **[J305] Geiger Counter** | A geiger counter application for the Flipper Zero You need a **geiger counter** board to run this application. This board can be used : https://aliexpress.com/item/1005004074447209.html |
| **[MH-Z19] CO2 Logger** | App for reading CO2 ppm from MH-Z19 sensor and saving to a local csv file. **CO2 Logger Viewer** — Plug in your Flipper, click connect, and instantly see your CO2 data as an interactive chart. No drivers, no apps, just your browser. |
| **[MH-Z19] CO2 Sensor** | Application for measuring carbon dioxide (CO2) with mh-z19 sensor and Flipper zero - Clone application source code |
| **[Mx2125] Accelerometer** | This is a demostration of using GPIO interrupts to interpret data from the Memsic 2125 (Mx2125) Dual-Axis Accelerometer. This project is intended to be overlayed on top of an existing firmware repo. |
| **[RCWL-516] Motion Sensor** | This fap plugin (app) for RCWL-0516 radar. RCWL-0516 is a doppler radar microwave motion sensor module |
| **[RCWL0516] Radar Scan** | The Flipper Zero Radar Scanner is an application designed for the Flipper Zeros GPIO capabilities, utilizing the RCWL-0516 Microwave Radar module to detect the presence and movement of living beings or people. This powerful radar module is capable... |
| **[RCWL0516] Sleep Counter** | This is a flipper app that counts how much you move in your sleep. Shoutout to MatthewKuKanich https://github.com/MatthewKuKanich/flipper-radar he is the one who made the original app for this Radar module. |
| **[USPING] Dist. Sensor** | *No description available* |
| **Gas Sensor** | Simple application that reads BME680 and SGP30 data. Note that the BEM680 needs to have SD0 and CS connected to the "high" line. |
| **MAX31855** | *No description available* |
| **MH-Z19 UART** | *No description available* |
| **Plantower PMSx003 sensor reader** | A simple Flipper Zero air quality monitoring application. It displays measured PM1.0/PM2.5/PM10 [Particulate Matter] concentrations |
| **Radiation Sensor** | RadSens Application is not official application allows you to get level of current radiation measured by RadSens dosimeter module based on a Geiger tube. The Application stores amount of particles between launches. RadSens counts particles when po... |
| **UV Meter [AS7331]** | A Flipper Zero application designed to measure ultraviolet (UV) radiation levels using the AS7331 sensor. It supports individual measurements for UV-A, UV-B, and UV-C wavelengths. The easiest way to hook everything up is to use a breakout board, s... |
| **VEML7700 Lux Meter** | This is an application for the **Flipper Zero** device that measures ambient light intensity using a **VEML7700 sensor** and displays the result in lux ($\text{lx}$). \| Main Screen \| Gain Settings \| I2C Address Settings \| |
| **VL6180X Distance Sensor [I2C]** | This is a simple app the makes uses of the Adafruit VL6180X distance sensor. The code for configuring and reading from the VL6180X is a port of the Circuit Python code provided by Adafruit. There is also a C++ library for use with an Arduino. |
| **Water Sensor Reader** | **Water Sensor Reader** is a simple Flipper Zero application that reads an analog water sensor connected to a GPIO pin and displays its values in real-time. The app shows both the raw ADC value and the corresponding voltage in millivolts, along wi... |

### Settings (7 apps)

| App | Description |
|-----|-------------|
| **Animation Switcher** | A Flipper Zero application for creating, managing, and switching **animation playlists** - template manifest.txt files that control which background animations play on your Flipper. Supports up to 128 animations per playlist. - **Create Playlist**... |
| **Antidepressants** | Flipper application that allows changing of the background XP and Butthurt values Modified By Lambda to also be a level changer. Renamed to Dolphin Trainer. |
| **Extend Range** | Extend SubGHz Range Instead of manually changing the configuration of the subghz module (to enable the full potential), this application does it for you. |
| **Gauge Tool** | *No description available* |
| **Name Changer** | *No description available* |
| **Smack My Dolphin Up!** | > [!IMPORTANT] > This repository is archived and no longer is going to be updated. |
| **Theme Manager** | Manage dolphin animation themes directly from your Flipper Zero — no PC required. - **Flipper Apps Catalog** |

### Tools/Educational (7 apps)

| App | Description |
|-----|-------------|
| **ASCII Table** | A pocket ASCII table for your Flipper Zero. You never know when you might need one ;) - **UP / DOWN** - Pretty self-explanatory |
| **Crypto Dictionary** | Cryptography Dictionary is a comprehensive reference tool that provides detailed info on various algorithms. |
| **Extra Demo** | A pack of extra commands for the Flipper Zero (a C library). Check out the demo here! In your app directory run: |
| **FAP Boilerplate** | Use this code as a foundation for developing Flipper Zero Applications, change the contents of this file when developing a new project - Start Screen |
| **Flipper Doctor** | This Flipper application lets you view all information's regarding your flippers experience and butthurt. * General XP and Butthurt View |
| **Plugin Demo** | A simple plugin for the FlipperZero written as a tutorial example [ie. excessive documentation &amp; error handling] Since the move to FAP, your "plugins" are now known as "applications" |
| **The C Prog. Language** | The authors present the complete guide to ANSI standard C language programming. by Brian W. Kernighan, Dennis M. Ritchie. |

### Gpio/Debug (10 apps)

| App | Description |
|-----|-------------|
| **[AVR] AVR Flasher** | *No description available* |
| **[FTDI232H] FlipTDI** | *No description available* |
| **[I2C] i2c Tools** | Set of i2c tools for Flipper Zero >/!\ Target must use 3v3 logic levels. If you not sure use an i2c isolator like ISO1541 |
| **[INA2xx] INA Meter** | INA Meter is an application for Flipper Zero that allows you to read I2C-connected current/power monitors from Texas Instruments. - **INA219** – 0–26V, 16-bit resolution (±0.5% accuracy) |
| **[SPI] SPI Mem Manager** | *No description available* |
| **[SWD-JTAG] DAP Link** | Flipper Zero as a Free-DAP based SWD\JTAG debugger. Free-DAP is a free and open source firmware implementation of the CMSIS-DAP debugger. SWD, JTAG , CMSIS-DAP v1 (18 KiB/s), CMSIS-DAP v2 (46 KiB/s), VCP (USB-UART). |
| **[SWD] SWD Probe** | Modern microcontrollers have support for the two wire debug interface SWD, which makes wiring a lot simpler. When reverse engineering, finding these two pins is a lot easier than with JTAG, where you had to wire up twice or more pins. However, fin... |
| **[W5500] Ethernet** | This is a small helper that lets you connect your Flipper via RJ45 to your network You can then set the interface-mac and test if you can get an IP via DHCP and the ping addresses. |
| **AVR Flasher** | *No description available* |
| **Ethernet Troubleshooter W5500** | This is a small helper that lets you connect your Flipper via RJ45 to your network You can then set the interface-mac and test if you can get an IP via DHCP and the ping addresses. |

### Gpio/Esp32 (22 apps)

| App | Description |
|-----|-------------|
| **[CAM] Morse Flash v0.1** | Flipper Zero app to control various devices via UART interface. - Read log and command output by uart |
| **[ESP32] Camera** | Requires: Esp32-Cam module. Download and install instructions: Latest release. |
| **BLE Killer** | The Bluetooth expansion board (ble ext) based on flipper zero realizes the scanning of surrounding low-power Bluetooth devices and the integrated Bluetooth device controller (currently comes with the control method of oklok ble Bluetooth padlock).... |
| **BLE Scanner** | *No description available* |
| **BT Audio** | A Flipper Zero application that plays MP3 audio (Bluetooth A2DP audio streaming) by controlling ESP32-based boards over UART. The ESP32 acts as an **A2DP source** by default, streaming audio TO Bluetooth headphones and speakers. This project is li... |
| **Evil BLE** | *No description available* |
| **FlipDownloader** | Download apps and assets directly to your Flipper Zero using WiFi. This app is not affiliated with Flipper Devices. - App Catalog |
| **FlipLibrary** | The **FlipLibrary** app for Flipper Zero is a versatile and user-friendly application that offers a wide range of features to enhance your Flipper Zero experience. The app includes a **dictionary**, **random facts**, and numerous additional functi... |
| **FlipRPI** | Use your Flipper Zero to control your Raspberry Pi. - Raspberry Pi |
| **FlipSocial** | The first social media app for Flipper Zero. Connect with other users directly on your device through WiFi. The highlight of this app is customizable pre-saves, which, as explained below, aim to address the challenges of typing with the directiona... |
| **FlipTrader** | FlipTrader is an app for the Flipper Zero that uses WiFi to fetch the prices of stocks and currency pairs directly on your device. It utilizes the FlipperHTTP flash for the WiFi Devboard, originally introduced in the WebCrawler app: https://github... |
| **FlipWeather** | FlipWeather is an innovative app for the Flipper Zero that uses WiFi to fetch GPS and weather information, making your Flipper Zero even more versatile. It leverages the FlipperHTTP flash for the WiFi Devboard, originally introduced in the WebCraw... |
| **FlipWiFi** | FlipWiFi is the companion app for the popular FlipperHTTP firmware. It allows you to scan and save Wi-Fi networks for use across all FlipperHTTP apps, set up captive portals, and send deauthentication attacks. - Wi-Fi Developer Board, BW16, Raspbe... |
| **FlipWorld** | The first open-world multiplayer game for the Flipper Zero, best played with the VGM. Here's a video tutorial: https://www.youtube.com/watch?v=Qp7qmYMfdUA - Discord: https://discord.gg/5aN9qwkEc6 |
| **Gemini IA** | Currently in development (Beta phase), anyone who wants to join and help improve it can make their PR and also C developers who want to help improve the Flipper native app are welcome. You can find the first version in the releases section or at h... |
| **Gravity** | This Flipper Zero application (FAP) is a companion application to ESP32-Gravity. ESP32-Gravity is a wireless exploration framework written for the ESP32. It |
| **Postman** | Flipper Postman is a companion app for Flipper Postman Board Software for ESP32S2 (Flipper Dev Board), it provides convenient Ui for all of the custom board actions. - List all available networks |
| **Ray Hunter Client** | *No description available* |
| **Rogue AP Detector** | *No description available* |
| **Web Crawler** | Browse the web, fetch API data, and more on your Flipper Zero. - WiFi Developer Board, Raspberry Pi, or ESP32 device flashed with FlipperHTTP version 1.6 or higher: https://github.com/jblanked/FlipperHTTP |
| **Wendigo BT+BLE+WiFi Monitor** | Wendigo is a creature from North American mythology that stalks and overpowers its victims. If The Wendigo has you in its sights there's no escaping. You won't hear it coming and what happens next is up to The Wendigo. Table of Contents |
| **WiFi Mapping** | Check the ESP32 side at: esp32-wifi-map And do not forget to have both sides on the last version. |

### Gpio/Esp (10 apps)

| App | Description |
|-----|-------------|
| **[ESP32] Evil Portal** | An evil captive portal Wi-Fi access point using the Flipper Zero and Wi-Fi dev board **This project is a work in progress.** |
| **[ESP32] Ghost ESP** | A Flipper Zero application for interfacing with the GhostESP: Revival firmware. - **Scanning & Probing** |
| **[ESP32] WiFi Marauder** | *No description available* |
| **[ESP32CAM] Camera Suite** | *No description available* |
| **[ESP32GPS] Wardriver** | *No description available* |
| **[ESP8266] Deauther** | *No description available* |
| **[ESP8266] Deauther v2** | Flipper Zero esp8266 deauther app. Based off the WiFi Marauder App from 0xchocolate. |
| **[ESP8266] IFTTT Btn** | *No description available* |
| **[ESP] ESP Flasher** | *No description available* |
| **[ESP] WiFi Scanner** | *No description available* |

### Main (1 apps)

| App | Description |
|-----|-------------|
| **Clock** | - We were the first clock created for Flipper Zero - Has 10 different clock faces |

### Rfid (8 apps)

| App | Description |
|-----|-------------|
| **DCF77 Clock Sync** | Emulates the DCF77 time signal on the RFID antenna and on GPIO A4 pin. Uses PWM with frequency of 77.5 kHz on the GPIO pin to simulate the signal. |
| **DCF77 Transmitter** | Sends the DCF77 time signal (badly) on the 125khz LFRFID antenna. This should allow you to update radio clocks. You can send it at the original 77.5kHz, but I've had better range using 155kHz. The baseband signal is output on C3. Building a portab... |
| **EM4100 Key Generator** | A program that generates universal keys from a EM4100 key Click the FAP badge, connect your flipper and click "Install". |
| **FDX-B Maker** | Construct fully ISO-compliant FDX-B animal microchip data right on your Flipper Zero! The data gets stored as plain RFID files on the SD card, which you can then write to T5577 or EM4305 cards/chips as normal. 0 to 999. A 3-digit number correspond... |
| **Simultaneous UHF RFID Reader** | *No description available* |
| **T5577 Multiwriter** | PoC of https://forum.dangerousthings.com/t/hack-store-2-to-3-different-em41xx-on-a-single-t5577/12116 |
| **T5577 Raw Writer** | An easy to use T5577 raw writer app for Flipper Zero. Discord project page. Configure the modulation, RF Clock, number of blocks, and block data in the 'Config' menu. |
| **YRM100 UHF RFID** | display: grid; grid-template-columns: repeat(2, 1fr); |

### Gpio/Flipperhttp (11 apps)

| App | Description |
|-----|-------------|
| **FlipDownloader** | Download apps and assets directly to your Flipper Zero using WiFi. This app is not affiliated with Flipper Devices. - App Catalog |
| **FlipLibrary** | The **FlipLibrary** app for Flipper Zero is a versatile and user-friendly application that offers a wide range of features to enhance your Flipper Zero experience. The app includes a **dictionary**, **random facts**, and numerous additional functi... |
| **FlipMap** | A Flipper Zero app to find Flipper Zero Users. - Wi-Fi Developer Board, BW16, Raspberry Pi, or ESP32 device flashed with FlipperHTTP v2.1 or higher: https://github.com/jblanked/FlipperHTTP |
| **FlipSocial** | The first social media app for Flipper Zero. Connect with other users directly on your device through WiFi. The highlight of this app is customizable pre-saves, which, as explained below, aim to address the challenges of typing with the directiona... |
| **FlipTelegram** | Flipper Zero Telegram Client - WiFi Developer Board, Raspberry Pi, BW16, or ESP32 device flashed with FlipperHTTP v2.1 or higher: https://github.com/jblanked/FlipperHTTP |
| **FlipTrader** | FlipTrader is an app for the Flipper Zero that uses WiFi to fetch the prices of stocks and currency pairs directly on your device. It utilizes the FlipperHTTP flash for the WiFi Devboard, originally introduced in the WebCrawler app: https://github... |
| **FlipWeather** | FlipWeather is an innovative app for the Flipper Zero that uses WiFi to fetch GPS and weather information, making your Flipper Zero even more versatile. It leverages the FlipperHTTP flash for the WiFi Devboard, originally introduced in the WebCraw... |
| **FlipWiFi** | FlipWiFi is the companion app for the popular FlipperHTTP firmware. It allows you to scan and save Wi-Fi networks for use across all FlipperHTTP apps, set up captive portals, and send deauthentication attacks. - Wi-Fi Developer Board, BW16, Raspbe... |
| **FlipWorld** | *No description available* |
| **Free Roam** | 3D Open World Multiplayer Game for the Flipper Zero - WiFi Developer Board, BW16, Raspberry Pi, or ESP32 device flashed with FlipperHTTP v2.0 or higher: https://github.com/jblanked/FlipperHTTP |
| **Web Crawler** | Browse the web, fetch API data, and more on your Flipper Zero. - WiFi Developer Board, Raspberry Pi, or ESP32 device flashed with FlipperHTTP version 1.6 or higher: https://github.com/jblanked/FlipperHTTP |

### Gpio/Flipboard (8 apps)

| App | Description |
|-----|-------------|
| **FlipBoard Blinky** | The FlipBlinky application turns your Flipper Zero + FlipBoard into a blinky badge for wearing at conferences.  The application is written in C and uses the FlipBoard common code for the majority of the functionality. You can order your FlipBoard ... |
| **FlipBoard Blinky** | The FlipBlinky application turns your Flipper Zero + FlipBoard into a blinky badge for wearing at conferences.  The application is written in C and uses the FlipBoard common code for the majority of the functionality. You can order your FlipBoard ... |
| **FlipBoard Keyboard** | The FlipKeyboard application turns your Flipper Zero and FlipBoard into a MacroPad Keyboard.  It also becomes a musical light-up keyboard that you can use to play songs. It supports both USB *and* BLE connections. Automate repetitive typing tasks ... |
| **FlipBoard Keyboard** | The FlipKeyboard application turns your Flipper Zero and FlipBoard into a MacroPad Keyboard.  It also becomes a musical light-up keyboard that you can use to play songs. It supports both USB *and* BLE connections. Automate repetitive typing tasks ... |
| **FlipBoard Signal** | The FlipSignal application turns your Flipper Zero and FlipBoard into an Infrared and Sub-GHz signal sender.  Turn on/off devices using the buttons on your FlipBoard, or send a sequence of IR signals. The application is written in C and uses the F... |
| **FlipBoard Signal** | The FlipSignal application turns your Flipper Zero and FlipBoard into an Infrared and Sub-GHz signal sender.  Turn on/off devices using the buttons on your FlipBoard, or send a sequence of IR signals. The application is written in C and uses the F... |
| **FlipBoard Simon** | The FlipSimon application turns your Flipper Zero and FlipBoard into a memory game.  FlipSimon was originally written as a step-by-step tutorial to teach C, Flipper Zero Code, and FlipBoard common code - so you can create your own applications.  B... |
| **FlipBoard Simon** | The FlipSimon application turns your Flipper Zero and FlipBoard into a memory game.  FlipSimon was originally written as a step-by-step tutorial to teach C, Flipper Zero Code, and FlipBoard common code - so you can create your own applications.  B... |

### Gpio/Nrf24 (11 apps)

| App | Description |
|-----|-------------|
| **[NRF24] Batch** | Flipper Zero application for nRF24L01 external board. Sends batch commands. Приложение для Flipper Zero, предназначено для отправки пакетных команд на удаленные устройства, использующие радио nRF24L01. |
| **[NRF24] Channel Scan** | *No description available* |
| **[NRF24] Mouse Jacker** | *No description available* |
| **[NRF24] Scanner** | An NRF24 driver for the Flipper Zero device. The NRF24 is a popular line of 2.4GHz radio transceivers from Nordic Semiconductors. NRF24L01+ Enhanced ShockBurst packet decoder example using Python: nrf24_packet_decoder.py |
| **[NRF24] Sniffer** | *No description available* |
| **AZERTY Mouse Jacker** | An NRF24 driver for the Flipper Zero device. The NRF24 is a popular line of 2.4GHz radio transceivers from Nordic Semiconductors. This library is not currently complete, but functional. This repo contains two Flipper Zero apps that utilize the NRF... |
| **FZ nRF24 Jammer** | Welcome to the **FZ nRF24 Jammer** repository! 🎉 Dive into the world of RF interference with this project based on the Flipper Zero and NRF24. - 🚀 What Can You Do with This? |
| **Mouse Jacker MS** | Various tools for Flipper Zero See README inside directory. |
| **NRF24 Jammer** | Simple Flipper Zero nRF24 jammer for the 2.4Ghz spectrum I use to study wireless protocol hardening. Turn your Flipper Zero into a bluetooth or wifi jammer. Works with the latest Unleashed firmware. See the release on how to install the external a... |
| **NRF24 Monitor** | A real-time SPI communication tester for NRF24L01+ modules on Flipper Zero, featuring hardware status monitoring and register reading capabilities. - **Real-time monitoring** of NRF24L01+ connection status |
| **Sniffer MS** | Various tools for Flipper Zero See README inside directory. |

### Gpio/Malveke (8 apps)

| App | Description |
|-----|-------------|
| **[GB/GBC] Cartridge** | MALVEKE Prototype V2.3 **Official** \| **Unleashed** \| **RogueMaster** \| **Xtreme** |
| **[GB] Emulator** | From Talcahuano 🇨🇱 with ❤ |
| **[GB] Link-Camera** | **Official** \| **Unleashed** \| **RogueMaster** \| **Xtreme** :- \| :- \| :- \| :- |
| **[GB] Live Camera** | MALVEKE Prototype V2.3 https://github.com/EstebanFuentealba/MALVEKE-Flipper-Zero/assets/442927/8c11cb8a-2a05-47e2-a0e0-1d3a1fe386ea |
| **[GB] Photo** | **Official** \| **Unleashed** \| **RogueMaster** \| **Xtreme** :- \| :- \| :- \| :- |
| **[GB] PIN Test** | *No description available* |
| **[GBA] Cartridge** | MALVEKE Prototype V2.3 **Official** \| **Unleashed** \| **RogueMaster** \| **Xtreme** |
| **GB Printer** | A Game Boy Printer Emulator for Flipper Zero that saves images as PNG on the microSD card. The Game Boy is connected to the Flipper Zero's GPIO pins via a GBC style Game Link Cable. The Flipper GB Link module is an easy way to connect a Game Boy v... |

### Gpio/Gps (2 apps)

| App | Description |
|-----|-------------|
| **[NMEA] GPS** | A simple Flipper Zero application for NMEA 0183 serial GPS modules. Heavy lifting (NMEA parsing) provided by [minmea]. |
| **[NMEA] Nearby Files** | A GPS-enabled file browser for Flipper Zero that displays SubGHz, NFC, and RFID files sorted by distance from your current location. Show Preview |

### Ibutton (2 apps)

| App | Description |
|-----|-------------|
| **iButton Converter** | **iButton Converter** is a Flipper Zero application designed for converting iButton key dumps from *Metakom* or *Cyfral* formats into the *Dallas* format. The application supports multiple conversion modes for each format, including well-known and... |
| **iButton Fuzzer** | **Not meant for any illegal activity/purposes.** You can check your readers for vulnerabilities using the default UIDs and frequently used UIDs. |

### Gpio/Mayhem (6 apps)

| App | Description |
|-----|-------------|
| **[MAYHEM] Camera** | *No description available* |
| **[MAYHEM] Marauder** | *No description available* |
| **[MAYHEM] Morse Flash** | Flipper Zero app to control various devices via UART interface. - Read log and command output by uart |
| **[MAYHEM] Motion Detection** | *No description available* |
| **[MAYHEM] Nanny Cam** | *No description available* |
| **[MAYHEM] QR Code** | *No description available* |

### Gpio/Games (2 apps)

| App | Description |
|-----|-------------|
| **UART Pong** | *No description available* |
| **VL6180X Pong** | This is a copy of the Flipper Zero Pong game, but uses a VL6180x module to control the paddle instead of the buttons. See the VL6180X README for more details on setting the Flipper up with the VL6180X. |

### Gpio/Vgm (4 apps)

| App | Description |
|-----|-------------|
| **[VGM] Air Mouse** | *No description available* |
| **[VGM] Video Game Module Tool** | *No description available* |
| **VGM Game Remote** | Collection of files for the Video Game Module. The VGM Engine is not affiliated with Flipper Devices and utilizes the PicoDVI and pico-game-engine libraries. Developers can create their own games or recreate their Flipper games with the advantages... |
| **Video Game Module Console** | Turn your Flipper Zero into a console for the Video Game Module — handles display output and button input so developers can focus on their hardware. 1. Connects to the Video Game Module via UART (FlipperHTTP). |

<h2 align="center">Install</h2>

There are 4 methods to install Momentum, we recommend you use the **Web Updater**, but choose whichever one you prefer:

> <details><summary><code>Web Updater (Chrome)</code></summary><ul>
>   <li>Make sure qFlipper is closed</li>
>   <li>Open the <a href="https://momentum-fw.dev/update">Web Updater</a></li>
>   <li>Click <code>Connect</code> and select your Flipper from the list</li>
>   <li>Select which update <code>Channel</code> you prefer from the dropdown</li>
>   <li>Click <code>Install</code> and wait for the update to complete</li>
> </ul></details>

> <details><summary><code>Flipper Lab/App (chrome/mobile)</code></summary><ul>
>   <li>(Desktop) Make sure qFlipper is closed</li>
>   <li>(Mobile) Make sure you have the <a href="https://docs.flipper.net/mobile-app">Flipper Mobile App</a> installed and paired</li>
>   <li>Open the <a href="https://github.com/Next-Flip/Momentum-Firmware/releases/latest">latest release page</a></li>
>   <li>Click the <code>☁️ Flipper Lab/App (chrome/mobile)</code> link</li>
>   <li>(Desktop) Click <code>Connect</code> and select your Flipper from the list</li>
>   <li>(Desktop) Click <code>Install</code> and wait for the update to complete</li>
>   <li>(Mobile) Accept the prompt to open the link in the Flipper Mobile App</li>
>   <li>(Mobile) Confirm to proceed with the install and wait for the update to complete</li>
> </ul></details>

> <details><summary><code>qFlipper Package (.tgz)</code></summary><ul>
>   <li>Download the qFlipper package (.tgz) from the <a href="https://github.com/Next-Flip/Momentum-Firmware/releases/latest">latest release page</a></li>
>   <li>Make sure the <code>WebUpdater</code> and <code>lab.flipper.net</code> are closed</li>
>   <li>Open <a href="https://flipperzero.one/update">qFlipper</a> and connect your Flipper</li>
>   <li>Click <code>Install from file</code></li>
>   <li>Select the .tgz you downloaded and wait for the update to complete</li>
> </ul></details>

> <details><summary><code>Zipped Archive (.zip)</code></summary><ul>
>   <li>Download the zipped archive (.zip) from the <a href="https://github.com/Next-Flip/Momentum-Firmware/releases/latest">latest release page</a></li>
>   <li>Extract the archive. This is now your new Firmware folder</li>
>   <li>Open <a href="https://flipperzero.one/update">qFlipper</a>, head to <code>SD/update</code> and simply move the firmware folder there</li>
>   <li>On the Flipper, hit the <code>Arrow Down</code> button, this will get you to the file menu. In there simply search for your updates folder</li>
>   <li>Inside that folder, select the Firmware you just moved onto it, and run the file thats simply called <code>Update</code></li>
> </ul></details>

<br>

<h2 align="center">Build it yourself</h2>

```bash
To download the repository:
$ git clone --recursive --jobs 8 https://github.com/Next-Flip/Momentum-Firmware.git
$ cd Momentum-Firmware/

To flash directly to the Flipper (Needs to be connected via USB, qFlipper closed)
$ ./fbt flash_usb_full

To compile a TGZ package
$ ./fbt updater_package

To build and launch a single app:
$ ./fbt launch APPSRC=your_appid
```

<h2 align="center">Stargazers over time</h2>

[![Stargazers over time](https://starchart.cc/Next-Flip/Momentum-Firmware.svg?variant=adaptive)](https://starchart.cc/Next-Flip/Momentum-Firmware)

<h2 align="center">❤️ Support</h2>

If you enjoy the firmware please __**spread the word!**__ And if you really love it, maybe consider donating to the team? :D

> **[Ko-fi](https://ko-fi.com/willyjl)**: One-off or Recurring, No signup required

> **[PayPal](https://paypal.me/willyjl1)**: One-off, Signup required

> **BTC**: `1EnCi1HF8Jw6m2dWSUwHLbCRbVBCQSyDKm`

**Thank you <3**
