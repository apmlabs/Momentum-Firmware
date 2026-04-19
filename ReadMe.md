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

<details><summary><h3>🎮 Games — 141 apps</h3></summary>

#### 1D Pacman *(by easiwork)*
> One-dimensional Pacman — the classic dot-munching game compressed into a single horizontal line. Navigate left and right to eat dots while avoiding a ghost. Built on the Flipper Zero Game Engine.

#### 2048 *(by @eugene-kirzhanov)*
> Slide tiles on a 4×4 grid to combine matching numbers and reach 2048. Use d-pad to slide all tiles in one direction. Progress saves automatically on exit. Can go up to 65K.

#### 2048 (Original) *(by OlegSchwann)*
> Alternative version of the 2048 tile-sliding puzzle. Same core gameplay — combine matching numbers on a 4×4 grid — with a different visual style.

#### 4 in a Row *(by leo-need-more-coffee)*
> Classic connect-four game. Drop pieces into columns to get four in a row before your opponent. Two-player on one device.

#### 99 Bottles *(by bolknote)*
> Displays the lyrics to "99 Bottles of Beer on the Wall." Scroll through the verses with the d-pad. A novelty/joke app.

#### Air Arkanoid
> Arkanoid game that supports the Video Game Module motion sensor. Tilt to move the paddle instead of using buttons. Falls back to d-pad controls without VGM.

#### Air Labyrinth *(by @CodeAllNight (MrDerekJamison))*
> Labyrinth game with VGM motion sensor. Tilt the device to guide a ball through a maze. Can also be played with the d-pad if no VGM is attached.

#### ArduGolf *(by apfxtech)*
> 3D minigolf game with real ball physics, ported from Arduboy. Play through 18 holes — plan your shots, bounce off walls, and use slopes to finish each course in as few strokes as possible. D-pad to aim and shoot.

#### Ardudrivin *(by apfxtech)*
> Retro racing game ported from Arduboy. Runs at 67 FPS with smooth pseudo-3D road rendering. Steer left/right to avoid obstacles and stay on the road.

#### Arduventure *(by apfxtech)*
> Retro RPG/adventure ported from Arduboy. Explore a pixel world across 4 locations, fight enemies in turn-based combat, and grow your character. Features 8-bit music and character progression.

#### Arkanoid *(by @xMasterX & @gotnull)*
> Classic brick-breaking game. Use left/right to move paddle, bounce ball to destroy all bricks.

#### Asteroids *(by @antirez & @SimplyMinimal)*
> Space asteroids shooter. Rotate with left/right, thrust with up, shoot with OK. Features auto rapid-fire, title screen, and Drone Buddy power-up.

#### Avocado Zero *(by Endika)*
> Avocado pit care simulator. Suspend a pit over a glass of water and keep it alive long enough to grow roots. Clean the water when it gets dirty, manage hydration levels. Days tick by in real time — neglect it too long and it's game over. Win when roots reach full length.

#### Banana *(by DrEverr)*
> Banana clicker/tracker. Tap OK to increment your banana count. Features banana facts, consumption tracking, and the simple joy of watching a number go up. A novelty app by DrEverr.

#### BlackJack *(by @teeebor)*
> Casino blackjack card game. Hit, stand, or double down to beat the dealer to 21. Dealer follows traditional casino rules.

#### Blackjack By RocketGod *(by RocketGod-git)*
> Alternative blackjack implementation with a different visual style. Full blackjack gameplay — hit, stand, bust. Dealer follows standard casino rules.

#### Bomber Fox *(by Electric Fox)*
> Two-player Bomberman over Sub-GHz radio. Choose Wolf or Fox, then battle on the same map wirelessly. Fox is Player 1 and picks the level. D-pad to move, OK to place bombs. Requires two Flippers.

#### Bomberduck *(by @leo-need-more-coffee & @xMasterX)*
> Bomberman-style game. Place bombs to destroy walls and enemies while avoiding your own explosions.

#### BzzBzz *(by Koray Er)*
> Haptic rhythm-matching game using vibration instead of visuals. The Flipper generates a 5-step vibration sequence with randomized timing gaps (200/500/800ms). Replicate the pattern using any button. Accuracy is calculated from timing offsets — need 60%+ to advance. Patterns auto-repeat after 5 seconds of inactivity.

#### CarJacker *(by RocketGod-git)*
> Satirical pirate-themed car stealing game. Navigate menus with the d-pad in this tongue-in-cheek adventure. A joke/novelty game — no actual vehicles are harmed.

#### Catacombs Of The Damned *(by apfxtech)*
> First-person dungeon crawler ported from Arduboy. Explore 10 procedurally generated floors, destroy monsters with fireballs, and collect loot. Inspired by the classic Catacomb 3D series. Features smooth real-time 3D rendering and sound effects.

#### Cells Lab *(by PilotOfAsuka)*
> Cellular automaton with genetic algorithm simulation. Cells have coordinates, energy levels, and DNA. They mutate during reproduction, lose energy each turn, die at zero energy, and reproduce when energy is high. Watch evolution play out on the 128×64 screen.

#### Checkers *(by @Claude + H4W9)*
> Classic checkers board game. Move diagonally, jump to capture opponent pieces.

#### Chess *(by Struan Clark (xtruan))*
> Full chess game for Flipper. Includes AI opponent. Select pieces with d-pad, confirm with OK.

#### Chess Clock *(by ihatecsv)*
> Simple chess clock for timed games. Two countdown timers that alternate — press OK to switch turns. Useful for any timed two-player board game.

#### Cigarette *(by fuckmaz)*
> Animated cigarette smoking simulator. Watch a cigarette slowly burn down on screen. A novelty/joke app — "finally a way to smoke on the Flipper Zero."

#### City Bloxx *(by Milk-Cool)*
> Tower-building game ported to Flipper Zero. Stack blocks as precisely as possible to build the tallest tower. Misaligned blocks get trimmed, making each level narrower.

#### Cognizant Flipper *(by Luke Gamertsfelder)*
> Random word generator that pretends your Flipper is sentient. Press OK and the Flipper "reveals" words from its "digital mind." A fun novelty app — the screen fills with cryptic messages as if the device is trying to communicate.

#### Color Guess *(by Leedave)*
> Color guessing game. Try to identify colors displayed on the 1-bit screen through patterns.

#### Connect Wires *(by AlexTaran)*
> Puzzle game about rotating pipe segments to connect a power source to consumers. Three difficulty levels. Rotate pieces to create a complete circuit. A classic pipe-puzzle concept.

#### CountDown *(by Oscar Rodriguez)*
> Numbers game inspired by the Countdown TV show. Select 6 numbers, choose a 3-digit target, and the solver tries to reach it using +, −, ×, ÷. D-pad to move cursor, OK to select.

#### Crossy Road *(by Mikael098)*
> Frogger-style road-crossing game. Guide your character across lanes of traffic and obstacles. Work in progress.

#### DeadZone *(by retrooper)*
> Top-down shooter with AI enemies. Fight through challenges, dodge enemy fire, and avoid falling obstacles. Left/Right/Up to move, OK to fire. Enemy NPCs use neural networks (362 parameters, 37 neurons each) for AI behavior.

#### Decision Maker *(by jacki)*
> Roulette-style random decision picker. Add up to 20 custom choices (20 chars each), spin the wheel, and let hardware true-random (not software PRNG) decide for you. Smooth deceleration animation before landing on the result.

#### Devillier's Platformer
> Side-scrolling platformer. Control a character navigating through levels filled with obstacles and platforms, aiming to reach the finish line. D-pad to move and jump.

#### Dice [RM] *(by Lambda)*
> Multi-sided dice roller for tabletop/D&D games. Roll d2 (coin), d4, d6, d8, d10, d12, d20, and d100. Displays roll date/time on each roll.

#### Digital Kaleidoscope *(by J. Randall jr3d.co.uk)*
> Animated visual toy with four pattern modes: rotating starburst, expanding/contracting concentric arcs, gradient noise (brighter at center), and mirrored random dots. Press buttons to switch patterns. A mesmerizing screensaver-style app.

#### DnD Dice *(by Ka3u6y6a)*
> Dice rolling app for tabletop RPGs. Supports coin flip, d4, d6, d8, d10, d12, d20, and d100. Clean interface with visual dice display.

#### DOOM *(by @xMasterX & @Svarich & @hedger (original code by @p4nic4ttack))*
> Yes, it runs Doom. A Doom-like clone adapted for the 128×64 screen. Navigate with d-pad, shoot with OK.

#### Drifter *(by Jed Lejosne)*
> Boat steering game inspired by the TI-89 classic. Use left/right to affect drift — longer keypresses have bigger impact. Score multiplier increases when no key is pressed, rewarding steady hands. OK to restart after a crash.

#### DVD Bouncer
> The classic DVD screensaver logo bouncing around the screen. Watch it and wait for it to hit the corner perfectly. That's the whole app.

#### Executor Keychain *(by Esteban Fuentealba)*
> Emulates the iconic 80s Echo Keyller/Executor Keychain sounds. 8 retro laser-style sound effects played through the Flipper's speaker. A nostalgia trip for anyone who remembers these classic keychains.

#### Fighter Jet *(by Erbonator3000)*
> Simple fighter jet game. Fly around, shoot enemies, and don't lose your health. Left/Right to turn, OK to shoot, Down to brake. A demo/prototype.

#### Five Nights at Flipper's
> FNAF-style horror game for Flipper Zero. Up for door button, Down for lights, OK for camera monitor. Long-press Back for main menu. Survive the night by managing doors and cameras.

#### Flappy Bird *(by @DroomOne)*
> The classic mobile game. Press any button to flap, navigate through pipes.

#### Flight Assault *(by evillero)*
> Space combat game. Control a spacecraft fighting enemy ships approaching from different directions. Maneuver to avoid collisions, destroy enemies to score points. Play as long as you can for the highest score.

#### Flipper Hero *(by Mentoster)*
> Arrow-speed reaction game. Arrows appear on screen and you must press the matching d-pad direction as fast as possible. Tests reflexes and pattern recognition.

#### Flippy Road *(by rkilpadi)*
> Frogger-style arcade game. Cross roads and rivers by timing your movements between obstacles. Simple controls — d-pad to move.

#### Fortune Cookie *(by evillero)*
> Random motivational quote generator. Open the app and receive a random fortune or inspirational quote. Press OK for a new one. Simple and uplifting.

#### Furious Birds *(by Dmitry Ermashev)*
> Angry Birds-style physics game. Launch a furious bird at pigs hiding behind structures. Each level has more pigs to eliminate. You get 3 attempts per level. Aim and adjust power to knock them all down.

#### Game 15 *(by @x27)*
> Sliding puzzle (15-puzzle). Slide numbered tiles to arrange them in order.

#### Game of Life *(by @tgxn (original by @itsyourbedtime))*
> Conway's Game of Life cellular automaton. Watch patterns evolve or draw your own starting state.

#### Geometry Dash
> Side-scrolling rhythm game. Tap to jump over obstacles. Timing is everything.

#### Groks Adventure *(by DigiMancer3D)*
> Infinite roguelike dungeon crawler set in the Backrooms. Procedurally generated platforms, walls, and pickups on a single 128×64 screen. Battle enemies, collect items, and survive as long as possible. Features AI-driven enemy behavior.

#### Guess The Number *(by kWAYTV)*
> Enhanced number guessing game with progressive feedback. Distance-based hints (very close, close, far), persistent high score tracking, dynamic LED colors, haptic patterns, and sound effects. Fine (+/-1) and coarse (+/-10) adjustment modes.

#### Hangman *(by Evgeny Stepanischev)*
> Classic hangman word-guessing game. The Flipper picks a word, you guess letters. Wrong guesses draw the hangman. Supports four language dictionaries: English, Spanish, Russian, and Tatar.

#### Hanoi Towers *(by AlexTaran)*
> Classic Tower of Hanoi puzzle. Move discs between three pegs — only smaller discs can go on top of larger ones. Complexity increases each time you solve it.

#### Heap Defence *(by @xMasterX (original implementation by @wquinoa & @Vedmein))*
> Stack Attack arcade game. Catch falling blocks and stack them. Don't let the stack reach the top.

#### Hirn - Crack Color Codes *(by F Greil)*
> Mastermind-style code-breaking game. The Flipper creates a secret 4-symbol code from 6 colors. Guess the code and receive clues: black pegs for correct color+position, grey pegs for correct color but wrong position. Fewest attempts wins.

#### Hold 'Em
> Single-player Texas Hold'em poker. Play against up to 4 AI bots with real betting rounds, side-pot-aware showdowns, and save/load support. Full poker rules with a UI designed for the 128×64 screen.

#### Hunter Killer *(by josephburnett)*
> Submarine warfare simulation ported from Pico-8. Navigate using heading and velocity controls, use sonar to discover procedurally generated terrain (diamond-square algorithm), and fire torpedoes to destroy obstacles. Realistic submarine physics.

#### Impostor *(by Endika)*
> Undercover/Spyfall-style party game. Pass the Flipper around — everyone except the impostors sees the same secret word; impostors see only a hint. Discuss and vote to find the impostor. Supports custom player counts and impostor numbers. No phone needed.

#### Infinite Tic Tac Toe *(by Kyle Diller)*
> Tic Tac Toe with a twist — pieces only last 3 turns, then disappear. This means the game can never end in a draw. Play continues until someone gets three in a row.

#### Insults *(by ERSUCC)*
> Random insult generator. Press OK to generate a random (humorous) insult. A novelty/joke app.

#### Jetpack Joyride
> Side-scrolling action game. Hold to fly up, release to fall. Avoid obstacles.

#### Jumping Pawns *(by Tyl3rA)*
> Strategy board game — race to move all your pieces to the other side. Play locally against a friend (PvP) or challenge a minimax AI (PvE) with adjustable difficulty.

#### KC Line *(by Andrew Diamond)*
> One-line Pac-Man variant. A character eats dots on a single row while being chased by a ghost. Left/Right to move, OK to pause. Simple but addictive.

#### Laser Tag *(by @RocketGod-git & @jamisonderek)*
> IR-based laser tag between two Flippers. Choose teams (Red vs Blue), fire with IR, detect hits automatically. Features health/ammo bars, sound effects, and RFID power-up scanning. Supports add-on blasters like the Rabbit Labs Masta-Blasta via GPIO.

#### Lifecounter *(by Antsy)*
> Life point tracker for collectible card games (Magic: The Gathering, Flesh and Blood, Lorcana, Sorcery, etc.). Track life totals for multiple players with increment/decrement buttons.

#### Lights Out Flipper Zero *(by DigiMancer3D)*
> Star Wars-themed Lights Out puzzle. Toggle cells on a 17-cell grid to turn all lights off — toggling one cell flips its neighbors too. Features an AI opponent (Sith Lord Flipper) that fights back, a scrolling Star Wars intro crawl, and themed win/loss screens.

#### Magic 8-ball *(by Steven Quinn)*
> Shake (press OK) and receive a random Magic 8-ball answer. Will it be "Yes," "No," or "Ask again later"? The classic fortune-telling toy, digitized.

#### Mancala *(by F. Greil)*
> Ancient strategy board game. 6 pits and 1 store per player, 4 stones per pit. Pick up stones from a pit and distribute them counter-clockwise. Capture opponent's stones by landing in an empty pit opposite their stones. Left/Right to select pit, OK to play.

#### Mandelbrot Set *(by Possibly-Matt)*
> Interactive Mandelbrot fractal viewer. Zoom in and explore the infinite complexity of the Mandelbrot set on the 128×64 screen. Navigate with d-pad, zoom with OK. A mathematical visualization tool.

#### Matagotchi *(by MrModd)*
> Tamagotchi-like virtual pet. Your pet grows from egg → baby → child → teen → adult → death. Feed it, play with it, keep it happy. The pet continues living even when the app isn't running — neglect has consequences.

#### Mine Sweeper *(by Alexander Rodriguez)*
> Classic minesweeper. Uncover tiles without hitting mines. Numbers show adjacent mine count. Long-press to flag.

#### Minesweeper *(by panki27 & xMasterX)*
> Alternative minesweeper implementation. Arrow buttons to move, center button to open a field, hold center to toggle flag. Press center on an open field with correct flag count to auto-open remaining neighbors.

#### Mode 7 Demo *(by CookiePLMonster)*
> Pseudo-3D rendering demo inspired by the SNES Mode 7 effect. Navigate a textured plane that rotates and scales in perspective. Up/Down/Left/Right to move camera, OK to rotate, Back to switch backgrounds. A tech demo, not a game.

#### Monster Slayer *(by ratmanZorry)*
> Action game built on the Flipper Zero Game Engine. Fight monsters in a side-scrolling environment. A showcase/example project for the game engine.

#### Monty Hall *(by DevMilanIan)*
> The Monty Hall probability puzzle. Pick a door, one wrong door is revealed, then decide: stick or switch? Demonstrates the famous counter-intuitive probability problem. Play multiple rounds to see the statistics converge.

#### Morse Master *(by P1X / w84death)*
> Comprehensive Morse code learning toolkit. Learning Mode shows visual patterns and plays audio for each character (A-Z, 0-9). Practice Mode tests your knowledge. Switch between letters and numbers with Left/Right. Audio playback with clear timing.

#### Multi Counter *(by JadePossible & Roro)*
> 4-player score counter for tabletop games. Up/Down to add/subtract from the selected player, Left/Right to switch between players. OK to toggle sound. Simple and practical for board game nights.

#### Multi-Dice
> Roll multiple dice of various types (d4, d6, d8, d10, d12, d20). Press OK to roll.

#### Mystic Balloon *(by apfxtech)*
> Atmospheric 2D platformer with 39 levels. Control a character using balloons — slowly descend while suspended, float above fan air currents to cross gaps. Collect coins, keep balloons intact, and avoid traps. Precision platforming on a tiny screen.

#### Nah 2 da Nah Nah Nah *(by DigiMancer3D)*
> Mini-game collection with multiple game modes. Zero Hero is a rhythm game — press buttons to hit falling notes with streak tracking. Flip Zip is a runner — navigate lanes and jump obstacles. More modes planned. Dynamic animated menu.

#### Network Defender *(by w84death)*
> Game & Watch-style cybersecurity game. Defend 4 computer systems from cyber attacks. Move between systems with d-pad, accept packets with OK, hold OK for 3 seconds to patch compromised systems. Prevent hacking and keep packet levels below critical threshold.

#### Nu pogodi *(by sionyx)*
> Recreation of the classic Soviet handheld game "Elektronika IM-02" (Nu, pogodi! / Well, Just You Wait!). Catch eggs rolling down from four directions before they fall. Based on the famous 1984 LCD game.

#### P1X Adventure *(by w84death)*
> Text-based adventure game. Navigate through rooms, make choices, and explore a pixel-art world. By the P1X game development collective.

#### P1X Your Own Adventure *(by w84death)*
> Choose-your-own-adventure text game. Read story passages and make decisions that branch the narrative. Multiple paths and endings.

#### PacMan WIP *(by DanieleMaffi)*
> Classic Pac-Man game (work in progress). Navigate the maze, eat dots, avoid ghosts, grab power pellets to turn the tables.

#### Panis - A Grumpy Bread *(by F Greil)*
> Platformer starring a grumpy bread character. Left/Right to move, single press Up for small jump (~25px), hold Up for big jump (~50px). Simple and charming.

#### Paper Plane *(by Larry-the-Pig)*
> Guide a paper plane through obstacles to get the highest score. Ported from a TI calculator game. The plane descends through the level — steer to avoid walls and obstacles. Score based on distance traveled.

#### Pinball0 *(by Roberto De Feo)*
> Pinball game. Use left/right for flippers, launch ball with OK.

#### Pocket Battle+ *(by HermeticCode)*
> Enhanced Pokemon battle game with authentic data from the Pokemon Yellow disassembly. All 151 original Pokemon with accurate base stats, proper type effectiveness, turn-based battles with speed-based move order, and status conditions (sleep, poison, burn, freeze, paralysis).

#### Pong *(by @nmrr & @SimplyMinimal)*
> Classic pong. Two-player on one device — one player uses up/down, other uses left/right.

#### Puck Girl *(by F Greil)*
> Pac-Man-style maze chase game. Control a pie-shaped character through a maze, collecting dots while avoiding ghosts with distinct AI personalities. Power pills make ghosts temporarily vulnerable. Arrow keys to move, OK to restart, Back to pause.

#### Quadrastic *(by @ivanbarsukov)*
> Addicting arcade game inspired by the Arduboy game. Dodge enemies and collect targets. Fast-paced with increasing difficulty.

#### Questions *(by nikilark)*
> Party game — displays questions to help people get to know each other better. Pass the Flipper around and answer the prompts. Great for social gatherings and icebreakers.

#### Race *(by zyuhel)*
> Retro racing game inspired by BrickGame 9999-in-1. Three-lane vertical scrolling — dodge oncoming vehicles. Classic handheld gaming nostalgia.

#### Race Game *(by mrc19056)*
> 3-lane vertical scrolling racer with power-ups. Dodge motorcycles (fast), sedans (normal), and boss trucks (2-lane wide). Collect shields for invincibility, magnets for auto-coin-collection, and fuel for extra lives. Coin combo system with increasing multiplier.

#### Reaction Test *(by Milk-Cool)*
> Simple reaction time test. Wait for the prompt, then press a button as fast as possible. Measures your reaction time in milliseconds.

#### Reaction Time *(by ihatecsv)*
> Visual reaction time game with multi-sensory feedback. Screen shows "Get ready..." then flashes "PRESS" with a blue LED and vibration. Press any button as fast as possible. Displays your reaction time.

#### Reversi *(by @dimat)*
> Classic Othello/Reversi board game. Place pieces to flip opponent's pieces. Long-press OK for new game menu.

#### Rock Paper Scissors *(by jamisonderek)*
> Multiplayer rock-paper-scissors over Sub-GHz radio. Play against another Flipper wirelessly. Both players choose simultaneously, results are transmitted and compared. Requires two Flippers.

#### Rock Paper Scissors 2 *(by benwoo1110)*
> Local rock-paper-scissors game. Play against the Flipper's random choice. Simple single-player version — no second device needed.

#### Roots of Life *(by @Xorboo)*
> Zen puzzle game from Global Game Jam 2023 (theme: Roots). Grow roots through the soil.

#### Rubik's Cube Scrambler
> Generates random scramble sequences for a Rubik's cube. Follow the moves (R=Right, L=Left, U=Up, D=Down, F=Front, B=Back) to scramble your physical cube. Toggle vibration notification on/off. Press "New" for a fresh scramble.

#### Sand Simulation *(by JJoeDev)*
> Falling sand physics simulation on the 128×64 screen. Watch sand particles pile up, flow, and interact with gravity. Runs at 10 FPS by design. A mesmerizing physics toy.

#### Scorched Tanks *(by @jasniec)*
> Artillery game inspired by Scorched Earth. Adjust angle and power to hit opponent tanks across terrain.

#### Secret Toggle *(by nostrumuva)*
> Grid puzzle game. Arrow buttons move around the board, OK toggles a square — but some squares are secretly connected and toggle together. Turn all squares on (light colored) to pass each level. Levels get progressively harder.

#### Simon Says *(by @SimplyMinimal & @ShehabAttia96)*
> Memory pattern game. Watch the sequence of buttons, then repeat it. Gets longer each round.

#### Slot Machine *(by @Daniel-dev-s)*
> Casino slots simulator. Press OK to spin. Match symbols to win.

#### Slots *(by vh8t)*
> 81-way slot machine with custom hand-drawn graphics. Unlike traditional slots with fixed lines, matching symbols anywhere on adjacent reels from left to right count as wins. Features haptic feedback, RGB LED celebrations, and a clean sidebar showing cash, bet, and last win.

#### Snake 2.0 *(by @Willzvul)*
> Advanced snake game. Eat food to grow, don't hit walls or yourself. D-pad to steer.

#### Snake Game
> Classic snake game — the original simple version. Eat food, grow longer, don't crash into yourself.

#### Sokoban *(by Racso)*
> Classic box-pushing puzzle game. Push all boxes onto target positions. You can only push (not pull) one box at a time. Score is based on number of pushes — fewer is better. Multiple levels of increasing difficulty.

#### Solitaire *(by doofy-dev)*
> Klondike solitaire card game. Move cards between columns, build foundation piles Ace to King.

#### Space Impact *(by Ka3u6y6a)*
> Port of the classic Nokia phone game. Side-scrolling space shooter — fly your ship and destroy enemies. Multiple enemy types with explosion effects. Nostalgic mobile gaming on the Flipper.

#### Space Impact II *(by Erbonator3000 & VoidXH)*
> Pixel-perfect clone of Nokia's Space Impact sequel. All credit to original creator Bence Sgánetz. Faithful port preserving the original gameplay on the Flipper's 128×64 screen.

#### Space Invaders *(by PavelZurek)*
> Classic Space Invaders adaptation. Move left/right, shoot up at descending alien waves. Multiple enemy types, explosions, and increasing difficulty.

#### Stratagem Zero *(by Nymda)*
> Stratagem Hero clone (from Helldivers 2). Input directional sequences as fast as possible to call in stratagems. Designed to be as close to the original as possible. Practice your stratagem inputs on the go.

#### Sudoku *(by profelis)*
> Classic 9×9 Sudoku puzzle. OK to increment number, Back to clear, long-press Back to pause. Cursor keys to navigate the grid. Fill every row, column, and 3×3 box with digits 1-9.

#### SUPER-CHIP *(by Milk-Cool)*
> CHIP-8 / SUPER-CHIP emulator. Load .ch8 ROM files from the SD card (stored in apps_data/schip/) and play classic CHIP-8 games. Supports the extended SUPER-CHIP instruction set for higher resolution games.

#### Swimmy Fish *(by Invizabel)*
> Flappy Bird variant with a swimming fish theme. Tap to swim upward, avoid obstacles. Same addictive one-button gameplay with an underwater twist.

#### T-Rex Runner *(by @Rrycbarm)*
> Chrome browser T-Rex game port. Press OK to jump over cacti and duck under birds. Features moving background, gravity physics, and increasing speed.

#### Tamagotchi
> Virtual pet (P1 emulator). Feed, play, and care for your digital pet. Stack fixed to 4KB in our build. Press Up for menu (layout, speed, save). Long-press Back to save & exit.

#### Tanks
> Tank battle game. Navigate maze, shoot opponents. D-pad to move, OK to fire.

#### Tarot *(by pionaiki & tihyltew)*
> Tarot card reader with major arcana spread. Draws cards in a linear spread with upright and reversed positions. Uses radio-based random number generation for true randomness. Minor arcana planned for future update.

#### Tetris *(by @xMasterX & @jeffplang & @noiob)*
> Classic Tetris. Rotate pieces with OK, move with left/right, drop with down. Clear lines to score.

#### Tetris Modern *(by xMasterX & jeffplang & noiob & Alloy-Lou)*
> Updated Tetris implementation with modern features. Same classic gameplay — rotate and drop tetrominoes to clear lines — with quality-of-life improvements.

#### Tic Tac Toe *(by @xMasterX & @gotnull)*
> Two-player on one device. Take turns placing X and O on a 3×3 grid.

#### Tic-Tac-Toe Multi *(by RouNNdeL)*
> Multiplayer Tic-Tac-Toe — play against another Flipper wirelessly or locally. Same classic rules with networked play support.

#### Ultimate Tic-Tac-Toe *(by Racso)*
> Strategic variant with 9 Tic-Tac-Toe boards in a 3×3 grid. Your move determines which board your opponent plays on next. Win small boards to claim them, win 3 small boards in a row to win the game. Much deeper strategy than regular Tic-Tac-Toe.

#### Umpire Indicator *(by @RocketGod-git)*
> Baseball/softball ball-strike-out counter. Use d-pad to increment counts during a game.

#### Vexed *(by dlvoy)*
> Classic Palm OS puzzle game reborn. Clear levels by pushing matching colored bricks together — they disappear when touching. Gravity pulls bricks down after each move. Goal: clear all bricks in the fewest moves. Multiple level sets from the original game.

#### Video Poker *(by @PixlEmly)*
> Five-card draw poker. Select cards to hold, draw replacements, try to make the best hand.

#### Wave *(by sergo)*
> Side-scrolling obstacle avoidance game. Hold OK or Up to fly upward, release to fall. Avoid obstacles on the way. Inspired by Geometry Dash. Features pause (Back button) and language switching.

#### Wolfenduino *(by apfxtech)*
> Wolfenstein 3D demake ported from Arduboy FX. Play through 10 levels of the shareware episode "Escape from Wolfenstein" with first-person 3D gameplay adapted for the 128×64 screen. Classic FPS action on a tiny display.

#### Yappy Invaders *(by @dagnazty)*
> Space Invaders variant. Move left/right, shoot up at descending alien waves.

#### Yatzee *(by @emfleak)*
> Yahtzee dice game. Roll dice up to 3 times per turn, score in categories.

#### ZERO! *(by Racso)*
> UNO-style card game against AI opponents. Match cards by suit, number, or action. Draw if you can't play. Action cards (skip, reverse, draw) add strategy. First player to empty their hand wins.

#### Zombiez *(by @DevMilanIan & @xMasterX)*
> Defend your walls from approaching zombies. Shoot them before they break through.

</details>

<details><summary><h3>📡 Nfc — 49 apps</h3></summary>

#### AmiTool *(by Firefox2100)*
> NTAG215 toolkit for gaming console tags. Read, write, emulate, generate, and randomize UIDs. Place `key_retail.bin` in `apps_data/ami_tool/` for full features. Read physical tags, generate from character database, emulate for consoles, write to blank NTAG215 tags, or randomize UIDs so one tag works as multiple different tags.

#### Amusement IC *(by object-Object)*
> Reader for amusement/arcade IC cards. Displays vendor name, PMm (manufacturer parameters), and other card data. Useful for exploring the NFC technology behind arcade and amusement park systems.

#### Cyborg Detector *(by @RocketGod-git)*
> Generates a continuous NFC field to make body modification implant LEDs glow. Hold the Kiisu near an NFC implant (typically between thumb and index finger) and the field powers the tiny LED inside, making it light up through the skin. No buttons needed — field stays active until you press Back.

#### GhostBook *(by Digi)*
> Encrypted NFC contact sharing. Store your contact info and share it with a tap — emulates NTAG215 for compatibility with other Flippers. Protected by a variable-length passcode (6-10 button combinations, 46K to 60M possibilities). Auto-wipes on failed unlock attempts.

#### ISO 15693-3 NFC Writer
> Advanced read/write tool for ISO 15693-3 NFC tags (ICODE SLIX). Write Single Block, FF to All Blocks (quick format), Lock Block (permanent — cannot be undone), Write AFI (Application Family Identifier), and Write DSFID (Data Storage Format). Common in library systems, industrial tracking, and access control.

#### Metroflip *(by luu176)*
> Transit card reader — port of the Metrodroid project. Reads and parses metro/transit cards worldwide, extracting trip history, balance, card number, and stored data. Supports Suica (Japan), RENFE (Spain), Bip (Chile), CharlieCard (Boston), and many more. Hold your transit card flat against the back of the Kiisu.

#### MFDesfire Auth *(by Gatorixx / NullPageGh0st)*
> Legacy authentication tool for MIFARE DESFire cards. Performs authentication handshakes with DESFire cards using legacy protocols. Useful for security research and understanding DESFire authentication mechanisms.

#### MFKey *(by @noproto)*
> MIFARE Classic key recovery tool (v4.1). Calculates encryption keys on-device using Mfkey32 and Nested algorithms. Use after the built-in NFC app's "Extract MF Keys" collects nonces. 40% faster than v3, Static Encrypted Nested (SEN) dictionary makes recovery 10x faster. Keys saved to NFC cache automatically.

#### MFP Reader *(by Defensor7)*
> MIFARE Plus SL3 reader, dumper, and emulator. Implements the full MFP SL3 protocol over ISO 14443-4A using only stock firmware APIs — no firmware modifications required. Read, dump, and emulate MIFARE Plus Security Level 3 cards.

#### Mi Band NFC *(by LucaTomei)*
> NFC data manager for Xiaomi Mi Band devices. Read, write, emulate, and verify NFC data on Mi Band smart bands. Quick UID check, load NFC dumps from SD card, automatic backups, detailed logging, and real-time progress tracking during writes.

#### MIFARE Classic Editor *(by TollyH)*
> View and edit raw hex data inside saved MIFARE Classic .nfc files, sector by sector. Browse to any .nfc file, view hex dump organized by sector, modify individual bytes with OK. Save changes and emulate or write to a blank card using the main NFC app.

#### Mifare Fuzzer *(by @spheeere98 @Sil333033)*
> Emulate MIFARE Classic cards with various UIDs to test reader behavior. Cycles through random, sequential, or file-based UIDs. Helps identify readers that rely solely on UID-based authentication (which is insecure). Logs successful UIDs.

#### MiZip Balance Editor *(by Téo Humeau)*
> Modify MiZip tag balances directly. Load a MiZip NFC dump, edit the stored balance value, and write it back. Work in progress — contributions welcome.

#### COGS MyKey *(by luhf)*
> Read and edit COGES MyKey NFC dongles used in vending machines. Released as open-source because others were selling similar tools in violation of the MIKAI license. For educational purposes only.

#### Networking NFC + QR *(by DonJulve)*
> Dual-mode contact sharing — simultaneously emulates an NFC card and displays a QR code on screen. Perfect for networking events: the other person can either tap their phone to the NFC or scan the QR code. Load .nfc and .qrcode files from SD card.

#### NFC APDU Runner *(by SpenserCai)*
> Run sequences of APDU commands against NFC cards from pre-written script files. Create .apdu files with hex commands (one per line) in `apps_data/nfc_apdu_runner/`. The app sends each command and displays the card's response. Requires knowledge of the target card's APDU command set.

#### NFC Comparator *(by acegoal07)*
> Compare a physical NFC card against a saved digital copy. Checks UID, UID length, and protocol. Also includes a finder feature that searches for matches between physical cards and your saved collection — useful for finding duplicates or locating which saved file matches a card in hand.

#### NFC Dict Manager *(by grugnoymeme a.k.a. 47LeCoste)*
> Advanced NFC dictionary management tool. Manage, optimize, and organize mf_classic_dict.nfc and mf_classic_dict_user.nfc files directly on-device without a PC. Backup dictionaries, merge files, remove duplicates, and organize keys. Six management options from the main menu.

#### NFC-Eink *(by RebornedBrain)*
> Write images to NFC-powered e-ink displays from Waveshare and Goodisplay. Supports 2.13", 2.7", 2.9", 4.2", and 7.5" Waveshare displays plus several Goodisplay models. Emulate or write image data directly. Save/load screen data between different display types.

#### NFC Fuzzer *(by PentestSuite & barkandbite)*
> NFC protocol fuzzer with 11 profiles and 4 strategies for testing reader/tag robustness. Systematically tests how NFC readers handle malformed or unexpected data. For security research and reader testing.

#### NFC Keyboard *(by Patrick762)*
> Scan an NFC card and paste its UID as keyboard input. Quick way to type card UIDs into a computer without manually reading and entering the hex values.

#### NFC Login *(by Play2BReal)*
> Scan an NFC card to automatically type a stored password via USB or Bluetooth keyboard emulation. Store multiple cards linked to different passwords. Passwords encrypted using the Flipper's secure enclave. Supports auto-matching, passcode protection, and international keyboard layouts.

#### NFC Magic
> Write to "magic" NFC tags with modifiable Block 0 (UID/manufacturer block). Used for cloning MIFARE Classic cards to writable tags. Supports Gen1a, Gen2, and other magic tag types. The app handles special write commands automatically.

#### NFC Maker *(by @Willy-JL)*
> Create NFC NDEF files from scratch — contacts (vCard), URLs, WiFi credentials, email, phone numbers, and plain text. Generated .nfc files can be emulated (hold Kiisu near a phone) or written to blank tags. Great for sharing WiFi passwords or contact info.

#### NFC Playlist *(by @acegoal07)*
> Cycle through a playlist of saved .nfc files automatically. Create a .txt file listing paths to .nfc files, and the app emulates them one after another with configurable timing. Useful for testing which card works with a reader, or for demonstrations.

#### NFC Relay *(by leommxj)*
> Relay NFC APDU commands and responses between two Flippers via UART. One Flipper acts as the card-side, the other as the reader-side. Demonstrates relay attack concepts for NFC security research. Hardcoded to pins C1/C0 at 38400 baud.

#### NFC Sniffer *(by Kyle Diller)*
> Log NFC commands sent by readers in real-time. Displays commands on screen and saves them to `nfc_sniffer_logs/` on SD card. Supports ISO14443-3A and ISO15693-3. Note: captures reader commands only, not tag responses.

#### NFC URL *(by Yasper De Jong)*
> Generate and emulate NFC tags that open URLs when tapped by a phone. Create http(s), ftp, mailto, and other URL types. Save a list of URLs for later use. Quick way to share links via NFC tap.

#### OpenPrintTag *(by Houzvicka)*
> Read and write OpenPrintTag NFC tags for 3D printing filament spools. OpenPrintTag is Prusa Research's open-source NFC standard that stores material info (brand, type, color, usage data) on tags attached to spools. Read existing tags or write new ones.

#### Passport Reader *(by bettse)*
> Read electronic passports (eMRTD/ePassports) via the embedded NFC chip. Enter MRZ data (document number, date of birth, expiry date) to derive encryption keys. Hold passport flat against the Kiisu. Tested with US, Chilean, and French passports. Reading takes a few seconds — keep steady.

#### PicoPass
> HID iClass (PicoPass) credential reader and emulator. Read iClass access control credentials, save to SD card, and emulate later. Supports Standard Edition (SE) and Standard Reader (SR) card types. Uses loclass and mbedtls crypto libraries.

#### SaFlip *(by Aaronjamt)*
> Saflok hospitality lock tool — read, emulate, create, edit, and write Saflok hotel lock cards. Reads Basic Access data and variable keys from Saflok MIFARE Classic cards. Can read log entries from cards and locks in real-time. Currently MFC only; Ultralight C planned.

#### Seader *(by bettse)*
> SAM-based credential reader — communicates with a Secure Access Module for high-security access control systems. Supports config cards for SAM initialization, Proxmark3-compatible commands, and SIO SE/SR credential type detection. Requires SAM hardware.

#### Seos Compatible *(by bettse)*
> Read and emulate Seos-compatible cards, fobs, and mobile credentials. Supports multiple key sets — switch between facilities via the menu. Create key files in `apps_data/seos/`. Default uses all-zero keys; real credentials need proper key files.

#### SLI Writer *(by Julienbxl)*
> Write .nfc files to magic ISO15693 tags with changeable UIDs. Supports UID-addressed mode for real writes to SLI magic cards. Also available as an Android app.

#### Sonicare Head ID *(by mbirth.uk)*
> Read NFC data from Philips Sonicare brush heads. Each modern brush head contains an NTAG213 chip that identifies the brush type to the handle unit. See what data your toothbrush is storing.

#### T-Union Master
> Chinese transit card (交通联合卡) query tool. Read card number, name, type, expiry date, balance, plus 10 transaction records and 30 trip records (transport type, line, station). Offline query — no WiFi/Bluetooth needed. Uses ISO/IEC 14443 Type 4A protocol.

#### TonUINO Writer *(by Bastelsaal)*
> NFC card writer for TonUINO DIY audio boxes. Create and manage NFC cards with folder, mode, and special settings. Read existing card configurations. Rapid Write Mode for quickly programming multiple cards. TonUINO is a popular DIY music player for kids.

#### TuLlave *(by zqu4rtz)*
> Read TuLlave transit cards from Bogotá, Colombia. Displays card balance and transaction history. TuLlave is a MIFARE Classic contactless card used for public transport in Bogotá.

#### UDECard *(by Alexander Hahn / hahnworks)*
> Analyze student ID cards from the University of Duisburg-Essen (Intercard system). Read and display card data including payment balance and library authentication info.

#### UID Brute Smarter *(by fbettag)*
> Advanced NFC UID analysis and testing tool. Load NFC files with metadata extraction, detect patterns in UIDs, and build comprehensive security assessments. For authorized penetration testing and access control auditing only.

#### UL-C Bruteforce *(by noproto)*
> Dictionary attack against MIFARE Ultralight C authentication keys. Tries keys from system and user dictionaries in `nfc/assets/`. If a key is found, the card can be fully read. Add custom keys to the user dictionary for targeted testing.

#### UL-C Bruteforce Optimized *(by noproto)*
> Enhanced Ultralight C bruteforce implementing the attack described in the academic paper Section 10.1 "Online Enhancements." Faster and more efficient than the standard bruteforce approach.

#### UL-C Relay *(by noproto)*
> Relay MIFARE Ultralight C authentication between a real reader and card, with the Kiisu as man-in-the-middle. Demonstrates relay attack vulnerability in contactless authentication. Present the Kiisu to the reader while the real card is held against the NFC antenna.

#### ULCFKey *(by noproto)*
> Crack counterfeit MIFARE Ultralight C cards with weak or predictable authentication keys. Many cheap clones ship with default keys. The app exploits those weaknesses to recover keys for full read/write access.

#### ULCFKey (Next)
> Enhanced ULCFKey with tearing support. Cracks counterfeit Ultralight C cards using additional attack vectors including tearing (interrupting write operations to exploit state inconsistencies).

#### VB Migration Assistant *(by cyanic)*
> Makes transferring characters between VB Lab and VBC Lab less cumbersome. Designed for Vital Bracelet (Digimon/other) virtual pet device users who need to move character data between apps.

#### VK Thermo *(by VivoKey Technologies)*
> Read temperature from VivoKey Thermo NFC implants. Supports multi-thermo tracking for users with multiple temperature-sensing NFC implants. Displays readings from the implant's temperature sensor.

#### Weebo *(by bettse)*
> NTAG215 power tool — parse, write, emulate, remix, and duplicate tags. The "remix" feature modifies the UID to create a new identity (useful for games that track tag UIDs). Prevents invalid UIDs. Place `key_retail.bin` in `apps_data/weebo/` for full features.

</details>

<details><summary><h3>🔧 Tools — 98 apps</h3></summary>

#### Access Audit *(by matthewkayne)*
> Defensive NFC/RFID access control auditor. Tap a card, get an instant risk score and plain-English security advice. Classifies MIFARE Classic 1K/4K, DESFire EV1-EV3, MIFARE Plus, Ultralight C, NTAG series, FeliCa, SLIX, ST25TB, and 125 kHz RFID (EM4100, HID H10301). Saves named session reports to SD card. For authorized security professionals only.

#### Air Level *(by CodeAllNight (MrDerekJamison))*
> Spirit level/bubble tool using the Video Game Module's motion sensor. Hold the Flipper flat and the on-screen bubble shows how level the surface is. Requires VGM attached.

#### Analog Clock *(by scrolltex)*
> Displays an analog clock face on the Flipper's screen. A simple, clean timepiece with hour and minute hands.

#### Authenticator (TOTP)
> Software-based TOTP/HOTP authenticator — generates time-based one-time passwords for two-factor authentication. Add your accounts' secret keys, and the app generates the 6-digit codes that rotate every 30 seconds. Works like Google Authenticator but runs on your Flipper. Codes stored encrypted on SD card.

#### Barcode *(by @Kingal1337)*
> Display various barcodes on screen. Supports UPC-A, EAN-8, EAN-13, Code-39, Codabar, and Code-128 formats. Create barcodes by entering the data, save them to SD card, and display them for scanning. Barcodes are stored in `apps_data/barcodes/`.

#### BG Loader *(by twisted-pear)*
> Background app loader — allows compatible apps to be loaded and remain running in the background. Select a FAP file from the file browser and it continues running even after you navigate away. Useful for apps that need persistent background operation.

#### Big Clock *(by Eris-Margeta)*
> Full-screen bedside clock with large, easy-to-read digits visible from across the room. Adjustable brightness from 0-100% using Up/Down buttons. Designed for nightstand use.

#### BlackJack Counter *(by grugnoymeme)*
> Card counting assistant for blackjack. Track the running count during a game to practice card counting techniques. Simple increment/decrement interface.

#### Brainfuck *(by nymda)*
> Brainfuck programming language interpreter and editor. Write and execute Brainfuck programs directly on the Flipper. Supports text input/output. Blue LED indicates program is running. An esoteric programming tool for the truly dedicated.

#### Caesar Cipher *(by @panki27)*
> Encrypt and decrypt text using the classic Caesar cipher (letter shift). Enter your text with the on-screen keyboard (use underscores for spaces), and the app shows all 26 possible shifts so you can find the right decryption. Simple but educational crypto tool.

#### Calculator *(by @n-o-T-I-n-s-a-n-e)*
> Simple expression calculator. Enter mathematical expressions and get results. Supports basic arithmetic operations. Use the d-pad to navigate the on-screen number pad and operators.

#### Calendar *(by Adiras)*
> Monthly calendar view for the Flipper Zero. Navigate between months and years with the d-pad. See day-of-week for any date. Simple and practical.

#### Calendar of Weeks
> Weekly calendar view showing the current week plus previous and next weeks. Today's date is highlighted with inverted colors. Monday is the first day. Uses Zeller's congruence for accurate day-of-week calculation including leap years.

#### CAN Tools *(by Matthew KuKanich)*
> CAN bus DBC file management and data decoding tool. Create and save DBC-style signal definitions (name, CAN ID, bit layout, scaling, unit, min/max), view saved signals, and manually decode CAN frames against your signal database. Companion to CAN Commander for offline analysis work.

#### Chronometer *(by nmrr)*
> Millisecond-accurate stopwatch using the STM32's TIM2 hardware timer at 64 MHz. Much more precise than software-based timers. Stops at 99:59:59. Start/stop/reset with button presses.

#### CLI-GUI Bridge
> Bridge between the Flipper's command-line interface and the GUI screen. Run CLI commands (like `subghz chat`, `log`, `top`) directly from the screen without a USB connection. Enter commands with the on-screen keyboard, view output on screen. Long-press Back to exit. Gives untethered access to system diagnostics and tools.

#### Clock *(by Lambda)*
> Simple clock display. Shows the current time on the Flipper's screen. Basic timekeeping.

#### Combo Cracker
> Crack Master Lock combination padlocks in 8 attempts or less. Based on Samy Kamkar's research into mechanical vulnerabilities in Master Lock padlocks. The app guides you through a side-channel attack — apply tension to the shackle, observe dial behavior at specific positions, and the app calculates the combination from the feedback. A mechanical exploit, not brute force.

#### Count Down Timer *(by @0w0mewo)*
> Simple countdown timer. Use Up/Down to set hours, minutes, and seconds. Left/Right to select which value to change. OK to start/stop the countdown. Long-press OK to reset. Beeps when the timer reaches zero.

#### Counter *(by @Krulknul)*
> Simple increment/decrement counter. Press Up to add one, Down to subtract one. Long-press OK to enable vibration feedback on each count. Long-press Back to reset to zero. Useful for counting anything — people, items, reps, laps.

#### Device Info
> Displays detailed information about your Flipper Zero hardware and firmware — model, firmware version, build date, and other system details. Quick reference for your device specs.

#### Doc Viewer Lite *(by C0d3-5t3w)*
> Lightweight text document viewer. Read .txt files stored on the SD card with page up/down navigation and line-by-line scrolling. Configurable font sizes (small, medium, large). Simple and efficient for reading notes or documentation on-device.

#### DTMF Dolphin *(by @litui & @xMasterX)*
> DTMF (Dual-Tone Multi-Frequency) dialer, Bluebox, and Redbox tone generator. Generates the tones used by telephone systems. The Dialer mode produces standard phone keypad tones. Bluebox mode generates the 2600 Hz tone and MF tones used in legacy phone switching. Redbox mode produces coin deposit tones (US/UK). Educational tool for learning about phone phreaking history.

#### Enigma *(by Struan Clark (xtruan))*
> Enigma M3 cipher machine emulator. Simulates the WWII German Naval Enigma with three rotors and plugboard. Enter plaintext and watch it encrypt in real-time, or decrypt intercepted messages. Uses the enigma-machine library for accurate emulation. A fascinating piece of cryptographic history on your Flipper.

#### Eye Saver *(by paul-sopin)*
> Eye strain prevention tool using the 20-20-20 rule. Runs a repeating 20-minute timer with haptic feedback. During the 20-second break, displays a reminder to look at something 20 feet away. Helps prevent eye fatigue during long screen sessions.

#### F0 Forth *(by twisted-pear)*
> Forth programming language interpreter for Flipper Zero. Adds a `forth` command to the CLI for an interactive Forth shell. Based on pForth. Write and execute Forth programs directly on the device. For programming enthusiasts.

#### Financial Calculator *(by schaene)*
> Time Value of Money (TVM) calculator. Solve financial problems: future value, present value, payment amounts, interest rates, and number of periods. Enter your variables and the app calculates the missing one. Useful for quick financial calculations without a phone.

#### Flipfetch *(by Ismael A. Rodríguez)*
> System information display (fastfetch-style). Shows firmware version, build date, battery percentage and voltage, and other Flipper system info in a clean summary screen.

#### FlipBIP Crypto Wallet *(by Struan Clark (xtruan))*
> BIP32/39/44 cryptocurrency wallet for Flipper Zero. Generate and store crypto wallet keys using Trezor's crypto libraries. Supports BIP39 mnemonic seed phrases and BIP44 key derivation. WARNING: uses nearly maximum Flipper RAM — may crash on first launch, just try again. Not recommended for storing significant funds due to the device's security limitations.

#### FlipCrypt *(by Tyl3rA)*
> Encrypt, decrypt, and hash text using a wide variety of classic and modern crypto tools. Supports AES-128, Affine, Atbash, Baconian, Beaufort, Caesar, Playfair, Polybius Square, Porta, Rail Fence, RC4, ROT-13, Scytale, and Vigenère ciphers. Also includes hash functions. Can emulate results via NFC, generate QR codes, or save to .txt files.

#### Flipper Aid *(by spaghety)*
> Emergency response tool with pulse timer and CPR timer. Helps maintain proper CPR rhythm and timing during emergencies. A potentially life-saving utility.

#### Flipper Printer *(by alxcrt)*
> Thermal printer app for the T7-US printer module connected via GPIO. Print custom text entered via on-screen keyboard. Also includes a coin flip game with statistics tracking that can print results.

#### Flipper Wedge *(by Dangerous Things)*
> Type RFID/NFC UIDs as keyboard input via USB or Bluetooth. Scan any RFID (125 kHz: EM4100, HID Prox, Indala) or NFC (13.56 MHz: ISO14443A/B, MIFARE, NTAG) tag, and the Kiisu instantly types the UID as if it were a keyboard. Supports 5 scan modes and NDEF text record parsing. Perfect for inventory systems, attendance tracking, or any workflow that needs tag data typed into a computer.

#### Flipper95 *(by Silent / CookiePLMonster)*
> Stress test your Flipper by finding Mersenne primes using the Lucas-Lehmer Test — like Prime95 for desktop PCs. Starts from M2 by default. Watch your Flipper crunch heavy math. Can be controlled via CLI to start from higher Mersenne numbers.

#### FlipperPwn *(by PentestSuite & barkandbite)*
> Modular pentest payload framework with OS detection and WiFi Dev Board support. A toolkit for security testing workflows.

#### FlipperZero Clock *(by mdaskalov)*
> Customizable clock with multiple display modes. Toggle between split-view (clock + date + weekday) and full-screen mode with OK. Choose from multiple clock faces including analog styles. Adjust digit width with Left/Right. Settings saved to config file and persist between launches.

#### FlipNote *(by morty517)*
> Real text editor for Flipper Zero — open, edit, save, and create text files with a full on-screen keyboard. Supports Open/Save/Save As with Momentum's native file picker. Proves that 5 buttons and a 128×64 screen are enough for text editing.

#### FlipPar *(by jsammarco)*
> Golf and disc golf scorecard tracker. Set number of holes (up to 27) and players (up to 10), rename players, record par and scores hole-by-hole. Export a plain-text score sheet to SD card. Perfect for the course.

#### Ford Radio Codes *(by DavidB)*
> Ford radio unlock code generator. Enter the radio's M or V series serial number using Up/Down buttons and instantly get the 4-digit unlock code. Requires the `radiocodes.bin` file on SD card. Saves a trip to the dealer.

#### GnomishTool *(by Andreeved88)*
> Multi-tool with ruler (3cm with 0.25mm divisions, plus inch marks), pulse counter with X5 repeater, and other utility functions. A Swiss army knife of measurement tools themed as a "Gnomish army knife from Gnomeregan."

#### HEX Editor *(by @dunaevai135)*
> Edit any file on the SD card in hexadecimal, byte by byte. Navigate to a file, view its hex dump, and modify individual bytes with OK. Useful for editing NFC dump files, tweaking saved Sub-GHz captures, or modifying any binary file without a computer. Save changes back to the file when done.

#### HEX Viewer *(by @QtRoS)*
> View any file on the SD card as a hex dump. Efficiently handles files up to 16MB. Can switch between hex and text representation of bytes. Has a "Scroll to..." feature to jump to any percentage of the file. Very efficient rendering — only 8 canvas draw calls per repaint.

#### HyperFocus Calc *(by Endika)*
> Hyperfocal distance calculator for photographers. Enter focal length (mm) and aperture (full-stop sequence), select your sensor size, and get the hyperfocal distance. Circle of confusion auto-calculated from sensor dimensions. Useful for landscape photography where you want maximum depth of field.

#### IconEdit *(by Roberto De Feo)*
> Full icon and animation editor directly on the Flipper. Create images up to 128×64 pixels with pixel-level editing. Draw lines, circles, and rectangles. Create animated icons with configurable frame rates. Save as PNG, .C source code, or XBM format. Send images to your PC without swapping the SD card. Dramatically improves the app development workflow for creating custom icons.

#### ID Card *(by evillero)*
> Personal ID card manager. Store and display your name, phone number, address, and notes. Data saved to `id.txt` on the device. Quick way to share your contact info by showing the Flipper's screen.

#### Key Copier *(by Torron)*
> Physical key measurement and duplication guide. Place a real key on top of the Flipper's screen, align it with the on-screen contour, and adjust each pin's depth until they match visually. The app displays the bitting code you can take to a locksmith. Supports dozens of key formats including house keys, car keys, and double-sided keys.

#### LED Blinker *(by Cupprum)*
> Blinks the Flipper's LED with decreasing frequency over time. Unlike traditional Pomodoro timers, provides visual feedback through LED blinks that gradually slow down. Configurable parameters. A unique approach to time awareness.

#### LED Pulsator *(by Leed)*
> LED light control proof-of-concept. Demonstrates advanced LED capabilities beyond the basic on/off that the standard firmware offers. Pulsing, fading, and color effects.

#### Letter Beacon *(by nmrr)*
> Morse code beacon using RFID (125 kHz) and NFC (13.56 MHz) interfaces. Transmits letters, numbers, and symbols in Morse code via the Flipper's radio interfaces. Like Russia's HF letter beacons, but pocket-sized. Supports full alphabet A-Z, digits 0-9, and symbols.

#### Lightning Distance *(by HyperMuffin12)*
> Calculate the distance of a lightning strike. Start the timer when you see the flash, stop when you hear thunder. The app calculates the distance based on the speed of sound. Simple physics made practical during storms.

#### Lishi *(by evillero)*
> Save values obtained from the Lishi lock decoding tool. Input pin depth values from the decoding process, store them for reference. Supports various Lishi key types. A digital notebook for locksmiths using Lishi tools.

#### ListEM *(by Clawzman)*
> UID dictionary generator for RFID, NFC, and iButton protocols — directly on the Flipper. Generate large customizable UID lists for testing readers. Includes a Fuzzing Mode with bit mutation engine for reader security testing. Previously required Python scripts on a PC; now runs natively on-device.

#### Math Wiz *(by Papa_Ghost)*
> Polynomial solver up to 6th degree. Evaluate polynomials for any x value, calculate derivatives and integrals, and visualize results on screen. A pocket math tool for students and engineers.

#### Moon Phases *(by w84death)*
> Displays the current phase of the moon with a visual representation. Shows illumination percentage and additional lunar information. Know whether it's a full moon, new moon, or anything in between.

#### Multi Converter *(by @theisolinearchip)*
> Multi-unit converter with an expandable system for adding new units. Convert between various measurement systems — length, weight, temperature, and more. Practical for quick conversions without a phone.

#### Multi Timer *(by C0d3-5t3w)*
> Run up to 10 simultaneous timers with preset quick options (1, 5, 10, 15, 20, 30 minutes, and 1 hour). Perfect for cooking multiple dishes, workout intervals, or productivity sessions. Features a charming dolphin mascot.

#### NFC/RFID Detector *(by SkorP)*
> Identify whether a reader is NFC (13.56 MHz), RFID (125 kHz), or both. Hold the Kiisu near an unknown reader and the app tells you what frequency it operates on. Useful for determining what type of card or tag a reader expects before trying to interact with it. Simple one-screen display.

#### Nickname Generator *(by disaxq)*
> Random name generator. Press Left/Right to change the subject/style of names generated. Creates unique nicknames on the fly. Fun for coming up with usernames, character names, or project codenames.

#### Nightstand Clock *(by @nymda & @Willy-JL)*
> Clock optimized for bedside use at night. Backlight stays on constantly (unlike the normal clock). Control brightness with Up/Down — at 0 brightness, press Down to toggle a dim red notification LED. Date and AM/PM positions are swapped for better nighttime readability.

#### Notes for FZ
> Simple note-taking app. Write and save text notes to the SD card. Work in progress.

#### Orgasmotron *(by Leedave)*
> Vibration motor demo with multiple vibration patterns and modes. Demonstrates the Flipper's haptic capabilities beyond the basic on/off. A proof-of-concept for creative vibration motor usage.

#### Paranoia *(by C0d3-5t3w)*
> Anti-surveillance field tool that scans for hidden wireless cameras and suspicious RF activity. Designed for privacy-conscious users who want to check hotel rooms, Airbnbs, or other spaces for covert recording devices.

#### Password Generator *(by @anakod & @henrygab)*
> Generate random passwords of configurable length. Press OK to generate a new random password. The generated password is displayed on screen. Simple and quick — no internet connection needed. Useful when you need a random password and don't have your phone handy.

#### Password Manager *(by Rrycbarm)*
> Store usernames and passwords on the Flipper and type them on your PC via USB keyboard emulation. Add credentials (website, username, password), then select an entry and the Flipper types it as if it were a keyboard. A hardware password manager.

#### Pet a Dolphin *(by EugeneSmile)*
> Reset your Flipper's dolphin companion mood to happy. A quick way to cheer up your digital pet without waiting for the mood to improve naturally.

#### Pet Your Dolphin *(by dwight9339)*
> Interactive dolphin petting app. Pet your Flipper's dolphin companion to soothe it while earning XP. A more engaging way to interact with your digital pet.

#### Pomodoro Timer *(by sbrin)*
> Pomodoro technique timer based on Francesco Cirillo's time management method. Work intervals (typically 25 minutes) separated by short breaks. Multiple timer presets available. Helps maintain focus and productivity.

#### Flipp Pomodoro
> Alternative Pomodoro timer implementation with a clean interface. Work/break cycle timer for productivity.

#### Programmer Calculator *(by armixz)*
> Hex/decimal/binary/octal converter for programmers. Enter a number in any base and instantly see it converted to all other bases. Supports decimal↔binary, decimal↔hex, hex↔binary, and decimal↔character conversions. Essential for embedded development and protocol analysis.

#### QR Code *(by Bob Matcuk)*
> Display QR codes on the Flipper's screen. Create .qrcode text files in the `qrcodes/` folder on SD card — each file contains the QR data. The app renders the QR code on the 128×64 screen for scanning with a phone camera. Useful for sharing URLs, WiFi credentials, contact info, or any text data.

#### QRCode Generator *(by Thomas Schwarz (aka qw3rtty))*
> Generate QR codes on-device from text input. Type your text using the on-screen keyboard and the app creates and displays the QR code immediately. No pre-made files needed — generate on the fly.

#### Quac! *(by Roberto De Feo)*
> Quick Action remote control — organize previously recorded signals of ANY type (Sub-GHz, RFID, Infrared, NFC, iButton) into a folder structure for instant one-press playback. No more remembering whether a door uses Sub-GHz or RFID — just navigate to the action and press OK. The app doesn't record signals — use the built-in apps first, then organize the saved files for Quac!

#### Quadratic Solver *(by paul-sopin)*
> Solve quadratic equations in the form ax² + bx + c = 0. Enter a, b, and c values and get the solutions. Quick math tool for students.

#### Reality Clock *(by Eris-Margeta)*
> Dimensional stability monitor using multi-band electromagnetic ratio analysis via the CC1101 radio. Based on the theoretical framework that changes in fundamental physical constants would alter signal propagation ratios across frequencies. Part clock, part physics experiment, part conversation starter.

#### Resistance Calculator *(by Lewis Westbury)*
> Calculate resistance from colored bands on physical resistors. Pick from 3, 4, 5, or 6 band resistors. Use Left/Right to move between bands, Up/Down to change colors. Resistance, tolerance, and temperature coefficient update instantly. Much faster than looking up color codes on your phone.

#### Roman Decoder
> Convert Roman numerals to decimal values. Interactive input — use buttons to navigate through Roman characters (I, V, X, L, C, D, M). Real-time conversion as you enter characters.

#### ROT13 *(by nothingbutlucas)*
> Simple ROT13 cipher tool. Enter text and get the ROT13-encoded version. Since ROT13 is its own inverse, the same operation decodes it. Quick and simple text obfuscation.

#### Rubiks Cube Timer *(by KHOPAN)*
> Speedcubing timer for Rubik's cube solvers. Time your solves with precision. Track your progress and personal bests. Essential for the cubing community.

#### SD Info *(by sergo)*
> Display detailed information about the inserted SD card — capacity, free space, filesystem type, and card status. Can also run a performance test. Quick diagnostic for SD card health.

#### Segment Clock *(by Sladkisnovraper)*
> Simple seven-segment style clock display. Press Up to toggle between 12-hour and 24-hour mode. Clean, retro aesthetic.

#### Sig Diary *(by C0d3-5t3w)*
> Passive background signal scanner that logs and annotates IR, RF, and NFC signals. Automatically identifies common signals (garage doors, remote controls, access cards) based on fingerprinting. Maintains a timestamped log. Perfect for hobbyist recon or daily security audits.

#### SKIDcity *(by H4W9)*
> Educational app that explains what the Flipper Zero actually can and cannot do. Addresses common misconceptions from TikTok and Discord — no, it can't hack traffic lights, clone any card, or steal car keys. Explains why, with real technical details. Genuinely helpful for new Flipper owners.

#### Smartra VIN2PIN *(by evillero)*
> Calculator for SMARTRA2 immobilizer PINs for Hyundai and KIA vehicles. Enter the VIN and get the PIN. Press OK to save the result. Supports most Hyundai/KIA models using SMARTRA2.

#### Space Travel Calculator *(by ejfox)*
> KSP-style interplanetary transfer planner. Select a destination (Mars, Venus, Jupiter, Europa, Titan, etc.), scroll through launch dates, and see Hohmann transfer orbits drawn on screen. Outputs delta-v requirements and transfer times. For space enthusiasts and KSP players.

#### Spindle Calculator *(by Jordan M)*
> Trade tool for carpenters — calculate spindle counts and spacings for balustrades. Enter length and spindle size, get the number of spindles that fit within the 99mm building regulation gap limit, plus exact spacing. Uses IMU for stair angle measurement. A real job-site tool.

#### Tasks *(by Stanislav Vasilev(Madman10K))*
> Simple to-do list application. Add tasks, mark them complete, and delete them. Tasks persist on the SD card between sessions. Navigate with d-pad, OK to toggle completion, and use the menu to add or remove items.

#### TECHART Calendar
> Monthly calendar with intuitive navigation. Left/Right to change months, Up/Down for years. Displays days of the month in a clean grid layout.

#### Text Viewer *(by @Willy-JL)*
> View text files on the Flipper's screen. Browse to any .txt file on the SD card and read it line by line. Scroll with Up/Down. Useful for reading notes, documentation, or log files stored on the SD card without connecting to a computer.

#### Time Dilation Calculator *(by ejfox)*
> Special relativity calculator. Set a velocity (fraction of the speed of light) and a duration, see how much time passes for you versus an observer on Earth. Auto-scales units from seconds to years. For physics enthusiasts and sci-fi fans.

#### Time Tracker *(by MassivDash (hhtps://spaceout.pl))*
> Multi-task time tracker. Track multiple tasks simultaneously with start/stop/resume. Data stored in CSV format for easy export to Excel. Tasks continue tracking in the background or even when the device is off. Perfect for freelancers tracking billable hours.

#### Timer *(by nagamine-git)*
> Pomodoro-style timer with 30 minutes of break and 20 minutes of work intervals. Simple start/stop interface.

#### Tone Generator *(by Gerald McAlister)*
> Generate sound tones through the Flipper's speaker. Select frequency and the Flipper plays a continuous tone. Useful for audio testing, signal generation, or just making noise.

#### Tree Identification *(by fgreil)*
> Identify trees by answering questions about leaf shapes. A guided decision tree that narrows down the species based on your observations. Educational botany tool.

#### Type Aid
> Predictive text keyboard with word suggestions. Type faster with auto-complete suggestions as you enter text. Compare with the standard Flipper keyboard to see the difference.

#### uPython *(by Oliver Fabel)*
> Compile and execute MicroPython scripts directly on the Flipper. Includes a full Python REPL (interactive prompt) and can run .py script files from the SD card. Requires ~80KB of RAM to start — may crash on first launch due to memory fragmentation, just try again. Access Flipper hardware APIs from Python for rapid prototyping.

#### VIN Decoder *(by evillero)*
> Vehicle Identification Number decoder. Enter a VIN and get information about the vehicle — manufacturer, model year, country of origin, and more. Decode any 17-character VIN on the spot.

#### Voltage Calculator *(by HappyAmos)*
> Calculate voltage, resistance, or current using Ohm's Law (V = R × I). Enter any two values and the app calculates the third. Use underscore character for decimal points (Flipper keyboard limitation). Simple but handy for quick electrical calculations in the field.

#### Voyah Password *(by bolknote)*
> Engineering menu password generator for Voyah vehicles. The password depends on the current date calculated in the Chinese time zone. Set your time zone after launching, enter the date, and get the password.

</details>

<details><summary><h3>⚡ Gpio — 82 apps</h3></summary>

#### 24Cxx Programmer *(by Dr.Mosfet)*
> EEPROM programmer for 24Cxx series chips via I2C. Read and write data to common I2C EEPROMs. Connect the chip to the Flipper's I2C pins and use the app to dump or program contents.

#### Air Stats *(by thevan4)*
> CO2 + climate monitor. Reads CO2 (MH-Z19B/C) and temperature/humidity/pressure (BME280, DHT22, etc.) through GPIO. Supports two sensors simultaneously. Inherited from the Unitemp project.

#### Air Mouse
> Turn Flipper into a wireless air mouse using BMI160 or LSM6DS3 motion sensor. Wave in the air to move cursor. USB or Bluetooth modes. Up=left click, Down=right click, OK=middle click, Left/Right=scroll.

#### Analog Output *(by jamisonderek)*
> DAC demo using MCP4725 on I2C and PWM on pin A7 with RC filter. Input on pin C3. Demonstrates analog output capabilities.

#### Atari SIO Emulator *(by cepetr)*
> Atari 8-bit computer SIO peripheral emulator. Connect the Flipper to an Atari via SIO and emulate disk drives or other peripherals.

#### Battery Checker *(by Programistich)*
> Simple battery voltage checker. Read voltage from a battery connected to GPIO. Quick field measurement tool.

#### BunnyConnect *(by C0d3-5t3w)*
> USB serial communication tool. Send and receive data over USB serial. Useful for communicating with microcontrollers and other serial devices.

#### C5 Lab *(by Lab_Team)*
> WiFi Deauther controller for Lab_C5 module via UART. Control a C5 WiFi module from the Flipper's screen.

#### CAN Commander
> Full CAN bus reverse engineering toolkit. Sniff live frames, inject custom messages, analyze traffic, track bit-level changes. Import DBC files for human-readable signal names. Create injection profiles. 20KB stack — the largest app.

#### CAN Transceiver (MCP2515)
> CAN bus interface using MCP2515 transceiver. Send and receive messages on standard CAN bus including vehicle networks. Sniffing and injection capabilities.

#### CANBUS *(by ElectronicCats)*
> CAN bus interaction app. Sniff and inject CAN bus transmissions. Basic tasks for interacting with vehicle and industrial CAN networks.

#### CANBUS ATTACK *(by jorgepnaranjo25)*
> CAN bus security testing tool for educational/research purposes. Developed as part of a university cybersecurity research project. For authorized testing only.

#### Canon Intervalometer *(by TravisCG)*
> Camera intervalometer for Canon DSLRs via GPIO. Set interval and the Flipper triggers the shutter repeatedly for time-lapse photography.

#### Coffee EEPROM *(by wh00hw)*
> Vending machine EEPROM pentesting tool. Read and write EEPROM data from certain vending machines for security research.

#### Continuity Tester *(by Ben Allen)*
> Simple continuity tester — beeps if a wire is connected between test pins. No menus, no configuration. Touch both ends of a wire to the pins and listen for the beep.

#### DelfiRTL *(by gorebrau)*
> WiFi penetration testing using RTL8720DN module. Perform attacks on 2.4GHz and 5GHz networks. Requires RTL8720DN hardware connected via GPIO.

#### Digimon F-Com *(by TylerWilley)*
> Communicate with Digimon V-Pet devices. Tested with DM20. Connect the Flipper to a Digimon virtual pet and exchange data.

#### E220 LoRa Configurator *(by crackerjacques)*
> Configure E220-900T22S LoRa modules. Set parameters like frequency, power, and data rate for the E220 series LoRa radio modules.

#### Encoder Reader *(by Engineegor)*
> Read incremental quadrature encoders. Shows absolute and relative coordinates. Connect an encoder to GPIO pins and see position data in real-time.

#### Evil BW16 Controller *(by dagnazty)*
> WiFi Deauther controller for Evil-BW16 module via UART. Control the BW16 WiFi attack module from the Flipper's screen.

#### Fencing Test Box *(by Aaron Janeiro Stone)*
> Test box for fencing blade and body wire assessment. Connect fencing equipment to GPIO and verify electrical continuity and resistance. A specialized tool for the sport.

#### Flashlight
> Enables 3.3V on pin 7/C3 when you press OK. Stays on after exit. Wire an LED with resistor between pin 7 and GND. One-button operation.

#### Flipagotchi *(by Matt-London)*
> Interface the Flipper with a Pwnagotchi. Display Pwnagotchi status and data on the Flipper's screen. Connect via GPIO.

#### FlipGemini *(by JBlanked)*
> Chat with Google Gemini AI via an ESP module. Send prompts and receive AI responses on the Flipper's screen. Requires WiFi-capable ESP module.

#### Flipper Blackhat
> Control the Flipper Blackhat A33 device via GPIO. Interface with the dedicated security testing hardware module.

#### Flippy Temp *(by n0ciple)*
> Temperature logging app for the Flipy Sensor Board. Record temperature readings over time with the dedicated sensor board.

#### FM Radio
> Listen to FM radio using TEA5767 receiver via I2C. Control frequency, volume, and presets. Requires headphone cable as antenna. VCC→3V3, GND→pin 18, SCL→C0, SDA→C1.

#### FM Radio w Vol
> FM Radio with external volume control. TEA5767 receiver plus PT2257 I2C volume controller. Two I2C devices for full radio experience.

#### FM Transmitter
> Broadcast audio over FM using KT0803 module via I2C. Connect audio source to AUX input, select frequency, and broadcast. Any FM radio nearby can tune in.

#### GS1 Parser (UHF)
> Parse GS1 UHF RFID payloads based on Tag Data Standard v2.2. Currently supports SGTIN formatted EPCs. For UHF RFID inventory and supply chain applications.

#### GPIO 7-Segment Output
> Control a 7-segment display with GPIO pins. Display numbers and characters on an external 7-segment LED display connected to the Flipper.

#### GPIO Badge
> Companion app for MakeItHackin GPIO Diagnostics Board. LED light effects: sequential patterns, alternating blinks, and "stop the light" game. Left/Right for speed, Up/Down for patterns.

#### GPIO Controller
> Visual GPIO pin control. See all pins with current state, toggle any pin high/low with OK. Essential for quick circuit testing and manual hardware control.

#### GPIO Explorer
> Three modes: WS2812 RGB LED control, standard LED control, and GPIO Reader. The most complete GPIO exploration app for beginners.

#### GPIO Reader (aureli1c)
> Read and display all GPIO pin states in real-time. Configure pull-up/pull-down resistors. Continuous updates show changes as they happen.

#### GPIO Reader (biotinker)
> Alternative GPIO reader implementation. Real-time pin state monitoring with a different interface style.

#### GPIO with I2C
> Standard GPIO app enhanced with I2C scanner and SFP module EEPROM reader. Combines GPIO control with I2C device discovery.

#### I2C Explorer *(by 4mb3rz)*
> Interactive I2C bus exploration tool. Discover devices on the bus, read and modify register states. Essential for debugging I2C peripherals.

#### Lab401/DigiLab *(by Lab401 & tixlegeek)*
> Lab401 DigiLab interface. Control Lab401 hardware modules from the Flipper.

#### Lab401/Light Messenger *(by Lab401 & tixlegeek)*
> Lab401 Light Messenger interface. Control LED message displays via the Lab401 hardware platform.

#### LaserCat *(by DroWez)*
> Turn on a GPIO-connected laser module. Simple on/off control for a laser pointer connected to GPIO.

#### LD2410 Human Detector *(by Oscar Rodriguez)*
> Human presence detector using HLK-LD2410 mmWave radar sensor. Detects human presence through walls and obstacles using millimeter-wave radar. Connect sensor to GPIO.

#### Logic Analyzer *(by g3gg0)*
> Use Flipper as an OpenBench Logic Sniffer (OLS) compatible logic analyzer in PulseView. Capture and analyze digital signals on GPIO pins.

#### Longwave Clock
> Decode or simulate long-wave time signals. Supports DCF77 (Germany), MSF (UK), WWVB (US), JJY (Japan). Demo mode generates synthetic signals without hardware.

#### LoRa Sample *(by ElectronicCats)*
> LoRa radio sample application. Starting point for LoRa-based projects using Electronic Cats or similar LoRa modules on GPIO.

#### LoRa Relay *(by ElectronicCats)*
> LoRa sniffer using Electronic Cats flipper add-on. Capture and relay LoRa packets.

#### LoRA Terminal
> UART terminal for LoRa modules. Send and receive data through LoRa radio modules connected via UART.

#### LoRadar *(by baptleduc)*
> Localize stratospheric balloons via LoRa. Receive LoRa telemetry from weather balloons and display position data.

#### MagSpoof
> Wirelessly emulate magnetic stripe data using an electromagnetic coil module. Based on Samy Kamkar's project. Requires dedicated GPIO module with H-bridge and coil. Create .mag files in `apps_data/magspoof/`. For authorized testing only.

#### MODBUS *(by ElectronicCats)*
> Modbus RTU communication tool. Send and receive Modbus commands over RS-485 via GPIO. For industrial automation and building management systems.

#### Moisture Sensor *(by smallwat3r)*
> Read values from a capacitive moisture sensor v1.2 connected to GPIO. Display soil moisture levels in real-time. For gardening and agriculture projects.

#### Noptel LRF Sampler *(by Giraut)*
> Interface with Noptel LRF laser rangefinders. Sample distance measurements from the professional-grade rangefinder module.

#### Oscilloscope
> Basic oscilloscope on the 128×64 screen. Connect signal to pin 16/PC0 (0-2.5V only — higher damages ADC). Real-time waveform display with frequency measurement. OK to pause/unpause.

#### PIR HC-SR501 Alarm *(by ku6a-cj)*
> Proximity alarm using HC-SR501 PIR motion sensor. Triggers an alarm when motion is detected. Simple security/notification system.

#### Pokemon Trading *(by EstebanFuentealba & kbembedded)*
> Trade Pokemon between Flipper and Game Boy. Supports Gen I (Red/Blue/Yellow) and Gen II (Gold/Silver/Crystal). Emulates a second Game Boy on the link cable. Modify traded Pokemon: EVs/IVs, Pokerus, held items.

#### Pokemon Trading (GB)
> Alternative Pokemon trading implementation for Game Boy. Same core functionality — trade between Flipper and Game Boy via link cable.

#### RC2014 ColecoVision
> Use Flipper as a ColecoVision controller for RC2014 retro computer. D-pad maps to directions, OK/Back to fire buttons. Requires RC2014 with TMS9918A video card.

#### RGB LED
> Control RGB LEDs via GPIO. Set colors and brightness for standard RGB LEDs connected to GPIO pins.

#### SD SPI *(by Gl1tchub)*
> SD card SPI lock management. Manage SPI bus access for the SD card interface. Utility for debugging SPI conflicts.

#### Sentry Safe
> Exploit vulnerability in Sentry Safe and Master Lock electronic safes. Sends signal sequence through GPIO to the solenoid. Built-in help page with connection diagrams. For testing your own safes only.

#### Servotester *(by Alexander Semion)*
> Servo motor tester using A7 pin for PWM output. Manual mode for direct control, plus automated sweep modes.

#### Servo Tester 2 *(by mhasbini)*
> Alternative servo tester using A7 pin. Test and calibrate servo motors with PWM signals.

#### SI4713 Tuner *(by rullinoiz)*
> Basic Adafruit SI4713 FM transmitter tuner. Configure the SI4713 FM transmitter module via I2C.

#### Signal Generator
> Generate digital signals (square waves) on any GPIO pin at configurable frequency. Select pin, set frequency, press OK to start. 3.3V square wave output.

#### SPI Flash Dump *(by PentestSuite & barkandbite)*
> Read SPI NOR flash chips via GPIO for firmware extraction. Dump chip contents to SD card for backup or analysis.

#### SPI Terminal
> Interactive SPI communication terminal. Send raw hex data, see responses in real-time. Configure clock speed, SPI mode, and chip select.

#### Spotify Remote *(by Ben Russell)*
> Control Spotify playback from the Flipper. Requires companion setup for connectivity.

#### Step Counter
> Pedometer using Memsic 2125 accelerometer module. Count steps and track movement via GPIO-connected sensor.

#### StroboMeter *(by L. Jakschik)*
> Stroboscope for measuring cyclical motion frequency. Flash at adjustable rates to "freeze" rotating or vibrating objects and determine their frequency.

#### Tesla Mod *(by hypery11)*
> Tesla CAN bus toolkit — FSD region-gate bypass. Interface with Tesla vehicle CAN bus for research purposes.

#### Timelapse
> Simple intervalometer for cameras with wired remote shutter. Set interval, press OK to start. Triggers shutter at each interval for time-lapse photography.

#### UART Sniff *(by PentestSuite / barkandbite)*
> UART protocol sniffer. Capture bytes on GPIO pins 13/14. Monitor serial communication between devices.

#### UART Terminal
> Full serial terminal for UART devices. Send text, AT commands, fast commands, or raw hex. Configurable baud rates. Toggle text/hex display. Save frequently-used commands.

#### u-blox GPS *(by liamur)*
> Display and log data from u-blox GPS modules over I2C. Alternative to UART-based GPS apps for I2C-connected u-blox modules.

#### LAN Tester *(by Dok2d)*
> Portable LAN tester and security analyzer using W5500 Ethernet module. ARP scanning, port checking, and network diagnostics via GPIO.

#### WAV Recorder *(by scrolltex)*
> Record audio to WAV files using an external microphone on GPIO. Save recordings to SD card.

#### WCH SWIO Flasher
> Debugger and flasher for WCH CH32V003 RISC-V microcontrollers. Program and debug via the Single Wire Interface (SWIO).

#### Wiegand Reader *(by jamisonderek)*
> Read Wiegand protocol data (W4, W26, W34, etc.) from access control systems. Connect to Wiegand data lines and capture card reads.

#### Wii EC Analyser
> Protocol analyser for Wii Extension Controllers. See joystick positions, button presses, accelerometer data in real-time via I2C. Includes calibration tools.

#### Wire Tester
> Simple continuity tester — beeps if wire is connected between pins 6 and 8. No menus, no configuration. Just plug and test.

#### WS2812B LED Tester *(by jamisonderek)*
> Test WS2812B addressable LEDs. Verify that individual LEDs in a strip are functioning correctly. Set colors and patterns.

#### YuriCable Pro Max *(by arag0re.eth && PooruTorie)*
> SWD/DCSD cable app for the Flipper Zero. Interface with Apple devices using specialized debug cables.

</details>

<details><summary><h3>📻 Sub-GHz — 33 apps</h3></summary>

#### CaiXianlin Remote
> Remote control for CaiXianlin shock collars via Sub-GHz radio. Send shock, vibrate, or beep commands with adjustable strength (0-100). For authorized use with your own devices only.

#### Chief Cooker *(by Denr01)*
> Restaurant pager Swiss army knife — receive, decode, edit, and resend pager signals. Auto-detects station/pager numbers. Save stations by name in categories. Supports Princeton and SMC5326 protocols with Retekess TD157 encoding. Works with external CC1101 for extended range.

#### ClayLoop *(by Bobby Gibbs)*
> Clay pigeon/skeet shooting controller. Queue up to 4 .sub files and transmit them in sequence with configurable delay, duration, interval, and repeat count. LED/beep countdown, vibration feedback, mid-countdown cancellation. Persistent per-group file path memory.

#### Enhanced Sub-GHz Chat
> Encrypted text messaging between Flippers over Sub-GHz radio. Multiple encryption methods: none, random key, password-derived, hex key, or NFC key sharing. Uses mbedtls AES encryption. Real-time chat interface. Lock keyboard with long-press OK.

#### Flipper Share *(by @lomalkin)*
> Direct wireless file transfer between Flippers via Sub-GHz — no cables, phones, or internet. Broadcast-style so multiple receivers can download simultaneously. 60-byte packets with MD5 verification and auto-retry. ~800 bytes/sec, files up to 1.6MB tested.

#### Frequency Analyzer (External)
> Sub-GHz frequency analyzer extracted from Unleashed firmware. Detect active frequencies in your area using the CC1101 radio. Shows frequency and signal strength.

#### FRSScan *(by thecybermind)*
> Scans common FRS and GMRS frequencies and plays them through the speaker. Press center button to lock onto a frequency. Listen for radio activity on Family Radio Service and General Mobile Radio Service bands.

#### Genie Door Recorder *(by jamisonderek)*
> Extract codes from Genie garage door remotes into .GNE files and play them back. No firmware modifications needed — includes the Genie protocol encoder/decoder. Connect remote to GPIO for code extraction.

#### HAM Scanner *(by Clawzman)*
> Lightweight Sub-GHz radio scanner with real-time RSSI live view during scanning. Behaves like a traditional radio scanner for common bands. Shows signal strength as you scan through frequencies.

#### Marmalade *(by RocketGod-git)*
> Frequency and preset adjustable Sub-GHz radio frequency tool. Supports multiple frequencies and modulation schemes. Exercise caution and follow local laws. By RocketGod.

#### Music to Sub-GHz Radio
> Converts Flipper music files (.FMF/.TXT) into .SUB files for Sub-GHz transmission. Another Flipper receives and plays the music through its speaker. Works with the UberGuidoZ music collection. AM650 modulation recommended.

#### OpenShock *(by OpenShock Team)*
> Control OpenShock-compatible shockers over 433 MHz. Configure model, ID, channel, command type (shock/vibrate/beep), and intensity. Hold OK to transmit continuously. For use with your own OpenShock devices only.

#### POCSAG Pager *(by @xMasterX & @Shmuma)*
> Capture and decode POCSAG paging messages in real-time at 512/1200/2400 baud. Decodes alphanumeric and numeric messages. Default frequency: DAPNET (439.9875 MHz). Add custom frequencies via settings file on SD card.

#### ProtoPirate
> Rolling-code analysis for automotive key fobs — Kia, Ford, Subaru, Suzuki, VW/VAG, PSA, Fiat, StarLine, Scher-Khan. Captures and decodes rolling-code protocols showing manufacturer, button, counter, and encrypted payload. Transmission disabled by default. Educational/research only.

#### ProtoView *(by @antirez & (fixes by @xMasterX))*
> Digital signal Swiss army knife. Visualize raw RF pulses as waveforms, auto-detect protocols (TPMS, Keeloq, Oregon Scientific, PT2262). Edit decoded messages field-by-field and retransmit. Resample signals across different frequencies/modulations.

#### Radio Scanner *(by @RocketGod-git)*
> Scan CC1101 frequencies and play received signals through the speaker as audio. NOT an FM radio — demodulates Sub-GHz signals into audible sound. Useful for finding active frequencies by ear. OK adjusts spectrum width, Up/Down zoom, Left/Right switch bands.

#### Restaurant Pager *(by leedave)*
> Brute-force trigger restaurant pagers. Supports Retekess T119, TD157, TD165, TD174. Configure station and pager ranges, cycles through all combinations. Yellow LED = generating, Purple LED = transmitting.

#### Rolling Flaws
> Educational rolling code simulator by @CodeAllNight. Simulates a receiver with configurable security flaws for practicing replay, clone, future, rollback, and KGB/Subaru MF attacks in a safe sandbox. 12 tutorial scenarios with YouTube walkthrough.

#### RS41 Tracker *(by OK1CHP / yeckel)*
> Vaisala RS41 radiosonde receiver and decoder. Receives weather balloon telemetry using the CC1101 or external CC1101 module on GPIO. Decodes position, altitude, temperature, and other meteorological data.

#### ShapShup *(by derskythe)*
> Sub-GHz RAW file viewer. Opens saved .sub files and displays the signal waveform as it appears in the Sub-GHz RAW read mode. Scroll through the signal to analyze pulse patterns.

#### Spectrum Analyzer *(by @xMasterX & @theY4Kman & @ALEEF02 (original by @jolcese))*
> Real-time RF spectrogram showing signal amplitude vs frequency. Spikes indicate nearby transmitters. OK adjusts width, Up/Down zoom, Left/Right switch bands. Essential first step before capturing — find the frequency, then decode.

#### Sub Analyzer *(by RocketGod)*
> Offline .sub file analyzer. Opens saved captures and extracts frequency, modulation, protocol, data rate, bit count, and raw pulse timing. No radio hardware used — purely file analysis. Great for reviewing captures away from the target.

#### Sub Duplicate Finder *(by Endika)*
> Identify and clean up duplicate .sub files in storage. Scans your Sub-GHz folder, finds files with identical signal data, and helps you remove duplicates to keep your collection organized.

#### Sub-GHz Bruteforcer
> Systematically try all possible codes for static Sub-GHz protocols. Supports CAME, Nice, Princeton, Linear, and others. Each code sent 3x by default (adjustable). CAME 12bit 433MHz is the most common target — 4096 possible codes.

#### Sub-GHz Jammer Detect *(by PentestSuite / barkandbite)*
> Detects sustained RF carrier waves that indicate Sub-GHz jamming attacks. Monitors for continuous transmissions that could be blocking legitimate signals. Security awareness tool.

#### Sub-GHz Playlist *(by @darmiel)*
> Play a sequence of .sub files automatically from a text playlist. Configurable delay between files, looping options. Useful for testing multiple signals in sequence or automated demonstrations.

#### Sub-GHz Playlist Creator *(by coolerUA)*
> Visual editor for creating and managing Sub-GHz playlists. Create new playlists, browse SD card to add .sub files, reorder entries. Saves as .txt files compatible with the Sub-GHz Playlist player.

#### Sub-GHz Remote *(by gid9798 xMasterX)*
> Map up to 5 saved .sub files to d-pad buttons for instant one-press playback. Create remote configurations for frequently-used signals — garage doors, gates, lights. One press, no menus.

#### Sub-GHz Scheduler *(by Patrick Edwards)*
> Send Sub-GHz signals at configurable intervals (1 second to 24 hours). Relative or Precise timing modes. Supports individual .sub files or playlists. Configurable TX repeats (1-6x). For periodic beacon testing or timed automation.

#### Sub-GHz Spectrum *(by PentestSuite & barkandbite)*
> Real-time Sub-GHz spectrum analyzer with waterfall display. Shows signal activity over time with a scrolling waterfall visualization in addition to the standard amplitude graph.

#### SubGhz Toolkit *(by RocketGod)*
> Reverse engineer Flipper Zero Sub-GHz protocols and decrypt KeeLoq manufacturer codes. Extract protocol implementation details for security research.

#### TPMS Reader *(by wosk)*
> Read and activate Tire Pressure Monitoring System sensors using Sub-GHz radio and RFID. Decode tire pressure, temperature, and sensor ID. Can activate dormant sensors with wake-up signals. Multiple scan modes for different use cases.

#### Weather Station
> Receive and decode wireless weather sensor data. Supports Oregon Scientific, Acurite, LaCrosse, Ambient Weather, Solight, and many more. Shows temperature, humidity, wind, rain, and battery status. Sensors appear automatically as they broadcast.

</details>

<details><summary><h3>🔴 Infrared — 27 apps</h3></summary>

#### Cross Remote *(by Leedave)*
> One-click multi-command remote. Combine IR and Sub-GHz commands into playlists that execute with a single button press. Perfect for "movie night" — turn on TV, Blu-ray, surround sound all at once. Supports pause timing between commands.

#### Fire String *(by Ryan Aboueljoud)*
> Generate truly random strings using IR noise as entropy. Capture infrared bursts from sparking a flame as an unpredictable entropy source. Save and load strings from storage or transfer via USB.

#### Flame RNG *(by OrionW06)*
> Random number generator using IR flame sources. Point the IR receiver at a candle or lighter — the flickering IR pattern creates unpredictable data for true hardware randomness.

#### FlipIRFreq *(by jsammarco)*
> Transmit IR in carrier or low-frequency pulse modes with burst or continuous output. Configure frequency, duty cycle, burst length, and output pin. Two signal paths: carrier mode for standard IR and pulse mode for low-frequency.

#### Flipper Tag *(by thejudge156)*
> Laser tag compatible with Nerf Phoenix LTX laser tag guns. Use the Flipper's IR to play laser tag with actual Nerf blasters.

#### Hard Hat Brigade IR *(by Anomalous68)*
> Send IR messages to DEF CON 33 hard hats. Type your message, point at the receiver, press Save. After a vibration, the message is transmitted to the hard hat's display.

#### Hitachi AC Remote
> Hitachi air conditioner control. Emulates PC-LHxx series remote (PC-LH3 and PC-LH6Q variants). Power, temperature, fan speed, and mode control. See the included usage manual.

#### HTW AC Remote *(by sokogen)*
> IR remote for HTW air conditioners. Full protocol implementation with all AC modes (Off, Cool, Auto, Dry, Heat, Fan). Complete temperature and fan speed control.

#### InfraFi *(by amd989)*
> Transmit WiFi credentials from Flipper to a Linux server via infrared. Point, press Send, connected. Built for headless servers (NAS boxes, Intel NUCs) where typing WiFi passwords is painful.

#### IR Blaster *(by Mateusz Szafraniec)*
> Guess IR remote control codes for devices when you don't have the original remote. Cycles through common IR codes to find what works with your device.

#### IR Decoder *(by prplecake)*
> Shows IR address and command values like the standard IR app but without the remote interface. Also shows a value usable with lirc (Linux Infrared Remote Control).

#### IR Intervalometer *(by @Nitepone)*
> Camera shutter control for Pentax, Canon, Nikon, and Sony cameras using IR. Set an interval and the app fires the IR shutter release repeatedly. Useful for time-lapse photography without a wired remote.

#### IR Remote *(by @Hong5489 & @friebel & @d4ve10)*
> Map IR remote buttons to the Flipper's d-pad for quick access. Assign any saved IR signal to Up, Down, Left, Right, or OK. One-screen remote where each button fires a different IR command.

#### IR Scope *(by @kallanreed)*
> View incoming IR signals in real-time. Shows raw pulse timing data as signals are received. Useful for debugging remotes, verifying transmission, or analyzing unknown protocols.

#### IR Signal Generator *(by Sacriphanius / temboohms68)*
> Advanced IR signal generator with RF spectrum and external IR support. Create and emit custom infrared signals for security assessments and hardware testing.

#### IR Transfer *(by WBP20)*
> File transfer over infrared between two Flippers using the NEC protocol. Send and receive any file from the SD card via IR. Robust flow with error handling. No cables or radio needed.

#### Lasko 2519 Timer *(by LN4CY)*
> Automated timer for Lasko 2519 tower fans. Configurable ON/OFF cycle using the Flipper's IR blaster. Set desired intervals and the app automatically toggles fan power.

#### LIDAR Emulator *(by regaly)*
> Emulate LIDAR signals using the IR transmitter. Generate pulse patterns mimicking LIDAR distance measurement. Supports external IR boards for higher power. For testing LIDAR receivers or understanding protocols.

#### Midea AC Remote
> Midea air conditioner control. Full remote for Midea brand AC units — power, temperature, fan speed, swing, and mode. Compatible with RG10B(B)/BGEF and similar remote types.

#### Miele Scout RX2 *(by Kalle Varavas)*
> Remote control for Miele Scout RX2 robot vacuum with Drive and Menu modes. Control your robot vacuum via IR from the Flipper.

#### Mitsubishi AC Remote
> Mitsubishi Electric air conditioner control. Power, temperature, fan speed, and mode settings via IR. Available on the Flipper App Catalog.

#### Netflix TV Remote *(by ablaran)*
> IR remote for Netflix on Sony BRAVIA 4K VH2 TV. Death Note-themed interface with Netflix, Play/Pause, Mute, Power, and Volume buttons.

#### Pause Timer *(by Matt-London)*
> Pause your show when the ad break ends. Set a countdown and the app sends a pre-recorded IR signal (like Play/Pause) when the timer expires. Never miss the start of your show after a bathroom break.

#### TagTinker *(by i12bp8)*
> Educational ESL (Electronic Shelf Label) research toolkit. Protocol study, signal analysis, and controlled display experiments on authorized hardware. For studying how electronic price tags work.

#### Timed Remote *(by A. Damsgaard)*
> Send IR commands after a configurable time delay. Browse and select IR signals from .ir files, set a countdown timer (HH:MM:SS), and the signal fires when the timer completes.

#### Xbox Controller *(by @gebeto)*
> Xbox One IR remote control. Power on/off, navigate menus, media playback controls. Uses the Xbox One's built-in IR sensor. Point the Flipper at the Xbox and use d-pad for navigation.

#### XRemote *(by @kala13x)*
> Advanced IR remote with macro support. Create complex multi-step IR command sequences, organize remotes by device. More powerful than the built-in IR remote — supports timing delays, custom layouts, and saved configurations.

</details>

<details><summary><h3>🔵 Bluetooth — 11 apps</h3></summary>

#### Anki Remote *(by Blue5GD)*
> Customizable BLE keyboard remote. Assign any keyboard key to each Flipper button. Save multiple presets to SD card. Perfect for Anki flashcard review or any app that uses keyboard shortcuts. Create, edit, rename, and switch between presets.

#### BLE Clicker *(by Roman)*
> BLE HID keyboard for voice-driven Claude Code workflow on iTerm2. Turns Flipper into a wireless remote — dictate, submit, navigate panes and tabs without touching the keyboard. Designed to be held vertically.

#### BLE Spam *(by @Willy-JL @ECTO-1A @Spooks4576)*
> Flood BLE advertisements to cause spammy popups on nearby Apple, Android, and Windows devices. Generates fake pairing requests (AirPod popups on iPhone, Fast Pair on Android). Range ~10 meters. Use responsibly.

#### BT Trigger *(by @Nem0oo)*
> Control your smartphone camera remotely via Bluetooth. The Kiisu pairs as a BLE HID device. Right button takes a photo, OK for continuous shooting with adjustable delay. Shot count displayed, Left to reset. Works with iOS and Android.

#### BTHome *(by Alessandro Ghedini)*
> Turn the Flipper into a BTHome beacon for home automation integration. Compatible with Home Assistant and other systems supporting the BTHome protocol. Broadcast sensor data from your Flipper to your smart home.

#### Cookie Clicker *(by jamisonderek)*
> BLE mouse auto-clicker. Sends mouse clicks to your phone at a configurable frequency via Bluetooth. If you're playing Cookie Clicker or similar tap games, this automates the tapping. Only connect to devices you own.

#### Flight Monitor *(by Dr.Mosfet)*
> War Thunder flight simulator dashboard. Monitor aircraft parameters from the game displayed on the Flipper's screen via Bluetooth. See speed, altitude, and other telemetry in real-time.

#### Govee Control *(by devdotbo)*
> Control Govee H6006 Smart LED Bulbs via BLE. Power on/off control with BLE device discovery. Control your smart lights directly from the Flipper.

#### MagicBand Plus Lights
> Trigger light effects on compatible MagicBand+ wristbands via BLE. Not affiliated with Disney — names used only to describe compatibility.

#### PC Monitor *(by Olejka)*
> Monitor your computer's CPU, RAM, GPU, and temperature on the Flipper's screen via Bluetooth. Requires the PC Monitor backend running on your PC. Auto-connects and streams system metrics over BLE Serial.

#### Remote for Kodi *(by coded-with-claws)*
> Bluetooth remote for Kodi media center. Navigate menus, control playback. Long press OK to switch between Space and Return keys (useful for Kodi menu navigation). Based on the original BT HID app with Kodi-specific features.

</details>

<details><summary><h3>🎵 Media — 29 apps</h3></summary>

#### ATM Player
> Music player using the ATM synthesizer engine from Arduventure/Arduboy. Reads .atm text files from assets and plays them through the speaker. Chiptune-style audio playback.

#### BPM Tapper *(by @panki27)*
> Tap the center button along with a song to measure its beats per minute. BPM updates in real-time as you tap. Useful for DJs, musicians, or anyone who needs to know a song's tempo.

#### DVD Screensaver *(by shantih19)*
> The classic DVD player screensaver — a logo bouncing around the screen. Watch it and wait for the perfect corner hit. A mesmerizing time-waster.

#### Etch a Sketch *(by @SimplyMinimal)*
> Classic drawing toy. Use d-pad to move the cursor and draw lines. Press OK to draw a dot, hold OK for continuous drawing. Hold Back to clear. Simple pixel art on the 128×64 canvas.

#### Fart Sound Generator *(by F. Greil)*
> Adjustable flatulence sound generator. Configure Wet/Dry (frequency variation), duration, and other parameters. Juvenile but entertaining. Uses the Flipper's speaker for maximum effect.

#### Flizzer Tracker *(by LTVA)*
> Advanced 4-channel chiptune music tracker. Compose 8-bit music with SID-like sound channels. Supports external audio output (PA6 pin) and internal buzzer. A surprisingly capable music creation tool.

#### Fmatrix *(by misterwaztaken)*
> Matrix rain screensaver — the iconic falling green characters from The Matrix. Open and watch. Press any button to exit.

#### FMF2USBMIDI *(by crackerjacques)*
> Convert Flipper Music Player files to MIDI signals and transmit via USB. Connect to a DAW or music software and play Flipper music files as MIDI.

#### Guido Music Score Reader *(by F Greil)*
> Reader for Guido musical notation files. Load score files, start/stop playback with OK, browse for different files with Up. Plays music from notation files through the speaker.

#### Image Scroller *(by F Greil)*
> Navigate large annotated images on the Flipper's screen. Scroll around tiled images (128×64px tiles) with a cursor. CSV files specify tile arrangement and annotations.

#### Image Viewer *(by @polioan)*
> View images stored on the SD card. Create images with img2fbm converter, drop on SD card, and browse them on the 128×64 screen.

#### Karl Eido *(by F Greil)*
> Kaleidoscope simulator using 3 virtual mirrors to generate beautiful symmetrical patterns. Debug info shown in top-right corner. A visual toy for the tiny screen.

#### Metronome *(by @panki27 & @xMasterX)*
> Musical metronome. Set BPM with Up/Down and the Flipper clicks at that tempo. Essential for musicians practicing timing. Simple and reliable.

#### MIDI Ocarina *(by crackerjacques)*
> Push buttons to play MIDI notes over USB. A simple MIDI instrument — each button sends a different note. Connect to a DAW to hear the output.

#### MIDI RX *(by crackerjacques)*
> Receive MIDI signals and play them through the Flipper's speaker. Connect via USB and the Flipper acts as a MIDI sound module.

#### Morse Code *(by @wh00hw & @xMasterX)*
> Morse code encoder and decoder. Enter text and hear it as Morse code through the speaker. Up/Down for volume, Left/Right for dit length in milliseconds.

#### Music Beeper
> Simple music playback through the Flipper's speaker. Basic tone generation for music files.

#### Music Player
> Play RTTTL (Ring Tone Text Transfer Language) music files through the speaker. Thousands of songs available online in RTTTL format. Place .fmf or .txt files on SD card and browse to play.

#### Music Tracker *(by DrZlo13)*
> Tracker-style music player that plays hardcoded tracker songs. MVP (minimum viable player) stage.

#### Ocarina *(by invalidna-me)*
> Ocarina of Time instrument. Controls match the N64 version — OK button replaces the A button. Play the iconic Zelda melodies on your Flipper.

#### Paint *(by @n-o-T-I-n-s-a-n-e)*
> Basic drawing application. D-pad to move cursor, OK to draw. Simple pixel art tool on the 128×64 screen.

#### SAM AYBABTU
> Text-to-speech using SAM (Software Automatic Mouth) — the classic 1982 speech synthesizer. "All Your Base Are Belong To Us" themed. Type text and hear it spoken in SAM's robotic voice.

#### Snowflake *(by F Greil)*
> Aesthetic snowflake growth visualization. Watch iterative growth of a 60° or 120° snowflake on 64×64 pixels. Up/Down to adjust parameters. Beautiful generative art.

#### Space Playground *(by Alan Silva)*
> Space simulation screensaver with planets and asteroids. Highly customizable — configure the simulation to your liking. A relaxing visual experience.

#### Text to SAM *(by @Round-Pi & (Fixes by @Willy-JL))*
> Convert text to speech using SAM (Software Automatic Mouth). Type a message, press back, and SAM speaks it through the speaker. The same engine that powered speech on the Commodore 64.

#### Tuning Fork *(by @besya)*
> Generate precise reference tones for tuning instruments. Supports guitar (Standard, Drop D, DADGAD, and more), and other instruments. Select your tuning and the Flipper plays the reference pitch.

#### USB-MIDI
> Turn the Flipper into a USB MIDI instrument. Connect to a computer via USB and it appears as a MIDI device. Use buttons to send MIDI notes. Works with any DAW or music software. Monophonic.

#### Video Player *(by LTVA)*
> Play video files with synchronized audio on the 128×64 screen. Videos must be converted to Flipper's format (1-bit, 128×64). Surprisingly watchable on the tiny monochrome display.

#### WAV Player *(by @DrZlo13 & (ported)*
> Play WAV audio files through the speaker. Supports correct playback speed for different sample rates and mono files. Recommended: unsigned 8-bit PCM format. Browse .wav files on SD card.

</details>

<details><summary><h3>🔌 USB — 26 apps</h3></summary>

#### Agentic Remote USB *(by Kasen Sansonetti)*
> One-handed USB remote for AI coding agents (Claude Code). 5-button d-pad control — no keyboard required. Dictate and submit code without touching the keyboard.

#### Bad Duck3 *(by dutchpatriot)*
> DuckyScript 3.0 interpreter — the first full implementation for Flipper Zero. USB and Bluetooth HID support. Full control flow: variables, conditionals, loops, functions, and more.

#### BadUSB Pro *(by PentestSuite & barkandbite)*
> Advanced USB HID keystroke injection with LED feedback. Enhanced version of BadUSB with additional features and visual status indicators.

#### BarCode Scanner Emulator
> Emulates a barcode scanner for testing cash registers (Evotor). Sends barcode data over USB at 19200 baud. Why buy a scanner when you have a Flipper?

#### CCID Emulator *(by PentestSuite & barkandbite)*
> Programmable USB smartcard emulator via CCID (Chip Card Interface Device). Emulate smart cards over USB for testing card readers and applications.

#### Chameleon Ultra *(by Chameleon Flipper Team / muylder)*
> Control and manage Chameleon Ultra devices via USB or Bluetooth. Manage slots, configure emulation settings. Quick start guide and animation documentation included.

#### Click Recorder *(by 0x78f1935)*
> Mouse automation tool. Record, replay, randomize, or spam mouse clicks over USB or Bluetooth. Capture exact click timing and replay perfectly. No software needed on the target computer.

#### Clippy *(by mtormento)*
> Cross-device shared clipboard. Copy text on one device, paste on another via the Flipper. Turns your Flipper into a USB keyboard that types whatever you've stored. Solves the "WiFi password on new phone" problem.

#### FlipDeck *(by FlipDeck / ismail-oe)*
> USB HID macro pad with configurable pages. Media keys work instantly on any OS. Custom keys trigger configurable actions through a lightweight host daemon. Extensible plugin system.

#### FlipPass *(by Arckanos)*
> KeePass KDBX 4 vault browser with USB and Bluetooth HID typing. Open KeePass vaults stored on SD card, browse groups and entries, and type credentials to another device. Read-only — doesn't modify the database.

#### FlipperTrack *(by pinkpwningclub)*
> USB HID activity simulator — generates periodic mouse movement, keystroke bursts, and window-cycling inputs. Keeps systems active during presentations or testing. Mouse jiggler with extra features.

#### Gatekeeper *(by enexis1337)*
> Secure BadUSB password launcher with combo lock. Store credentials with labels and icons, protected by a combination lock. Select and inject passwords via USB HID.

#### HID Exfil *(by PentestSuite & barkandbite)*
> HID-based data exfiltration via keyboard LED feedback channel. Demonstrates how data can be extracted from a computer using the keyboard LED status bits as a covert channel. Security research tool.

#### HID File Transfer *(by Kavakuo)*
> Exchange files using HID protocol at ~260KB/s. Especially useful when mass storage is blocked on a PC. Requires a Windows/Linux client executable for the data exchange.

#### LEGO Dimensions Toy Pad *(by Seger)*
> Emulate a LEGO Dimensions USB Toy Pad. Play LEGO Dimensions on a console without the physical pad. Place virtual characters and vehicles from the Flipper's screen. Requires MIFARE Classic 1k NFC files.

#### Mass Storage
> Turn the Kiisu into a USB mass storage device for disk images. Mount .img files from SD card as a removable USB drive. Supports spoofing USB identity (manufacturer, product, VID/PID) for testing.

#### Mouse Jiggler *(by Jacob-Tate)*
> Simple USB mouse jiggler. Keeps your computer awake by generating tiny mouse movements. Prevents screen lock and sleep mode.

#### MTP *(by Alex4386)*
> Use the Kiisu as an MTP (Media Transfer Protocol) device. Browse and transfer SD card files through your computer's file manager. Alternative to qFlipper for file management. Works on Windows, macOS, and Linux.

#### Nintendo Switch Controller
> Emulate a Nintendo Switch Pro Controller over USB with macro recording and playback. Three control modes: D-Pad, Left Stick, Right Stick. Record button sequences and replay them.

#### PC Monitor USB *(by DonJulve & TheSainEyereg)*
> Monitor PC resources (CPU, RAM, GPU, temperature) via USB Virtual COM Port. USB version of the Bluetooth PC Monitor app. Requires the PC Monitor backend on your computer.

#### Portal Of Flipper *(by sanjay900)*
> USB emulator for portal-style NFC toy figures (Skylanders, Disney Infinity). Emulates the USB portal hardware. Load NFC figure data from saved .nfc files. Requires MIFARE Classic 1k format.

#### SD Card Mounter *(by Aaronjamt)*
> Use the Flipper as an SD card reader over USB. Raw card contents passed directly to the computer — supports filesystems not normally handled by the Flipper.

#### USB Consumer Control *(by piraija)*
> Send USB HID Consumer Control key presses — media keys like play/pause, volume, mute, next track, browser home, calculator launch. Useful for kiosk breakout testing or HID research.

#### USB HID Autofire *(by pbek)*
> Send rapid left-clicks as a USB HID device. Simple auto-clicker over USB. Configurable click rate.

#### USB MIDI Capturing
> Capture and monitor MIDI signals via USB. Real-time display of Note On/Off, Control Changes, Program Changes, Pitch Bend, and more.

#### USB Game Controller *(by crapbass)*
> XInput game controller emulator (Xbox 360). Connect via USB and the computer sees a standard Xbox 360 controller. D-pad = left joystick, OK = A, Back = B. Works with any PC game supporting XInput.

</details>

<details><summary><h3>🏷️ RFID — 8 apps</h3></summary>

#### DCF77 Transmitter *(by arha & xMasterX)*
> Sends the DCF77 time signal on the 125kHz RFID antenna and GPIO C3 pin. Synchronize radio-controlled clocks. Can transmit at original 77.5kHz or 155kHz for better range. Baseband signal output on C3 for external antenna.

#### DCF77 Clock Sync *(by mdaskalov)*
> Emulate DCF77 time signal on the RFID antenna and GPIO A4 pin. Uses PWM at 77.5kHz on GPIO. Clocks typically sync in 2-5 minutes depending on signal strength.

#### EM4100 Key Generator *(by @Milk-Cool)*
> Generate EM4100 key lists from a selected .rfid key file for use with the RFID fuzzer app. Creates systematic variations of a known key to test readers.

#### FDX-B Maker *(by snowsign / Skye Gibbs)*
> Construct ISO-compliant FDX-B animal microchip data on-device. Enter country code (0-999), national ID, and other fields. Saves as standard RFID files for writing to T5577 or EM4305 chips.

#### Simultaneous UHF RFID Reader *(by @RileyHaffner)*
> Read up to 150 UHF RFID tags per second using ThingMagic readers (M6E Nano, M7E Hecto, YRM1000). High-speed inventory scanning via GPIO. 28KB stack for dual-reader buffering.

#### T5577 Multiwriter *(by @Leptopt1los)*
> Write multiple different RFID keys to a single T5577 rewritable tag. Store 2-3 different EM41xx identities on one chip. Proof of concept from Dangerous Things forum.

#### T5577 Raw Writer *(by Torron)*
> Write raw data directly to T5577 tags. Configure modulation, RF clock, number of blocks, and block data. Load .t5577 files or configure manually. More control than standard RFID write.

#### YRM100 UHF RFID
> Read UHF RFID tags using a YRM100 module via GPIO. UHF operates at 860-960 MHz with much longer range than 125 kHz. Displays tag EPC data. For inventory management and asset tracking.

</details>

<details><summary><h3>🔑 iButton — 2 apps</h3></summary>

#### iButton Converter *(by @leptopt1los)*
> Convert Cyfral and Metakom iButton keys to Dallas format. Multiple conversion modes for different key types. Useful when you need a key in a different format for compatibility.

#### iButton Fuzzer *(by gid9798 xMasterX)*
> Fuzzer for iButton readers. Test readers with default UIDs and frequently used UIDs. Identifies "denial of service" vulnerabilities from incorrect UIDs or rapid access. Visual style inspired by the Sub-GHz bruteforcer.

</details>

<details><summary><h3>⚙️ Settings — 7 apps</h3></summary>

#### Animation Switcher *(by lsalik2)*
> Create, manage, and switch animation playlists on the fly. Template manifest.txt files control which background animations play. Supports up to 128 animations per playlist. Fine-tune per-animation settings.

#### Antidepressants (Dolphin Trainer)
> Change your Flipper's XP level and butthurt values. Adjust background XP, modify dolphin emotional state, and change levels directly. By Lambda.

#### Extend Range
> Extend Sub-GHz range by automatically configuring the CC1101 module for full potential. Does what manual configuration changes do, but with one click.

#### Gauge Tool *(by Aku)*
> Gauge calibration tool. Use only if you know what you're doing — modifies hardware calibration settings.

#### Name Changer
> Change your Flipper's display name. Set a custom name that appears on the device and in Bluetooth advertising.

#### Smack My Dolphin Up! *(by siberianbot)*
> Reset dolphin emotional state for those who don't want to deal with the virtual pet's mood system. Quick fix for a grumpy dolphin.

#### Theme Manager *(by Hoasker)*
> Manage dolphin animation themes from SD card — no PC required. Browse, preview, and switch between animation themes directly on-device. Available on the Flipper App Catalog.

</details>

<details><summary><h3>🔧 Tools/Educational — 7 apps</h3></summary>

#### ASCII Table *(by x10102)*
> Pocket ASCII reference table. Scroll through all ASCII characters with their decimal, hex, and binary values. Up/Down to scroll, Left/Right to skip 10 symbols. You never know when you might need one.

#### C Programming Language
> The C Programming Language reference by Kernighan and Ritchie. Browse the complete guide to ANSI standard C on your Flipper's screen. A programmer's pocket reference.

#### Crypto Dictionary *(by armixz)*
> Comprehensive cryptography reference. Detailed information on various cryptographic algorithms — their history, how they work, and their security properties. Educational reference tool.

#### Extra Demo *(by Milk-Cool)*
> A pack of extra commands for the Flipper Zero (C library). Demo application showcasing additional capabilities. Useful as a reference for developers.

#### FAP Boilerplate *(by leedave)*
> Template/starting point for new Flipper Zero applications. Includes start screen, menu, and basic app structure. Change the contents when developing a new project.

#### Flipper Doctor *(by JulanDeAlb)*
> View all information about your Flipper's experience and butthurt values. General XP and butthurt display. Diagnostic tool for understanding your dolphin's state.

#### Plugin Demo *(by csBlueChip)*
> Simple plugin tutorial with excessive documentation and error handling. Written as an educational example for developers learning to create Flipper Zero applications.

</details>

<details><summary><h3>📦 Misc — 1 apps</h3></summary>

#### Clock (DAB Timer)
> Clock with stopwatch and configurable alarm (default 80 seconds, adjustable in 5-second intervals). Sound options configurable. A utility clock with timer functionality.

</details>

<details><summary><h3>⚡ GPIO/Sensors — 21 apps</h3></summary>

#### Plantower PMSx003
> Air quality monitor using Plantower PMS sensors. Displays PM1.0/PM2.5/PM10 particulate matter concentrations and calculated Air Quality Index (AQI).

#### Atomic Dice Roller
> True random number generator using a J305 Geiger-Müller tube. Radioactive decay events generate genuinely random dice rolls. Hardware RNG at its most literal.

#### CO2 Logger (MH-Z19)
> CO2 data logger with CSV export. Record carbon dioxide levels over time using an MH-Z19 sensor. Export data for analysis.

#### Distance Sensor (USPING)
> Ultrasonic distance measurement using Parallax PING))) sensor (SKU 28015, 3-wire). Displays distance readings in real-time.

#### Gas Sensor *(by iancohee)*
> Read BME680 and SGP30 environmental sensor data. Monitor air quality, temperature, humidity, and pressure with these popular I2C sensors.

#### Geiger Counter (J305)
> Radiation detector using J305 Geiger-Müller tube on external board. Displays counts per minute and estimated radiation dose.

#### Distance Sensor (HC-SR)
> Ultrasonic distance measurement using HC-SR04 sensor. Displays distance in centimeters. Simple and accurate for short-range measurements.

#### Lightmeter (BH1750)
> Photography lightmeter using BH1750 ambient light sensor via I2C. Measure light levels for exposure calculations.

#### MAX31855 *(by skotopes)*
> Thermocouple temperature reader using MAX31855 sensor. Read high-temperature measurements from K-type thermocouples.

#### Accelerometer (Mx2125)
> Read acceleration data from Memsic 2125 dual-axis accelerometer. Display tilt and motion data.

#### CO2 Sensor (MH-Z19)
> Real-time CO2 measurement using MH-Z19 sensor. Display current carbon dioxide concentration.

#### MH-Z19 UART *(by Aku)*
> Alternative MH-Z19 CO2 sensor interface via UART. Direct serial communication with the sensor.

#### Radar Scanner (RCWL0516)
> Detect movement of living things using RCWL-0516 microwave radar module. Detects motion through walls and obstacles.

#### Radiation Sensor *(by sionyx)*
> Interface with RadSens external radiation detection module. Display radiation levels from the dedicated sensor board.

#### Motion Sensor (RCWL-516)
> Motion detection using RCWL-516 radar module. Simple presence detection without line-of-sight requirement.

#### Sleep Counter (RCWL0516)
> Count how much you move in your sleep using RCWL-0516 radar. Place near your bed and track nighttime movement. Interesting sleep quality data.

#### Unitemp
> Universal sensor reader supporting 20+ temperature, humidity, pressure, and CO2 sensors. DHT11/22, DS18B20, BMP280, BME280, SHT30, AHT10/20, and many more. Auto-detects connected sensors. The go-to app for environmental monitoring.

#### UV Meter (AS7331)
> Measure UV radiation using AS7331 sensor via I2C. Monitor ultraviolet light levels — useful for sun safety and UV curing applications.

#### VEML7700 Lux Meter *(by kamylwnb / Dr.Mosfet)*
> Measure ambient light in lux using VEML7700 sensor via I2C. Displays real-time light level readings.

#### VL6180X Distance Sensor
> Distance measurement using VL6180X time-of-flight sensor via I2C. Accurate short-range distance readings. Ported from Adafruit's CircuitPython library.

#### Water Sensor Reader *(by MatveyStrelov)*
> Read analog water sensor values via GPIO. Displays raw ADC value and corresponding voltage in millivolts. For leak detection and water level monitoring.

</details>

<details><summary><h3>⚡ GPIO/ESP32 — 14 apps</h3></summary>

#### BLE Killer *(by H4lo)*
> Bluetooth expansion board scanner. Scan surrounding BLE devices and controllers using ESP32 Dev Board.

#### BLE Scanner *(by PentestSuite / barkandbite)*
> BLE advertisement scanner via ESP32 Dev Board. Detects AirTags and other BLE beacons in the area.

#### BT Audio *(by Father Divine)*
> Control ESP32 Bluetooth audio via UART. Use the Flipper to manage an ESP32-based Bluetooth speaker or audio receiver.

#### Camera (ESP32)
> ESP32-CAM live feed and photo capture. View camera feed on Flipper's screen in real-time. Left/Right for orientation, Up/Down for brightness, OK to save screenshot.

#### Gravity *(by chris-bc)*
> Control ESP32 Gravity wireless exploration platform. Comprehensive WiFi and Bluetooth toolkit running on dedicated ESP32 hardware.

#### Evil BLE *(by PentestSuite / barkandbite)*
> BLE advertisement cloning. Scan BLE advertisements with ESP32 and clone/replay them. For BLE security research.

#### FlipRPI *(by jblanked)*
> Control your Raspberry Pi from the Flipper Zero. Send commands and monitor status via GPIO connection.

#### Gemini IA *(by d4rks1d33 & jamisonderek)*
> Interact with Google Gemini AI using ESP32 for WiFi connectivity. Send prompts and receive AI responses on the Flipper's screen.

#### Morse Flash (CAM)
> ESP32-CAM app to stream messages in Morse code using the camera's flashlight LED. Powerful flash visible at distance. Unplug USB cable before testing.

#### Postman *(by MassivDash)*
> Send HTTP requests from the Flipper using ESP32S2 with Postman firmware. Make GET/POST requests and see responses. API testing from a tiny device.

#### Ray Hunter Client *(by PentestSuite / barkandbite)*
> IMSI catcher detection. Displays Ray Hunter status via ESP32 WiFi bridge. Detect fake cell towers (stingrays) in your area.

#### Rogue AP Detector *(by PentestSuite / barkandbite)*
> Detect evil twin and rogue WiFi access points via ESP32 Dev Board. Identify suspicious access points that may be impersonating legitimate networks.

#### Wendigo
> BT + BLE + WiFi monitor. Monitor nearby radio signals and their strength using ESP32. See all wireless activity around you.

#### WiFi Mapping *(by carvilsi)*
> WiFi mapping with ESP32 and Flipper. Scan and map WiFi networks with location data. Check the ESP32 side at esp32-wifi-map.

</details>

<details><summary><h3>⚡ GPIO/ESP — 10 apps</h3></summary>

#### Camera Suite
> ESP32-CAM live feed and photo capture. View camera feed on screen, change orientation/mode, adjust brightness, save screenshots. Requires ESP32-CAM module. Unplug USB before testing.

#### ESP8266 Deauther
> Interface for DSTIKE ESP8266 Deauther module. Control WiFi deauthentication from the Flipper's screen. Select targets, start/stop, monitor status.

#### ESP Flasher
> Flash firmware onto ESP32/ESP8266 modules directly from the Flipper. Supports Marauder, Ghost ESP, BlackMagic, FlipperHTTP. Handles bootloader mode automatically.

#### Evil Portal
> Create captive portal WiFi access point using ESP module. Victims see a customizable login page. Credentials captured and displayed on Flipper. Custom HTML pages on SD card.

#### Ghost ESP
> Companion for Ghost ESP firmware on ESP32. WiFi: scan, beacon spam, deauth, packet capture, evil portal. Bluetooth: Flipper discovery, Pineapple detection, AirTag scanning. GPS: wardriving in WiGLE format.

#### IFTTT Button
> Trigger IFTTT webhooks from Flipper using ESP8266. Press a button to fire an IFTTT event over WiFi. For home automation and notifications.

#### Wardriver
> Sniff WiFi APs and log with GPS coordinates in WiGLE format. Requires ESP for WiFi + GPS module. Drive around logging every network. Upload to WiGLE.net.

#### WiFi Deauther v2
> Interface for ESP8266 Deauther v2 by SpacehuhnTech. More advanced — scanning, target selection, multiple attack modes. ESP8266 handles WiFi; Flipper provides UI.

#### WiFi Marauder
> Companion for ESP32 Marauder firmware. Comprehensive WiFi toolkit: scan APs/stations, sniff packets, deauth, beacon spam. Supports Marauder v1.8+.

#### WiFi Scanner
> Scan nearby WiFi networks using ESP module. Shows SSID, signal strength (RSSI), channel, and encryption type. Quick WiFi survey tool.

</details>

<details><summary><h3>⚡ GPIO/FlipperHTTP — 11 apps</h3></summary>

#### FlipDownloader *(by JBlanked)*
> Download apps and assets via WiFi directly to Flipper. Browse GitHub repos, download .fap files, install without a computer. Requires FlipperHTTP ESP module.

#### FlipLibrary *(by JBlanked)*
> Retrieve data from 20+ APIs via WiFi. Random facts, quotes, weather, dictionary definitions, and more. Each API accessible from a simple menu.

#### FlipMap *(by JBlanked)*
> Find other Flipper Zero users on a map. Register your location, see where others are worldwide. Community-driven, privacy-conscious.

#### FlipSocial *(by JBlanked)*
> Social media platform for Flipper Zero. Post messages, read feeds, comment, interact with other users. A social network on a 128×64 screen.

#### FlipTelegram *(by JBlanked)*
> Telegram messaging client. Send and receive messages from the Flipper's screen. Configure with your bot token.

#### FlipTrader *(by JBlanked)*
> Stock prices and currency exchange rates. Enter a ticker or currency pair, get real-time price data via WiFi.

#### FlipWeather *(by JBlanked)*
> GPS coordinates and weather information. Current temperature, conditions, wind speed, forecast. Fahrenheit and Celsius.

#### FlipWiFi *(by JBlanked)*
> FlipperHTTP companion and configuration app. Set up ESP module WiFi — enter SSID/password, test connectivity. Run this first before other FlipperHTTP apps.

#### FlipWorld *(by JBlanked)*
> First open-world multiplayer game for Flipper. Explore 2D world, encounter players, fight enemies, collect items. Best with Video Game Module.

#### Free Roam *(by JBlanked)*
> 3D open-world multiplayer game. Minimap, first-person perspective on the tiny screen. Requires FlipperHTTP for multiplayer.

#### Web Crawler *(by JBlanked)*
> Browse the web from your Flipper. Enter a URL, fetch page content, display text. Also makes API calls and shows JSON responses.

</details>

<details><summary><h3>⚡ GPIO/NRF24 — 11 apps</h3></summary>

#### NRF24 Batch
> Batch operations for NRF24L01+ modules. Run multiple NRF24 operations in sequence.

#### NRF24 Channel Scan
> Scan 2.4GHz frequency band for usage data. See which channels are busy and which are clear. Useful for finding clean channels for your wireless projects.

#### NRF24 Mouse Jacker
> Perform mousejack attacks using captured addresses from the NRF24 Sniffer app. Inject keystrokes into vulnerable wireless mice and keyboards.

#### NRF24 Scanner
> Scan for NRF24L01+ devices in the area. Discover active NRF24 transmitters and their addresses.

#### NRF24 Sniffer
> Capture NRF24L01+ addresses for use with Mouse Jacker. Passively listen for wireless mouse/keyboard communications.

#### NRF24 Monitor *(by CyberDemon73)*
> Monitor and analyze NRF24L01+ wireless traffic in real-time. See packet data and communication patterns.

#### NRF24 Jammer (FZ)
> 2.4GHz interference tool using NRF24L01+ module. Creates interference on the 2.4GHz band.

#### NRF24 Jammer (huuck)
> Alternative 2.4GHz jammer leveraging NRF24 module. Different implementation of 2.4GHz interference.

#### AZERTY Mouse Jacker *(by mothball187 & xMasterX)*
> Mouse Jacker variant for AZERTY keyboard layout. Performs mousejack attacks with French keyboard mapping.

#### Mouse Jacker MS *(by coded-with-claws)*
> Mouse Jacker hardcoded for Microsoft wireless mice. Targeted implementation for Microsoft wireless peripherals.

#### Sniffer MS *(by coded-with-claws)*
> NRF24 sniffer and mousejacker hardcoded for Microsoft wireless mice. Combined sniff-and-attack for Microsoft devices.

</details>

<details><summary><h3>⚡ GPIO/FlipBoard — 5 apps</h3></summary>

#### FlipBoard Blinky
> Turn your FlipBoard into a blinky badge with LED patterns and effects.

#### FlipBoard Keyboard
> Turn your FlipBoard into a USB/BLE keyboard with customizable key mappings.

#### FlipBoard Signal
> Turn your FlipBoard into a signal sender for Sub-GHz, IR, or other protocols.

#### FlipBoard Simon *(by codeallnight)*
> Simon memory game on the FlipBoard hardware. Watch the LED sequence and repeat it.

#### FlipBoard Simon (alt)
> Alternative Simon game implementation for FlipBoard.

</details>

<details><summary><h3>⚡ GPIO/Debug — 8 apps</h3></summary>

#### AVR Flasher
> Flash AVR microcontrollers (ATmega, ATtiny) directly from the Flipper via ISP. Program Arduino-compatible chips without a computer.

#### DAP Link
> Use Flipper as a CMSIS-DAP debug probe for ARM devices via SWD/JTAG. Debug ARM microcontrollers with standard tools like OpenOCD.

#### Ethernet (W5500)
> Test Ethernet connections with W5500 module. Verify network connectivity and diagnose Ethernet issues.

#### FlipTDI (FTDI232H)
> FTDI232H USB-to-serial emulator. Use the Flipper as an FTDI-compatible serial adapter.

#### I2C Tools
> Set of I2C diagnostic tools. Scan the bus, read/write registers, and debug I2C communication issues.

#### INA Meter (INA2xx)
> Read TI INA-series current/voltage/power sensors. Monitor power consumption of circuits in real-time.

#### SPI Mem Manager
> Read and write 25-series SPI flash chips (W25Q, AT25SF, MX25L). Dump firmware, write images, verify contents. Chip identification via JEDEC ID.

#### SWD Probe
> ARM Single Wire Debug probe. Debug ARM microcontrollers using the SWD protocol directly from the Flipper.

</details>

<details><summary><h3>⚡ GPIO/MAYHEM — 6 apps</h3></summary>

#### MAYHEM Camera
> ESP32-CAM live feed and photo capture for Mayhem boards. Unplug USB cable before testing.

#### MAYHEM Marauder
> ESP32-CAM Marauder with camera and flashlight control. All original Marauder functionality plus camera features.

#### MAYHEM Morse Flash
> Stream Morse code messages using ESP32-CAM's powerful flashlight LED.

#### MAYHEM Motion Detection
> ESP32-CAM motion detection with beep alert. Detects movement in the camera's field of view.

#### MAYHEM Nanny Cam
> ESP32-CAM remote camera. Start a WiFi-accessible camera stream.

#### MAYHEM QR Code
> ESP32-CAM QR code reader. Decode QR codes using the camera.

</details>

<details><summary><h3>⚡ GPIO/VGM — 4 apps</h3></summary>

#### VGM Air Mouse
> Turn Flipper with Video Game Module into an air mouse. Tilt to move cursor, buttons for clicks.

#### Video Game Module Tool
> Standalone firmware updater/installer for the Video Game Module. Keep your VGM up to date.

#### VGM Game Remote *(by JBlanked)*
> Companion app for the VGM Game Engine. Control games running on the Video Game Module.

#### Video Game Module Console *(by JBlanked)*
> Flipper Zero console for VGM — handles display rendering and input forwarding so developers can focus on hardware.

</details>

<details><summary><h3>⚡ GPIO/Games — 2 apps</h3></summary>

#### UART Pong *(by Olejka)*
> Multiplayer Pong over UART. Connect two Flippers via UART and play Pong against each other.

#### VL6180X Pong *(by Kyle Diller)*
> Pong controlled by VL6180X distance sensor. Wave your hand to move the paddle instead of pressing buttons.

</details>

<details><summary><h3>⚡ GPIO/GPS — 2 apps</h3></summary>

#### GPS (NMEA)
> Display real-time GPS data from UART NMEA modules. Latitude, longitude, altitude, speed, heading, satellites. GPS TX→pin 14, VCC→3V3, GND→pin 18. Default 9600 baud.

#### Nearby Files
> GPS-enabled file browser showing saved .sub/.nfc/.rfid files sorted by distance from current location. Find which saved signal belongs to a nearby gate or door. Web version at flipper-map.stichoza.com.

</details>

<details><summary><h3>⚡ GPIO/MALVEKE — 8 apps</h3></summary>

#### GB/GBC Cartridge
> Read Game Boy and Game Boy Color cartridges using MALVEKE hardware. Dump ROM and save data.

#### GB Emulator
> Game Boy emulator proof-of-concept using MALVEKE hardware. Run Game Boy ROMs on the Flipper.

#### GB Link-Camera
> Extract Game Boy Camera pictures via WiFi using MALVEKE. Print as usual and the device stores images on a web server.

#### GB Live Camera
> Use a Game Boy Camera cartridge as a live camera with MALVEKE. View the camera feed on the Flipper's screen and take snapshots.

#### GB Photo
> Save Game Boy Camera RAM photos as BMP files from the Flipper. Extract photos without the original Game Boy.

#### GB PIN Test
> ESP32 Game Boy Camera live feed and photo capture test. Verify camera functionality with MALVEKE hardware.

#### GBA Cartridge
> Read Game Boy Advance cartridges using MALVEKE hardware. Dump GBA ROM data.

#### GB Printer *(by kbembedded)*
> Emulate a Game Boy Printer. Receive photos from the Game Boy Camera and save as sequentially numbered PNG files on SD card.

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
