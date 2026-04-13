package dk.dtu.compute.se.pisd.roborally.controller;

import dk.dtu.compute.se.pisd.roborally.exceptions.ImpossibleMoveException;
import dk.dtu.compute.se.pisd.roborally.model.*;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class GameControllerTest {

    private final int TEST_WIDTH = 8;
    private final int TEST_HEIGHT = 8;

    private GameController gameController;

    @BeforeEach
    void setUp() {
        Board board = new Board(TEST_WIDTH, TEST_HEIGHT);
        gameController = new GameController(board);
        for (int i = 0; i < 6; i++) {
            Player player = new Player(board, null,"Player " + i);
            board.addPlayer(player);
            player.setSpace(board.getSpace(i, i));
            player.setHeading(Heading.values()[i % Heading.values().length]);
        }
        board.setCurrentPlayer(board.getPlayer(0));
    }

    @AfterEach
    void tearDown() {
        gameController = null;
    }

    /**
     * Test for Assignment 6a (can be deleted later once Assignment 6a was shown to the teacher)
     */
    @Test
    void testV1() {
        Board board = gameController.board;

        Player player1 = board.getCurrentPlayer();
        Player player2 = board.getPlayer(1);
        gameController.moveCurrentPlayerToSpace(board.getSpace(0, 4));

        Assertions.assertEquals(player1, board.getSpace(0, 4).getPlayer(), "Player " + player1.getName() + " should be on Space (0,4)!");
        Assertions.assertNull(board.getSpace(0, 0).getPlayer(), "Space (0,0) should be empty!");
        Assertions.assertEquals(player2, board.getCurrentPlayer(), "Current player should be " + player2.getName() +"!");
    }

    @Test
    void testCheckpoint() throws ImpossibleMoveException {
        Board board = gameController.board;
        Space spaceCheck1 = board.getSpace(0,1);
        Space spaceCheck2 = board.getSpace(0,2);

        Checkpoint check = new Checkpoint();
        check.setNumber(1);
        spaceCheck1.getActions().add(check);

        Checkpoint check2 = new Checkpoint();
        check2.setNumber(2);
        spaceCheck2.getActions().add(check2);

        Player player1 = board.getCurrentPlayer();

        //første test - Bør ikke tælle
        gameController.moveToSpace(spaceCheck2,player1,Heading.WEST);
        for (FieldAction action : spaceCheck2.getActions()) {
            action.doAction(gameController, spaceCheck2);
        }
        Assertions.assertEquals(0, player1.getCheckpoint(), "Player checkpoint attribute is: " + player1.getCheckpoint() +
                " should be 0");

        //anden test - Bør tælle
        gameController.moveToSpace(spaceCheck1,player1,Heading.WEST);
        for (FieldAction action : spaceCheck1.getActions()) {
            action.doAction(gameController, spaceCheck1);
        }
        Assertions.assertEquals(1, player1.getCheckpoint(), "Player checkpoint attribute is: " + player1.getCheckpoint() +
                " should be 1");

        //tredje test - Bør tælle
        gameController.moveToSpace(spaceCheck2,player1,Heading.WEST);
        for (FieldAction action : spaceCheck2.getActions()) {
            action.doAction(gameController, spaceCheck2);
        }
        Assertions.assertEquals(2, player1.getCheckpoint(), "Player checkpoint attribute is: " + player1.getCheckpoint() +
                " should be 2");
    }
    @Test
    void testMovement(){
        Board board = gameController.board;
        Player player1 = board.getCurrentPlayer();
        player1.setHeading(Heading.SOUTH);
        Space start = new Space(board, 0,0);
        player1.setSpace(start);
        Assertions.assertEquals(player1.getSpace(), start, "player is at: [" + player1.getSpace().y + "," + player1.getSpace().y + "]" +
                " should be at: [" + start.y + "," + start.x + "]");

        gameController.moveForward(player1); //[0,1]
        gameController.fastForward(player1); //[0,3]
        gameController.turnLeft(player1);    //[0,3]
        gameController.fastForward(player1); //[2,3]
        gameController.uTurn(player1);       //[2,3]
        gameController.moveForward(player1); //[1,3]
        gameController.back(player1);        //[2,3]


        Space end = new Space(board,    2,3);

        Assertions.assertEquals(player1.getSpace().y, end.y, "player is at: [" + player1.getSpace().y + "," + player1.getSpace().y + "]" +
                " should be at: [" + end.y + "," + end.x + "]");
        Assertions.assertEquals(player1.getSpace().x, end.x, "player is at: [" + player1.getSpace().y + "," + player1.getSpace().y + "]" +
                " should be at: [" + end.y + "," + end.x + "]");



    }
    /*
    @Test
    void moveForward() {
        Board board = gameController.board;
        Player current = board.getCurrentPlayer();

        gameController.moveForward(current);

        Assertions.assertEquals(current, board.getSpace(0, 1).getPlayer(), "Player " + current.getName() + " should beSpace (0,1)!");
        Assertions.assertEquals(Heading.SOUTH, current.getHeading(), "Player 0 should be heading SOUTH!");
        Assertions.assertNull(board.getSpace(0, 0).getPlayer(), "Space (0,0) should be empty!");
    }
    */

    // TODO and there should be more tests added for the different assignments eventually

}