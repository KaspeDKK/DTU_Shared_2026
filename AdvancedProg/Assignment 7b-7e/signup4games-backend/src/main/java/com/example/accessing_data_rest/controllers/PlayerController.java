package com.example.accessing_data_rest.controllers;

import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("roborally/player")
public class PlayerController {

    @PostMapping(
            produces = "application/json"
    )
    public void gameSignUp(){

    }

    // @Autowired
    // PlayerService playerService;

    // TODO Assignment 7c: For adding players to a game, you will need to add a @PostMapping here
    //      and you will need to add corresponding service class PlayerService in package services,
    //      and implement the respective logic there.

    // TODO Assignment 7d for a player (user) leaving the game, you need to have a delete method for
    //      players here.

}
