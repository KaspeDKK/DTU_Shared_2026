package com.example.accessing_data_rest.controllers;

import com.example.accessing_data_rest.model.User;

import com.example.accessing_data_rest.services.UserService;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("roborally/user")
public class UserController {

    @Autowired
    private UserService userService;

    @GetMapping(value = "/search", produces="application/json")
    public List<User> searchUsers(@RequestParam("name") String name) {
        return userService.searchUsers(name);
    }

}
