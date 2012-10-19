## Multiplayer games in javascript

#### Notes

A couple files have been added: breakout.html and fancy.html. They are both just small experiments to play around with and practice some simple technics.

Written and tested on OSX Mountain Lion (10.8.2) using Xcode included gcc (gcc version 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2336.11.00))

This is the dataframe diagram directly from the RFC 6455:

    +-+-+-+-+-------+-+-------------+-------------------------------+
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-------+-+-------------+-------------------------------+
    |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
    |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
    |N|V|V|V|       |S|             |   (if payload len==126/127)   |
    | |1|2|3|       |K|             |                               |
    +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
    |     Extended payload length continued, if payload len == 127  |
    + - - - - - - - - - - - - - - - +-------------------------------+
    |                               | Masking-key, if MASK set to 1 |
    +-------------------------------+-------------------------------+
    | Masking-key (continued)       |          Payload Data         |
    +-------------------------------- - - - - - - - - - - - - - - - +
    :                     Payload Data continued ...                :
    + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
    |                     Payload Data continued ...                |
    +---------------------------------------------------------------+

##### The communication process

The game will have a communication process. Packets of data that will reside in the payload portion of the dataframe will look something like this:

     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
    +---------------+---------------+---------------+---------------+
    |             OPCODE            |             Params            |
    +---------------+---------------+---------------+---------------+
    |                         Params Cont...                        |
    +---------------+---------------+---------------+---------------+

* This is not exact as there is a bit more info we need to send. But it will mostly likely be sent in a flat manner.


We will have 6 bytes for data and 2 bytes for a unique function.

When a user starts a game the client will attempt a connection to the server. If no connection is made then 1 player mode starts. However, if a connection is made the server will create a player object for the new player and will return an id which will be used to identify that player from the client. instruction will only be accepted from clients that send an id with a returned hash from the original ip.

The server will run on ticks. Something around 100 ticks per second. Meaning that each client will get updated 100 times per second, or the server will attempt to update the client 100 times. Whenever the server sends instructions out it will be with a tick number and a time stamp. Both of those will help the client draw what it needs to draw where it needs to draw it. The server will attempt to keep track of cheating by giving a score of how likely an action is to have really taken place based on the rules of the game.

#### Stuff To Get it Working

##### Compile the server

The server was first played around with in python as a POC. I have rewritten it in C as C better suites the needs of what I am trying accomplish. To compile you just need to type 'make'.

#### Design

The game will be somewhat normal with an update and draw during each frame. The frames will be timed so that proper physics steps can be made.

The main loop will switch on gamestate so that different states can be executed without disturbing the rest of the gaame. 

Example: We need a pause state but we dont want to bother executing the rest of the game when we are paused. So pause will needs its own update()/draw() loop. The gamestate can switch up these states and the update() functions in each state can be the boss of switching states if need be.

##### TODO

* Create a level format to be used throughout the game. This should help with the viewport design as well. Though they may not be directly related they can probably help with design ideas.

* Start creating a wiki and commenting on functions for documentation's sake.

* Create an input thread to listen for keyboard input as a way to interact with the server while it is running. Perhaps simple interrupts will do.

* Make the viewport its own object meaning that it either accepts an object to follow or methods are exposed to keep all the logic, currently in the loop, to the viewport object itself.

* Start formalizing the game objects and how the game will play.
    * The Car object has had some good work done. Though it doesnt have any collision mechanics.
    * The Screen should follow the player.

* Game networking should have a clear rule set defined in an object perhaps? Or maybe we just document the instructions and use JS blobs for data transfer.

* Start working on menus that are easy for the player to understand and follow.

* Start working on visual and audible assets.
    * Each action should have an audible compenent as well as a visual component.
    * Visual style should be consistent throughout the game.

##### FINISHED

* Make the server a singleton

#### Sources

##### Base 64 Encode
* The new b64encoder is from: http://www.adp-gmbh.ch/cpp/common/base64.html
* libb64 was found from reading stackoverflow: http://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
	* http://libb64.sourceforge.net/

##### SHA1 Hash
* sha1.c & sha1.h were found on stackoverflow: http://stackoverflow.com/questions/6839120/using-sha1-in-c
	* http://www.koders.com/c/fidECEAC97A9FC979C1D3882B85349FEFAC5C8F89A5.aspx
