# MAME compatibility results

The comparison used the [pinned MAME driver](https://github.com/mamedev/mame/blob/ce8812e6a1dcafc97fbafc6d48b0c8184984845f/src/mame/handheld/gamate.cpp) and the Windows executable from [MAME CI run 44883418648](https://github.com/mamedev/mame/actions/runs/44883418648). The SHA-256 of the downloaded CI archive was `049a8c9c4270454523738bc48cf0d4ca9d82b2d5e68a249831a7641f6d2fc77f`.

Both UMC and BIT BIOS revisions booted every cartridge in the 62-game collection. The harness applied the same Start and A inputs in each emulator. All 874 sampled LCD frames matched at every pixel with MAME colors and ghosting off. All 168,236 AY writes matched in register, value and order with a timestamp tolerance of one CPU cycle.

The comparison uses the overlapping stereo PCM at 48 kHz, before frontend filters or volume scaling. Across **240,594,240** signed 16-bit channel samples, the maximum absolute error was **6** and the largest per-run RMS error was **0.543** sample units. Capture lengths can differ at the last frontend audio buffer. The tests check boot, title screens and scripted early play. Later levels and physical handheld timing have not been characterized.

Each row combines both BIOS runs. Frames were sampled at 60, 180, 300, 360, 600, 900 and 1200. Three cartridges have an additional checkpoint at 1800. The last column reports the largest absolute PCM sample error for the two runs.

| Cartridge | MAME board | Matching frames | AY writes | Peak PCM error |
|---|---|---:|---:|---:|
| 4 in 1 - Cube Up + Mini Golf + Vindicators + Brick Blaster (World) | multi | 16 | 4536 | 5 |
| Bao Qing Tian (Taiwan) | banked | 14 | 1100 | 5 |
| Baseball (World) | banked | 14 | 1296 | 5 |
| Basketball (World) | banked | 14 | 1722 | 5 |
| Bomb Blaster (World) (Alt) | plain | 14 | 2212 | 6 |
| Bomb Blaster (World) | plain | 14 | 2212 | 6 |
| Boom! (World) | plain | 14 | 1604 | 5 |
| Box Forum (World) | plain | 14 | 726 | 5 |
| Brick Blaster (World) | plain | 14 | 2098 | 4 |
| Cosmic Fighter (World) | plain | 14 | 1730 | 5 |
| Cube Up (World) | plain | 16 | 3698 | 5 |
| Devil Castle (World) | plain | 14 | 982 | 5 |
| Dino Ball (World) | plain | 14 | 2576 | 5 |
| Dino Bibo (World) | plain | 14 | 2820 | 5 |
| Dinosaur Park (World) | banked | 14 | 2430 | 5 |
| Enchanted Bricks (World) | plain | 14 | 2414 | 3 |
| Famous 7 (World) | banked | 14 | 468 | 5 |
| Fantasy Travel (World) | banked | 14 | 1076 | 4 |
| Fist of Thunder (World) | banked | 14 | 846 | 3 |
| Flipuzzle (World) | plain | 14 | 2500 | 3 |
| Flying Goblin (World) | banked | 14 | 700 | 5 |
| Fortress of Fierceness (World) | plain | 14 | 1018 | 4 |
| Fortune 'n Luck (World) | banked | 14 | 1014 | 4 |
| Further Adventures of Hannibal The Cat (World) | banked | 14 | 710 | 4 |
| Galaxy Invaders (World) | plain | 14 | 4228 | 5 |
| Golden Pyramid, The (World) | banked | 14 | 1834 | 4 |
| GP Race (World) | banked | 14 | 476 | 4 |
| Hot Hero (World) | banked | 14 | 716 | 4 |
| Incantational Couple (World) | plain | 14 | 2864 | 3 |
| Insect War (World) | banked | 14 | 622 | 3 |
| Jewelriss (World) | plain | 14 | 944 | 4 |
| Kiki Inland (World) | banked | 14 | 1314 | 4 |
| Kill Shot (World) | plain | 14 | 38464 | 3 |
| Kung-Fu Fighter (World) | plain | 14 | 1754 | 5 |
| Legend of Dragon Knight (World) | banked | 14 | 1702 | 5 |
| Magic Jigsaw (World) | plain | 14 | 1404 | 5 |
| Marauder (World) | plain | 14 | 3004 | 4 |
| Mars Voyage (World) | banked | 14 | 2196 | 5 |
| Mighty Boxer (World) | banked | 14 | 1584 | 5 |
| Mighty Tank (World) | plain | 14 | 1426 | 4 |
| Mini Golf (World) | plain | 14 | 2684 | 5 |
| Money Maze (World) | plain | 14 | 2280 | 5 |
| Monster Pitfall (World) | plain | 14 | 1324 | 3 |
| Myth of Asamia (World) | plain | 14 | 1500 | 4 |
| Nightmare of Santa Claus (World) | plain | 14 | 1208 | 5 |
| One Million Whys (World) | banked | 16 | 1382 | 5 |
| Pharaoh Revenger (World) | plain | 14 | 1848 | 5 |
| Pipemania (World) | plain | 14 | 942 | 3 |
| Punk Boy (World) | banked | 14 | 1502 | 4 |
| QuizFighter (World) | banked | 14 | 468 | 4 |
| Riddle of the Ancient Tomb (World) | banked | 14 | 2810 | 4 |
| Robin Hood (World) | banked | 14 | 2768 | 5 |
| Snowman Legend (World) | plain | 14 | 1572 | 4 |
| Tennis (World) | plain | 14 | 1578 | 5 |
| Time Warrior (World) | plain | 14 | 1562 | 5 |
| Tornado (World) | plain | 14 | 24196 | 5 |
| Tough Guy (World) | banked | 14 | 3134 | 5 |
| Treasure Hunter (World) | plain | 14 | 2870 | 5 |
| Vindicators (World) | plain | 14 | 1426 | 4 |
| Volcano Panic (World) | banked | 14 | 1782 | 6 |
| Witty Apee (World) | plain | 14 | 772 | 4 |
| World Cup Soccer (World) | banked | 14 | 1608 | 5 |
