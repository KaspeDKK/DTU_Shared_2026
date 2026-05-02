package com.example.accessing_data_rest.exceptions;

public class CouldNotFindPlayerException extends RuntimeException {
    public CouldNotFindPlayerException(String message) {
        super(message);
    }
}
