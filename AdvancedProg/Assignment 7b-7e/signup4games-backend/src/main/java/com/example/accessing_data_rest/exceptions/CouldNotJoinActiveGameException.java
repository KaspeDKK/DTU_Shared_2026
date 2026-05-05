package com.example.accessing_data_rest.exceptions;

public class CouldNotJoinActiveGameException extends RuntimeException {
    public CouldNotJoinActiveGameException(String message) {
        super(message);
    }
}
