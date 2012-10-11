## Multiplayer games in javascript

#### Notes

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

#### Getting Started

##### Compile the server

The server was first played around with in python as a POC. I have rewritten it in C as C better suites the needs of what I am trying accomplish. To compile you just need to type 'make'.

##### TODO

* _DONE_ Make the server a singleton

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

#### Sources

##### Base 64 Encode
* The new b64encoder is from: http://www.adp-gmbh.ch/cpp/common/base64.html
* libb64 was found from reading stackoverflow: http://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
	* http://libb64.sourceforge.net/

##### SHA1 Hash
* sha1.c & sha1.h were found on stackoverflow: http://stackoverflow.com/questions/6839120/using-sha1-in-c
	* http://www.koders.com/c/fidECEAC97A9FC979C1D3882B85349FEFAC5C8F89A5.aspx
