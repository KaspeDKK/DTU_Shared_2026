package com.example.accessing_data_rest.controllers;

import com.example.accessing_data_rest.exceptions.CouldNotUpdateGameStateException;
import com.example.accessing_data_rest.model.Game;

import com.example.accessing_data_rest.services.GameService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("roborally/game")
public class GameController {

    // field injection to hide dependencies. Constructor injection could be prefered and since we have no constructor
    // Spring could actually inject the constructor automatically into
    @Autowired
    private GameService gameService;


    /**
     * Produces a list of all games in the repository in JSON format.
     * The method calls the getGames method of the GameService to obtain the list of games and returns it as a response to the client.
     * @return list of games in JSON format
     */
    @GetMapping(value = "/getGames", produces = "application/json")
    public List<Game> getGames() {
        return gameService.getGames();
    }

    /**
     * endpoint for updating the state of a game.
     * The method takes a gameUid as a path variable and a Game object as input in the request body.
     * @returns the updated game with the new state in JSON format.
     */
    @PatchMapping("/{gameUid}")
    public Game updateGameState(@PathVariable long gameUid, @RequestBody Game game) {
        return gameService.updateGameState(gameUid, game.getGameState());
    }


    /**
     * endpoint for posting a game to the repository.
     * The method takes a Game object as input, which is expected to be in JSON format in the request body.
     * The method calls the createGame method of the GameService to create a new game in the repository and returns the created game as a response to the client in JSON format.
     * @param game
     * @return game from repository in JSON format
     */
    @PostMapping(
            value = "",
            consumes = "application/json",
            produces = "application/json"
    )
    public Game postGame(@RequestBody Game game) {
        return gameService.createGame(game);
    }

    /**
     * endpoint for deleting a game from the repository.
     * The method calls the deleteGame method of the GameService to delete the specified game from the repository.
     * The method does not return any content in the response.
     * @param gameUid
     */
    @DeleteMapping(value = "{id}")
    public void deleteGame(@PathVariable("id") long gameUid) {
        gameService.deleteGame(gameUid);
    }
    // DONE Assignment 7c-7e: At some point you might want to implement an
    //      endpoint for obtaining open games (open for joining) only or
    //      games that have started.

    /**
     * handles exceptions in a way,
     * where it sends the exception to the client side
     * with a message and a status code of 404 Not Found instead of 500 Internal Server Error.
     * @param ex exception
     * @return response entity with exception message and 404 status code
     */
    @ExceptionHandler(CouldNotUpdateGameStateException.class)
    public ResponseEntity<String> handleCouldNotUpdateGameStateException(CouldNotUpdateGameStateException ex) {
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.NOT_FOUND);
    }
}
