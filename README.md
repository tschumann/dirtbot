Dirtbot
=======

Getting started
---------------

Clone and get the submodules.
```
git clone https://github.com/tschumann/dirtbot
git submodule init
git submodule update --init --recursive
```

Installation
------------

Linux:
```
sudo pip3 install --break-system-packages alliedmodders/ambuild/
```

Windows:
```
pip install  alliedmodders/ambuild/
```


Compiling
---------

Linux:
```
./build.sh
```

Windows:
```
./build.ps1
```


Running
-------

Start the game with the following parameters: `-dev -insecure`

In the console type `rcbot addbot`


Testing
-------

```
./test_unit.ps1
```


Updating MetaMod: Source
------------------------

Download the latest MetaMod: Source from https://www.sourcemm.net/downloads.php?branch=stable

Copy the `addons` directory from the .zip and paste it in `release/dirtbot/Half-Life 2 Deathmatch/hl2mp/`

Note that for Source 2006/Episode One based games that the path in metamod.vdf is incorrect and needs ../gamedir/ prepended to it.


License
-------

Affero GPL 3.0 (and BSD Zero Clause for `rcbot/logging.cpp` and `rcbot/logging.h`) because RCBot2 is Affero GPL 3.0 (and BSD Zero Clause for `rcbot/logging.cpp` and `rcbot/logging.h`).


## Information:-

This is a fork of [the official RCBot2 plugin][rcbot2] written by Cheeseh.
Special thanks to pongo1231 for adding more TF2 support and NoSoop for adding AMBuild support and many more!


## Credits:-

- Founder - Cheeseh
- Bot base code - Botman's HPB Template
- Linux Conversion and Waypointing - [APG]RoboCop[CL]
- TF2 support and enhancements - Ducky1231/Pongo
- SourceMod and AMBuild support - nosoop
- Synergy, MvM and CSS support - Anonymous Player/caxanga334
- TF2 Classic support - Technochips
- Linux Black Mesa and SDK2013 mathlib fix - Sappho
- Dystopia support - Soft As Hell
- Major waypointer for TF2 - LK777, RussiaTails, Witch, Francis, RevilleAJ
- Major waypointer for DoDS - INsane, Gamelarg05, genmac

## Waypointers:-

- NightC0re
- wakaflaka
- dgesd
- naMelesS
- ati251
- Sandman[SA]
- Speed12	
- MarioGuy3
- Sjru	
- Fillmore
- htt123
- swede
- YouLoseAndIWin
- ChiefEnragedDemo
- madmax2
- Pyri
- Softny		
- Wolf
- TesterYYY		
- darkranger		
- Emotional
- J@ck@l		
- YuriFR
- Otakumanu		
- 芝士人
- Eye of Justice
- TheSpyhasaGun (ScoutDogger)		
- NifesNforks
- parkourtrane
- assface
- Doc Lithius
- Kamaji
- Geralt
- Smoker		
- dzoo11
- Combine Soldier		
- cyglade
- TFBot_Maverick
- apdonato
- Sntr
- mehdichallenger		
