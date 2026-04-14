package dk.dtu.compute.se.pisd.roborally.exceptions;

/**
 * This exception should be used for an illegal move in roborally.
 */
public class ImpossibleMoveException extends Exception{
    public ImpossibleMoveException() {
        super();
    }

    public ImpossibleMoveException(String message) {
        super(message);
    }
}

