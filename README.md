
# Zork - Russina Nightmare
Máster Advanced Programming for AAA Video Games - UPC Talent School - Test
#
# Author
Jordi Gil Gonzalez
#
# Info
This game is a basic text adventure, it works through a console, it doesn't have any kind of graphics support. Through a series of commands you must get to the end of the game.
# 
You can find the code for this game here: https://github.com/Jordi-Gil/zork
# Guide
1. You start out in a room and you're chained. By interacting with the bed you can end up breaking it to get a spring that will allow you to unlock the lock.
```
interact bed
interact bed
pick spring
unlock padlock with spring
open south
go south
```
2. Once you have managed to leave the room you have to go to the north-est corridor of the house and interact with the painting. As you interact with it, a hole will appear in the wall. Once the hole is accessible, we interact with it to see the code that will allow us to open the next door.
```
go east
go north
interact painting1
interact wallhole
go west
unlock south with 9821
open south
go south
```
3. To continue we must find a pair of keys, but first we must arm ourselves to get them. To do this we must go to the clock corridor. We interact with the clock. Once we've interacted, a door in the west wing will open.
```
open south-east
go south-east
interact grandfatherclock
```
4. Before going to the west wing room that we have opened, we will take a fire extinguisher from the kitchen and elements that will allow us to raise the life to be able to leave this adventure successfully.
```
open east
go east
look table
pick apple from table
go south
pick extinguisher
pick medkit
interact medkit
interact apple
``` 
5. When we have the fire extinguisher, and we have increased our life, we go back to the Living Room.
```
go north
go north-west
go west
```
6. Since we already have the fire extinguisher, we can interact with the fireplace to open the bathroom door.
```
interact fireplace
```
7. Before we go to the bathroom, we'll go get a gun from the west wing.
```
open north-west
go north-west
```
8. We take the bust, so we can break the display case and get a weapon.
```
pick bust1
go north
throw bust1 to showcase1
pick katana
equip katana
```
9. With the weapon equipped and the improved health we went to the bathroom and looted the body of the boss to get another key.
```
go south
go east
go south-west
loot bossbody
``` 
10. We get out of the bathroom and go to the guest room. We kill the butler and loot his body.
```
go east
open south
go south
attack butler
loot butler
```
11. Now we have to go to the boss's office in the east wing.
```
go north
go south-east
go east
open east
go east
unlock north-west with masterkey
open north-west
go north-west
```
12. If you've followed the steps, you'll survive and be able to continue.
13. We loot the hitman and with the key to the office we open the door and go in.
```
loot hitman1
unlock north with officekey
open north
go north
```
14. Once we are in the office we take the aram and vest from the safe and the medical material from the first aid kit.
```
look safe
pick ak47 from safe
equip ak47
pick bulletproofvest from safe
equip bulletproofvest
look firstaidlocker
pick bandage1 from firstaidlocker
pick bandage2 from firstaidlocker
pick morphine from firstaidlocker
interact bandage1
interact bandage2
interact morphine
```
15. Now we're heading to the security room to open the front door.
 ```
go south
go east
go south-west
unlock south-west with masterkey
open south-west
go south-west
go south
open north-east
go north-east
interact computer
go south
go east
 ```
#
# License
MIT License

Copyright (c) 2020 Jordi Gil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
