# QMaze #
QMaze is a simple maze game. It creates mazes the player has to pass.
In order to complete a level the player needs to find a key first.


## Build ##
To build QMaze you need the following prerequisites (Ubuntu):
cmake, build-essential qt5-default, qttools5-dev-tools

Then you can run:
`cd /path/to/QMaze`
`mkdir build && cd build`
If you want to create a package of *QMaze* you can type:
`cmake -DDEB_PKG ..`
for a **deb** package, or:
`cmake -DRPM_PKG ..`
for a (OpenSUSE) **rpm** package. If you don't need a package, just type:
`cmake ..`
And finally:
`make package`
You will find a deb package in your build folder then. Just install it.


## License ##
Copyright 2016 Daniel Steinhauer

QMaze is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QMaze is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with QMaze.  If not, see <http://www.gnu.org/licenses/>.