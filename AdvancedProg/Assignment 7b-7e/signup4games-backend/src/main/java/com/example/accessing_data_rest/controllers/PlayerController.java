package com.example.accessing_data_rest.controllers;

import com.example.accessing_data_rest.exceptions.*;
import com.example.accessing_data_rest.services.PlayerService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.repository.query.Param;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import com.example.accessing_data_rest.model.Player;

import java.util.List;

@RestController
@RequestMapping("roborally/player")
public class PlayerController {


    @Autowired
    PlayerService playerService;

    // TODO Assignment 7c: For adding players to a game, you will need to add a @PostMapping here
    //      and you will need to add corresponding service class PlayerService in package services,
    //      and implement the respective logic there.

    @PostMapping(
            produces = "application/json"
    )
    public void signUpPlayer(@RequestBody Player player){
            playerService.signUpPlayer(player);
    }

    // TODO Assignment 7d for a player (user) leaving the game, you need to have a delete method for
    //      players here.
    @DeleteMapping
    public void leaveGame(@RequestBody Player player) {
        playerService.leaveGame(player);
    }

    @ExceptionHandler(CouldNotCreatePlayerException.class)
    public ResponseEntity<String> handleCouldNotCreatePlayerException(CouldNotCreatePlayerException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.NOT_FOUND);
    }

    @ExceptionHandler(CouldNotDeletePlayerException.class)
    public ResponseEntity<String> handleCouldNotDeletePlayerException(CouldNotDeletePlayerException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

    @ExceptionHandler(CouldNotFindPlayerException.class)
    public ResponseEntity<String> handleCouldNotFindPlayerException(CouldNotFindPlayerException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.NOT_FOUND);
    }

    @ExceptionHandler(CannotLeaveActiveGameException.class)
    public ResponseEntity<String> handleCannotLeaveActiveGameException(CannotLeaveActiveGameException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

    @ExceptionHandler(CannotJoinActiveGameException.class)
    public ResponseEntity<String> handleCannotJoinActiveGameException(CannotJoinActiveGameException ex){
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

}
