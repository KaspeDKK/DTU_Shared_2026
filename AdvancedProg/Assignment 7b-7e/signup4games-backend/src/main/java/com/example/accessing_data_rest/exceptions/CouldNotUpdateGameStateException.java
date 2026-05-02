package com.example.accessing_data_rest.exceptions;

public class CouldNotUpdateGameStateException extends RuntimeException {
    public CouldNotUpdateGameStateException(String message) {
        super(message);
    }
}
