package com.example.accessing_data_rest.services;

import com.example.accessing_data_rest.exceptions.CouldNotCreateUserException;
import com.example.accessing_data_rest.model.User;

import com.example.accessing_data_rest.repositories.UserRepository;
import jakarta.transaction.Transactional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class UserService {

    @Autowired
    private UserRepository userRepository;

    //TODO javadoc
    public List<User> searchUsers(String name) {
        // DONE Assignment 7b: obtain a list of users with the given name
        //      from the userRepository and return the result (instead
        //      the empty list below).
        List<User> users = userRepository.findByName(name);

        return users;
    }
    //TODO javadoc
    @Transactional
    public User createUser(User user) {
        if (user.getName().length() < 4) {
            throw new CouldNotCreateUserException("User name must be at least 4 characters long");
        }
        if (!userRepository.findByName(user.getName()).isEmpty()) {
            throw new CouldNotCreateUserException("User with name " + user.getName() + " already exists");
        }
        return userRepository.save(user);
    }
}
