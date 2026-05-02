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

    // DONE Assignment 7b: add a method with @GetMapping, which obtains
    //      a list of all games from the games repository  (via the service
    //      getGames) and returns this list (in JSON representation).
    //      See class UserController for inspiration and class GameService

    @GetMapping(value = "/getGames", produces = "application/json")
    public List<Game> getGames() {
        return gameService.getGames();
    }

    //TODO javadoc
    @PatchMapping("/{gameUid}")
    public Game updateGameState(@PathVariable long gameUid, @RequestBody Game game) {
        return gameService.updateGameState(gameUid, game.getGameState());
    }


    // TODO Assignment 7b: Create a post method in this controller for creating a new game
    //      this method should call the corresponding service for creating a game
    @PostMapping(
            value = "",
            consumes = "application/json",
            produces = "application/json"
    )
    public Game postGame(@RequestBody Game game) {
        return gameService.createGame(game);
    }

    // TODO Assignment 7d: Create a method and @RequestMpping for deleting a game
    @DeleteMapping(value = "{id}")
    public void deleteGame(@PathVariable("id") long gameUid) {
        gameService.deleteGame(gameUid);
    }
    // TODO Assignment 7c-7e: At some point you might want to implement an
    //      endpoint for obtaining open games (open for joining) only or
    //      games that have started.

    @ExceptionHandler(CouldNotUpdateGameStateException.class)
    public ResponseEntity<String> handleCouldNotUpdateGameStateException(CouldNotUpdateGameStateException ex) {
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.BAD_REQUEST);
    }
}
