package com.example.accessing_data_rest.exceptions;

public class CannotLeaveActiveGameException extends RuntimeException {
    public CannotLeaveActiveGameException(String message) {
        super(message);
    }
}
