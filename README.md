# InsecureSnake

This is all the code to go along with my blog post: kylehalladay.com/blog/2019/12/04/Recreating-A-Dirty-Gamedev-Hack.html

The point of this program was to recreate a story posted on gamasutra, where a live game was patched via a buffer overflow in the system that downloaded and displayed the EULA.

The server script requires user input in order to execute the machine code payload that gets downloaded, so if you're trying to run this at home, my steps were: 

* Launch the Server
* Launch the snake game
* Press enter on the terminal running the server to trigger the packet handler
* Continue with snake game as usual.

If you don't trigger the packet handler, the snake game will display a malformed EULA message but will otherwise run normally. 