package com.example.accessing_data_rest.services;

import com.example.accessing_data_rest.model.User;

import com.example.accessing_data_rest.repositories.UserRepository;
import jakarta.transaction.Transactional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.RequestBody;

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

    @Transactional
    public User createUser(@RequestBody User user) {
        try {
            return userRepository.save(user);
        } catch (RuntimeException ex) {
            ex.printStackTrace();
        }
        throw new RuntimeException();
    }
}
