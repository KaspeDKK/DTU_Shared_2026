package com.example.accessing_data_rest.exceptions;

public class IllegalPlayerCountException extends RuntimeException {
    public IllegalPlayerCountException(String message) {
        super(message);
    }
}
