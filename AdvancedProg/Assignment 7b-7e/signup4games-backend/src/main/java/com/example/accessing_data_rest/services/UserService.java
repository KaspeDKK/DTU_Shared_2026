package com.example.accessing_data_rest.services;

import com.example.accessing_data_rest.model.User;

import com.example.accessing_data_rest.repositories.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.util.List;

@Service
public class UserService {

    @Autowired
    private UserRepository userRepository;

    public List<User> searchUsers(String name) {
        // DONE Assignment 7b: obtain a list of users with the given name
        //      from the userRepository and return the result (instead
        //      the empty list below).
        List<User> users = userRepository.findByName(name);

        return users;
    }

}
