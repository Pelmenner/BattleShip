# BattleShip
A battleship game created with Qt Quick 2

## Info

Game supports local and online modes (however, server for online game is not available on GitHub yet) for 2 players.

Ships are drawn as cells on a field. Available actions in arrangement stage:
- Place a ship: click on first and then on last cell
- Delete a ship: use left click or tap and hold on any cell belonging to the ship
- Clear the field
- Get random arrangement

Supported platforms: Windows, Android. (Note that Qt is multiplatform, so it should be possible to build it for some other OS)

## Screenshots

<p align="center">
  <img alt="demo_name" src="https://user-images.githubusercontent.com/70016123/184539115-16bfe7b0-17a1-4e13-a1e7-e4ff19c9efd3.png" width=18%>
  <img alt="demo_online" src="https://user-images.githubusercontent.com/70016123/184539111-2ddfb55d-b63e-4a21-852b-11d6bb40b6d6.png" width=18%> 
  <img alt="demo_init" src="https://user-images.githubusercontent.com/70016123/184539120-b913dac0-24f6-44f4-9ade-43b137a5ecd8.png" width=18%>
  <img alt="demo_play" src="https://user-images.githubusercontent.com/70016123/184539108-56d4b090-a167-41d1-aebc-88c05bce0fda.png" width=18.1%> 
  <img alt="demo_restart" src="https://user-images.githubusercontent.com/70016123/184539103-5c070765-1268-4221-a2f9-aeacf41c6c85.png" width=18%> 
</p>

## Technical details

C++ is used for backend, including logic handling  
QML is used for displaying objects, animations, etc.

Requires Qt 6
