package com.example.accessing_data_rest.controllers;

import com.example.accessing_data_rest.model.Game;

import com.example.accessing_data_rest.services.GameService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("roborally/game")
public class GameController {

    @Autowired
    private GameService gameService;

    // DONE Assignment 7b: add a method with @GetMapping, which obtains
    //      a list of all games from the games repository  (via the service
    //      getGames) and returns this list (in JSON representation).
    //      See class UserController for inspiration and class GameService

    @GetMapping(value = "/getGames",produces="application/json")
    public List<Game> getGames(){
        return gameService.getGames();
    }


    // TODO Assignment 7b: Create a post method in this controller for creating a new game
    //      this method should call the corresponding service for creating a game
    @PostMapping(
            value = "",
            consumes = "application/json",
            produces = "application/json"
    )
    public Game postGame(@RequestBody Game game){
        return gameService.createGame(game);
    }
    // TODO Assignment 7d: Create a method and @RequestMpping for deleting a game

    // TODO Assignment 7c-7e: At some point you might want to implement an
    //      endpoint for obtaining open games (open for joining) only or
    //      games that have started.
}
