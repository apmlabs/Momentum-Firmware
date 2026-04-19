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

<h2 align="center">📱 External Applications Catalog</h2>

This firmware includes **667** external applications across **27** categories. Each app is ready to use — just launch it from your Flipper Zero's menu.

<details><summary><h3>🎮 Games — 145 apps</h3></summary>

#### 1D Pacman *(by easiwork)*
> Implementation of 1D Pacman using the Flipper Zero Game Engine as boilerplate. Flipper Zero 1D Pacman an implementation of 1D Pacman using the Flipper Zero Game Engine as boilerplate..

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### 2048 *(by @eugene-kirzhanov)*
> Play the port of the 2048 game on Flipper Zero. "2048" game for Flipper Zero - play up to 65K - progress is saved on exit

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### 2048 (Original) *(by OlegSchwann)*
> 2048 (Original) game for Flipper Zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### 4 in a Row *(by leo-need-more-coffee)*
> 4 in a Row game for Flipper Zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### 99 Bottles *(by bolknote)*
> 99 bottles lyrics guide for Flipper Zero

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Air Arkanoid
> Arkanoid game that supports the Video Game Module motion sensor

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Air Labyrinth *(by @CodeAllNight (MrDerekJamison))*
> Labyrinth game v0.1 that supports the Video Game Module motion sensor. Written by @CodeAllNight (https://youtube.com/MrDerekJamison/about) This is a simple game where you have to navigate a ball through a maze. The game is controlled by tilting the Flipper Zero with a Video Game Module attached.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Apple Grabber *(by d7d8 / Julio Rodriguez)*
> Apple grabber game, default mode is vertical Apple Grabber Game for Flipper Zero Flip the your device on vertical mode and use LEFT/RIGHT to move, the objective is to grab all the apples. The speed increases each turn.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Ardudrivin *(by apfxtech)*
> Race game ported to Flipper Zero. 'Ard Drivin' is an Arduboy racing game Technical info so far: - Game runs at a constant rate of 67 frames per second

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### ArduGolf *(by apfxtech)*
> Play the port of Wolfenduino game on your Flipper Zero. ArduGolf is a 3D minigolf game with solid ball physics, ported to the Flipper Zero handheld device. Play through 18 holes, plan your shots, bounce off walls, and use slopes to finish each course in as few strokes as possible. Good ball physics

**Usage:** Launch from the Games menu. Use d-pad to aim and shoot. Navigate through 18 holes of minigolf.

#### Arduventure *(by apfxtech)*
> Play the port of Arduventure game on your Flipper Zero. A lightweight retro RPG/adventure for Flipper Zero, based on the classic Arduventure for Arduboy. Explore a pixel world, fight enemies, and.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Arkanoid *(by @xMasterX & @gotnull)*
> Arkanoid game for Flipper Zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Asteroids *(by @antirez & @SimplyMinimal)*
> Asteroids game This is an implementation of the classic Asteroids game for the Flipper Zero. Inside you will find a simple 2D engine that can be reused to implement other games. Note: This one is SimplyMinimal's.

**Usage:** Auto rapid fire (less wear on the buttons this way too) Up button applies thrusters

#### Avocado Zero *(by Endika)*
> Avocado pit care sim: clean water, grow roots, avoid grime. A tiny care sim for Flipper Zero: you suspend an avocado pit over a glass of water (toothpicks and all) and try to keep it alive long enough to grow a full root system. Time passes, the water gets ...

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Banana *(by DrEverr)*
> Banana. Everyone loves bananas. Welcome to the Banana App! 🍌 This app is all about bananas. Everyone loves bananas and want to share that love with you. Whether you're looking to track your banana consumption, find new banana recipes, or learn fun facts abou...

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### BlackJack *(by @teeebor)*
> Blackjack Game !maintained - yes !contributions - welcome Blackjack for Flipper Zero

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Blackjack By RocketGod *(by RocketGod-git)*
> Blackjack! Flipper-Zero-Blackjack Blackjack! for your Flipper Zero - Full Blackjack gameplay:

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Bomber Fox *(by Electric Fox)*
> Two Player Bomber game over Sub-Ghz Radio BomberFox !In Development !Powered By Bomberman for the FlipperZero utilising the SubGHz radio for multiplayer functionality with other nearby flipper devices! For more information about the SubGHz protocol, please see protocol.md

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Bomberduck *(by @leo-need-more-coffee & @xMasterX)*
> Bomberduck(Bomberman) Game flipperzero-bomberduck Bomberman clone on flipper zero!

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### BzzBzz *(by Koray Er)*
> Haptic rhythm-matching game BzzBzz - Rhythm-Matching Game for Flipper Zero Haptic rhythm-matching game focusing on tactile feedback rather than visual cues. - The app generates a 5-step vibration sequence.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### CarJacker *(by RocketGod-git)*
> Car stealing app - The Pirates Plunder style. flipper-zero-carjacker Flipper Zero Carjacker App - Private Unleashed V2 Follow the white rabbit. He will lead you through the passage to the land of wiggly roosters.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Catacombs Of The Damned *(by apfxtech)*
> Play the port of the arduboy3d game on your Flipper Zero. Catacombs of the Damned! Catacombs of the Damned! is a first-person shooter / dungeon crawler for the Arduboy miniature game system.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Cells Lab *(by PilotOfAsuka)*
> Simple Cellular Automaton with Genetic Algorithm for Flipper Zero Simple Cellular Automaton with Genetic Algorithm for Flipper Zero Это приложение для Flipper Zero, представляющее собой простой клеточный автомат с базовым генетическим алгоритмом.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Checkers *(by @Claude + H4W9)*
> Checkers for Flipper A classic checkers (draughts) game for the Flipper Zero. This was inspired and based off of Flipper Chess by xtruan - Play against a friend or a CPU opponent

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Chess *(by Struan Clark (xtruan))*
> Chess for Flipper HOW ABOUT A NICE GAME OF CHESS? Chess game for Flipper Zero - Built against 1.1.2 Flipper Zero firmware release

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Chess Clock *(by ihatecsv)*
> A simple chess clock

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Cigarette *(by fuckmaz)*
> A well needed smoke break. Flipper Cigarette Finally a way to smoke a cigarette on the Flipper Zero Plug in your Flipper Zero and build the app from within the firmware base-directory:

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### City Bloxx *(by Milk-Cool)*
> City Bloxx for the Flipper Zero! City Bloxx for the Flipper Zero!

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Cognizant Flipper *(by Luke Gamertsfelder)*
> Random Word Generator Cognizant-Flipper This mysterious app reveals the hidden consciousness of your Flipper Zero. With each press of the OK button, the Flipper shares a word from its digital mind, slowly unveiling its thoughts and desires. !Screenshot-20240926-234530

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Color Guess *(by Leedave)*
> Color Guessing Game Flipper Zero Color Guessing Game As a web developer I enjoy guessing colours by HEX Code. This game is targeted at other Devs and graphic designers that also enjoy this.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Connect Wires *(by AlexTaran)*
> A puzzle game about rotating differently shaped wires to connect power source with consumers. "Connect Wires" game for Flipper Zero - famous puzzle concept - play on 3 different complexity levels

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### CountDown *(by Oscar Rodriguez)*
> Solve target number from 6 numbers like the CountDown TV Show Countdown — Numbers Game for Flipper Zero A game inspired by the "Countdown" numbers round. Select 6 numbers, choose a 3-digit target,

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Crossy Road *(by Mikael098)*
> Frogger clone WIP. Crossy Road de Mikael Fortier [Technologie Émergente]

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### DeadZone *(by retrooper)*
> Fight through tough challenges, dodge enemy fire, and avoid the falling obstacles to survive! DeadZone - Flipper Zero Video Game Fight through tough challenges, dodge enemy fire, and avoid the falling obstacles to survive!

**Usage:** Use the Left, Right, and Up arrow keys to move the player.\ Press OK to fire a bullet.\

#### Decision Maker *(by jacki)*
> Spin a roulette to pick a random option for you. Random Decision Maker A Flipper Zero app that lets you type your own choices and let a roulette wheel decide — powered by the device's hardware true-random-number generator. - Add up to 20 custom decisions (up to 20 characters each)

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Devillier's Platformer *(by adevil5)*
> This is a simple platformer game developed for the Flipper Zero. The game allows players to control a character as they navigate through a level filled with obstacles and platforms, aiming to reach the finish line.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Dice [RM] *(by Lambda)*
> Roll multi-sided dice for RolePlaying/D&D games. d2, d3, d4, d6, d8, d10, d12, d20, d100. Including SEX/WAR/8BALL/WEED/DRINK DICE. Dice (By Lambda) - Displays roll date/time on each roll Including d2, d3, d4, d6, d8, d10, d12, d20 & d100 (Works Great for Dungeons and Dragons)

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Digital Kaleidoscope *(by J. Randall jr3d.co.uk)*
> A Digital Kaleidoscope Visualiser Digital Kaleidoscope Digital Kaleidoscope is a simple, animated visualizer for Flipper Zero. It displays four different patterns that shift and change, turning your Flipper into a miniature kaleidoscope. - Four Animated Styles

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### DnD Dice *(by Ka3u6y6a)*
> Dice rolling, types: Coin, d4, d6, d8, d10, d12, d20, d100 Flipper Zero DnD Dice DnD Dice is a dice rolling application for your Flipper Zero. Dice types: Coin, d4, d6, d8, d10, d12, d20, d100

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### DOOM *(by @xMasterX & @Svarich & @hedger (original code by @p4nic4ttack))*
> Will it run Doom? Doom Flipper Zero edition Will it run Doom? As tradition goes, Doom is being ported to almost every possible embedded electronic device. Therefore I did an attempt to come up with something close to Doom and still compatible on the Flipper.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Drifter *(by Jed Lejosne)*
> A boat game Drifter is a boat game for the Flipper Zero

**Usage:** Use left and right keys to affect the drift. The longer the keypress, the bigger the impact. The score multiplier keeps increasing when no key is p... Press OK after a crash to start a new game, and back to exit.

#### DVD Bouncer
> Flipper-DVD-Bounce simple dvd-bounce application for flipper Y'know how dvd players got that thing that bounces around?

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Executor Keychain *(by Esteban Fuentealba)*
> Echo Keyller/Executor Keychain 🔫 🔊 Flipper Keyller

**Usage:** Flipper Keyller is an app for the Flipper Zero that emulates the iconic sounds of the classic 80s keychain: the Echo Keyller (also known as the Exe... - 🎵 8 retro laser-style sound effects

#### Fighter Jet *(by Erbonator3000)*
> Fighter jet simulator Flipper zero fighter jet Simple game about flying a fighter jet. Fly around, shoot the enemies, and dont loose your healt. This is just a demo, might make more finalized version later.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Five Nights at Flipper's *(by sillygir1)*
> Five Nights at Flipper's FNAF game for Flipper Zero. - up - door button

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Flappy Bird *(by @DroomOne, @xMasterX, @jaylikesbunda, @the1anonlypr3)*
> Flappy Bird Game

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Flight Assault *(by evillero)*
> Flight Assault is a dynamic action game where players control a spacecraft engaged in combat against enemy ships approaching from different directions.

**Usage:** 2. Use the control keys to maneuver the spacecraft. 3. Destroy as many enemy ships as possible to score points.

#### Flipper Hero *(by Mentoster)*
> Arrow Speed Game Welcome to Flipper Hero, an engaging typing practice game ported for the Flipper Zero device! Flipper Hero is inspired by the beloved minigame "Stratagem Hero, " found in the "Helldivers 2" universe.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Flippy Road *(by rkilpadi)*
> Flippy Road Game A simple arcade game for the Flipper Zero After plugging in your Flipper Zero, either The game will automatically be launched after compilation.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Fortune Cookie *(by evillero)*
> Get inspired with a random motivational quote or fortune each time you open the app. The Fortune Cookie app delivers a random motivational quote or fortune each time you open it, providing a small dose of inspiration to brighten your day. Simple app for Flipper Zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Furious Birds *(by Dmitry Ermashev)*
> Well-known birds and pigs game. The bird is really furious! Pigs are under attack! Next level has even more pigs to eliminate. And you have just 3 attempts! "Furious Birds" game for Flipper Zero Well-known birds and pigs game.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Game 15 *(by @x27)*
> Logic Game Game "15" for Flipper Zero Logic game Wikipedia

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Game of Life *(by @tgxn (original by @itsyourbedtime))*
> Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Geometry Dash
> Flipper Zero Geometry Flip Geometry Dash Demake with level system, basic physics engine, coyote time physics and basic UI. I am making real ship, ball and ufo physics!

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Geometry Dash *(by goosedev72-projects)*
> Geometry Dash Demake with level system, basic physics engine, coyote time physics and basic UI. Flipper Zero Geometry Flip Geometry Dash Demake with level system, basic physics engine, coyote time physics and basic UI. I am making real ship, ball and ufo physics!

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Groks Adventure *(by DigiMancer3D)*
> A Grok made micro-Adventure Grok's Adventure v3.3 Grok's Adventure v3.3 is the smallest infinite liminal space souls-like game for the Flipper Zero, built with AI code assistance from Grok 3 (xAI). Set in the eerie Backrooms, players control Grok ...

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Guess The Number *(by kWAYTV)*
> An enhanced number guessing game with progressive feedback, score tracking, and immersive multi-sensory experience. Guess The Number - Flipper Zero - Progressive Hint System - Distance-based feedback (very close, close, far) - Score Tracking - Personal best score persistence across games

**Usage:** - Intuitive Controls - Fine (+/-1) and coarse (+/-10) adjustment modes - Professional UI - Centered layout with visual emphasis and clear typography

#### Hangman *(by Evgeny Stepanischev, Eva Stepanischeva)*
> Hangman for Flipper Игра «Виселица» («Балда») Первая русскоязычная программа для Flipper Zero. Правила очень просты — «Флиппер» загадывает слово, вам надо его отгадать, выбирая буквы. Если буква встречается, она будет нарисована на той позиции или позициях,

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Hanoi Towers *(by AlexTaran)*
> In this classic puzzle game, your objective is to move all disks to another stick. You are allowed to move one disk at once. You cannot put a bigger disk on a smaller disk. "Hanoi Towers" game for Flipper Zero - famous puzzle concept - complexity level increases each time you solve the puzzle

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Heap Defence *(by @xMasterX (original implementation by @wquinoa & @Vedmein))*
> Heap Defence game from hackathon (aka Stack Attack)

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Hirn - Crack Color Codes *(by F Greil)*
> A deduction game to crack a 4-digit code A simple Flipper Zero combination cracking game. A one-player logic game where Flipper Zero creates a secret color code with 4 sybmols of 6 different colors. The user has to guess it in the fewest attempts. She/He receives clues: Black pegs for c...

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Hold 'em *(by code-phreak)*
> Single-player Texas Hold'em with smart bots Hold 'em for Flipper Zero Native single-player Texas Hold'em built specifically for Flipper Zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Hunter Killer *(by josephburnett)*
> A submarine warfare simulation game ported from Pico-8 to Flipper Zero using the Flipper Zero Game Engine. Hunter Killer - Flipper Zero Submarine Game A submarine warfare simulation game ported from Pico-8 to Flipper Zero using the Flipper Zero Game Engine.

**Usage:** - Realistic submarine physics - Navigate using heading and velocity controls - Procedural terrain generation - Diamond-square algorithm creates unique underwater landscapes

#### Impostor *(by Endika)*
> Undercover-style word game: impostors see a hint, everyone else shares one secret word. Flipper Zero external application (FAP). Install it on the microSD of your Flipper and run it from Apps → Games → Impostor Game. It is a device-only party helper: you do not need a phone or PC duri...

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Infinite Tic Tac Toe *(by Kyle Diller)*
> Tic Tac Toe but Infinite Infinite Tic-Tac-Toe This is an extension of the favorite childhood game of Tic-Tac-Toe but with pieces only lasting 3 turns. This allows for the game to go till there is a winner.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Insults *(by ERSUCC)*
> Insult generator

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Jetpack Joyride
> flipper-jetpack-game JETPACKS, ROCKETS, AND ADVENTURE AWAITS! Game Remake of Jetpack Joyride for Flipper Zero

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Jumping Pawns *(by Tyl3rA)*
> A simple board game where you race to move all of your pieces to the other side of the board. Jumping Pawns is a strategy game for the Flipper Zero. Play locally against a friend or challenge a minimax algorithm. From the main menu: - PvP – Local two-player mode.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### KC Line *(by Andrew Diamond)*
> One line dot munching game! A one line/row version of the game where a character eats little bits and gets chased around the screen by a ghost. It's pretty obvious where the inspiration for this game came from, but if you have questions, hit me up

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Laser Tag *(by @RocketGod-git & @jamisonderek)*
> Laser Tag game for Flipper Zero Flipper-Zero-Laser-Tag 🚀 Real World Team based Laser Tag game Use Flipper Zero as your laser blaster, RFID scan for power-ups, and automatic detection of add-on weapons to GPIO such as the Rabbit Labs Masta-Blasta for arena style play.

**Usage:** - Team Battles: Choose your team and face off in epic Red vs. Blue laser battles. - Real-Time Gameplay: Smooth and responsive laser firing and hit detection.

#### Laser Tag *(by RocketGod-git & jamisonderek)*
> Laser Tag game for Flipper Zero Flipper-Zero-Laser-Tag 🚀 Real World Team based Laser Tag game Use Flipper Zero as your laser blaster, RFID scan for power-ups, and automatic detection of add-on weapons to GPIO such as the Rabbit Labs Masta-Blasta for arena style play.

**Usage:** - Team Battles: Choose your team and face off in epic Red vs. Blue laser battles. - Real-Time Gameplay: Smooth and responsive laser firing and hit detection.

#### Lifecounter *(by Antsy)*
> Application to record life situation of a collectible card game A Flipper Zero life tracker for collectible card games such as Flesh and Blood, Magic the Gathering, Sorcery, Lorcana etc. Development setup - Buy a Flipper Zero device

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Lights Out Flipper Zero *(by DigiMancer3D)*
> Turn off the lights on the sith lor...*coughs*... Lights Out Flipper Zero (LOFZ) Lights Out Flipper Zero (LOFZ) is a Star Wars-inspired puzzle game for the Flipper Zero, based on the classic "Lights Out" game. As the last Jedi, you must toggle a 17-cell grid of lights to turn t...

**Usage:** - Star Wars Aesthetic: Features a scrolling intro crawl, a 16x16 Sith Flipper mascot (1bpp bitmap) that bounces or moves under stress ( 981), and t...

#### Magic 8-ball *(by Steven Quinn)*
> A simple Magic 8-ball A magic 8 ball game for the Flipper Zero. Press the center button to get a random answer to your question.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Mancala *(by F. Greil)*
> Old strategy board game with 6 pits and 1 store per player and 4 stones per pit. Left / Right-Button. Moves cursor between the 6 user pits OK picks up stones from selected pit and perform the distributions of the stones according to the rules Holding Back quits the App.

**Usage:** Visual cursor to select your moves Status messages for game events

#### Mandelbrot Set *(by Possibly-Matt)*
> The Mandelbrot set is the set of all so-called (complex) numbers that meet Mandelbrots simple arithmetic criterion.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Matagotchi *(by MrModd)*
> Tamagotchi like game Tamagotchi(R) like game for Flipper Zero

**Usage:** The pet grows from an egg to an adult and then dies of old age. The stages are !Egg -> !Baby -> !Child -> !Teen -> !Adult -> !Dead

#### Mind Probe *(by Miksang)*
> This is a simple version of the online game Akinator/20 Questions I first saw reddit user EscanorSensi asking if a 20 question style game could be implemented on the flipper and it got me thinking.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Mine Sweeper *(by Alexander Rodriguez)*
> Flipper Zero Minesweeper Implementation Minesweeper Implementation for the Flipper Zero. !Mine Sweeper Example Gameplay Gif

**Usage:** Added features unique to this implementation: -	Enable board verifier for board generation to ensure unambiguous boards!

#### Minesweeper *(by panki27 & xMasterX)*
> Minesweeper Game This is a Minesweeper implementation for the Flipper Zero device.

**Usage:** - Arrow buttons to move - Push center button to open field

#### Mode 7 Demo *(by CookiePLMonster)*
> A demo of pseudo-3D Mode 7 rendering Flipper Zero Mode 7 Demo A simple proof-of-concept of pseudo 3D rendering, modelled after a famous Mode 7 from SNES. Up/Down/Left/Right - Move the camera

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Monster Slayer *(by ratmanZorry)*
> Monster Slayer game for Flipper Zero Flipper Zero Game Engine Example

**Usage:** Cloning source code

#### Monty Hall *(by DevMilanIan)*
> Monty Hall asks you to guess which closed door a prize is behind.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Morse Master *(by P1X / w84death)*
> Learn Morse code using this toolkit. Morse Master: Morse Code Learning Toolkit for Flipper Zero !Morse Master Title Screen A comprehensive Morse code learning and practice app for Flipper Zero. Final release of version 1.0.

**Usage:** - Character Sets: Switch between letters (A-Z) and numbers (0-9) with the LEFT/RIGHT buttons - Visual Representation: See the Morse code pattern for each character

#### Multi Counter *(by JadePossible & Roro)*
> Counter App with 4 counter for tabletop games Flipper Multi Counter This is a simple external application for the Flipper Zero modified from VS Counter version. The app give you access to up to 4 counters to count your scores for tabletop games for example.

**Usage:** The UP and DOWN buttons are for adding or subbing one from the selected player And de LEFT and RIGHT to switch between players

#### Multi-Dice
> A customizable dice roller for tabletop gaming. Roll multiple dice of different types (d4, d6, d8, d10, d12, d20) simultaneously with animated results on screen.

**Usage:** Launch from Games. Use the d-pad to select dice type and quantity, press OK to roll. Results display with a satisfying animation.

#### Mystic Balloon *(by apfxtech)*
> Play the port of Mystic Balloon game on your Flipper Zero. Mystic Balloon is an atmospheric 2D platformer featuring 39 levels, each requiring precision, careful planning, and attention to detail.

**Usage:** The player controls a character who navigates the levels using balloons. The hero can slowly descend while suspended by balloons and float above ai... Player objectives:

#### Nah 2 da Nah Nah Nah *(by DigiMancer3D)*
> Games Nah2-Nah3: Flipper Zero Mini-Game Suite Nah2-Nah3 is a fun and engaging mini-game application for the Flipper Zero, featuring rhythm and action-based gameplay optimized for the device's 128x64 display and input system.

**Usage:** - Zero Hero: A rhythm game where you press buttons to hit falling notes, with dynamic difficulty and streak tracking. - Flip Zip: An running game where you navigate a mascot through lanes, jumping over obstacles with a speed bar and tap-based boosts.

#### Network Defender *(by w84death)*
> A Game & Watch style network security game for the Flipper Zero. Network Defender is a retro-style game inspired by the classic Nintendo Game & Watch handhelds. You play as a system administrator defending your network from incoming data packets and hacking attempts.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Nu pogodi *(by sionyx)*
> Elektronika IM-02 - Nu, Pogodi! / (Game & Watch - Egg) - Soviet retro game Игра Ну, погоди! для Flipper Zero

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### P1X Adventure *(by w84death)*
> Adventure game from P1X

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### P1X Your Own Adventure *(by w84death)*
> Choose your own adventure text game

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### PacMan WIP *(by DanieleMaffi)*
> The classic PacMan game.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Panis - A Grumpy Bread *(by F Greil)*
> A simple jump'n'run game where you control a grumpy bread Just a grumpy bread walking and jumping around. - Left/Right: Move Panis left or right - Up (single press): Small jump (~25px high)

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Paper Plane *(by Larry-the-Pig)*
> Guide a paper plane through obstacles to get the highest score. Port of this game to the Flipper Zero. !plane flying down Your goal is to fly a paper plane through obstacles and get to the bottom of the map.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Pinball0 *(by Roberto De Feo)*
> Pinball game Pinball0 (Pinball Zero) Play pinball on your Flipperzero! Get the latest version:

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Pocket Battle+ *(by HermeticCode)*
> Enhanced Pokemon Battle Game with authentic pokeyellow data Pokemon Yellow+ for Flipper Zero An enhanced Pokemon battle game for the Flipper Zero, featuring authentic Pokemon data integrated from the pokeyellow disassembly project. Authentic Pokemon Experience

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Pong *(by @nmrr & @SimplyMinimal)*
> Simple pong game flipperzero-pong A Pong game for the Flipper Zero Assuming the toolchain is already installed, copy flipper_pong directory to applications_user

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Puck Girl *(by F Greil)*
> A maze chase game where you control a little pie-shape girl to eat all the dots while avoiding be eaten by three ghosts

**Usage:** A simple Flipper Zero arcade chase game. The player controls a pie-shaped character ("Puck girl") through a maze, collecting dots while avoiding gh... Like in the classic game, each ghost has distinct personality: one targets directly, another one patrols.

#### Quadrastic *(by @ivanbarsukov)*
> Quadrastic is a simple addicting game inspired by the Arduboy game of the same name Flipper Zero Quadrastic Game Quadrastic is a simple addicting game inspired by the Arduboy game of the same name. This, in turn, was based on a popular PSP homebrew game called Squarez.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Questions *(by nikilark)*
> Questions to better know each other Flipper Questions Flipper app designed to foster meaningful connections by helping you discover more about each other. Perfect for getting in touch with new people or strengthening existing bonds.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Race *(by zyuhel)*
> Race game inspired by Race game in BrickGame 9999 in 1. Flipperzero Race Game Race game inspired by Race game in BrickGame 9999 in 1. Source is heavily base on tetris game from jeffplang's repo and this article.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Race Game *(by mrc19056)*
> 3-lane racing game with power-ups, combos, and night mode 🏎️ Race Game for Flipper Zero A fast-paced 3-lane vertical scrolling car racing game for Flipper Zero. - 3 Lane Racing — Dodge incoming vehicles on a scrolling road

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Reaction Test *(by Milk-Cool)*
> A simple reaction test game fz-reaction-game A simple reaction test for the flipper zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Reaction Time *(by ihatecsv)*
> A simple reaction time game Flipper Zero Reaction Game This is a simple reaction game for the Flipper Zero device. The game is designed to test your reaction times, giving you immediate feedback on how quickly you respond to visual prompts. - The game begins with a "Press to start" screen.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Reversi *(by @dimat)*
> Reversi game, the game controls should be intuitive. Longs press on OK opens the menu to start a new game. Reversi game for Flipper Zero 2. Go to the applications_user directory 3. Create a symlink to this repo assuming that these two repos are on the same level:

**Usage:** The game controls should be intuitive.

#### Rock Paper Scissors *(by jamisonderek)*
> Play the rock-paper-scissors game with your friends using the Flipper Zero Sub-GHz radio! Rock Papper Scissors !Flipper Zero Game This multi-player application relies on a SubGHz API that was recently changed. Older firmware will get a build error, due to not supporting the new API definitions.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Rock Paper Scissors 2 *(by benwoo1110)*
> Rock Paper Scissor app Rock Paper Scissor A fun rock paper scissor game for the Flipper Zero device.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Roots of Life *(by @Xorboo)*
> A zen-puzzle game for FlipperZero, puzzle made on GlobalGameJam23 (theme: Roots) A zen-puzzle game for FlipperZero puzzle made on GlobalGameJam'23 (theme: "Roots") !FlipperZero with the game on it Nothing interesting in the code, as you would expect from a jam-rushed development.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Rubik's Cube Scrambler *(by @RaZeSloth)*
> App generates random moves to scramble a Rubik's cube. Rubik's Cube Scrambler FAP Where to start? What does what?

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Sand Simulation *(by JJoeDev)*
> A sand simulation that can run on the Flipper Zero FZSandSimulation Simulation is running at a blazing 10FPS by design. (Can be changed in code) Simulation is running on 128x64 (F0 screen size)

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Scorched Tanks *(by @jasniec)*
> A Flipper Zero game inspired by scorched earth Scorched tanks - flipper zero game A flipper zero game inspired by scorched earth. Current state is shown below:

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Secret Toggle *(by nostrumuva)*
> A game that toggles squares. A game for the Flipper Zero device.

**Usage:** - Arrow buttons move around the board. - Push center OK button to toggle a square. Some squares are connected.

#### Simon Says *(by @SimplyMinimal & @ShehabAttia96)*
> A Simon Says Game FlipperZero-SimonSays Think you can beat Simon? !Image of dolphin saying Simon Says

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Slot Machine *(by @Daniel-dev-s)*
> Simple Slots simulator game Slots for Flipper Zero :) Simple slots with highscores for flipper zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Slots *(by vh8t)*
> 81-way slot machine A high-fidelity slot machine for your Flipper Zero. When you're bored and feeling lucky, jump into this "all-ways" simulator and see if you can hit the jackpot! 💎 The 81-Way Experience 81-Way Slots brings the excitement of the casino floor to your pocket.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Snake 2.0 *(by @Willzvul)*
> Advanced Snake Game (Remake of original Snake)

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Snake Game
> Classic Snake Game

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Sokoban *(by Racso)*
> Sokoban on Flipper Zero. Solve your path to victory! Sokoban is a classic puzzle game. Your objective is to push all the boxes onto the targets. Rules are simple: 1. You can only push boxes, not pull them.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Solitaire *(by doofy-dev)*
> Klondike Solitaire card game !maintained - yes !contributions - welcome Solitaire - Klondike for Flipper Zero

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Space Impact *(by Ka3u6y6a)*
> Space Impact game port from Nokia for Flipper Zero Space Impact game port from Nokia for Flipper Zero Work in progress Gameplay video on Nokia phone:

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Space Impact II *(by Erbonator3000 & VoidXH)*
> Space Impact II clone Flipper zero Space-Impact-II port Pixel-perfect clone of Nokia's Space Impact, one of the biggest classics in mobile gaming. All credit for the game goes to Bence Sgánetz, original creator of the clone. Port was made with effort to preserve as much of the

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Space Invaders *(by PavelZurek)*
> Adaptation for classic Space Invaders Space Invaders adaptaion for Flipper Zero. Check github actions to download or build your own with ufbt. - [x] Multiple enemy types

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Stratagem Zero *(by Nymda)*
> Stratagem hero clone Stratagem hero clone for the Flipper Zero Designed to be as close to the original as possible !Screenshot-20240521-151712

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Sudoku *(by profelis)*
> Sudoku game - ok - increment number - back - clear number - long back - pause game

**Usage:** - cursor keys - move cursor

#### SUPER-CHIP *(by Milk-Cool)*
> SUPER-CHIP (Chip8) emulator for the Flipper Zero A SUPER-CHIP emulator for the Flipper Zero I know there's something wrong with it but i can't figure out what it is so the current state of this project is public testing. Please let me know if there are any issues with it.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Swimmy Fish *(by Invizabel)*
> Swimmy Fish. One goal, Survive. One button, Enter Flappy, er, Swimmy Fish

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### T-Rex Runner *(by @Rrycbarm)*
> Play the port of the Chrome browser T-Rex game on your Flipper Zero. Flipper Zero port of Chrome's running T-rex game - Added command to move DINO - Added gravity

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Tamagotchi
> Tama P1 Emulator for Flipper Zero This is a tama P1 Emulator app for Flipper Zero, based on TamaLIB. Create a tama_p1 folder in your microSD card, and put the ROM as rom.bin.

**Usage:** in portrait mode are the same as landscape mode, but turned 90 degrees.

#### Tanks
> A two-player tank battle game using Sub-GHz radio for local multiplayer. Drive your tank around the battlefield and shoot your opponent before they get you.

**Usage:** Two Flipper Zeros needed. Both players launch the app, one hosts and the other joins via Sub-GHz. Use d-pad to move your tank and OK to fire.

#### Tarot *(by pionaiki & tihyltew)*
> Tarot card reader Tarot spread for Flipper Zero. This app can give you a linear spread of the major arcana cards (minor arcana is planned to be added). - Official app store - Auto-built releases are available on the Github actions page

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Tetris *(by @xMasterX & @jeffplang & @noiob)*
> Tetris game for Flipper Zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Tetris Modern *(by xMasterX & jeffplang & noiob & Alloy-Lou)*
> Tetris Modern game for Flipper Zero.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Tic Tac Toe *(by @xMasterX & @gotnull)*
> Tic Tac Toe game, for 2 players, play on one device

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Tic-Tac-Toe Multi *(by RouNNdeL)*
> Multiplayer Tic-Tac-Toe game

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Ultimate Tic-Tac-Toe *(by Racso)*
> Ultimate Tic-Tac-Toe: play on a big board, where each square is a Tic-Tac-Toe board itself! About Ultimate Tic-Tac-Toe Ultimate Tic-Tac-Toe is a strategic variant of Tic-Tac-Toe where you have 9 Tic-Tac-Toe boards arranged in a 3x3 grid.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Umpire Indicator *(by @RocketGod-git)*
> Umpire indicators keep track of balls, strikes, and outs for Baseball and Softball ⚾ Flipper Zero Umpire Indicator ⚾ A baseball umpire indicator app for the Flipper Zero device! This app simulates a traditional baseball umpire's counter, tracking balls, strikes, and outs with a classic.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Vexed *(by dlvoy)*
> Vexed - classic Palm.OS puzzle game !Vexed splash screen logo Vexed for Flipper Zero Vexed is puzzle game, where your goal is to clear level from bricks in smallest possible count of moves. Easy to learn, hard to master

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Video Poker *(by @PixlEmly)*
> Video poker is a casino game based on five-card draw poker

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Wave *(by sergo)*
> Game about a wave that avoids obstacles, it's cool This game is almost entirely AI-generated through all circles of hell. The logic is simple: the arrow falls down, and when you hold down the OK or UP button, you start flying up.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Wolfenduino *(by apfxtech)*
> Play the port of Wolfenduino game on your Flipper Zero. Wolfenduino is a demake of id software's Wolfenstein 3D for the Arduboy FX. The game recreates the classic shareware episode "Escape from Wolfenstein" with original-style gameplay adapted for portable hardware.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Yappy Invaders *(by @dagnazty)*
> Yappy version of Space Invaders. Yappy Invaders is a space invaders clone designed specifically for the Flipper Zero device. It combines the classic gameplay of space invaders with the unique hardware capabilities of Flipper Zero,...

**Usage:** Simple and intuitive controls using Flipper Zero's built-in buttons. Score tracking for each game session.

#### Yatzee *(by @emfleak)*
> Yahtzee game flipperzero-yatzee Yahtzee game for flipperzero Its not beautiful, but it works and now I can play Yahtzee on my flipper while I shit at work.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### ZERO! *(by Racso)*
> ZERO! Get rid of all your cards before your opponents do! ZERO! is a card game where the aim is to get rid of all your cards before your opponents! The game implements the rules of the classic card game UNO, with adaptations to make it playable on the Fli... 1.

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

#### Zombiez *(by @DevMilanIan & @xMasterX, (original By @Dooskington))*
> Defend your walls from the zombies

**Usage:** Launch from the Games menu. Use d-pad to control and OK to select/action. Back to exit.

</details>

<details><summary><h3>🔧 Tools — 107 apps</h3></summary>

#### [WIP] Skeleton Sample App *(by jamisonderek)*
> Skeleton Sample App. This is intended to be used as a starting point for new applications with one primary screen. You can use this application as a starting point for creating your own application. It contains the skeleton framework to get an application running on the Flipper Zero.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Access Audit *(by matthewkayne)*
> Audit NFC, RFID, and HID iCLASS access-control cards. Detects DESFire EV1/EV2/EV3, MIFARE Classic/Plus SL1-SL3, NTAG, EM4100, HID iCLASS Legacy, and more. Instant risk score with per-card advice.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Air Level *(by CodeAllNight (MrDerekJamison))*
> Flipper Level using the Video Game Module motion sensor This is a simple level that can use the motion-tracking sensor of the Video Game Module. - Copy the air_level project into your firmware's applications_user folder.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Analog Clock *(by scrolltex)*
> Shows analog clock on Flipper screen Analog Clock app for Flipper Zero

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Authenticator *(by Alexander Kopachov (@akopachov))*
> Software-based TOTP/HOTP authenticator for Flipper Zero device

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Barcode *(by @Kingal1337)*
> App allows you to display various barcodes on flipper screen Barcode Generator A barcode generator for the Flipper Zero that supports UPC-A, EAN-8, EAN-13, Code-39, Codabar, and Code-128[1] Table of Contents

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### BG Loader *(by twisted-pear)*
> This plugin allows compatible apps to be loaded and to remain running in the background. Flipper Zero Background Loader This is a plugin for the Flipper Zero that allows compatible apps to be loaded and to remain running in the background.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Big Clock *(by Eris-Margeta)*
> Bedside clock with adjustable brightness (0-100%). UP/DOWN to adjust. !Big Clock Screenshot ⬇️ Download Latest Release - Get the .fap file and copy to /ext/apps/Tools/ on your Flipper. Turn your Flipper Zero into a bedside or desk clock with:

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### BlackJack Counter *(by grugnoymeme)*
> Simple application to keep the count of blackjack cards during the game. flipperzero-blackjack_counter-fap A BlackJack Counter Fap for Flipper Zero embedded Device.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Brainfuck *(by nymda)*
> Brainfuck language interpreter FlipperZeroBrainfuck Brainfuck interpreter and editor for the F0. Supports text inputs and outputs.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Caesar Cipher *(by @panki27)*
> Encrypt and decrypt text using Caesar Cipher A caesar cipher encoder for the Flipper Zero device. Start app, painfully input your ciphertext with the onscreen keyboard. Replace spaces with underscores. Hit "Save", scroll output.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Calculator *(by @n-o-T-I-n-s-a-n-e)*
> Calculator, that can calculate simple expressions

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Calendar *(by Adiras)*
> Simple Flipper Zero calendar application Flipper Zero calendar application - Flipper Zero Official Website. A simple way to explain to your friends what Flipper Zero can do. - Flipper Zero Firmware Update.

**Usage:** - User Documentation. Learn more about your dolphin: specs, usage guides, and anything you want to ask. How to set up and build the application

#### Calendar of weeks *(by F. Greil)*
> Simple app to organize your week mitzi-cal-weeks This Calendar of week is an application for the Flipper Zero that displays a weekly view of dates &mdash; the current week along with the previous and next week. Today's date is highlighted with i...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Calendar TECHART *(by TechArtDev)*
> Simple calendar app Simple Calendar App for Flipper Zero the source code for a simple calendar application designed for the Flipper Zero device. The app provides an intuitive display of the days of the month and supports basic na... - Monthly View: Display the days of the current month.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### CAN Tools *(by Matthew KuKanich)*
> CAN Tools, DBC management, Data decoding CAN Tools is a Flipper Zero external app for creating simple DBC-style signal definitions and decoding CAN frames with them. - Create and save DBC signal entries (name, CAN ID, bit layout, scaling, unit, min/max).

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Chronometer *(by nmrr)*
> This chronometer is accurate to the millisecond. TIM2 internal timer of the STM32 MCU is used to generate a 64 MHz clock signal. This signal is used to count elapsed time.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### CLI-GUI Bridge
> flipperzero-cli-bridge Allows CLI control from GUI, giving untethered access to sub-ghz chat, system diagnostics, and more. Easy way - get a .fap file from the releases page

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Clock *(by kowalski7cc)*
> Simple clock app

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Combo Cracker
> Crack combo locks in 8 attempts or less ComboCracker-FZ Combo Cracker is an on-the-go combination lock cracking tool for the Flipper Zero, inspired by security researcher Samy Kamkar’s work on the mechanical vulnerabilities in Master Lock combination pa...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Combo Cracker *(by CharlesTheGreat77)*
> Crack combo locks in 8 attempts or less ComboCracker-FZ Combo Cracker is an on-the-go combination lock cracking tool for the Flipper Zero, inspired by security researcher Samy Kamkar’s work on the mechanical vulnerabilities in Master Lock combination pa...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Count Down Timer *(by @0w0mewo)*
> Simple count down timer Simple count down timer application for flipper zero up/down: set second/minute/hour value. ok: start/stop counting.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Counter *(by @Krulknul)*
> Simple counter Dolphin counter This is a simple plugin for the Flipper Zero.

**Usage:** It gives you access to a counter which you can increment and decrement using the up and down buttons respectively. Long-pressing the OK button will enable vibro feedback for adding, and long-pressing the back button will clear the counter.

#### Device Info
> Device Info application for Flipper Zero.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Doc Viewer Lite *(by C0d3-5t3w)*
> A simple document viewer Doc Viewer Lite A lightweight text document viewer application for Flipper Zero that allows you to read text files directly on your device. - View text files (.txt) stored on your Flipper Zero's SD card

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### DTMF Dolphin *(by @litui & @xMasterX)*
> DTMF (Dual-Tone Multi-Frequency) dialer, Bluebox, and Redbox. DTMF (Dual-Tone Multi-Frequency) dialer, Bluebox, and Redbox.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Enigma *(by Struan Clark (xtruan))*
> Enigma for Flipper Enigma machine emulator for Flipper Zero - Built against 0.104.0 Flipper Zero firmware release - Uses enigma-machine lib

**Usage:** Key features of the M3 Enigma included:

#### Eye Saver *(by paul-sopin)*
> Simple eye strain prevention tool using the 20-20-20 rule. What does this app do? This app is designed to prevent eye strain when looking at a screen for extended periods of time. 69 percent of Americans have reported eye strain at least once a week, and this app is designed to ...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### F0 Forth *(by twisted-pear)*
> Once started the plugin will add a forth command to the CLI that can be used to call up a basic forth shell. Flipper Zero Forth This is a plugin for the Flipper Zero that ports Once started the plugin will add a forth command to the CLI that can be used

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Financial Calculator *(by schaene)*
> Solve TVM Problems with this calculator Flipper-Financial-Calculator Very basic TVM calculator for Flipper Zero Have you ever thought to yourself "Man, I wish I could figure out the Future Value of a $12,000 investment, over the span of 3 years, with a 7.5% interest rate, in which I pay another.

**Usage:** - Press the arrow keys to move around the pinpad.

#### FlipBIP Crypto Wallet *(by Struan Clark (xtruan))*
> Crypto wallet for Flipper FlipBIP - BIP32/39/44 Crypto toolkit for Flipper Zero - Last built against 1.0.1 Flipper Zero firmware release

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### FlipCrypt *(by Tyl3rA)*
> Encrypt, decrypt, and hash text using a wide variety of classic and modern crypto tools. FlipCrypt is a Flipper Zero app that provides a collection of classic cipher algorithms, cryptographic hash functions, and some text encoding methods to explore and learn about. You can emulate the...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### FlipCrypt *(by TAxelAnderson)*
> Encrypt, decrypt, and hash text using a wide variety of classic and modern crypto tools. FlipCrypt is a Flipper Zero app that provides a collection of classic cipher algorithms, cryptographic hash functions, and some text encoding methods to explore and learn about. You can emulate the...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Flipfetch *(by Ismael A. Rodríguez)*
> A simple app that shows some flipper info Sistema de información estilo fastfetch para Flipper Zero. - Firmware version - Batería % y voltaje

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### FlipNote *(by morty517)*
> A real text editor for Flipper Zero. Open, edit and save files directly on device.

**Usage:** A real text editor for Flipper Zero — bc 5 buttons and a 128×64 screen are more than enough. - Open / Save / Save As — full file browser powered by Momentum's native file picker

#### Flipp Pomodoro
> Flipp Pomodoro application for Flipper Zero.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### FlipPar *(by jsammarco)*
> Track golf and disc golf rounds on Flipper Zero. FlipPar is a Flipper Zero external app for tracking golf or disc golf rounds on-device. It lets you set the number of holes and players, rename players, record par and score values hole-by-hole, an...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Flipper Aid *(by spaghety)*
> Flipper application for emergency response with pulse timer and CPR timer Flipper application for emergency response with pulse timer and CPR timer

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Flipper Printer *(by alxcrt)*
> Thermal printer app with coin flip game and text printing Flipper Printer A Flipper Zero external application that combines a coin flip game with thermal printer functionality using the T7-US thermal printer module.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Flipper Wedge *(by Dangerous Things)*
> Read RFID/NFC tags and type their UIDs as HID keyboard input via USB or Bluetooth. Supports 5 scan modes and NDEF text records. Brought to you by Dangerous Things A Flipper Zero application that transforms your device into a contactless tag-to-keyboard interface.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Flipper95 *(by Silent / CookiePLMonster)*
> Stress test your Flipper by crunching prime numbers Application to stress test the Flipper by finding Mersenne primes using the Lucas-Lehmer Test, much like Prime95 does. By default, the app starts from the Mersenne prime M2. This can be overridden from the CLI,

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### FlipperPwn *(by PentestSuite & barkandbite)*
> Modular pentest payload framework with OS detection and WiFi Dev Board support Modular pentest payload framework for Flipper Zero

**Usage:** FlipperPwn is a Metasploit-inspired payload framework (v1.5) that turns Flipper Zero into a full USB HID attack platform. Load .fpwn modules from a... Table of Contents

#### FlipperZero Clock *(by mdaskalov)*
> FlipperZero Customizable Clock Displays customizable clock in full-screen or split-view with date and weekday. Customizations are saved in a configuration file and retrieved on next application start. Use the ok key to switch between split-view and full-screen mode.

**Usage:** In split-view mode use the left key to select the desired clock face. In full-screen mode use the left and right keys to change the width. Use the up and down arrow keys to select which hour digits are shown for the analog faces.

#### Ford Radio Codes *(by DavidB)*
> Ford Radio 'M' & 'V' Unlock Code Generator! Ford Radio Codes - Flipper Zero Ford Radio 'M' &amp; 'V' Unlock Code Generator For Flipper Zero! Instantly generate unlock codes for for 'M' and 'V' series radios directly on your Flipper Zero!

**Usage:** Simply enter the radio's serial number using the UP and DOWN buttons and the 4-digit unlock code will be shown. Remember to copy the 'radiocodes.bin' file to your SD Card.

#### GnomishTool *(by Andreeved88)*
> Gnomish army multitool. Assembled in Gnomeregan Gnomish army knife for Flipper Zero owners. Civilian market version! Another gift from the brothers from Gnomeregan. - Ruler : 3 cm with a division of up to 0.25 mm (1 px).

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### HEX Editor *(by @dunaevai135)*
> Read text files line by line and edit them without a computer or smartphone. flipper-zero-hex_editor inspired by QtRoS/flipper-zero-hex-viewer Read any file line by line, and by Ok allow change char. Useful for NFC file "Edit Dump" feature with out smartphone.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### HEX Viewer *(by @QtRoS)*
> App allows to view various files as HEX Hex Viewer application for Flipper Zero! The app allows you to view various files as HEX - Written with pure C in a very simple and effective manner

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### HyperFocus Calc *(by Endika)*
> Hyperfocal distance calculator: focal and aperture on-device with sensor CoC. HyperFocus Calc (Flipper Zero) External Flipper Zero app that computes hyperfocal distance from focal length (mm), aperture (full-stop sequence), and each sensor’s circle of confusion (CoC).

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### IconEdit *(by Roberto De Feo)*
> Icon editor !build status badge Icon editor for the FlipperZero Create images and animated icons up to 128x64 in size directly on the Flipper! Easily preview how your images, icons, and animations will look directly on the device. Send your images to your PC.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### ID Card *(by evillero)*
> This program represents a simple application for managing an ID card. This program represents a simple application for managing an ID card for Flipper Zero.

**Usage:** In the main menu, users have three options: ID Card: View personal data.

#### Key Copier *(by Torron)*
> @README.md A Flipper Zero app for measuring key bitting patterns. The best way to install the latest build would be download it from Flipper Zero's Official App Store: To measure your key:

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Key Copier *(by zinongli)*
> Measure and save Kwikset/Schlage physical keys. A Flipper Zero app for measuring key bitting patterns. To measure your key: 1. Place it on top of the screen.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### LED Blinker *(by Cupprum)*
> Blinking LED light, which gradually slows down. A Flipper Zero application that blinks LEDs with a decreasing frequency over time. Unlike traditional Pomodoro timers, it provides visual feedback through LED blinks that gradually slow down. This application can be downloaded from Flipper Lab.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### LED Pulsator *(by Leed)*
> Flipper Zero LED Pulse Animations This app is a proof-of-concept to show abilities of the LED light integrated in the Flipper Zero device.

**Usage:** By default the Flipper Zero firmware framework offers various controls to the LED, including - Selection of few basic colors (red, green, blue, yellow, magenta, cyan etc)

#### Letter Beacon *(by nmrr)*
> A letter/number morse beacon on 125 kHz using RFID interface of the Flipper Zero flipperzero-letterbeacon A letter/number/symbol Morse beacon on RFID/NFC interfaces of the Flipper Zero (banner has been made with Dall.E 2)

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Lightning Distance *(by HyperMuffin12)*
> An app that calculates the distance of a lightning strike based on time between seeing the flash and hearing the thunderclap. Flipper Zero Lightning Distance An app that calculates the distance of a lightning strike based on time between seeing the flash and hearing the thunderclap.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Lishi *(by evillero)*
> App for saving obtained vaules from lishi tool. An app for saving values obtained from the Lishi tool, which is used for lock decoding. This app allows users to input, view, and store values from the decoding process for easy reference and organ...

**Usage:** - Key Types: The app supports various types of LISHI keys. - Input values directly from the Lishi tool into the app.

#### ListEM *(by Clawzman)*
> NFC / RFID / iButton List Generator 📟 ListEM — Advanced UID List Generator for Flipper Zero ListEM is a Flipper Zero application that generates large, customizable UID dictionaries for RFID, NFC, and iButton protocols directly on your Flipper!.

**Usage:** ListEM brings advanced list generating features (previously done via my Python scripts) straight onto the Flipper Zero. Now with Fuzzing Mode / Bit Mutation engine! A real, usable reader testing FuZZ engine!

#### Math Wiz *(by Papa_Ghost)*
> Does some trig/calculus calculations Polynomial-Solver Polynomial-Solver is an application for the Flipper Zero that enables users to solve and manipulate polynomials up to the sixth degree. With this app, you can evaluate polynomials, calculate deriva...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Mitzi Type Aid Demo *(by F Greil)*
> A type-aid for Flipper Zero My take on a Flipper Zero type-aid. You can compare my version with out-complete with the standard keyboard. From the main screen, you have three options: - OK opens the keyboard with with word suggestions

**Usage:** On the new keyboard, the user navigates using the directional buttons, OK selects the highlighted character or button.

#### Moon Phases *(by w84death)*
> A simple application to display the current phase of the moon Moon Phases App A simple application to display the current phase of the moon along with additional lunar information. For Flipper Zero device.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Multi Converter *(by @theisolinearchip)*
> A multi-unit converter written with an easy and expandable system for adding new units and conversion methods

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Multi Timer *(by C0d3-5t3w)*
> Timer app with presets, multiple custom timers and background support. 🐬⏰ Flipper Zero MultiTimer A feature-rich multi-timer application for Flipper Zero with a charming dolphin mascot!

**Usage:** • Installation • Usage • Development • Screenshots ⏱️ Multiple Timer Management

#### NFC/RFID Detector *(by SkorP)*
> Identify the reader type: NFC (13 MHz) and/or RFID (125 KHz).

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Nickname Generator *(by disaxq)*
> Easily create unique names using this app for flipper zero. Nickname-Generator Nickname Generator in flipper zero Easily create unique names using this app for flipper zero.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Nightstand Clock *(by @nymda & @Willy-JL)*
> Clock with screen brightness controls FlipperNightStand Fork of the standard clock app, for use as a bedside clock at night -Date and AM/PM have their places swapped

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Notes for FZ (WIP)
> Notes for FZ (WIP) application for Flipper Zero.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Orgasmotron *(by Leedave)*
> Vibrate Flipper in different modes Flipper Zero Orgasmotron Update This app is a proof-of-concept to show abilities of the vibration motor in the Flipper Zero device. I'm also using this to learn how to code for Flipper Zero

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Paranoia *(by C0d3-5t3w)*
> Anti-Surveillance Field Tool that scans for hidden wireless cameras, RFID skimmers, and IR monitoring devices Paranoia Mode for Flipper Zero Paranoia Mode is an anti-surveillance tool for Flipper Zero that scans for hidden wireless cameras, RFID skimmers, and infrared monitoring devices.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Password Generator *(by @anakod & @henrygab)*
> Simple password generator flipper_passgen This is a simple Password Generator plugin (fap) for the Flipper Zero.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Password Manager *(by Rrycbarm)*
> This app stores your usernames and password and can write them on your PC acting as a keyboard Password Manager Use you flipper zero as a password manager and authenticator! The flipper stores your usernames and password and can write them on your PC acting as a keyboard!

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Pet a Dolphin *(by EugeneSmile)*
> Flipper app to make your dolphin a little bit happier (Reset mood) Flipper app to make your dolphin a little bit happier (Reset mood) 1. Put directory into applications_user directory of flipperzero firmware sdk 5. Run application, press OK to make Dolphin more happy

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Pet Your Dolphin *(by dwight9339)*
> Lt's you check in on your dolphin buddy daily and give them some well-deserved pets. Petting your dolphin improves its mood and provides some XP but these effects are capped by the firmware so you're limited to two pets a day.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Pomodoro Timer *(by sbrin)*
> Use your Flipper Zero as a Pomodoro Timer. flipperzero_pomodoro The Pomodoro Technique is a time management method developed by Francesco Cirillo in the late 1980s.[1] It uses a kitchen timer to break work into intervals, typically 25 minutes in length,.

**Usage:** Flipper Zero is a portable Tamagotchi-like multi-functional device developed for interaction with access control systems. The device is able to rea... Pomodoro timer application for Flipper Zero

#### Programmer Calculator *(by armixz)*
> Calculator, for Programmers! Programmer Calculator a calculator application designed to run on Flipper Zero. - Decimal to Binary

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Programmer Calculator *(by @armixz)*
> Calculator, for Programmers! Programmer Calculator a calculator application designed to run on Flipper Zero. - Decimal to Binary

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### QR Code *(by Bob Matcuk)*
> Display qrcodes flipperzero-qrcode Display qrcodes on the [Flipper Zero] !qrcode screenshot

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### QRCode Generator *(by Thomas Schwarz (aka qw3rtty))*
> Generates and displays QRCodes on flipper zero. Flipper Zero: QRCode Generator Generates and displays QRCodes on the flipper zero. Grab the qrcode_generator.fap from the latest release.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Quac! *(by Roberto De Feo)*
> Quick Action remote control app Quick Action Remote Control for Flipperzero Get the latest version: Flipper Lab - recommended

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Quadratic Solver *(by paul-sopin)*
> A simple quadratic equation solver app What does this app do? This app takes a, b, and c values from a quadratic equation in form "ax^2 + bx + c = 0" Select a number when hovering over a, b, or c with LEFT to decrease number, and RIGHT to increase.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Reality Clock *(by Eris-Margeta)*
> Dimensional stability monitor using multi-band EM ratio analysis. Reality Dimension Clock v4.1 !Reality Dimension Clock Screenshot ⬇️ Download Latest Release - Get the .fap file and copy to /ext/apps/Tools/ on your Flipper.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Resistance Calculator *(by Lewis Westbury)*
> Resistor calculations Resistance calculator A simple app to calculate resistance from the coloured bands. See: Pick from 3, 4, 5, or 6 band resistor.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Resistance Calculator *(by Lewis Westbury)*
> Resistor calculations Resistance calculator A simple app to calculate resistance from the coloured bands, based on original work by Lewis Westbury. Pick from 3, 4, 5, or 6 band resistor.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Roman decoder *(by evillero)*
> An app that converts Roman numerals to decimal values. An Flipper Zero app that converts Roman numerals to decimal values.

**Usage:** Roman Decoder is an application that allows users to easily convert Roman numerals to decimals. The app provides an interactive user interface that... - Interactive Dialing:Users can use the buttons to navigate through Roman numerals.

#### ROT13 *(by nothingbutlucas)*
> Cipher text with ROT13 I moved the source code to github because the flipper zero application catalog only accepts repos from github Actual repo: https://github.com/nothingbutlucas/flipperzero_rot13 ROT13 application

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Rubiks Cube Timer *(by KHOPAN)*
> Cuber's best friend

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### SD Info *(by sergo)*
> The application shows information about the SD card The application shows information about the SD card and you can also perform a test that will show the card status.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Segment Clock *(by Sladkisnovraper)*
> Simple segment clock. Simple segment clock. Press the up button to toggle between 12 and 24 hour mode.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Sig Diary *(by C0d3-5t3w)*
> Background scanner that logs signals and annotates them - Multi-protocol Scanning: Simultaneously monitors IR, RF, and NFC signals

**Usage:** - Automatic Annotation: Identifies common signals such as garage doors, remote controls, and access cards - Signal Logging: Maintains a timestamped log of all detected signals

#### SKIDcity *(by H4W9)*
> Educational: Don't be a SKID! "Your flipper can't do that. and if it could, you'd be in federal prison."

**Usage:** So you got a Flipper Zero. Maybe you saw it on TikTok. Maybe some guy on Discord told you it could hack traffic lights, clone any card, crash wifi ... This app is for you.

#### Smartra VIN2PIN *(by evillero)*
> Simple calculator for SMARTRA2 immobilizer pins for Hyundai/KIA vehicles Smartra VIN2PIN Flipper Zero app calculator for SMARTRA2 immobilizer pins for Hyundai and KIA vehicles - Supports most Hyundai/KIA models using SMARTRA2.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Space Travel Calculator *(by ejfox)*
> KSP-style interplanetary transfer planner Space Travel Calculator A minimalist trajectory calculator for the Flipper Zero. Plan real interplanetary missions with KSP-style visual feedback. Answer the critical mission planning questions:

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Spindle Calculator *(by Jordan M)*
> Spindle spacing calculator for stairs and straight runs. Uses IMU for stair angle. Spindle Calculator — Flipper Zero App A trade tool for carpenters and joiners to calculate spindle counts and spacings for balustrades, right on site — no phone or calculator needed.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Tasks *(by Stanislav Vasilev(Madman10K))*
> A simple to-do application. A TODO list application for the Flipper Zero using the UntitledFlipperZero framework. 1. Run ufbt in the root of the directory 1. To launch the application on the Flipper Zero, connect to it via USB and run ufbt launch

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Tasks *(by Stanislav Vasilev(Madman10K))*
> A simple to-do application. A TODO list application for the Flipper Zero using the UntitledFlipperZero framework. 1. Run ufbt in the root of the directory 1. To launch the application on the Flipper Zero, connect to it via USB and run ufbt launch

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Text Viewer *(by @Willy-JL)*
> Text viewer application

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Time Dilation Calculator *(by ejfox)*
> Time dilation calculator for relativistic travel Flipper Space Calculators A pair of minimalist space and physics calculators for the Flipper Zero. No games, no splash screens -- just the numbers. Space Travel Calculator

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Time Tracker *(by MassivDash (hhtps://spaceout.pl))*
> Flipper Time Tracker is an application for flipperzero device that allows the user to track multiple tasks at the same time. Application uses csv file to store and handle task data that can be easily exported to excel or similar for further analysis.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Timer *(by nagamine-git)*
> A pomodoro style timer app with 30 minutes of break and 20 minutes of work

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Tone Generator *(by Gerald McAlister)*
> A simple app to generate sound tones.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Tree Identification *(by fgreil)*
> Identify a tree by answering questions about the leave shapes mitzi-tree-ident Goal: Identify a tree by answering questions on your Flipper Zero. Sketch of user flow

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### uPython *(by Oliver Fabel)*
> Compile and execute MicroPython scripts MicroPython Flipper Zero Allows you to use the power of Python natively on your Flipper Zero. The application is available on the official Flipper Lab.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### uPython *(by Oliver Fabel)*
> Compile and execute MicroPython scripts MicroPython Flipper Zero Allows you to use the power of Python natively on your Flipper Zero. The application is available on the official Flipper Lab.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### VIN Decoder *(by evillero)*
> Vehicle Identification Number decoder VIN decoder for Flipper Zero VIN Decoder is a mini flipper zero tool that allows you to decode VIN (Vehicle Identification Number) numbers. This project provides users with the ability to enter their VIN and obtain information... Version history

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Voltage Calculator *(by Andrew Diamond)*
> Voltage (VRI) calculations Voltage Calculator VoltCalc is a simple voltage calculator to calculate values using Ohms Law V = R * I. !enter current value

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Voltage Calculator *(by HappyAmos)*
> Voltage (VRI) calculations Voltage Calculator VoltCalc is a simple voltage calculator to calculate values using Ohms Law V = R * I. !enter current value

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Voyah Password *(by bolknote)*
> This program allows you to generate the engineering menu password for Voyah vehicles. Since the password depends on the current date and is calculated according to the Chinese time zone, you will need to set your current time zone after launching the program.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

</details>

<details><summary><h3>📡 Nfc — 51 apps</h3></summary>

#### AmiTool *(by Firefox2100)*
> An NTAG215 toolkit for Flipper Zero to work with tags used with gaming consoles. An Flipper Zero toolkit to work with NTAG215 tags with gaming consoles. This project is usable but still in development.

**Usage:** Designed as a general-purpose toolkit for Flipper Zero to work with NTAG215 figures/cards for gaming consoles, this app aims to provide the followi... - Reading NTAG215 character NFC tags: Read data from physical NTAG215 gaming tags and store it on the Flipper Zero.

#### AmiTool *(by Firefox2100)*
> An NTAG215 toolkit for Flipper Zero to work with tags used with gaming consoles. An Flipper Zero toolkit to work with NTAG215 tags with gaming consoles. This project is usable but still in development.

**Usage:** Designed as a general-purpose toolkit for Flipper Zero to work with NTAG215 figures/cards for gaming consoles, this app aims to provide the followi... - Reading NTAG215 character NFC tags: Read data from physical NTAG215 gaming tags and store it on the Flipper Zero.

#### Amusement IC *(by object-Object)*
> A simple app "next page" in display scene to show overflowing info (full vendor name, PMm) emulate url card (see nfcurl)

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### COGS MyKey *(by luhf)*
> Read and edit COGES MyKey NFC cards. This is a Flipper application for editing and writing COGES Mykey Dongles. I’m releasing this because idiots are selling similar apps for profit and violating the MIKAI license. This release is for educational purposes only and comes with zero support.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Cyborg Detector *(by @RocketGod-git)*
> App generates a continous NFC field to make body mod implant LEDs glow. Tested on a Dangerous Things xSIID. Flipper-Zero-Cyborg-Detector 🤖 Activate Cyborg Implants LEDs App generates a continous NFC field to make body mod implant LEDs glow. Tested on a Dangerous Things xSIID.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### GhostBook *(by Digi)*
> Encrypted tap-to-share contacts with passcode lock Encrypted NFC Contact Sharing for Flipper Zero Share your contact info with a tap. Protected by passcode. Auto-wipes on failed attempts. - NFC Tap-to-Share — Emulates NTAG215, works with other Flippers

**Usage:** - AES-class Encryption — 256-bit keys with 10,000 iteration stretching

#### HID iClass (PicoPass)
> App to communicate with PicoPass(iClass) tags See readme: https://lab.flipper.net/apps/picopass/ To Build ASN1 (if you change sio.asn1) Run asn1c -D ./lib/asn1 -no-gen-example -no-gen-OER -no-gen-PER -pdu=all sio.asn1 in in root to generate asn1c files

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### ISO 15693-3 NFC Writer
> ISO15693-3 NFC Writer An application for the Flipper Zero device that provides advanced handling of ISO 15693-3 compliant NFC tags (commonly found as ICODE SLIX). The application allows reading and writing memory data, managing block locks, and modifying special AFI and DSFID registers.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Metroflip *(by luu176)*
> An implementation of metrodroid on the flipper Metroflip is a multi-protocol metro card reader app for the Flipper Zero, inspired by the Metrodroid project. It enables the parsing and analysis of metro cards from transit systems around the worl...

**Usage:** Launch from the NFC/Apps menu. Hold your metro card against the back of the Flipper to read and decode transit data.

#### MFDesfire Auth *(by Gatorixx / NullPageGh0st)*
> Application for legacy authentication with Mifare Desfire cards

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### MFKey *(by @noproto)*
> MIFARE Classic key recovery tool

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### MFP Reader *(by Defensor7)*
> MIFARE Plus SL3 reader, dumper and emulator MFP Reader for Flipper Zero A standalone Flipper Zero application for reading, dumping and emulating MIFARE Plus SL3 smart cards. Implements the full MFP SL3 protocol over

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Mi Band NFC *(by LucaTomei)*
> NFC writer for Mi Band with 0xFF keys Mi Band NFC Writer v1.0

**Usage:** A comprehensive Flipper Zero application for managing NFC data on Xiaomi Mi Band devices with advanced features including automatic backups, detail... This application provides a complete toolkit for reading, writing, emulating, and verifying NFC data on Mi Band smart bands. It supports the full w...

#### MIFARE Classic Editor *(by TollyH)*
> Application for viewing and editing MIFARE Classic .nfc files

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Mifare Fuzzer *(by @spheeere98 @Sil333033)*
> App emulates Mifare Classic cards with various UIDs to check how reader reacts on them Flipperzero Mifare Fuzzer This app allows you to fake Mifare UIDs. It can emulate either UIDs or full cards from your Flipper's storage.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### MiZip Balance Editor *(by Téo Humeau)*
> Easily modify your MiZip tags balance MiZip Balance Editor for Flipper Zero This app main goal is to make it easy to modify a MiZip tag balance. If you want to contribute to make this app better, feel free to submit PR!

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Networking NFC + QR *(by DonJulve)*
> This app allows users to manage NFC, and QR codes directly on the Flipper Zero. Networking NFC + QR para Flipper Zero Esta aplicación para Flipper Zero permite emular una tarjeta NFC y mostrar un código QR simultáneamente en la pantalla.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC APDU Runner *(by SpenserCai)*
> Run APDU commands from script files

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Comparator *(by acegoal07)*
> A tool to check if a physical NFC card is the same as a saved digital one FlipperZero_NFC_Comparator This is a simple tool for checking NFC cards using a Flipper Zero device. It allows you to compare a stored NFC card against a physical card, checking for UID, UID length, and protocol.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Dict Manager *(by grugnoymeme a.k.a. 47LeCoste)*
> Advanced NFC dictionary management tool for Flipper Zero Flipper Zero Dictionary Manager A powerful application for your Flipper Zero that lets you manage, optimize, and organize your NFC dictionaries with ease. Handle mf_classic_dict.nfc and mf_classic_dict_user.nfc files, without con...

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Fuzzer *(by PentestSuite & barkandbite)*
> NFC protocol fuzzer for testing reader/tag robustness NFC protocol fuzzer with 11 profiles and 4 strategies for Flipper Zero NFC Fuzzer operates the Flipper Zero's NFC hardware in both listener (card emulation) and poller (reader) modes to send malformed, boundary-case, and mutation-based data to.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Keyboard *(by Patrick762)*
> Paste NFC card ids to keyboard input flipper-nfc-keyboard Paste NFC card ids to keyboard input

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Login *(by Play2BReal)*
> NFC based desktop login using USB HID or BLE HID (BLE exit fix, OFW compatible) A Flipper Zero app that uses NFC cards to automatically type passwords on your computer via USB HID or BLE HID.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Magic
> Application for writing to NFC tags with modifiable sector 0

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Maker *(by @Willy-JL)*
> Create NFC files for BT MACs, Contacts, Links, Emails, Phones, Text and WiFis

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Playlist *(by @acegoal07)*
> A Tool used to running through a list of NFC cards FlipperZero_NFC_Playlist: The idea behind this app is to allow for you to test multiple copies of NFC's at once as a bulk test When starting the app you are greeted by a select file option where you choose the playlist you wanna run.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Relay *(by leommxj)*
> NFC Relay app for flipper zero, relay NFCA APDU and response between two flipper. NFC Relay app for flipper zero, relay NFCA APDU and response between two flipper. configurable UART pin and baudrate (Right now is hardcored using 15/16 aka.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC Sniffer *(by Kyle Diller)*
> A simple app to log NFC commands This is a simple Flipper app that listens for the commands being sent by a NFC reader, and displays them to the Flipper's screen. The commands are also logged to the nfc_sniffer_logs folder in the ...

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC URL *(by Yasper De Jong)*
> Create NFC tags that direct you to a URL A Flipper Zero application that generates and emulates NFC tags that point to URLs. - Generate http(s), (s)ftp, mailto, etc. URLs - Save a list URLs for later use

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### NFC-Eink *(by RebornedBrain)*
> Application for emulation and writing to NFC Eink tags - This app allows emulation and writing of NFC Eink tags from Waveshare and Goodisplay - Image saved as one screen type can be then loaded to screen of another type Here is the list of displays wich app supports now:

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### OpenPrintTag *(by Houzvicka)*
> Read and write OpenPrintTag NFC tags for 3D printing materials OpenPrintTag Reader for Flipper Zero A Flipper Zero application to read and write OpenPrintTag NFC tags used for 3D printing filament spools. About OpenPrintTag

**Usage:** OpenPrintTag is an open-source NFC tag standard for 3D printing materials, developed by Prusa Research. It stores material information like brand, ... - Specification: https://specs.openprinttag.org/

#### Passport Reader *(by bettse)*
> eMRTD Reader Passy: Flipper Zero Passport Reader Passy is a tool for reading electronic passports (ePassports/eMRTDs) using the Flipper Zero device. It extracts and decodes data securely stored in modern passports, designed for hobbyists, researc...

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### SaFlip *(by Aaronjamt)*
> SaFlip Hospitality Tool for Flipper Zero Application to communicate with Saflok credentials and readers. Currently only works on Momentum Firmware, due to missing Date/Time screen support until OFW PR4261 is merged. Emulating cards

**Usage:** It can read and write all of the main Basic Access data and variable keys.

#### Seader *(by bettse)*
> A Flipper Zero application (aka "fap") that read credential from HID: iClass, iClass SE, Desfire EV1/EV2, and Seos using a HID SAM and UART adapter. Latest release on the App Catalog. File issues in GitHub.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Seos Compatible *(by bettse)*
> Seos compatible reader/emulator Flipper app for reading and emulating Seos®-compatible cards/fobs/mobile credentials.

**Usage:** The app uses all zero keys by default. If you'd like to use your own keys/ADF OID, use the format of the keys-example.txt to specify them, and pla... - No key files: If no key files are present, the app defaults to all zeros (00) for keys and an ADF OID of 030107090000000000 ("0.3.1.7.9.0.0.0.0.0").

#### Seos compatible *(by bettse)*
> Seos compatible reader/emulator Flipper app for reading and emulating Seos®-compatible cards/fobs/mobile credentials.

**Usage:** The app uses all zero keys by default. If you'd like to use your own keys/ADF OID, use the format of the keys-example.txt to specify them, and pla... - No key files: If no key files are present, the app defaults to all zeros (00) for keys and an ADF OID of 030107090000000000 ("0.3.1.7.9.0.0.0.0.0").

#### SLI Writer *(by Julienbxl)*
> Write SLIX .nfc files to SLI Magic cards (UID addressed mode) - REAL WRITE SLI Writer — Magic ISO15693 UID Writer I made a simple Flipper app to write magic ISO15693 tags with changeable UID using .nfc files. 📱 Update (10/04)

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Sonicare Head ID *(by mbirth.uk)*
> Philips Sonicare brush head NFC reader Sonicare Brush Head ID Each modern brush head for a Philips Sonicare contains a Mifare Ultralight NFC chip (NTAG213). The chip not only signals to the head unit what type of brush it is (to pre-select the recommended

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### T-Union Master (China) *(by SocialSisterYi)*
> A tool for query T-Union card and show travel logs T-Union Master（交通卡大师）是基于 flipper zero 平台用以查询交通联合卡综合信息的工具。 查询内容包括卡号、卡名、卡种、到期日期、余额等基础信息，充值、交易记录（10 条），交通工具、线路、站台等行程信息（30 条），查询方式为离线查询，无需蓝牙 wifi 等。 本应用使用 flipper zero 设备内建 NFC 外设及系统固件提供的 ISO/IEC 14443 (Type 4A) 协议栈与卡片通讯，应用层协议参考 EMV 标准以及交通运输部 JT/T 978.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### TonUINO Writer *(by Bastelsaal)*
> NFC card writer for TonUINO audio boxes TonUINO Writer for Flipper Zero NFC card writer application for TonUINO audio boxes. Create and manage NFC cards for the TonUINO DIY music player system. - ✅ Write TonUINO Cards - Configure NFC cards with folder, mode, and special settings

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### TuLlave *(by zqu4rtz)*
> This is a project to add the capability of reading TuLlave cards to the Flipper Zero device. TuLlave is a contactless smart card used to pay for public transport services in Bogota, Colombia. It operates on the MIFARE Classic protocol, which is supported by the Flipper Zero NFC module.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### UDECard *(by Alexander Hahn / hahnworks)*
> Flipper application to analyse student ID cards from the University of Duisburg-Essen. Flipper Zero application to analyse student ID cards from the University of Duisburg-Essen (Intercard) !Screen capture of UDECard The University of Duisburg-Essen utilises a system known as Intercard for the.

**Usage:** These are MIFARE Classic 1K cards, whose keys are readily available in the Flippers dictionary. However, the Flipper lacks any support for useful information extraction.

#### UID Brute Smarter *(by fbettag)*
> Enhanced UID brute-force with key management UID Brute Smarter 🔐 UID Brute Smarter is a security research tool for authorized penetration testing and access control system auditing. It provides advanced NFC key management and intelligent pattern detection to bui... 🔍 Key Management

**Usage:** - Key Browser: Browse and manage loaded keys with UID display.

#### UL-C Bruteforce *(by noproto)*
> Bruteforce Ultralight C authentication

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### UL-C Bruteforce Optimized *(by noproto)*
> Bruteforce Ultralight C authentication. Implements the brute-force attack described in paper *Section 10.1 Online Enhancements*

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### UL-C Relay *(by noproto)*
> Relay Ultralight C authentication

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### ULCFKey *(by noproto)*
> Crack counterfeit ULC cards

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### ULCFKey *(by noproto)*
> Crack counterfeit ULC cards. With tearing support.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### VB Migration Assistant *(by cyanic)*
> Makes transferring characters with VB Lab less cumbersome VB Lab Migration Assistant for Flipper Zero This app is designed to make transferring your characters from VB Lab and VBC Lab more convenient.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### VK Thermo *(by VivoKey Technologies)*
> Read temperature from your VivoKey Thermo via NFC. Multi-thermo tracking, history, and graph views.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

#### Weebo *(by bettse)*
> An NTAG215 parser, writer, emulator, remixer, duplicator A powerful Flipper Zero NTAG215 parser, writer, emulator, remixer, and duplicator, designed for enthusiasts who want to unlock the full potential of their NFC experience. google "key_retail.bin" and you should be able to find kt.

**Usage:** Launch from the NFC/Apps menu. Hold a card or tag near the Flipper's back to read/interact.

</details>

<details><summary><h3>📻 Sub-Ghz — 37 apps</h3></summary>

#### Chief Cooker *(by Denr01)*
> A simple app Your ultimate Flipper Zero restaurant pager tool. Be a _real chief_ of all the restaurants on the food court! This app supports receiving, decoding, editing and sending restaurant pager signals. Developed & compatible with Momentum firmware.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Chief Cooker *(by Denr01)*
> This app supports receiving, decoding, editing and sending restaurant pager signals. Your ultimate Flipper Zero restaurant pager tool. Be a _real chief_ of all the restaurants on the food court! This app supports receiving, decoding, editing and sending restaurant pager signals.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### ClayLoop *(by Bobby Gibbs)*
> Clay pigeon/skeet shooting controller. Transmits Sub-GHz .sub files repeatedly with configurable delay, duration, interval, repeats (1-Inf), and 1-4 file groups. LED/beep countdown, vibration, mid-cancel, persistent settings/paths, reset combo.

**Usage:** Queue up to 4 .sub files and transmit them in sequence with configurable delay, duration, interval, and repeat count. Features LED/beep countdown, ... 2. Copy it to your Flipper Zero SD card at apps/Sub-GHz/clayloop.fap

#### Enhanced Sub-Ghz Chat *(by @twisted-pear & @xMasterX & more in ReadMe)*
> Send text messages over Sub-GHz radio to another Flippers Enhanced Sub-GHz Chat This is a plugin for the Flipper Zero that reimplements the Sub-GHz chat feature that is available on the CLI. In addition it allows for basic

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Flipper Share *(by @lomalkin)*
> Direct file transfer between two Flipper Zeros via Sub-GHz radio. Send and receive files wirelessly without needing a computer or Bluetooth connection.

**Usage:** Launch on both Flippers. One selects a file to send, the other waits to receive. Transfer happens over Sub-GHz radio — keep devices within range.

#### Frequency Analyzer w External *(by xMasterX)*
> Cut from unleashed FW subghz app directly, with external module support (no path isolate, get random signals from space!)

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### FRSScan *(by thecybermind)*
> Scans common FRS and GMRS frequencies and plays them over the speaker so you can hear them. Press the center button to lock. Flipper Zero FRSScan Scans common FRS and GMRS frequencies and plays them over the speaker so you can hear them. Press the center button to lock.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Genie Door Recorder *(by jamisonderek)*
> This application extracts the codes from a Genie garage door remote into a .GNE file. It also plays back a .GNE file to a Genie garage door opener.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### HAM Scanner *(by Clawzman)*
> HAM Scanner for Flipper 📡 Flipper Zero Ham Radio Scanner A lightweight SubGHz radio scanner, designed to behave like a scanner for common radio bands. 🆕 What’s New in v1.1

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Marmalade *(by RocketGod-git)*
> Frequency and preset adjustable subghz radio frequency marmalade for Flipper Zero. Exercise Caution and Follow Your Local Laws! RF Marmalade App by RocketGod ☠️📡 This RF Marmalade App for the Flipper Zero, made by RocketGod, is a powerful tool for marmalade across multiple radio frequencies and.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Music to Sub-GHz Radio
> Converts Flipper music files (.FMF and .TXT) into Sub-GHz files (.SUB). Music to Sub-GHz Radio The Music to Sub-GHz Radio application converts Flipper Music Files (.FMF) into a RAW .SUB file format that can be transmitted over the Sub-GHz radio! The Flipper Zero can receive the music and pla...

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### OpenShock *(by OpenShock Team)*
> Simple app to control shockers from your flipper OpenShock Flipper Zero A Flipper Zero application for controlling OpenShock compatible shockers over 433 MHz. - Transmit — send commands to any supported shocker by configuring model, ID, channel, command type, and intensity.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### POCSAG Pager *(by @xMasterX & @Shmuma)*
> App can capture POCSAG 1200 messages on CC1101 supported frequencies. Flipper POCSAG Receiver plugin Protocol implementation made by @Shmuma + 512, 2400 speeds support by @htotoo Plugin based on Weather Station from OFW

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### ProtoPirate
> Decode car key fob signals from Sub-GHz _for Flipper Zero_ ⚠️ Warning: Important Security & Project Update Read message by following link below:

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### ProtoPirate *(by RocketGod-git)*
> Decode car key fob signals from Sub-GHz _for Flipper Zero_ ⚠️ Warning: Important Security & Project Update Read message by following link below:

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### ProtoView *(by @antirez & (fixes by @xMasterX))*
> Digital signal detection, visualization, editing and reply tool

**Usage:** ProtoView is a digital signal detection, visualization, editing and reply tool for the Flipper Zero. The Flipper default application, called Subghz... !ProtoView screenshot raw signal

#### Radio Scanner *(by @RocketGod-git)*
> Scanner for Flipper Flipper-Zero-Radio-Scanner Scans frequencies available to the CC1101 and plays them over the speaker so you can hear them. - Does NOT play "FM radio stations" since those frequencies are not available.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Remote for CaiXianlin *(by Anty)*
> Remote control for CaiXianlin shock collar via Sub-GHz radio CaiXianlin Shock Collar Remote for Flipper Zero A Flipper Zero application to control CaiXianlin shock collar. WARNING: This application is intended for educational and research purposes only.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Restaurant Pager *(by leedave)*
> This app triggers restaurant pagers in a brute force manner, useful to test if devices are still functional. Flipper Zero Meal Pager Tool This app triggers restaurant pagers in a brute force manner, useful to test if devices are still functional. Supported Pagers

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Rolling Flaws
> Rolling code receiver (version 1.5), used to learn about rolling code flaws. Watch video at https://youtu.be/gMnGuDC9EQo Rolling Flaws (version 1.5) by @CodeAllNight.

**Usage:** YouTube demo of using Rolling Flaws application. The video shows how to use the application to simulate a receiver that has a Replay attack flaw, ... - Discord invite: https://discord.com/invite/NsjCvqwPAd

#### RS41 Tracker *(by OK1CHP / yeckel)*
> RS41 radiosonde receiver & decoder RS41 Tracker — Flipper Zero external app Receives and decodes Vaisala RS41 radiosondes using the Flipper Zero's built-in CC1101 sub-GHz radio — or an external CC1101 module on the GPIO header.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### ShapShup *(by derskythe)*
> A simple application for viewing SubGhz RAW files in the form of a signal level as it is shows in SubGHz RAW read mode. ShapShup App for Flipper Zero A simple application for viewing SubGhz RAW files in the form of a signal level as it is shows in SubGHz RAW read mode.

**Usage:** Also you can walk through the file with forward and backward buttons

#### Spectrum Analyzer *(by @xMasterX & @theY4Kman & @ALEEF02 (original by @jolcese))*
> Displays a spectrogram chart to visually represent RF signals around you. This application allows you to plot a chart showing the relationship between amplitude and frequency, detecting nearby signal sources. If there is a nearby source broadcasting a signal at the.

**Usage:** The app has the following controls: - The OK button adjusts the width of the spectrum.

#### Sub Analyzer *(by RocketGod)*
> Analyze SubGhz .sub files to extract all signal properties Flipper Zero SUB Analyzer Extract as much signal data from a Flipper Zero .sub file as possible

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub Duplicate Finder *(by Endika)*
> Identify and clean up duplicate .sub files in storage Sub-GHz Duplicate Finder for Flipper Zero An application for Flipper Zero to identify, manage, and clean up duplicate .sub files from Sub-GHz storage. Development Setup

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Bruteforcer
> SubGHz BruteForcer Application for Flipper Zero SubGhz BruteForcer from Unleashed Firmware This software is for experimental purposes only and is not meant for any illegal activity/purposes.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Jammer Detect *(by PentestSuite / barkandbite)*
> Detects sustained RF carrier waves indicating SubGHz jamming attacks

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Playlist *(by @darmiel)*
> App works with list of sub-ghz files from .txt file that contains paths to target files.

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Playlist Creator *(by coolerUA)*
> App for creating or editing SubGhz playlists SubGhz Playlist Creator A Flipper Zero application for creating and managing playlists of .sub files. - Create new playlists

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Playlist Creator *(by coolerUA)*
> App for creating or editing SubGhz playlists SubGhz Playlist Creator A Flipper Zero application for creating and managing playlists of .sub files. - Create new playlists

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Remote *(by gid9798 xMasterX)*
> SubGhz Remote, uses up to 5 .sub files

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Scheduler *(by Patrick Edwards)*
> Send a Sub-GHz signal repeatedly at a given interval. SubGHz Scheduler A Flipper Zero app to send SubGHz signals at a given interval. Individual .sub or playlist .txt files can be used. Schedule intervals selectable:

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Sub-GHz Spectrum *(by PentestSuite & barkandbite)*
> Real-time Sub-GHz spectrum analyzer with waterfall display SubGHz Spectrum ____ _ ____ _ / ___\| _ _\| \|__ / ___\|\| \|__ ____

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### SubGhz Toolkit *(by RocketGod)*
> Extract SubGhz protocol implementation details and decrypt KeeLoq keys RocketGod's SubGHz Toolkit Reverse engineer Flipper Zero SubGHz protocols and Keeloq Manufacturer Codes

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### TPMS Reader *(by wosk)*
> Reads Tire Pressure Monitoring System (TPMS) sensors wirelessly. Displays tire pressure and temperature data from nearby vehicles' TPMS sensors using Sub-GHz radio.

**Usage:** Launch from Sub-GHz menu. Walk near vehicles to pick up TPMS signals. The app decodes and displays pressure/temperature readings from detected sensors.

#### TPMS Reader *(by @wosk)*
> Use SubGHz and RFID to read or activate TPMS sensors

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

#### Weather Station
> Receive weather data from a wide range of supported Sub-1GHz remote sensor

**Usage:** Launch from the Sub-GHz/Apps menu. Use d-pad to navigate options and OK to select/capture.

</details>

<details><summary><h3>⚡ Gpio — 87 apps</h3></summary>

#### 24Cxx Programmer *(by Dr.Mosfet)*
> EEPROM 24Cxx programmer via I2C with read, write, erase and dump/restore options. 🔧 24cxxprog - EEPROM 24Cxx Programmer A Comprehensive EEPROM Programmer for Flipper Zero Menu główne z operacjami (Odczyt, Zapis, Kasowanie)

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [A33] Flipper Blackhat *(by machinehum)*
> Control of the flipper blackhat device The Flipper Blackhat app is used to control the blackhat.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [BMI/LSM] Air Mouse
> — Piper, a.k.a. Pez Naturally, the quote above applies to Flipper as well. The app allows you to turn your Flipper into a USB or Bluetooth air mouse (you do need an extra module, see the Hardware section below)...

**Usage:** Hold the Flipper in your hand with the buttons pointing towards the screen;

#### [CAN] CAN Commander
> GPIO-based tool that interfaces with external hardware modules.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [DIAG] GPIO Badge
> This is an app for the GPIO DIAGNOSTICS BOARD FOR FLIPPER ZERO by MakeItHackin.

**Usage:** - Left/Right buttons to change speed of the effect. - Up/Down buttons to change effect.

#### [GB] Pokemon Trading *(by Kris Bahnsen, Esteban Fuentealba, ProteanReverie, Darryn Cull)*
> Pokemon exchange from Flipper Zero to Game Boy, supports Generation I & II non-Japanese games Pokemon Trade Tool for Flipper Watch it in Action The video below trades a Bulbasaur from the Flipper to a Game Boy Color with Pokemon Silver. The Game Boy trades its Cyndaquil.

**Usage:** - Instructions for Use - Customizing Pokemon

#### [GPIO] 7-Segment Output *(by jamisonderek)*
> Control a 7-segment display with GPIO pins GPIO 7-Segment Output This is a GPIO push-pull output demo application for driving a 7-segment display on the Flipper Zero. The goal of this project is to show application developers how GPIO works for push-pull output...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Controller
> gpio_controller A visual tool to control the general purpose pins of the Flipper Zero

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Explorer *(by dun-crop)*
> The most complete app to start exploring the GPIO functionalities Just run the app and start exploring. It has: - Digital rgb led, for which you must have digital rgb led and at least 3 100 Ohm resisters or greater or the led might brown out.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Longwave Clock *(by @m7i-org)*
> Decode or demonstrate long wave time signals This is a Flipper Zero app to receive and decode, or simulate, multiple time signal broadcasts with different protocols and time formats. For receiving via GPIO, an inexpensive receiver connected t... Protocol support DCF77 (Europe, Germany)

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Oscilloscope *(by anfractuosity)*
> Oscilloscope application - apply signal to pin 16/PC0, with a voltage ranging from 0V to 2.5V and ground to pin 18/GND To compile and install Alternatively the binary can now be installed from https://lab.flipper.net/apps/flipperscope or the Flipper Mobile App.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Reader (aureli1c) *(by @aureli1c)*
> Read GPIO pins states, and display them on the screen flipperzero_GPIO_read Plugin to read the GPIOs on the Flipper Zero.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Reader (biotinker)
> flipperzero-gpioreader This is a fork of the gpio app built into the flipper, with added functionality to read GPIO inputs. Supports pulling high or low.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] RGB LED
> A simple application to control RGB LED's (WS2812, etc) via GPIO

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Sentry Safe *(by @H4ckd4ddy)*
> App exploiting vulnerability to open any Sentry Safe and Master Lock electronic safes without pin code. Sentry Safe - Flipper Zero Plugin Plugin exploiting vulnerability to open any Sentry Safe or Master Lock electronic safes. 🔓 Based on the vulnerability described here

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Signal Generator
> Control GPIO pins to generate digital signals

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Timelapse *(by Aurelius Rosenbaum)*
> Simple intervalometer/timelapse app english version below Blog post: theageoflove.ru TG: t.me/scuko_bled

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GPIO] Wire Tester *(by @unixispower)*
> Beeps if a wire is continuous Flipper Wire Tester This is a simple Flipper Zero app that beeps if a wire is continuous. favorite firmware. Run ./fbt launch APPSRC=applications_user/wire_tester to

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [KT0803] FM Transmitter
> flipperzero-kt0803-transmitter Use KT0803-type chips to transmit signal from module AUX port FM Transmitters are illegal if they are too powerful. Use in well-shielded environment or do not use it for too long.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [LED] Flashlight *(by @xMasterX)*
> Enables 3.3v on pin 7/C3 when you press Ok and leaves it on when you exit app Simple Flashlight special for @Svaarich by @xMasterX Icon by @Svaarich Connection diagram picture by @Kuronons

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [MAG] MagSpoof *(by Zachary Weiss)*
> Enables wireless transmission of magstripe data MagSpoof for the Flipper Zero MagSpoof-ing, as coined / invented by Samy Kamkar, is the use of an electromagnet to emulate the data on a traditional magstripe card — wirelessly!

**Usage:** This app ports the original MagSpoof project to the Flipper Zero, adding file management and emulation controls, along with some experimental featu... The GPIO module is a simple electromagnet consisting of an H-bridge, a capacitor, and a coil. One can build the pictured circuit, or purchase a Mag...

#### [Mx2125] Step Counter *(by 47lecoste a.k.a. grugnoymeme)*
> Step Counter/Pedometer using Memsic2125 module. Flipperzero-StepCounter-fap This is a simple StepCounter/Pedometer for FlipperZero using a Memsic2125 module. !Main menu view

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [RC2014] ColecoVision
> RC2014 ColecoVision Controller for Flipper Zero A Flipper Zero application and [RC2014] module allowing the Flipper to be used as a controller for ColecoVision games on Running ColecoVision Games on the RC2014

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [SPI] Terminal *(by Jan Wiesemann)*
> A Terminal Application for the SPI interface

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [TEA5767] FM Radio *(by coolshrimp)*
> FM Radio App FM Radio - Flipper Zero Application for flipperzero device to control radio boards. Supported boards

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [UART] UART Terminal *(by @cool4uma & @rnadyrshin & (some fixes by @xMasterX))*
> Control various devices via the Flipper Zero UART interface. UART Terminal for Flipper Zero Flipper Zero app to control various devices via UART interface. - Reading from UART in text or hex mode

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [WII] EC Analyser *(by BlueChip)*
> Wii Extension Controller Protocol Analyser [FlipperZero] Wii Extension Controller Protocol Analyser This Protocol Analyser offers a full Test and Calibrate system for Wii Extension Controllers.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Air Stats *(by thevan4)*
> CO2 + temperature/humidity/pressure monitor CO2 + climate monitor for Flipper Zero. Reads CO2 (MH-Z19B/C) and temperature/humidity/pressure (BME280, DHT22, etc.) through GPIO. Two sensors at once. Supported (not tested) Inherited from unitemp. Should work — report issues if not.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Analog Output *(by jamisonderek)*
> Analog output demo. Uses MCP4725 on I2C. Uses pin A7 PWM with 10uf cap+10K resistor. Input on pin C3

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Atari SIO Emulator *(by cepetr)*
> Atari 8-bit computer SIO peripheral emulator Atari SIO Peripheral Emulator for Flipper Zero

**Usage:** sio2flip is a Flipper Zero application that emulates SIO peripherals for Atari 8-bit computers. The project is still in its early stages, but it al... - Floppy drive emulation

#### Battery Checker *(by Programistich)*
> Simple App for check battery voltage ADC for Flipper with a reference voltage of 2.5V Suitable for measuring battery voltage up to 2.5V Using an external divider it is possible to change the measuring range

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### BunnyConnect *(by C0d3-5t3w)*
> USB serial communication tool A Flipper Zero application named after the desire to connct to a BashBunny, that bridges serial communication with USB HID keyboard functionality, enabling seamless text input and command execution...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### C5 Lab *(by Lab_Team)*
> WiFi Deauther controller for Lab_C5 module via UART Flipper Companion App The Flipper Zero companion app (Lab_C5.fap) mirrors the ESP32-C5 CLI workflows (scan, results, attacks, wardrive, Sniffer Dog, portal control) so you can steer the board from the handheld UI.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### CAN Transceiver [MCP2515] *(by Chainmanner)*
> This application allows a Flipper Zero to use an MCP2515 CAN transceiver to send and receive messages on a standard CAN bus. That includes those found in vehicles.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### CANBUS *(by ElectronicCats)*
> This app allows you to interact with CAN Bus networks. Now you can interact with CAN Bus transmissions using the Flipper Zero.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### CANBUS ATTACK *(by jorgepnaranjo25)*
> Flipper Zero application for performing basic security testing on CAN Bus networks. It was developed as part of a university cybersecurity research project and is intended for educational and ethical use only.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Canon Intervalometer *(by TravisCG)*
> Canon intervalometer for Flipper Zero. Flipper Zero intervalometer + 2 LTV4N35 octocopuler + 2 220 Ohm resistor

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Coffee EEPROM *(by wh00hw)*
> Allows you to pentest certain vending machine Coffee-EEPROM-FAP - Vendors Still Allow You to Hack Their Vending Machine - Read credit on load

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Continuity Tester *(by Ben Allen)*
> flipper-zero-apps A collection of applications I made for Flipper Zero

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### DelfiRTL *(by gorebrau)*
> This project utilizes the RTL8720DN WiFi module in combination with the Flipper Zero to perform various WiFi penetration testing attacks on 2.4GHz and 5GHz networks.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Digimon F-Com *(by TylerWilley)*
> A Flipper Zero app that allows the flipper to communicate with Digimon V-Pets. Currently tested: DM20, DMX, PenZ, DMC unsupported Flipper Zero F-Com A Flipper Zero app that allows the flipper to communicate with Digimon V-Pets. Currently tested devices:

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### E220 LoRa Configurator *(by crackerjacques)*
> E220-900T22S(JP), a configurator for LoRa modules used only in Japan. Flipper Zero E220 LoraModule Configurator E220-900T22S(JP), a configurator for LoRa modules used only in Japan. I hope to support other modules in due course.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Encoder Reader *(by Engineegor)*
> This is a simple app for flipper that can read incremental quadrature encoders that show absolute and relative coordinate. This is a simple app for flipper that can read incremental quadrature encoders and show absolute and relative coordinate. Configutarion menu - Start: run reader

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Evil BW16 Controller *(by dagnazty)*
> WiFi Deauther controller for Evil-BW16 module via UART Evil BW16 Controller - Flipper Zero App A Flipper Zero application for controlling the Evil-BW16 WiFi deauther module via UART. Category: GPIO/UART Applications

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Fencing Test Box *(by Aaron Janeiro Stone)*
> Test box for assessment of fencing blades and body wires. Fencing Testbox This is a simple emulator of a fencing testbox for the Flipper Zero. The main menu is shown below:

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Flipagotchi *(by Matt-London)*
> This program will interface the pwnagotchi with the flipper. This will be accomplished using custom code on the pwnagotchi's end to give the flipper simple rendering instructions over UART. Pwnagotchi-Flipper interface

**Usage:** This program will interface the pwnagotchi with the flipper. This will be accomplished using custom code on the pwnagotchi's end to give the flippe... This project is broken down into two parts: flipagotchi and pwnzero

#### FlipGemini *(by JBlanked)*
> Chat with Google's Gemini AI on your Flipper Zero Chat with Google's Gemini AI on your Flipper Zero! Original idea from https://github.com/d4rks1d33/Gemini-Flipper - Wi-Fi Developer Board, BW16, Raspberry Pi, or ESP32 device flashed with FlipperHTTP v2.1.4 or higher:.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Flippy Temp *(by n0ciple)*
> A temperature logging app that works with Flipy Sensor Board Flippy Sensors 🐬 flippy sensor is a WIP multi-sensor board for the flipper zero. It includes a SHT30 for temperature and humidity, a LSM6DS3TR-C for gyro and accelerometer and a PA1010D for GPS. A first batch is cu...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipTelegram *(by JBlanked)*
> Flipper Zero Telegram Client Flipper Zero Telegram Client - WiFi Developer Board, Raspberry Pi, BW16, or ESP32 device flashed with FlipperHTTP v2.1 or higher: https://github.com/jblanked/FlipperHTTP - 2.4 GHz or 5 GHz WiFi access point

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FM Radio w Vol [TEA+PT] *(by coolshrimp & pchmielewski1)*
> FM Radio app for controlling an FM receiver and (optionally) an external I2C volume controller flipperzero-radio-with-volume-control Flipper Zero external app for TEA5767 FM reception, PT2257/PT2259-S audio control, and RDS decoding from the TEA5767 MPXO path. Table of Contents

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FM Transmitter KT0803 *(by goosedev72-projects)*
> Use KT0803-type chips to transmit signal from module AUX port flipperzero-kt0803-transmitter Use KT0803-type chips to transmit signal from module AUX port FM Transmitters are illegal if they are too powerful. Use in well-shielded environment or do not use it for too long.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### GPIO with I2C
> THIS IS THE SAME AS THE GPIO APP BUT WITH I2C ADDITIONS FROM [I²C-Scanner #1431 (By GitChris3004)](https://github.com/flipperdevices/flipperzero-firmware/pull/1431) and [read EEPROM of SFP Modules using I2C (By marcusju)](https://github.com/Lambda/flipperzero-firmware-wPlugins/pull/198)

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### GS1 Parser [UHF] *(by Kyle Diller)*
> This is a GS1 UHF payload parser based on version 2.2 of the Tag Data Standard. This currently only supports parsing SGTIN formatted EPCs. Supporting other EPC formats is possible, but not currently planned. Req: U107 or YRM100.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### HC-11 Modem *(by Giraut)*
> HC-11 wireless modem HC-11 wireless modem emulator for the Flipper Zero Flipper Zero app send data to / from a HC-11 wireless RF UART communication module. See here, here, here, here or here for a few distributors of the HC-11 module around the world.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### I2C Explorer *(by 4mb3rz)*
> This is a simple tool for interactive exploration of an I2C bus topology and modification of device register states. Flipper Zero I2C Explorer This is a simple tool for interactive exploration of an I2C bus topology and visualization and modification of device register

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### INA Meter *(by cepetr)*
> Application for reading TI INAxxx sensors. INA Meter for Flipper Zero INA Meter is an application for Flipper Zero that allows you to read I2C-connected current/power monitors from Texas Instruments. Supported Sensors:

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Lab401/DigiLab *(by Lab401 & tixlegeek)*
> Lab401's Digilab companion app. Lab401's DigiLab by tixlegeek is a Flipper Zero® accessory that introduces a new way to explore electronics. Designed to be the first tool you reach for when you're developing, debugging, tinkering...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Lab401/Light Messenger *(by Lab401 & tixlegeek)*
> Lab401's LightMessenger companion app. LAB401's LightMessenger by tixlegeek is an additional hardware module designed for the Flipper Zero. It allows users to display messages and images in the air using POV on an array yof RGB Leds. Wi...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### LAN Tester *(by Dok2d)*
> Portable LAN tester & security analyzer: ARP, ping, DHCP, LLDP/CDP, port scan, SNMP, NTP, IPMI, RADIUS, TFTP, PXE boot/download, 802.1X, VLAN, rogue DHCP/RA detection via W5500 Flipper Zero LAN Tester (W5500) Turn your Flipper Zero + W5500 Lite module into a professional-grade portable LAN tester.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### LaserCat *(by DroWez)*
> Application to turn on a GPIO laser. Привет народ! Рад представить вам мое первое приложение для Flipper Zero. Все с чего-то начинают, и я решил начать с простого, но забавного проекта — лазера для кошки. Это приложение включает лазер... Скриншоты приложения Как компилировать приложение

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### LD2410 Human Detector *(by Oscar Rodriguez)*
> An application that turns your Flipper Zero into a sophisticated human presence detector using the HLK-LD2410 mmWave sensor. LD2410 Human Detector for Flipper Zero An application that turns your Flipper Zero into a sophisticated human presence detector using the HLK-LD2410 mmWave sensor.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Logic Analyzer *(by g3gg0)*
> Use flipper as Openbench Logic Sniffer (ols) logic analyzer in PulseView flipper-logic-analyzer Source: https://github.com/g3gg0/flipper-logic_analyzer I'm in the process of bringing this in line with the latest flipper firmwares.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### LoRa Relay *(by ElectronicCats)*
> LoRa Relay App. This is a LoRa sniffer using Electronic Cats flipper add-on

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### LoRa Sample *(by ElectronicCats)*
> LoRa Sample App. This is intended to be used as a starting point for new applications with one primary screen. !GitHub release (with filter) !GitHub actions Flipper LoRa Relay App :dolphin:

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### LoRA_Terminal
> LoRA Terminal for Flipper Zero Flipper Zero app to control Lora Breakout board. - Read log and command output by uart

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### LoRadar *(by baptleduc)*
> LoRadar - The Flipper Application to localize stratospheric ballon via LoRa LoRadar - The Flipper Application to localize stratospheric ballons via LoRa LoRadar is an app that allows users to localize stratospheric balloons using their Flipper Zero and LoRa modules.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### MODBUS *(by ElectronicCats)*
> A Flipper plugin with helpful Modbus RTU features, you will be able to read and write data into a Mobus RTU network. Requires the Electronic Cats Flipper Addon Modbus. !GitHub release (with filter) !GitHub actions flipper-rs485modbus

**Usage:** A Flipper plugin with helpful Modbus RTU features, you will be able to read and write data into a Mobus RTU network. Please visit the wiki for detailed information on how to use the app.

#### Moisture Sensor *(by smallwat3r)*
> Read values from a capacitive moisture sensor v1.2 Moisture Sensor Readings for Flipper Zero A Flipper Zero application that reads values from a Capacitive Moisture Sensor v1.2. !Flipper Zero Moisture Sensor

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Noptel LRF Sampler *(by Giraut)*
> Noptel LRF rangefinder sampler Noptel LRF rangefinder sampler app for the Flipper Zero Flipper Zero app to get range measurements from a Noptel LRF rangefinder. - Rangefinding in all modes

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### PIR HC-SR501 Alarm *(by ku6a-cj)*
> Simple proximity alarm app for Flipper Zero using HC-SR501 PIR sensor. Flipper Zero HC-SR501 PIR Proximity Alarm App Simple proximity alarm app for Flipper Zero using HC-SR501 PIR sensor.

**Usage:** Triggers vibration, LED alerts, and screen notifications. Features 3-second alarm duration with blinking feedback. - Motion Detection: HC-SR501 PIR sensor detects human motion within ~7m range

#### Pokemon Trading *(by EstebanFuentealba & kbembedded)*
> Pokemon exchange from Flipper Zero to Game Boy for Generation I (Pokemon Red, Blue, Yellow). Flipper Zero emulates a Slave Game Boy connected to a Game Link Cable to be able to exchange any Pokemon from the First Generation (Red, Blue, Yellow) to a real Game Boy.

**Usage:** - Instructions for Use - Customizing Pokemon

#### SD SPI *(by Gl1tchub)*
> SD SPI Lock Management Flipperzero-SD-SPI Flipper Zero FAP for Lock and Unlock SD card / Micro SD card through SPI protocol (CMD42). Pinout Without Flipper Zero SDBoard the SD card it must be connected as in the table below

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### SERMA CAN-FD-HS *(by serma-safety-security)*
> USB-to-CAN bridge that turns your Flipper Zero into a CAN bus interface. Compatible with Linux can-utils and slcan driver for automotive diagnostics and CAN bus analysis.

**Usage:** Requires the SERMA CAN FD hardware board plugged into GPIO. Connect Flipper to PC via USB, launch the app, then use can-utils on your computer to communicate with the CAN bus.

#### Servo Tester 2 *(by mhasbini)*
> App uses A7 pin for servo PWM output. Servo tester app for Flipper Zero This app aims to replicate the behavior of a standard RC servo) tester, by replicating a PWM RC servo signal. You can read more about it in this blog! - Connect the Servo PWM input to A7 Flipper Zero in.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Servotester *(by Alexander Semion)*
> Application uses A7 pin for servo PWM output. Has different modes: Manual, Center, Auto. Flipper servotester application Application uses A7 pin for servo PWM output. Has different modes: Manual, Center, Auto. Go to the releases, download apps.zip and extract it in your SD card.

**Usage:** - Follow official instructions

#### SI4713 Tuner *(by rullinoiz)*
> Basic Adafruit SI4713 tuner for the Flipper Zero

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### SPI Flash Dump *(by PentestSuite & barkandbite)*
> Read SPI NOR flash chips via GPIO for firmware extraction ____ ____ ___ _____ _ _ / ___\|\| _ \_ _\| \| ___\| \| __ _ ___\| \|__ \___ \\| \|_) \| \| \| \|_ \| \|/ _ / __\| '_ \

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Spotify Remote *(by Ben Russell)*
> Allows user to control spotify playback from flipper zero Flipper Zero Spotify Remote An app for the Flipper Zero that lets you control Spotify playback using the Flipper Zero and WiFi Devboard This app is included in the Lambda Firmware, or you can build it from source using ufbt or fbt

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### StroboMeter *(by L. Jakschik)*
> A stroboscope application to measure the frequency of cyclical motion. Flipper StroboMeter Flipper StroboMeter is an application designed to measure the RPM (revolutions per minute) of a rotating object using the Flipper Zero device. By connecting an LED to Pin 4(A4) and GND and setting ...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Tesla Mod *(by hypery11)*
> Tesla CAN bus toolkit — FSD region-gate bypass, nag killer, BMS dashboard, and more. Requires CAN Add-On. English \| 繁體中文 \| 简体中文 Tesla Mod for Flipper Zero

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### u-blox GPS *(by liamur)*
> App to display and log data from u-blox GPS modules over I2C !Flipper Zero connected to a u-blox GPS, running the u-blox app Flipper Zero app to read from a u-blox GPS over I2C. This app can display data, log a path to a KML file, and sync the Flipper's time to

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### UART Sniff *(by PentestSuite / barkandbite)*
> UART protocol sniffer — captures bytes on GPIO pins 13/14

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### WAV Recorder *(by scrolltex)*
> WAV Sound Player app

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### WHC SWIO Flasher *(by Vojtech Suk)*
> A WHC CH32V003 debugger/flasher tool

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Wiegand Reader *(by jamisonderek)*
> This application supports W4, W8, W24, W26, W32, W34, W37, W40 and W48 formats and can be used to test Wiegand readers and keypads. This application supports W4, W8, W24, W26, W32, W34, W37, W40 and W48 formats. This application can be used to test Wiegand readers and keypads.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### WS2812B LED Tester *(by jamisonderek)*
> WS2812B LED Tester App. This is intended to test that WS2812B LEDs are functioning correctly. WS2812B LED Tester This application is used to test WS2812B LEDs. You can connect the WS2812B LEDs to any available GPIO pin.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### YuriCable Pro Max *(by arag0re.eth && PooruTorie)*
> SWD/DCSD-Cable App implemented for the Flipper Zero !GitHub Repo stars !GitHub watchers !X (formerly Twitter) Follow

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>

<details><summary><h3>🔴 Infrared — 29 apps</h3></summary>

#### Cross Remote *(by Leedave)*
> One-Click, sends multiple commands Flipper Zero Cross Remote - Infrared working - SubGhz working

**Usage:** - IR Timing features working This app combines commands used in IR and SubGhz into playlists that can be run with one click

#### Fire String *(by Ryan Aboueljoud)*
> Generate truly random strings using IR noise as entropy. 🔥 Fire String 🔥 Generate truly random strings from the Flipper Zero using its IR sensor. Capture the infrared bursts from sparking a flame as an unpredictable entropy source. The app also allows saving and loading...

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Flame RNG *(by OrionW06)*
> An RNG intended for use with flames and other IR sources. Inspired by this repo I have implemented a similar functionality entirely in the flipper zero It uses the IR sensor on the Flipper Zero to gather entropy from IR sources (such as a lighter) to seed a random number generator.

**Usage:** I am also NOT responsible if you generate insecure keys using this as a source of entropy!

#### FlipIRFreq *(by jsammarco)*
> Transmit IR in carrier or low-frequency pulse modes with burst or continuous output. FlipIRFreq is a Flipper Zero external app for sending either a high-frequency IR carrier or a low-frequency IR pulse train with a user-selected frequency, duty cycle, burst length, and output pin.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Flipper Tag *(by thejudge156)*
> Laser Tag app for the Flipper Zero compatible with the Nerf Phoenix LTX laser tag guns. Flipper Zero FAP FlipperTag Use this code as a foundation for developing Flipper Zero Applications, change the contents of this file when developing a new project - Different Scenes / Views

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Hard Hat Brigade IR *(by Anomalous68)*
> Type message, then point at reciever and press enter. The slight pause is when the message is transmitting, once Message sent appears, the hat should display it. Hard Hat Brigade Flipper Zero App Flipper Zero app.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Hitachi AC Remote
> Hitachi Air Conditioner remote controller Hitachi AC Remote Hitachi AC remote controller for Flipper Zero. Emulates the PC-LHxx series remote controller, specifically the PC-LH3 series, and also the PC-LH6Q variant that can be found on AliExpress and similar places.

**Usage:** See the manual for detailed usage. Based on Mitsubishi AC Remote by @achistyakov and Midea AC Remote by @xakep666.

#### HTW AC Remote *(by sokogen)*
> IR remote control for HTW air conditioner HTW AC Remote for Flipper Zero Flipper Zero application for controlling HTW air conditioners via infrared. - Full IR Protocol Support - Complete implementation of HTW AC IR protocol

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### InfraFi *(by amd989)*
> Transmit WiFi credentials via IR to a Linux server Transmit WiFi credentials from a Flipper Zero to a Linux server using infrared. Point, press Send, connected. Built for headless servers (NAS boxes, Intel NUCs, etc.) where typing WiFi passwords is painful or impossible.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### IR Blaster *(by Mateusz Szafraniec)*
> IR Blaster for Flipper Zero. Flipper Zero - IR Blaster IR Blaster for Flipper Zero This application allows you to guess the IR remote control codes for devices.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### IR Decoder *(by prplecake)*
> Shows the same address and command values as the standard IR app, but without any of that silly remote nonsense! Also shows a value that may be usable with lirc. My Flipper Zero stuff. (Not to be confused with any of my projects pertaining to my cat, Flipper).

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### IR Intervalometer *(by @Nitepone)*
> Intervalometer for Pentax, Canon, Nikon, and Sony cameras. Uses IR shutter release. An Intervalometer App For Flipper Zero This is a simple configurable valometer app for Sony cameras. It provides a configurable shutter timer and uses the IR blaster to trigger the camera

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### IR Remote *(by @Hong5489 & @friebel & @d4ve10)*
> Bind any IR remote button to each button on flipper d-pad, provides another way to use flipper as IR remote. Alternative Infrared Remote for Flipperzero

**Usage:** It is a plugin like UniversalRF Remix but for infrared files. I do this plugin for convenience, because the main IR app need to navigate for differ... It supports short press and long press input for different ir remote buttons. Tested on the unleashed firmware version unlshd-023

#### IR Scope *(by @kallanreed)*
> App allows to see incoming IR signals.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### IR Signal Generator *(by Sacriphanius / temboohms68)*
> Advanced IR Signal Generator with RF Spectrum and External IR Support 🎛️ Flipper-Zero-IR-Signal-Generator - Generate Infrared Signals Easily

**Usage:** The Flipper Zero IR Signal Generator allows you to create and emit infrared signals, making it a valuable tool for various applications. This is pa... ⚙️ Requirements

#### IR Transfer *(by WBP20)*
> File transfer over infrared between two Flipper Zero devices using the NEC protocol for both control and data. IR Transfer (Flipper Zero) — NEC Only File transfer over infrared between two Flipper Zero devices using the NEC protocol for both control and data.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Lasko 2519 Timer *(by LN4CY)*
> Automates Lasko 2519 fans with a configurable ON/OFF timer cycle. Lasko 2519 Timer for Flipper Zero An automated timer application for the Lasko 2519 Tower Fan (and compatible models). This app uses the Flipper Zero's IR blaster to cycle the fan power ON and OFF at configurable intervals.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### LIDAR Emulator *(by regaly)*
> This app can be used to emulate infrared signals of different LIDARs. Enter the submenu and a LIDAR gun then press and hold OK button to transmit. It will transmit until you hold the OK button. Blue LED lights during the transmit.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Midea AC Remote
> Midea Electric Air Conditioner remote control Midea AC Remote Midea Electric AC remote control for Flipper Zero. Compatible with RG10B(B)/BGEF remote type and probably others similar: RG10B(B1)/BGEFU1, RG10B1(B)/BGEF, RG10B2(B)/BGCEF,

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Miele Scout RX2 *(by Kalle Varavas)*
> Remote control for Miele Scout RX2 robot vacuum with Drive and Menu modes Miele Scout RX2 - Flipper Zero Remote Flipper Zero app for controlling the Miele Scout RX2 robot vacuum over IR. Grab the pre-built dist/miele_scout.fap from this repo and copy it to your Flipper Zero SD card:

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Mitsubishi AC Remote
> Mitsubishi Electric Air Conditioner remote control Mitsubishi AC Remote Mitsubishi Electric AC remote control for Flipper Zero. Available to download on the Flipper App Catalog here

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Netflix TV Remote *(by ablaran)*
> A simple IR TV remote for Netflix for Sony BRAVIA 4K VH2 Netflix TV Remote for Sony BRAVIA 4K VH2 Infrared remote control app for Netflix on Sony BRAVIA 4K VH2 TV. Death Note-themed graphical interface.

**Usage:** - Buttons for: Netflix, Play/Pause, Mute, Power, Volume Up/Down. - D-pad for navigation (arrows + OK + Back).

#### Pause Timer *(by Matt-London)*
> Pause your show when the ad break ends Flipper app to ensure that you don't miss parts of your show when using the bathroom during an ad break! This app will play a pre-recorded IR signal back on a countdown, there are a few use cases b...

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### R.O.B. Control *(by John Riney III)*
> Control Nintendo R.O.B. flip_rob - Nintendo R.O.B./Family Computer Robot remote controller for Flipper Zero Do you have one of these? !Nintendo Family Computer Robot, aka R.O.B.

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### TagTinker *(by i12bp8)*
> Educational ESL study tool for owned hardware Infrared ESL Research Toolkit for Flipper Zero Protocol study • Signal analysis • Controlled display experiments on authorized hardware Owner-authorized lab display experiment

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Timed Remote *(by A. Damsgaard)*
> Send IR commands after timed duration A Flipper Zero application that sends IR (infrared) commands after a configurable time delay. - Browse and select IR signals from existing .ir files - Countdown Mode: Set a timer (HH:MM:SS) and send the signal when it completes

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Xbox Controller *(by gebeto)*
> Infrared remote control for Xbox One Xbox Controller for Flipper Zero - IR codes for XBOX: Lucaslhm/Flipper-IRDB/Consoles/Microsoft

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### Xbox Controller *(by @gebeto)*
> Infrared remote control for Xbox One Xbox Controller for Flipper Zero - IR codes for XBOX: Lucaslhm/Flipper-IRDB/Consoles/Microsoft

**Usage:** Launch from the Infrared/Apps menu. Point the Flipper's IR port at the target device and select a command.

#### XRemote *(by @kala13x)*
> Advanced infrared remote application Advanced IR Remote App for Flipper Device Version 1.3 - Changelog

**Usage:** Navigation to the menu to press each button individually can be often uncomfortable because it requires scrolling to the desired button and selecti... Learn new remote

</details>

<details><summary><h3>🔵 Bluetooth — 12 apps</h3></summary>

#### Anki Remote *(by Blue5GD)*
> A customizable BLE keyboard remote What does Anki-Remote do? It lets you assign any keyboard key to each button on your Flipper Zero and use it as a BLE (Bluetooth Low Energy) remote. Key mappings are saved to the SD card inside the apps_data folder.

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### BLE Clicker *(by Roman)*
> BLE HID clicker for voice-driven Claude Code workflow on iTerm2 Flipper Zero BLE Clicker BLE HID keyboard for voice-driven Claude Code workflow on iTerm2. Turns Flipper Zero into a wireless remote — dictate, submit, navigate panes and tabs without touching the keyboard.

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### BLE Spam *(by @Willy-JL @ECTO-1A @Spooks4576)*
> Flood BLE advertisements to cause spammy and annoying popups/notifications

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### BT Trigger *(by @Nem0oo)*
> Control your smartphone camera via your Flipper Zero Control your smartphone camera via your Flipper Zero With the BT Trigger app, you can remotely take pictures with the help of your Flipper Zero via Bluetooth Low Energy. To take a picture remotely, do as follows:

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### BTHome *(by Alessandro Ghedini)*
> BTHome implementation for the Flipper Zero This application turns the Flipper Zero into a BTHome beacon, and can be used to integrate the Flipper with home automation systems that support BTHome, such as Home Assistant.

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### Cookie Clicker *(by jamisonderek)*
> The goal of this application is use the Flipper Zero's bluetooth connectivity to send mouse clicks to your phone at a given frequency. If you are playing a game, such as Cookie Clicker 2, this will result in a 'screen tap' being simulated.

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### FindMy Flipper *(by MatthewKuKanich)*
> BLE FindMy Location Beacon FindMy Flipper - AirTag and SmartTag Emulator This app extends the functionality of the FlipperZero's bluetooth capabilities, enabling it to act as an Apple AirTag, Samsung SmartTag, or a Tile Tracker. It utilizes the FlipperZero's BLE beacon ... 1.

**Usage:** 3. Efficient Background Operation: The app is optimized to run in the background, ensuring that your FlipperZero can still be tracked with minimal ... Step 1: Installation

#### Flight Monitor *(by Dr.Mosfet)*
> Monitor aircraft parameters from flight simulator ﻿Flight Monitor - War Thunder Dashboard for Flipper Zero Real-time Aircraft Parameters Monitor for Flipper Zero This is a comprehensive flight monitoring application designed for the Flipper Zero that interfaces with War Thunder flight simulator via.

**Usage:** Flight Parameters Display The application provides accurate and real-time readings for complete flight telemetry:

#### Govee Control *(by devdotbo)*
> Control Govee BLE LED devices Govee Control for Flipper Zero Control Govee H6006 Smart LED Bulbs directly from your Flipper Zero! - BLE device discovery

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### MagicBand Plus Lights (Unofficial) *(by Henry Willis (Haw8411))*
> Trigger light effects on compatible bands. Not affiliated with Disney. Disney MagicBand + Lights at home Unofficial project. Not affiliated with, endorsed, or sponsored by The Walt Disney Company. MagicBand and MagicBand+ are trademarks of Disney Enterprises, Inc. Names are used only to

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### PC Monitor *(by Olejka)*
> Application for monitoring PC resources Flipper Application for monitoring PC resources A backend running on your PC is required Many thanks to Willy-JL for implementing BLE Serial for the new firmware and bringing the app back to life

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

#### Remote for Kodi *(by coded-with-claws)*
> Application Bluetooth remote Keynote for Kodi (original app + feature: long press on OK to switch between Space and Return (useful for Kodi to navigate the menus))

**Usage:** Enable Bluetooth on your Flipper. Launch the app and pair with the target device.

</details>

<details><summary><h3>🎵 Media — 29 apps</h3></summary>

#### ATM player *(by apfxtech)*
> ATM music player from Arduventure. ATM — это плеер музыки для Flipper Zero на базе ATM-синтезатора (движок из Arduventure/Arduboy). Приложение читает текстовые файлы .atm из assets/, компилирует их во внутренний бинарный ATM-формат и воспроизводит через ATM.play().

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### BPM Tapper *(by @panki27)*
> Tap center button to measure BPM A BPM Tapper for the Flipper Zero. 1. Navigate to the FAP Build GitHub action workflow, and select the most recent run.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### DVD Screensaver *(by shantih19)*
> DVD Screensaver clone Flipper Zero DVD Screensaver A DVD player inspired screensaver for the Flipper Zero.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Etch a Sketch *(by @SimplyMinimal)*
> Turn the Flipper Zero into an Etch A Sketch FlipperZero-Etch-A-Sketch Turn the Flipper Zero into an Etch A Sketch This is a modification of the original paint app.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Fart Sound Generator *(by F. Greil)*
> A simple app that produces a flatulent sound effect for light relief and to improve your morale during breaks. mitzi-windbreak Flipper Zero App to generate interesting noises. This a bit juvenile sound app allows your Flipper to break winds with adjustable parameters 💨

**Usage:** 1. Navigate: Use Up/Down buttons to select a parameter - Wet/Dry: Controls frequency variation - wet farts have more wobble, dry farts are more stable

#### Flizzer Tracker *(by LTVA)*
> An advanced Flipper Zero chiptune tracker with 4 channels Flizzer Tracker A Flipper Zero chiptune tracker. Supports 4 channels, external (through PA6 pin) and internal (built-in buzzer) audio output. Each channel has a functionality akin to MOS Technology SID sound chip ... Telegram channel

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Fmatrix *(by misterwaztaken)*
> Displays a 'matrix rain' screensaver. fmatrix is a simple Flipper Zero app that displays the famous "matrix rain" effect. (suggestions appreciated and welcome!) Make it rain! - Makes it rain (Displays the matrix effect.)

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### FMF2USBMIDI *(by crackerjacques)*
> Convert Flipper Zero Music Player file to MIDI Signal and transfer it via USB Port. Flipper_Zero_FMF2USBMIDI Convert Flipper Zero Music Player file to MIDI Signal and transfer it via USB Port. this code was tested in Flipper Zero SDK v50.1 to v54.0

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Guido Music Score Reader *(by F Greil)*
> A reader for Guido music score files A Guido musical notation reader for Flipper Zero OK opens file browser if no file is loaded. Otherwise starts/stops playback Up opens file browser to load a different file

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Image Scroller *(by F Greil)*
> A image scroller where you navigate a cursor over a (large, tiled) background image A simple Flipper Zero to view annotated, tiled images. The user can scroll around a large images (consisting of many small 128x64px-tiles saved in the assets/-folder).

**Usage:** - Arrow Keys: Move cursor around - OK Button: Appears when there is an annotation for the current image position

#### Image Viewer *(by @polioan)*
> Image viewer for flipper zero! Image viewer for flipper zero! 1. Create image with img2fbm 2. Drop image to SD card

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Karl Eido *(by F Greil)*
> An app that mimics an optical instrument to generate beautiful patterns by using 3 mirrors. mitzi-karl-eido A simple Flipper Zero kaleidoscope app. Detailed description

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Metronome *(by @panki27 & @xMasterX)*
> Metronome app A metronome for the Flipper Zero. 1. Navigate to the FAP Build GitHub action workflow, and select the most recent run.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### MIDI Ocarina *(by crackerjacques)*
> Push button to play midi note over USB Flipper_Zero_MIDI_Ocarina push button to play midi note. I made this to check if the MIDI library TX works.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### MIDI RX *(by crackerjacques)*
> receive midi and play song flipper_zero_midi_rx receive midi and play song

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Morse Code *(by @wh00hw & @xMasterX)*
> Simple Morse Code parser Simple Morse Code parser Flipper Application Package.

**Usage:** - Up/Down buttons to set volume - Left/Right buttons to set Dit length in milliseconds

#### Music Beeper
> Plays music files through the Flipper Zero's built-in speaker using simple beeper tones. Supports RTTTL and FMF music file formats stored on the SD card.

**Usage:** Place music files (.rtttl or .fmf) on your SD card. Launch from Media menu, browse to your file, and press OK to play. Use d-pad to navigate between tracks.

#### Music Player
> An app to play RTTL music files

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Music Tracker *(by DrZlo13)*
> App plays hardcoded tracker song Flipper Zero music tracker MVP Stage: minimum viable player

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Ocarina *(by invalidna-me)*
> A basic Ocarina (of Time), Controls are the same as the N64 version of the Ocarina of Time flipperzero-ocarina A basic Ocarina (of Time) for the Flipper Zero.

**Usage:** are the same as the N64 version of the Ocarina of Time, the Ok button takes the place of the A button

#### Paint *(by @n-o-T-I-n-s-a-n-e)*
> A basic Paint app, Click Ok to draw dot, hold Ok to enable drawing continuously, hold Back to clear the screen

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### SAM AYBABTU
> Media player/viewer application for Flipper Zero.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Snowflake *(by F Greil)*
> Simple app to show aesthetic images mitzi-snowflake A simple Flipper Zero app to create nice-looking 60° or 120° graphics. Watch the iterative growth of a snowflake on 64x64 pixels.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Space Playground *(by Alan Silva)*
> A simple space simulation playground for Flipper Zero Space Playground Link to the app: https://lab.flipper.net/apps/fzspground This is a "screensaver" that simulates a space environment with planets and asteroids.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Text to SAM *(by @Round-Pi & (Fixes by @Willy-JL))*
> Convert text to speech on your Flipper Zero with SAM (Software Automatic Mouth). - Type in a message and press the back button (or select save and press back at the text preview) - SAM will say the message and the app will exit.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Tuning Fork *(by @besya)*
> Tuning fork for tuning musical instruments and more Tuning fork for tuning musical instruments and more - 6 strings: Standard, Drop D, DADGAD (Dsus4), Standard D, CGCFGC(Csus4), Drop C, CGCFGC - 7 strings: Standard, Drop A, Standard A

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### USB-MIDI
> Turn Flipper into MIDI instrument This is a simple Flipper app, that turns it into a monophonic MIDI instrument. Launch the app, connect to computer via USB, use in music software.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### Video Player *(by LTVA)*
> An app that plays video along with sound on Flipper Zero. A Flipper Zero application to play video files (with sound). Uses custom video file format. Is proven to work at 30 FPS and 44100 Hz audio sample rate with full 128 by 64 pixels resolution. Maximum...

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

#### WAV Player *(by @DrZlo13 & (ported, fixed by @xMasterX), (improved by @LTVA1))*
> Audio player for WAV files, recommended to convert files to unsigned 8-bit PCM stereo, but it may work with others too A Flipper Zero application for playing wav files.

**Usage:** Launch from the Media/Apps menu. Use d-pad to browse and OK to play/select.

</details>

<details><summary><h3>🔌 Usb — 26 apps</h3></summary>

#### Agentic Remote USB *(by Kasen Sansonetti)*
> One-handed USB remote for AI coding agents (Claude Code). 5-button D-pad control, macros, offline manual + quiz.

**Usage:** A one-handed Flipper Zero remote for Claude Code and AI coding agents. Five buttons. No keyboard required. Built by Kasen Sansonetti & Wetware Labs.

#### Bad Duck3 *(by dutchpatriot)*
> DuckyScript 3.0 interpreter with USB/BLE HID support. Loops, variables, conditionals. Bad Duck3 - DuckyScript 3.0 for Flipper Zero The first DuckyScript 3.0 implementation for Flipper Zero with USB and Bluetooth HID support. DuckyScript 3.0 Language

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### BadUSB Pro *(by PentestSuite & barkandbite)*
> Advanced USB HID keystroke injection with LED feedback ____ _ _ _ ____ ____ ____

**Usage:** DuckyScript 3.0 keystroke injection engine for Flipper Zero BadUSB Pro turns Flipper Zero into a USB HID keyboard that executes DuckyScript 3.0 payloads against a connected computer. Unlike the Flipper's bui...

#### BarCode ScannerE *(by Igor Danilov / polarikus)*
> Emulates a barcode scanner for testing cash registers (Evotor). Why buy a scanner when you have a flipper? Bar code scanner emulator Scanner work at COM-port emulation. Characteristics

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### CCID Emulator *(by PentestSuite & barkandbite)*
> Programmable USB smartcard emulator via CCID ____ ____ ___ ____ _____ _ _ / ___/ ___\|_ _\| _ \\| ____\|_ __ ___ _ _\| \| __ _\| \|_ ___ _ __ \____\____\|___\|____/\|_____\|_\| \|_\| \|_\|\__,_\|_\|\__,_\|\__\___/\|_\|

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### Chameleon Ultra *(by Chameleon Flipper Team / muylder)*
> Control Chameleon Ultra via USB/Bluetooth - Manage slots, transfer tags, emulation Chameleon Flipper - Chameleon Ultra Controller for Flipper Zero Complete Flipper Zero application (.fap) to control and manage your Chameleon Ultra device via USB or Bluetooth.

**Usage:** 🎬 Features fun bar animation when devices connect! Watch Chameleon and Dolphin meet at a bar! 🍺 - 📖 Quick Start Guide - Get started in minutes

#### Click Recorder *(by 0x78f1935)*
> Record, shuffle, spam mouse clicks or use as a full mouse over USB or BLE HID Click Recorder for Flipper Zero Turn your Flipper Zero into a powerful mouse automation tool. Record, replay, randomize, or spam mouse clicks over USB or Bluetooth.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### Clippy *(by mtormento)*
> Implements a shared clipboard Has ever happened to you when you are setting up your new smartphone and you are asked for the wifi password to feel the extreme urge to start cursing every known divinity? Clippy solves this problem by turning your Flipper Zero into a cross-device shared clipboard.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### FlipDeck *(by FlipDeck / ismail-oe)*
> USB HID Macro Pad with configurable pages Turn your Flipper Zero into a USB macro pad.

**Usage:** FlipDeck transforms your Flipper Zero into a programmable USB macro pad with media controls, custom shortcuts, and an extensible plugin system. Med... Format: ACTION:Label:Symbol — one per line, up to 6 per page.

#### FlipperTrack *(by pinkpwningclub)*
> USB HID activity simulator: mouse jitter, keystrokes, and window cycling

**Usage:** USB HID activity simulator for Flipper Zero. Generates periodic mouse movement, keystroke bursts, and window-cycling inputs over USB HID — useful f...

#### Gatekeeper *(by enexis1337)*
> Secure BadUSB password launcher with combo lock Gatekeeper is a secure password manager for the Flipper Zero that leverages the BadUSB (HID emulation) capabilities. It allows you to store your most-used credentials and "type" them into any compu...

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### HID Exfil *(by PentestSuite & barkandbite)*
> HID-based data exfiltration via keyboard LED feedback channel _ _ ___ ____ _____ __ _ _ HID Exfil exploits the USB HID keyboard LED channel as a unidirectional covert data exfiltration path. Normally, keyboard LEDs (CapsLock, NumLock, ScrollLock) are controlled by the host OS to reflect lock state.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### HID File Transfer *(by Kavakuo)*
> This is an application for the Flipper Zero that allows to exchange files using the HID protocol with around 260kB/s. This approach is especially useful when access to mass storage devices is blocked on a PC.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### LEGO Dimensions Toy Pad *(by Seger)*
> USB Toy Pad emulator for Lego Dimensions Flipper Zero Lego Dimensions ToyPad Emulator Use your Flipper Zero as a Lego Dimensions ToyPad This project emulates the behavior of the original USB ToyPad, allowing you to interact with Lego Dimensions using your Flipper Zero.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### Mass Storage
> Implements a mass storage device over USB for disk images

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### Mouse Jiggler *(by Jacob-Tate)*
> Jiggles mouse over USB

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### MTP *(by Alex4386)*
> Use Flipper Zero as a MTP device. Flipper Zero: MTP See this in action on YouTube! - Latest Release: Download

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### PC Monitor USB *(by DonJulve & TheSainEyereg)*
> Application for monitoring PC resources via USB PC Monitor (USB Version) Flipper Application for monitoring PC resources via USB (Virtual COM Port). A backend running on your PC is required.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### Portal Of Flipper *(by sanjay900)*
> USB emulator Portal of Flipper Original App by bettse App Icon by mikeonut

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### SD Card Mounter *(by Aaronjamt)*
> Use Flipper as an SD card reader! Flipper Zero SD card mounter This is a small Flipper Zero app that lets you connect the internal micro SD card reader directly to your computer.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### Switch Controller *(by Claude)*
> Nintendo Switch controller emulator with macro recording/playback Switch Controller - Flipper Zero App A Flipper Zero application that emulates a Nintendo Switch Pro Controller over USB with macro recording and playback capabilities. ✅ Compatible with Official Firmware and Momentum Firmware

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### USB Consumer Control *(by piraija)*
> USB Consumer Control USB Consumer Control This is a Flipper Zero application for sending Consumer Control Button (CCB) presses as a USB HID device. This is useful for researching how various devices handle USB CCBs, for example in the cont... A more detailed writeup of the topic can be found here.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### USB Game Controller *(by crapbass)*
> An app that emulates XInput controllers USB Game Controller for Flipper Zero

**Usage:** This is an app for the Flipper Zero that emulates a game controller (specifically the Xbox 360 controller) and lets the user interact with the emul... It also contains a full implementation of XInput for the Flipper Zero that supports every input that a regular Xbox controller does, which you may ...

#### USB HID Autofire *(by pbek)*
> This is a simple Flipper Zero application to send left-clicks as a USB HID device. USB HID Autofire Latest release \| This is a simple Flipper Zero application to send left-clicks as a USB HID device.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### USB Midi Capturing *(by F Greil)*
> A simple Flipper Zero USB Midi capturing app Towards a capturing MIDI signals via USB using Flipper Zero. The goal is to allow real-time monitoring of MIDI events including Note On/Off, Control Changes, Program Changes, Pitch Bend, and more.

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

#### USB Remote *(by Cutch)*
> Use Flipper as a HID remote control over USB

**Usage:** Connect Flipper to a computer via USB. Launch the app and follow on-screen instructions.

</details>

<details><summary><h3>⚡ Gpio/Sensors — 22 apps</h3></summary>

#### [AS7331] UV Meter *(by Michael Baisch)*
> Measure UV radiation using the AS7331 sensor A Flipper Zero application designed to measure ultraviolet (UV) radiation levels using the AS7331 sensor. It supports individual measurements for UV-A, UV-B, and UV-C wavelengths. The easiest way t...

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [BH1750] Lightmeter *(by Oleksii Kutuzov)*
> Lightmeter app for photography Lightmeter app for photography An application that suggests settings for your manual camera based on the reading of the ambient light sensor. Can also be used in a pure lux meter mode. Supported sensors

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [GPIO] Unitemp *(by @quen0n & (fixes by @xMasterX))*
> Universal temperature sensors reader Unitemp - Universal temperature sensor reader Flipper Zero application for reading temperature, humidity, CO2 and pressure sensors like a DHT11/22, DS18B20, BMP280, HTU21, and more. List of supported sensors

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [HC-SR] Dist. Sensor *(by @xMasterX (first implementation by @Sanqui))*
> HC-SR(04) Distance sensor reader

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [J305] Atomic Dice Roller
> flipperzero-atomicdiceroller 🎲☢ An atomic dice roller for the Flipper Zero ☢🎲 Geiger Counter application must work on your Flipper Zero to be able to use this application. You need the same geiger board and this board must be connected in the same way as described in the.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [J305] Geiger Counter *(by @nmrr)*
> Works with J305 Geiger tube on external board flipperzero-geigercounter A geiger counter application for the Flipper Zero You need a geiger counter board to run this application. This board can be used : https://aliexpress.com/item/1005004074447209.html

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [MH-Z19] CO2 Logger *(by Harry O'Brien)*
> CO2 Logger with CSV export App for reading CO2 ppm from MH-Z19 sensor and saving to a local csv file. 🌐 Web App - Visualize Your Data CO2 Logger Viewer — Plug in your Flipper, click connect, and instantly see your CO2 data as an interactive chart. No drivers, no apps, just your browser.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [MH-Z19] CO2 Sensor *(by meshchaninov)*
> Application for measuring carbon dioxide (CO2) with mh-z19 sensor and Flipper zero flipper-zero-mh-z19 Application for measuring carbon dioxide (CO2) with mh-z19 sensor and Flipper zero Instalation Guide

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [Mx2125] Accelerometer
> This is a demostration of using GPIO interrupts to interpret data from the Memsic 2125 (Mx2125) Dual-Axis Accelerometer. This project is intended to be overlayed on top of an existing firmware repo. - Copy the "memsic_2125" folder to the \applications\plugins\memsic_2125 folder in your firmware.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [RCWL-516] Motion Sensor
> Rcwl-0516 Flippezero This fap plugin (app) for RCWL-0516 radar. RCWL-0516 is a doppler radar microwave motion sensor module

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [RCWL0516] Radar Scan *(by @MatthewKuKanich)*
> Detects the movement of living things using radar Flipper Zero Radar Scanner The Flipper Zero Radar Scanner is an application designed for the Flipper Zeros GPIO capabilities, utilizing the RCWL-0516 Microwave Radar module to detect the presence and movement of living being...

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [RCWL0516] Sleep Counter *(by Argentum / silverwolf2r)*
> Counts how much you move in your sleep Sleep Counter Flipper App This is a flipper app that counts how much you move in your sleep. Shoutout to MatthewKuKanich https://github.com/MatthewKuKanich/flipper-radar he is the one who made the original app for this Radar module.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### [USPING] Dist. Sensor *(by privet971)*
> Ultrasound measurments with PING))) Parallax sensor SKU 28015 (3 wires) FlipperZeroApps

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### Gas Sensor *(by iancohee)*
> Simple application that reads BME680 and SGP30 data. Simple application that reads BME680 and SGP30 data. Note that the BEM680 needs to have SD0 and CS connected to the "high" line. !wiring diagram

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### MAX31855 *(by skotopes)*
> MAX31855 Thermocouple Sensor Application

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### MH-Z19 UART *(by Aku)*
> GPIO-based tool that interfaces with external hardware modules.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### Plantower PMSx003 sensor reader *(by 3cky)*
> Plantower PMSx003 sensor reader Airmon for Flipper Zero A simple Flipper Zero air quality monitoring application. It displays measured PM1.0/PM2.5/PM10 [Particulate Matter] concentrations

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### Radiation Sensor *(by sionyx)*
> Works with RadSens external module Flipper application for RadSens module RadSens Application is not official application allows you to get level of current radiation measured by RadSens dosimeter module based on a Geiger tube. The Application stores amount of particles between launches.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### UV Meter [AS7331] *(by Michael Baisch)*
> Measure UV radiation using the AS7331 sensor A Flipper Zero application designed to measure ultraviolet (UV) radiation levels using the AS7331 sensor. It supports individual measurements for UV-A, UV-B, and UV-C wavelengths. The easiest way t...

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### VEML7700 Lux Meter *(by kamylwnb / Dr.Mosfet)*
> Measures ambient light in lux using a VEML7700 sensor via I2C bus and displays the value on the screen.

**Usage:** Simple Menu: Intuitive navigation using the Flipper Zero buttons (OK, arrows, Back).

#### VL6180X Distance Sensor [I2C] *(by Kyle Diller)*
> This is a simple app the makes uses of the Adafruit VL6180X distance sensor. The code for configuring and reading from the VL6180X is a port of the Circuit Python code provided by Adafruit. There is also a C++ library for use with an Arduino.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

#### Water Sensor Reader *(by MatveyStrelov)*
> Water Sensor Reader is a simple Flipper Zero application that reads an analog water sensor connected to a GPIO pin and displays its values in real-time. The app shows both the raw ADC value and the corresponding voltage in millivolts, along with a visual progress bar.

**Usage:** Connect the sensor to your Flipper's GPIO pins. Launch the app to start reading sensor data on screen.

</details>

<details><summary><h3>⚙️ Settings — 7 apps</h3></summary>

#### Animation Switcher *(by lsalik2)*
> Change your background animations on the fly! Flipper Animation Switcher A Flipper Zero application for creating, managing, and switching animation playlists - template manifest.txt files that control which background animations play on your Flipper. Supports up to 128 ...

**Usage:** Launch from Settings/Apps menu. Use d-pad to navigate options and OK to change values.

#### Antidepressants *(by nymda)*
> App to change the level, experience and mood/butthurt of your dolphin pet, EXP adjusted for Rogue Master Custom Firmware. FlipperAntidepressants Flipper application that allows changing of the background XP and Butthurt values !Screenshot-20230306-115930

**Usage:** Launch from Settings/Apps menu. Use d-pad to navigate options and OK to change values.

#### Extend Range
> flipperzero-extend-range Extend SubGHz Range Instead of manually changing the configuration of the subghz module (to enable the full potential), this application does it for you.

**Usage:** Launch from Settings/Apps menu. Use d-pad to navigate options and OK to change values.

#### Gauge Tool *(by Aku)*
> Gauge Tool (Use only if you know what you are doing)

**Usage:** Launch from Settings/Apps menu. Use d-pad to navigate options and OK to change values.

#### Name Changer
> Change your Flipper Zero's display name without connecting to a computer. The new name appears on the main screen, in Bluetooth, and when connecting to other devices.

**Usage:** Launch from Settings. Type your desired name using the on-screen keyboard and confirm. Your Flipper will reboot with the new name applied.

#### Smack My Dolphin Up! *(by siberianbot)*
> For those whom won't bother yourself with dolphin emotional state Smack My Dolphin Up! For those whom won't bother yourself with dolphin emotional state

**Usage:** Launch from Settings/Apps menu. Use d-pad to navigate options and OK to change values.

#### Theme Manager *(by Hoasker)*
> Manage dolphin animation themes from SD card 🎨 Theme Manager for Flipper Zero Manage dolphin animation themes directly from your Flipper Zero — no PC required. - Flipper Apps Catalog

**Usage:** Launch from Settings/Apps menu. Use d-pad to navigate options and OK to change values.

</details>

<details><summary><h3>🔧 Tools/Educational — 7 apps</h3></summary>

#### ASCII Table *(by x10102)*
> An ASCII table for your flipper! A pocket ASCII table for your Flipper Zero. You never know when you might need one ;) - UP / DOWN - Pretty self-explanatory - LEFT / RIGHT - Skip 10 symbols

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Crypto Dictionary *(by armixz)*
> Cryptography Dictionary (Flipper Zero Edition) Flipper-Zero-Crypto-Dictionary Cryptography Dictionary is a comprehensive reference tool that provides detailed info on various algorithms.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Extra Demo *(by Milk-Cool)*
> A pack of extra commands for the Flipper Zero (a C library). A pack of extra commands for the Flipper Zero (a C library). Check out the demo here! In your app directory run: To include it in a plugin:

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### FAP Boilerplate *(by leedave)*
> A Boilerplate FAP or FAP template to use as a starting point in your new custom app Flipper Zero FAP Boilerplate Use this code as a foundation for developing Flipper Zero Applications, change the contents of this file when developing a new project - Different Scenes / Views

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Flipper Doctor *(by JulanDeAlb)*
> Lets you view all information's regarding your flippers experience and butthurt. Flipper Doctor for OFW This Flipper application lets you view all information's regarding your flippers experience and butthurt. General XP and Butthurt View

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Plugin Demo *(by csBlueChip)*
> A simple plugin for the FlipperZero written as a tutorial example [ie. excessive documentation & error handling]. See code. FlipperZero_plugin_howto A simple plugin for the FlipperZero written as a tutorial example [ie.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### The C Prog. Language *(by armixz)*
> The C Programming Language, Flipper Zero Edition (by Brian W. Kernighan, Dennis M. Ritchie) Flipper-Zero-The-C-Programming-Language The authors present the complete guide to ANSI standard C language programming. by Brian W. Kernighan, Dennis M. Ritchie.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

</details>

<details><summary><h3>⚡ Gpio/Debug — 10 apps</h3></summary>

#### [AVR] AVR Flasher
> Application for flashing AVR microcontrollers

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [FTDI232H] FlipTDI *(by SkorP)*
> Flipper FTDI232H emulator.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [I2C] i2c Tools *(by @NaejEL)*
> Set of i2c tools flipperzero-i2ctools Set of i2c tools for Flipper Zero Look for i2c peripherals adresses

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [INA2xx] INA Meter *(by cepetr)*
> Application for reading TI INAxxx sensors. INA Meter for Flipper Zero INA Meter is an application for Flipper Zero that allows you to read I2C-connected current/power monitors from Texas Instruments. Supported Sensors:

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [SPI] SPI Mem Manager
> Application for reading and writing 25-series SPI memory chips

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [SWD-JTAG] DAP Link
> Enables use of Flipper as a debug probe for ARM devices, implements the CMSIS-DAP protocol Flipper Zero as CMSIS DAP/DAP Link Flipper Zero as a Free-DAP based SWD\JTAG debugger. Free-DAP is a free and open source firmware implementation of the CMSIS-DAP debugger.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [SWD] SWD Probe *(by @g3gg0 & (fixes by @xMasterX))*
> ARM SWD (Single Wire Debug) Probe ARM SWD (Single Wire Debug) Probe Modern microcontrollers have support for the two wire debug interface SWD, which makes wiring a lot simpler.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [W5500] Ethernet *(by @karasevia & @arag0re & @xMasterX)*
> Test your Ethernet connection with W5500 module and Flipper Flipper Zero Ethernet Troubleshooter This is a small helper that lets you connect your Flipper via RJ45 to your network You can then set the interface-mac and test if you can get an IP via DHCP and the ping addresses.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### AVR Flasher
> Application for flashing AVR microcontrollers

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Ethernet Troubleshooter W5500 *(by karasevia)*
> This is a small helper that lets you connect your Flipper via RJ45 to your network. You can then set the interface-mac and test if you can get an IP via DHCP and the ping addresses.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>

<details><summary><h3>⚡ Gpio/Esp32 — 22 apps</h3></summary>

#### [CAM] Morse Flash v0.1 *(by eried)*
> ESP32-CAM app to stream a message in morse using the powerful flashlight. [Unplug the USB cable to test with Mayhem] UART Terminal for Flipper Zero Flipper Zero app to control various devices via UART interface. - Read log and command output by uart

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### [ESP32] Camera *(by Z4urce)*
> ESP32-CAM live feed and photo capture Camera application for Flipper Zero - Up - Increase contrast - Down - Decrease contract

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### BLE Killer *(by H4lo)*
> A Bluetooth expansion board based on flipper zero to scan surrounding low-power Bluetooth devices and Bluetooth device controllers The Bluetooth expansion board (ble ext) based on flipper zero realizes the scanning of surrounding low-power Bluetooth devices and the integrated Bluetooth device.

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### BLE Scanner *(by PentestSuite / barkandbite)*
> BLE advertisement scanner via ESP32 Dev Board — detects AirTags, skimmers, rogue beacons

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### BT Audio *(by Father Divine)*
> Control ESP32 Bluetooth Audio via UART BT Audio - Bluetooth Audio Player for Flipper Zero A Flipper Zero application that plays MP3 audio (Bluetooth A2DP audio streaming) by controlling ESP32-based boards over UART. The ESP32 acts as an A2DP source by default, streaming audio TO Bluetoo...

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### Evil BLE *(by PentestSuite / barkandbite)*
> BLE advertisement cloning — scans with ESP32, re-broadcasts as clone via Flipper BLE

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### FlipDownloader *(by JBlanked)*
> Download apps and assets via WiFi directly to your Flipper Zero - Discord: https://discord.gg/5aN9qwkEc6 - YouTube: https://www.youtube.com/@jblanked - Instagram: https://www.instagram.com/jblanked

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### FlipLibrary *(by JBlanked)*
> Utilize WiFi to retrieve data from 20 different APIs FlipLibrary App for Flipper Zero

**Usage:** The FlipLibrary app for Flipper Zero is a versatile and user-friendly application that offers a wide range of features to enhance your Flipper Zero... The app includes a dictionary, random facts, and numerous additional functionalities, all accessible directly from your Flipper Zero device. It is ...

#### FlipRPI *(by jblanked)*
> Use your Flipper Zero to control your Raspberry Pi. Use your Flipper Zero to control your Raspberry Pi. - MicroSD Card (32GB or larger) - MicroUSB Cable (data-capable)

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### FlipSocial *(by jblanked)*
> Social media platform for the Flipper Zero. The first social media app for Flipper Zero. Connect with other users directly on your device through WiFi. The highlight of this app is customizable pre-saves, which, as explained below, aim to address the challenges of typing with the directional pad.

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### FlipTrader *(by JBlanked)*
> Use WiFi to get the price of stocks and currency pairs on your Flipper Zero. FlipTrader is an app for the Flipper Zero that uses WiFi to fetch the prices of stocks and currency pairs directly on your device. It utilizes the FlipperHTTP flash for the WiFi Devboard, originall...

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### FlipWeather *(by JBlanked)*
> Use WiFi to get GPS and Weather information on your Flipper Zero. FlipWeather is an innovative app for the Flipper Zero that uses WiFi to fetch GPS and weather information, making your Flipper Zero even more versatile. It leverages the FlipperHTTP flash for the W...

**Usage:** - WiFi Settings Management: Allows configuration of WiFi settings (SSID and password) for current and future features that utilize network connecti...

#### FlipWiFi *(by JBlanked)*
> FlipperHTTP companion app. FlipWiFi is the companion app for the popular FlipperHTTP firmware. It allows you to scan and save Wi-Fi networks for use across all FlipperHTTP apps, set up captive portals, and send deauthenticat...

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### FlipWorld *(by JBlanked)*
> The first open-world multiplayer game, best played with the VGM. The first open-world multiplayer game for the Flipper Zero, best played with the VGM.

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### Gemini IA *(by d4rks1d33 & jamisonderek)*
> This is an app to interact with Google Gemini IA using the ESP32. Currently in development (Beta phase), anyone who wants to join and help improve it can make their PR and also C developers who want to help improve the Flipper native app are welcome.

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### Gravity *(by chris-bc)*
> App to control ESP32 Gravity wireless exploration platform. Gravity for Flipper Zero The Unseen Force This Flipper Zero application (FAP) is a companion application to ESP32-Gravity.

**Usage:** supports a variety of wireless exploration, offensive and defensive features. ESP32-Gravity can be used without a Flipper, but Flipper-Gravity can't be used

#### Postman *(by MassivDash)*
> Flipper Postman can be used to send HTTP requests from a flipper device using the the esp32s2 postman firmware.

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### Ray Hunter Client *(by PentestSuite / barkandbite)*
> IMSI catcher detection — Ray Hunter status display via ESP32 WiFi bridge

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### Rogue AP Detector *(by PentestSuite / barkandbite)*
> Detects evil twin / rogue WiFi access points via ESP32 Dev Board

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### Web Crawler *(by JBlanked)*
> Browse the web, fetch API data, and more. Browse the web, fetch API data, and more on your Flipper Zero. - WiFi Developer Board, Raspberry Pi, or ESP32 device flashed with FlipperHTTP version 1.6 or higher: https://github.com/jblanked/FlipperHTTP - 2.4 GHz WiFi Access Point

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### Wendigo BT+BLE+WiFi Monitor *(by Chris BC)*
> Monitor nearby radios and their signal strength, allowing a device of interest to be identified and located based on its RSSI. [![Contributors][contributors-shield]][contributors-url] [![Forks][forks-shield]][forks-url] [![Stargazers][stars-shield]][stars-url]

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

#### WiFi Mapping *(by carvilsi)*
> An ESP32 and FlipperZero wifi mapping. Check the ESP32 side at: esp32-wifi-map https://github.com/carvilsi/esp32-wifi-map An ESP32 and FlipperZero wifi mapping Check the ESP32 side at: esp32-wifi-map And do not forget to have both sides on the last version.

**Usage:** Requires an ESP32 module connected via GPIO/UART. Launch the app and use the on-screen menu to interact with the module.

</details>

<details><summary><h3>⚡ Gpio/Esp — 10 apps</h3></summary>

#### [ESP32] Evil Portal *(by bigbrodude6119)*
> Create an evil captive portal Wi-Fi access point Flipper Zero Evil Portal An evil captive portal Wi-Fi access point using the Flipper Zero and Wi-Fi dev board This project is a work in progress.

**Usage:** Requires an ESP module connected via GPIO. Launch the app and follow the on-screen prompts.

#### [ESP32] Ghost ESP
> GhostESP Flipper Zero App A Flipper Zero application for interfacing with the GhostESP: Revival firmware. WiFi Operations

**Usage:** Requires an ESP module connected via GPIO. Launch the app and follow the on-screen prompts.

#### [ESP32] WiFi Marauder
> Companion app for the ESP32 WiFi Marauder firmware. Provides a full interface for WiFi scanning, packet capture, deauthentication, beacon spam, and other WiFi analysis tools.

**Usage:** Requires an ESP32 module flashed with Marauder firmware connected via GPIO/UART. Launch the app, select your attack or scan type from the menu, and control everything from the Flipper's screen.

#### [ESP32CAM] Camera Suite *(by @CodyTolene @Z4urce @leedave @rnadyrshin)*
> A camera suite application for the Flipper Zero ESP32-CAM module.

**Usage:** Requires an ESP module connected via GPIO. Launch the app and follow the on-screen prompts.

#### [ESP32GPS] Wardriver *(by @Sil333033)*
> Sniff WiFi access points with GPS location coordinates

**Usage:** Requires an ESP module connected via GPIO. Launch the app and follow the on-screen prompts.

#### [ESP8266] Deauther *(by @SequoiaSan & @xMasterX)*
> DSTIKE Deauther module interface, based on ESP8266

**Usage:** Requires an ESP module connected via GPIO. Launch the app and follow the on-screen prompts.

#### [ESP8266] Deauther v2 *(by @Timmotools & @xMasterX)*
> Works with ESP8266 Deauther v2 by @SpacehuhnTech (github) flipperzero_esp8266_deautherv2 Flipper Zero esp8266 deauther app. Based off the WiFi Marauder App from 0xchocolate.

**Usage:** Requires an ESP module connected via GPIO. Launch the app and follow the on-screen prompts.

#### [ESP8266] IFTTT Btn
> Turns your Flipper Zero into a wireless IFTTT trigger button using an ESP8266 module. Press a button on the Flipper to fire IFTTT webhooks and automate smart home actions.

**Usage:** Requires an ESP8266 module connected via GPIO. Configure your IFTTT webhook key in the app settings, then press OK to trigger your configured IFTTT applets.

#### [ESP] ESP Flasher
> Flash firmware onto ESP32/ESP8266/ESP32-S2/S3/C3 modules directly from your Flipper Zero. Includes built-in firmware binaries for popular projects like Marauder, Evil Portal, and more.

**Usage:** Connect your ESP module to the Flipper's GPIO pins. Launch the app, select the target board and firmware to flash, then press OK to begin. Progress is shown on screen.

#### [ESP] WiFi Scanner *(by @SequoiaSan & @xMasterX)*
> WiFi scanner module interface, based on ESP8266

**Usage:** Requires an ESP module connected via GPIO. Launch the app and follow the on-screen prompts.

</details>

<details><summary><h3>🏠 Main — 1 apps</h3></summary>

#### Clock *(by Lambda)*
> A clock with stopwatch and 80 seconds alarm. 80 second alarm can be changed at 5 second intervals and so can the sound options. Dab Timer / Clock (By Lambda) - We were the first clock created for Flipper Zero - Has 10 different clock faces

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

</details>

<details><summary><h3>🏷️ Rfid — 8 apps</h3></summary>

#### DCF77 Clock Sync *(by mdaskalov)*
> Emulate DCF77 time signal on the RFID antena and the A4 GPIO pin Flipper-Zero DCF77 Clock Sync Emulates the DCF77 time signal on the RFID antenna and on GPIO A4 pin. Uses PWM with frequency of 77.5 kHz on the GPIO pin to simulate the signal.

**Usage:** Launch from the RFID/Apps menu. Hold an RFID tag near the Flipper's bottom edge to read.

#### DCF77 Transmitter *(by arha & xMasterX)*
> Sends the DCF77 time signal (badly) on the 125khz LFRFID antenna and on GPIO C3 pin DCF77 Master Wall Clock? Sends the DCF77 time signal (badly) on the 125khz LFRFID antenna. This should allow you to update radio clocks.

**Usage:** Launch from the RFID/Apps menu. Hold an RFID tag near the Flipper's bottom edge to read.

#### EM4100 Key Generator *(by @Milk-Cool)*
> Generates EM4100 key lists from selected rfid key file for RFID fuzzer app fz-em4100-generator

**Usage:** A program that generates universal keys from a EM4100 key Click the FAP badge, connect your flipper and click "Install".

#### FDX-B Maker *(by snowsign / Skye Gibbs)*
> FDX-B format file maker for animal microchips Construct fully ISO-compliant FDX-B animal microchip data right on your Flipper Zero! The data gets stored as plain RFID files on the SD card, which you can then write to T5577 or EM4305 cards/chip... 0 to 999.

**Usage:** Launch from the RFID/Apps menu. Hold an RFID tag near the Flipper's bottom edge to read.

#### Simultaneous UHF RFID Reader *(by @RileyHaffner)*
> Simultaneous UHF RFID Reader that supports the M6E Nano, M7E Hecto, and YRM1000 series Readers. Read up to 150 UHF tags per second [Using ThingMagic Readers]!

**Usage:** Launch from the RFID/Apps menu. Hold an RFID tag near the Flipper's bottom edge to read.

#### T5577 Multiwriter *(by @Leptopt1los)*
> Application for writing several keys to one t5577 T5577 multivriter fap for flipper zero PoC of https://forum.dangerousthings.com/t/hack-store-2-to-3-different-em41xx-on-a-single-t5577/12116

**Usage:** Launch from the RFID/Apps menu. Hold an RFID tag near the Flipper's bottom edge to read.

#### T5577 Raw Writer *(by Torron)*
> @README.md T5577 Raw Writer App An easy to use T5577 raw writer app for Flipper Zero. Discord project page. Configure the modulation, RF Clock, number of blocks, and block data in the 'Config' menu.

**Usage:** Launch from the RFID/Apps menu. Hold an RFID tag near the Flipper's bottom edge to read.

#### YRM100 UHF RFID
> [UHF]RFID App for FlipperZero a UHF RFID application developed for FlipperZero, a versatile multi-tool device. The app leverages the YRM100 module to enable UHF RFID functionality.

**Usage:** - Added a settings page to allow for changing of the module's settings.

</details>

<details><summary><h3>⚡ Gpio/Flipperhttp — 11 apps</h3></summary>

#### FlipDownloader *(by JBlanked)*
> Download apps and assets via WiFi directly to your Flipper Zero - Discord: https://discord.gg/5aN9qwkEc6 - YouTube: https://www.youtube.com/@jblanked - Instagram: https://www.instagram.com/jblanked

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipLibrary *(by JBlanked)*
> Utilize WiFi to retrieve data from 20 different APIs FlipLibrary App for Flipper Zero

**Usage:** The FlipLibrary app for Flipper Zero is a versatile and user-friendly application that offers a wide range of features to enhance your Flipper Zero... The app includes a dictionary, random facts, and numerous additional functionalities, all accessible directly from your Flipper Zero device. It is ...

#### FlipMap *(by JBlanked)*
> Find Flipper Zero Users A Flipper Zero app to find Flipper Zero Users. - Wi-Fi Developer Board, BW16, Raspberry Pi, or ESP32 device flashed with FlipperHTTP v2.1 or higher: https://github.com/jblanked/FlipperHTTP - 2.4 GHz or 5 GHz Wi-Fi access point

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipSocial *(by JBlanked)*
> Social media platform for the Flipper Zero. The first social media app for Flipper Zero. Connect with other users directly on your device through WiFi. The highlight of this app is customizable pre-saves, which, as explained below, aim to address the challenges of typing with the directional pad.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipTelegram *(by JBlanked)*
> Flipper Zero Telegram Client Flipper Zero Telegram Client - WiFi Developer Board, Raspberry Pi, BW16, or ESP32 device flashed with FlipperHTTP v2.1 or higher: https://github.com/jblanked/FlipperHTTP - 2.4 GHz or 5 GHz WiFi access point

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipTrader *(by JBlanked)*
> Use WiFi to get the price of stocks and currency pairs on your Flipper Zero. FlipTrader is an app for the Flipper Zero that uses WiFi to fetch the prices of stocks and currency pairs directly on your device. It utilizes the FlipperHTTP flash for the WiFi Devboard, originall...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipWeather *(by JBlanked)*
> Use WiFi to get GPS and Weather information on your Flipper Zero. FlipWeather is an innovative app for the Flipper Zero that uses WiFi to fetch GPS and weather information, making your Flipper Zero even more versatile. It leverages the FlipperHTTP flash for the W...

**Usage:** - WiFi Settings Management: Allows configuration of WiFi settings (SSID and password) for current and future features that utilize network connecti...

#### FlipWiFi *(by JBlanked)*
> FlipperHTTP companion app. FlipWiFi is the companion app for the popular FlipperHTTP firmware. It allows you to scan and save Wi-Fi networks for use across all FlipperHTTP apps, set up captive portals, and send deauthenticat...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipWorld *(by JBlanked)*
> The first open-world multiplayer game, best played with the VGM.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Free Roam *(by JBlanked)*
> 3D Open World Multiplayer Game 3D Open World Multiplayer Game for the Flipper Zero - WiFi Developer Board, BW16, Raspberry Pi, or ESP32 device flashed with FlipperHTTP v2.0 or higher: https://github.com/jblanked/FlipperHTTP - 2.4 GHz or 5 GHz Wi-Fi access point

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Web Crawler *(by JBlanked)*
> Browse the web, fetch API data, and more. Browse the web, fetch API data, and more on your Flipper Zero. - WiFi Developer Board, Raspberry Pi, or ESP32 device flashed with FlipperHTTP version 1.6 or higher: https://github.com/jblanked/FlipperHTTP - 2.4 GHz WiFi Access Point

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>

<details><summary><h3>⚡ Gpio/Flipboard — 8 apps</h3></summary>

#### FlipBoard Blinky *(by jamisonderek)*
> FlipBoard Blinky turns your FlipBoard into a blinky badge. The FlipBlinky application turns your Flipper Zero + FlipBoard into a blinky badge for wearing at conferences. The application is written in C and uses the FlipBoard common code for the majority o...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipBoard Blinky
> FlipBoard Blinky turns your FlipBoard into a blinky badge. The FlipBlinky application turns your Flipper Zero + FlipBoard into a blinky badge for wearing at conferences. The application is written in C and uses the FlipBoard common code for the majority o...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipBoard Keyboard
> FlipBoard Keyboard turns your FlipBoard into a keyboard. The FlipKeyboard application turns your Flipper Zero and FlipBoard into a MacroPad Keyboard. It also becomes a musical light-up keyboard that you can use to play songs. It supports both USB and BL...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipBoard Keyboard *(by jamisonderek)*
> FlipBoard Keyboard turns your FlipBoard into a keyboard. The FlipKeyboard application turns your Flipper Zero and FlipBoard into a MacroPad Keyboard. It also becomes a musical light-up keyboard that you can use to play songs. It supports both USB and BL...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipBoard Signal
> FlipBoard Signal turns your FlipBoard into a signal sender.

**Usage:** The FlipSignal application turns your Flipper Zero and FlipBoard into an Infrared and Sub-GHz signal sender. Turn on/off devices using the buttons... The application sends a Sub-GHz signal (RAW or Protocol) and then it sends any IR signals. Configuration of signals is done by saving files using ...

#### FlipBoard Signal *(by jamisonderek)*
> FlipBoard Signal turns your FlipBoard into a signal sender.

**Usage:** The FlipSignal application turns your Flipper Zero and FlipBoard into an Infrared and Sub-GHz signal sender. Turn on/off devices using the buttons... The application sends a Sub-GHz signal (RAW or Protocol) and then it sends any IR signals. Configuration of signals is done by saving files using ...

#### FlipBoard Simon *(by codeallnight)*
> Simon memory game for the FlipBoard. The FlipSimon application turns your Flipper Zero and FlipBoard into a memory game. FlipSimon was originally written as a step-by-step tutorial to teach C, Flipper Zero Code, and FlipBoard common ...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### FlipBoard Simon *(by codeallnight)*
> Simon memory game for the FlipBoard. The FlipSimon application turns your Flipper Zero and FlipBoard into a memory game. FlipSimon was originally written as a step-by-step tutorial to teach C, Flipper Zero Code, and FlipBoard common ...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>

<details><summary><h3>📦 Misc — 3 apps</h3></summary>

#### FlipPass
> FlipPass lets Flipper Zero open a KeePass vault and send credentials to another device through USB HID or Bluetooth HID. - Browse KeePass KDBX 4 vaults stored on the SD card. - Navigate groups and entries without modifying the database.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### hc11_modem
> HC-11 wireless modem emulator for the Flipper Zero Flipper Zero app send data to / from a HC-11 wireless RF UART communication module. See here, here, here, here or here for a few distributors of the HC-11 module around the world.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### Prince Of Arabia
> FlipperPrinceOfArabia Inspired by a timeless classic, Press-Play-On-Tape presents 'Prince of Arabia'! Escape the dungeons and free the princess!

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

</details>

<details><summary><h3>⚡ Gpio/Nrf24 — 11 apps</h3></summary>

#### [NRF24] Batch
> Flipper Zero application for nRF24L01 external board. Sends batch commands. Приложение для Flipper Zero, предназначено для отправки пакетных команд на удаленные устройства, использующие радио nRF24L01. Можно использовать для настройки или чтения данных с удаленного устройства.

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### [NRF24] Channel Scan *(by HTotoo)*
> Scans 2.4Ghz frequency for usage data.

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### [NRF24] Mouse Jacker *(by @mothball187 & @xMasterX)*
> App works with NRF24 Sniffer app to perform mousejack attacks

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### [NRF24] Scanner
> NRF24 scanner with logging and resend ability for Flipper Zero An NRF24 driver for the Flipper Zero device. The NRF24 is a popular line of 2.4GHz radio transceivers from Nordic Semiconductors. NRF24L01+ Enhanced ShockBurst packet decoder example using Python: nrf24_packet_decoder.py

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### [NRF24] Sniffer *(by @mothball187 & @xMasterX)*
> App captures addresses to use with NRF24 Mouse Jacker app to perform mousejack attacks

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### AZERTY Mouse Jacker *(by mothball187 & xMasterX)*
> App works with NRF24 Sniffer app to perform mousejack attacks flipperzero-nrf24 An NRF24 driver for the Flipper Zero device. The NRF24 is a popular line of 2.4GHz radio transceivers from Nordic Semiconductors. This library is not currently complete, but functional.

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### FZ nRF24 Jammer *(by W0rthlessS0ul)*
> The FZ nRF24 jammer is a powerful tool that requires an Flipper Zero and NRF24 module to assemble. It is designed to create interference, disrupting the normal operation of Bluetooth devices, BLE technology, drones, Wi-Fi networks and Zigbee.

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### Mouse Jacker MS *(by coded-with-claws)*
> Applications Mousejacker for Microsoft mouse (hardcoded) flipperzero-tools Various tools for Flipper Zero See README inside directory.

**Usage:** Generates .sub for given 12bits keys with CAME and NICE protocols. It's an adaptation from UberGuidoZ's CAME_brute_force and there is also code re-use from tobiabocchi's flipperzero-bruteforce. URLs of those code b...

#### NRF24 Jammer *(by huuck)*
> 2.4Ghz jammer leveraging NRF24 For educational purposes only, inside a confined environment (e.g. faraday cage). JAMMING IS ILLEGAL! Flipper Zero nRF24 Jammer

**Usage:** Simple Flipper Zero nRF24 jammer for the 2.4Ghz spectrum I use to study wireless protocol hardening. Turn your Flipper Zero into a bluetooth or wif... For custom channels navigate to custom by pressing UP and then LEFT to select a .txt file in format ch1,ch2,ch3 and so on.

#### NRF24 Monitor *(by CyberDemon73)*
> Monitor and analyze NRF24L01+ wireless traffic NRF24L01+ Tester for Flipper Zero !Flipper Zero with NRF24 Module A real-time SPI communication tester for NRF24L01+ modules on Flipper Zero, featuring hardware status monitoring and register reading capabilities.

**Usage:** Requires an NRF24 module connected via GPIO/SPI. Launch the app and use the d-pad to navigate options.

#### Sniffer MS *(by coded-with-claws)*
> Applications NRF Sniff & Mousejacker for Microsoft mouse (hardcoded) flipperzero-tools Various tools for Flipper Zero See README inside directory.

**Usage:** Generates .sub for given 12bits keys with CAME and NICE protocols. It's an adaptation from UberGuidoZ's CAME_brute_force and there is also code re-use from tobiabocchi's flipperzero-bruteforce. URLs of those code b...

</details>

<details><summary><h3>⚡ Gpio/Malveke — 8 apps</h3></summary>

#### [GB/GBC] Cartridge *(by Esteban Fuentealba)*
> GAME BOY Cartridge (GB/GBC) MALVEKE MALVEKE Prototype V2.3 Official \| Unleashed \| RogueMaster \| Xtreme

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GB] Emulator *(by Esteban Fuentealba)*
> GAME BOY Emulator (POC). GAME BOY Emulator (POC) MALVEKE From Talcahuano 🇨🇱 with ❤

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GB] Link-Camera *(by Esteban Fuentealba)*
> extract your GAME BOY Camera picture via WIFI, so they can be easily shared with your phone, tablet or pc. Easy to use, just hook up to your GAME BOY and print as usual, the device will store the images and share them on a web server via WIFI.

**Usage:** These instructions assume that you are starting at the Flipper Zero desktop. Otherwise, press the Back button until you are at the desktop.

#### [GB] Live Camera *(by Esteban Fuentealba)*
> Insert a GAME BOY Camera cartridge, you can use it as a camera and take snapshots from the Flipper Zero. GAME BOY Live Camera MALVEKE MALVEKE Prototype V2.3 Official \| Unleashed \| RogueMaster \| Xtreme

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GB] Photo *(by Esteban Fuentealba)*
> Game Boy Camera save RAM photo to BMP from the Flipper Zero. GAME BOY PHOTO MALVEKE Official \| Unleashed \| RogueMaster \| Xtreme :- \| :- \| :- \| :-

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GB] PIN Test *(by Esteban Fuentealba)*
> ESP32 GAME BOY CAMERA live feed and photo capture, use left/right for exposition and center for saving a screenshot.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [GBA] Cartridge *(by Esteban Fuentealba)*
> GAME BOY ADVANCE Cartridge MALVEKE MALVEKE Prototype V2.3 Official \| Unleashed \| RogueMaster \| Xtreme

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### GB Printer *(by kbembedded)*
> Print photos from camera to flipper, saves are sequentially numbered and in a format meant for importing. Flipper Zero Game Boy Printer Emulator A Game Boy Printer Emulator for Flipper Zero that saves images as PNG on the microSD card. Hardware Interface

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>

<details><summary><h3>⚡ Gpio/Gps — 2 apps</h3></summary>

#### [NMEA] GPS *(by @ezod & @xMasterX)*
> Works with GPS modules via UART, using NMEA protocol. GPS for Flipper Zero A simple Flipper Zero application for NMEA 0183 serial GPS modules. Heavy lifting (NMEA parsing) provided by [minmea].

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [NMEA] Nearby Files *(by Stichoza)*
> A GPS-enabled file browser that displays files sorted by distance from your current location. A GPS-enabled file browser for Flipper Zero that displays SubGHz, NFC, and RFID files sorted by distance from your current location.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>

<details><summary><h3>🔑 Ibutton — 2 apps</h3></summary>

#### iButton Converter *(by @leptopt1los)*
> Cyfral and Metakom to Dallas converter iButton Converter iButton Converter is a Flipper Zero application designed for converting iButton key dumps from Metakom or Cyfral formats into the Dallas format. The application supports multiple conversion modes f...

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

#### iButton Fuzzer *(by gid9798 xMasterX)*
> Fuzzer for ibutton readers Flipper Zero app for checking ibutton and RFID(125khz) readers . Not meant for any illegal activity/purposes. You can check your readers for vulnerabilities using the default UIDs and frequently used UIDs.

**Usage:** Launch from the Applications menu on your Flipper. Use d-pad to navigate and OK to confirm.

</details>

<details><summary><h3>⚡ Gpio/Mayhem — 6 apps</h3></summary>

#### [MAYHEM] Camera *(by Z4urce)*
> ESP32-CAM live feed and photo capture, use left/right for orientation/mode, up/down for brightness and center for saving a screenshot. [Unplug the USB cable to test with Mayhem]

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [MAYHEM] Marauder
> ESP32-CAM version of Marauder. Includes all functionality from the original plus some options to trigger the camera and flashlight. [Unplug the USB cable to test with Mayhem]

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [MAYHEM] Morse Flash
> ESP32-CAM app to stream a message in morse using the powerful flashlight. [Unplug the USB cable to test with Mayhem] UART Terminal for Flipper Zero Flipper Zero app to control various devices via UART interface. - Read log and command output by uart

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [MAYHEM] Motion Detection *(by eried)*
> ESP32-CAM Motion detection. It generates a beep when motion is detected. Can be extended to trigger more stuff in the code. [Unplug the USB cable to test with Mayhem]

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [MAYHEM] Nanny Cam *(by eried)*
> ESP32-CAM simple app to start a remote camera. [Unplug the USB cable to test with Mayhem]

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [MAYHEM] QR Code *(by eried)*
> ESP32-CAM simple app to show a payload from QR codes. Can be extended to trigger more stuff in the code. [Unplug the USB cable to test with Mayhem]

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>

<details><summary><h3>⚡ Gpio/Games — 2 apps</h3></summary>

#### UART Pong *(by Olejka)*
> A multiplayer ping pong game using UART

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### VL6180X Pong *(by Kyle Diller)*
> Simple pong game with control from a VL6180X

**Usage:** This is a copy of the Flipper Zero Pong game, but uses a VL6180x module to control the paddle instead of the buttons. See the VL6180X README for more details on setting the Flipper up with the VL6180X.

</details>

<details><summary><h3>⚡ Gpio/Vgm — 4 apps</h3></summary>

#### [VGM] Air Mouse
> Turn Flipper Zero with the Video Game Module into an air mouse

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### [VGM] Video Game Module Tool
> This app is a standalone firmware updater/installer for the Video Game Module

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### VGM Game Remote *(by JBlanked)*
> Companion app for the VGM Game Engine Collection of files for the Video Game Module. The VGM Engine is not affiliated with Flipper Devices and utilizes the PicoDVI and pico-game-engine libraries. Developers can create their own games or recreate their Flipper games with the advanta...

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

#### Video Game Module Console *(by JBlanked)*
> Flipper Zero console handling game display rendering and input forwarding Video Game Module Console Turn your Flipper Zero into a console for the Video Game Module — handles display output and button input so developers can focus on their hardware. 1.

**Usage:** Connect the required hardware to GPIO pins. Launch from the GPIO menu and use d-pad to navigate.

</details>


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
