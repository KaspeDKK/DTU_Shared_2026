package com.example.accessing_data_rest.controllers;

import com.example.accessing_data_rest.exceptions.CouldNotCreateUserException;
import com.example.accessing_data_rest.model.User;

import com.example.accessing_data_rest.services.UserService;

import jakarta.transaction.Transactional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("roborally/user")
public class UserController {

    @Autowired
    private UserService userService;

    /**
     * endpoint for searching users by name.
     * The method calls the searchUsers method of the UserService to perform the search and returns the result as a response to the client.
     * @param name
     * @return name as JSON
     */
    @GetMapping(value = "/search", produces = "application/json")
    public List<User> searchUsers(@RequestParam("name") String name) {
        return userService.searchUsers(name);
    }

    /**
     * endpoint for signing up a new user.
     * The method takes a User object as input, which is expected to be in JSON format in the request body.
     * The method calls the createUser method of the UserService to create a new user in the repository
     * and returns the created user as a response to the client in JSON format.
     * @param user
     * @return user in JSON Format
     */
    @PostMapping(
            value = "",
            consumes = "application/json",
            produces = "application/json"
    )
    public User SignUp(@RequestBody User user) {
        return userService.createUser(user);
    }

    //Catches the CouldNotCreateUserException and returns a response with the exception message
    // and a 409 Conflict status code instead of 500 to the client

    @ExceptionHandler(CouldNotCreateUserException.class)
    public ResponseEntity<String> handleCouldNotCreateUserException(CouldNotCreateUserException ex) {
        return new ResponseEntity<>(ex.getMessage(), HttpStatus.CONFLICT);
    }

}
