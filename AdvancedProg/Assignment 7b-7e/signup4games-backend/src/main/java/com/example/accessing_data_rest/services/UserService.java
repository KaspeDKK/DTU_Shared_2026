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

    // TODO refactor to return a user instead of a list (since name is a unique). Could we also not just remove userId?
    /** This method will find a user by its name, returned in a list.
     * Since the method body uses a query, we are returning a list of users, but in reality its only going to find 1 user.
     *
     * @param name name of user to query for
     * @return a list of users (only element is the user belonging to given name)
     */
    public List<User> searchUsers(String name) {
        // DONE Assignment 7b: obtain a list of users with the given name
        //      from the userRepository and return the result (instead
        //      the empty list below).
        List<User> users = userRepository.findByName(name);

        return users;
    }

    /** creates a user. if name length is under 4 then throw an exception. If name already exists then throw an exception
     *
     * @param user FRONTEND user object
     * @return user with updated name
     */
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
