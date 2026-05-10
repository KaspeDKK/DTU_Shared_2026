package com.example.accessing_data_rest.exceptions;

public class CannotJoinFinishedGameException extends RuntimeException {
    public CannotJoinFinishedGameException(String message) {
        super(message);
    }
}
