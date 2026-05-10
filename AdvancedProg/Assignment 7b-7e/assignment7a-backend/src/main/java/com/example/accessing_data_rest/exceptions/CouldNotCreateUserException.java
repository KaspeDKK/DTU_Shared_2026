package com.example.accessing_data_rest.exceptions;

public class CouldNotCreateUserException extends RuntimeException {
    public CouldNotCreateUserException(String message) {
        super(message);
    }
}
