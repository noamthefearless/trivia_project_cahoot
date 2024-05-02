Welcome to noam and andrei's trivia project!

this project is multiplayer trivia!
the menu is pretty straightforword, and this file is only for setup.

here is how to play:

1 - activate the server. you can run the sln in visual studio, or build it with the 
correct the database in the same folder.

2 - activate the client. run the client again through visual studio in .NET framework, or build it.
(easier to run through visual studio in my opinion).

3 - as long as the server is running the client will auto connect to it. 

how to play with other people in the same network: you can change in the client communicator class
the ip of loopback to the ip of the specific machine on wich the server runs in the local network.
this way, several players can play in the same network.


how to play with people on different networks: for this, use port forwording on your router
to the ip of the server, with the port 9090 wich is the default port of the server.
change the client's ip in the communicator to the public ip of your router, and you should be
good to go.






have fun!






