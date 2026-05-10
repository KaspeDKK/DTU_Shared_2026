package com.example.accessing_data_rest.exceptions;

public class CannotJoinActiveGameException extends RuntimeException {
    public CannotJoinActiveGameException(String message) {
        super(message);
    }
}
