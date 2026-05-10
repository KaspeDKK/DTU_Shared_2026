\# Assignment 7 / Submission 4  
This assignment part focused on setting up an online version of the roborally, without game functionality. This included creating/expanding backend structure within the controllers, services and the model, as well as the database. In extension this included handling exceptions and validation.

\#\# Frontend

\#\#\# Onlinemodel  
We made some changes to the online model, to make sure the client side represents the logic of the back end, which is described in the README under backend/model.    
\#\#\# view

\#\#\#\# appDialogs  
Handles the dialog window popups. It contains the diags for signing in, signing up and creating a new online game. The info is passed on to OnlineController, where the functionality is.

\#\#\#\# gamesView  
We have implemented a way for buttons to be clickable and not clickable, depending on the OnlineState and gameState. This is a smart way to safeguard since we choose what the user/player is allowed to do, instead of handling, possibly, infinite possibilities.

\#\#\#\# OnlineController  
The game controller is the logic behind the frontend of roborally. Every button pressed on the client calls a method from the OnlineController that, depending on the button, sends a http request to the web server, that is either a post, patch, delete or a get. 

If the backend catches an error this is then thrown and handled via Springboot exceptionhandlers. These exceptionhandlers automatically sends a http status response back to the client that is then caught with the ´HttpClientErrorException.Conflict´  
or ´HttpClientErrorException.NotFound´ depending on which error it is. then an error message is given to the user with the message from the backend.

\#\# Backend 

\#\# Controllers  
\#\#\# GameController  
Handles game-related endpoints. It has 

\#\#\# PlayerController  
Handles player-related endpoints and exceptions (see ExceptionHandling for exceptionhandling explanation). 

\#\#\# UserController  
Handles user-related data endpoints

\#\# Model  
Using the JPA annotation, we can set up our backend model to reflect the logic of a database. For both Game, Player and User unique id is set as the primary key, to ensure that all objects are unique. 

(While it may have been better practise to query with the unique id, some of our functions uses name as the parameter)        

\#\#\# Game  
A game contains its unique id, name, minPlayers and maxPlayers.  
It also has an enumerator with two values for its two states. It can be in sign up state and active state. It has an owner and a list of players that are connected to the game.  
(It also has standard getters and setters) The list of players have an annotation @ondelete(action \= OnDeleteAction) which makes sure all of the players in the game are deleted when a game is deleted.

\#\#\# Player  
A player contains its unique id and name.  
It also contains references to the game and it belongs to, and the user connected to the player.  
(It also has standard getters and setters).

\#\#\# User  
The user contains its unique id and a unique username.  
It also contains a list of players connected to the user.  
(It also has standard getters and setters).

\#\# Repositories  
The repositories are responsible for managing access to the applications model for the backend. To access the objects of the repositories, the services can call methods which belong to the specific repository. These methods have been specified by us in the repositories.  

Some of the methods which are used in the services are inherited from the parent interfaces which our repositories extend.    

\#\#\# GameRepository  
Queries for:

- finding all games  
- deleting a game by Uid  
- finding a game by its Uid

\#\#\# PlayerRepository  
Query to find a player by its name

\#\#\# UserRepository  
Query to find a user by its name

\#\# Services

\#\#\# GameService  
Creates, updates, gets and deletes games.

\#\#\# PlayerService  
responsible for signing players up and for when they leave the game.

\#\#\# UserService  
Has a search method for users, and also a create method for new users. The search is based on a list of all users from the user repository, in which we call a findByName method. The searchUsers method will return a list of users with the parameter name from the user repository.

The createUser method also relies on the user repository to verify that a user with the name it's trying to create doesn't already exist. If it does not then it will save the new user in the userRepository.

\# Extra

\#\# Exception handling  
If the backend catches an error this is then thrown and handled via Springboot exceptionhandlers. These exceptionhandlers automatically sends a http status response back to the client that is then caught with the ´HttpClientErrorException.Conflict´  
or ´HttpClientErrorException.NotFound´ depending on which error it is. then an error message is given to the user with the message from the backend. This is also stated in the OnlineController section. 

This extra implementation was approved by Ekkart.   
