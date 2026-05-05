package com.example.accessing_data_rest.controllers;

import com.example.accessing_data_rest.exceptions.*;
import com.example.accessing_data_rest.services.PlayerService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import com.example.accessing_data_rest.model.Player;

@RestController
@RequestMapping("roborally/player")
public class PlayerController {


    @Autowired
    PlayerService playerService;

    // DONE Assignment 7c: For adding players to a game, you will need to add a @PostMapping here
    //      and you will need to add corresponding service class PlayerService in package services,
    //      and implement the respective logic there.

    /** POST mapping to endpoint to create a new player in our playerRepo
     *
     * @param player object body that will be sent in the request
     */
    @PostMapping(
            consumes = "application/json"
    )
    public void signUpPlayer(@RequestBody Player player){
            playerService.signUpPlayer(player);
    }

    // DONE Assignment 7d for a player (user) leaving the game, you need to have a delete method for
    //      players here.
    @DeleteMapping("/{playerUid}")
    public void leaveGame(@PathVariable Long playerUid) {
        playerService.leaveGame(playerUid);
    }

    @ExceptionHandler(CouldNotCreatePlayerException.class)
    public ResponseEntity<String> handleCouldNotCreatePlayerException(CouldNotCreatePlayerException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.NOT_FOUND);
    }

    @ExceptionHandler(CannotJoinFullGameException.class)
    public ResponseEntity<String> handleCannotJoinFullGameException(CannotJoinFullGameException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

    @ExceptionHandler(CouldNotDeletePlayerException.class)
    public ResponseEntity<String> handleCouldNotDeletePlayerException(CouldNotDeletePlayerException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

    @ExceptionHandler(CouldNotFindPlayerException.class)
    public ResponseEntity<String> handleCouldNotFindPlayerException(CouldNotFindPlayerException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.NOT_FOUND);
    }

    @ExceptionHandler(CouldNotJoinActiveGameException.class)
    public ResponseEntity<String> handleCannotLeaveActiveGameException(CouldNotJoinActiveGameException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

    @ExceptionHandler(CannotJoinActiveGameException.class)
    public ResponseEntity<String> handleCannotJoinActiveGameException(CannotJoinActiveGameException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

}
