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
        check.setIsFinal(false);
        spaceCheck1.getActions().add(check);

        Checkpoint check2 = new Checkpoint();
        check2.setNumber(2);
        check.setIsFinal(false);
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
        gameController.turnRight(player1);
        gameController.turnLeft(player1);
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

    @Test
    void testConveyorbelt() {
        Board board = gameController.board; //initiate board
        Player player1 = board.getCurrentPlayer(); //get player
        player1.setSpace(board.getSpace(0, 0)); //set starting place

        Space space = board.getSpace(0, 0); //Implement a chain of conveyorbelts
        ConveyorBelt action = new ConveyorBelt();
        action.setHeading(Heading.SOUTH);
        space.getActions().add(action);

        space = board.getSpace(0, 1);
        action = new ConveyorBelt();
        action.setHeading(Heading.SOUTH);
        space.getActions().add(action);

        space = board.getSpace(0, 2);
        action = new ConveyorBelt();
        action.setHeading(Heading.SOUTH);
        space.getActions().add(action);

        space = board.getSpace(0, 3);
        action = new ConveyorBelt();
        action.setHeading(Heading.SOUTH);
        space.getActions().add(action);

        space = board.getSpace(0, 4);
        action = new ConveyorBelt();
        action.setHeading(Heading.SOUTH);
        space.getActions().add(action);

        space = board.getSpace(0, 5);
        action = new ConveyorBelt();
        action.setHeading(Heading.EAST);
        space.getActions().add(action);
        //expected outcome: playerpos: [x:1][y:5]

        activateConveyors(player1.getSpace());

        Assertions.assertEquals(1, player1.getSpace().x, "player x.position is expected at 1. is currently: " + player1.getSpace().x);
        Assertions.assertEquals(5, player1.getSpace().y, "player y.position is expected at 5. is currently: " + player1.getSpace().y);
    }

    @Test
    void testRecursiveMovement(){
        Board board = gameController.board; //initiate board
        Player player1 = board.getCurrentPlayer(); //get player
        player1.setSpace(board.getSpace(0, 0));
        Player player2 = board.getPlayer(1);
        Player player3 = board.getPlayer(2);
        Player player4 = board.getPlayer(3);

        player1.setSpace(board.getSpace(0, 0));
        player2.setSpace(board.getSpace(1, 0));
        player3.setSpace(board.getSpace(2, 0));
        player4.setSpace(board.getSpace(3, 0));

        try {
            gameController.moveToSpace(player2.getSpace(),player1,Heading.EAST);
            Assertions.assertEquals(4,player4.getSpace().x);
            Assertions.assertEquals(0,player4.getSpace().y);
        } catch (ImpossibleMoveException e) {

        }

    }

    @Test
    void testExecuteOptionAndContinue() {
        Board board = gameController.board;
        Player current = board.getCurrentPlayer();

        current.setHeading(Heading.SOUTH);
        current.getProgramField(0).setCard(new CommandCard(Command.LEFT_OR_RIGHT));

        board.setStepMode(true); // ensure continuePrograms() runs exactly one step
        board.setStep(0);
        board.setPhase(Phase.PLAYER_INTERACTION);
        board.setSelectedOption(null);

        gameController.executeOptionAndContinue(Command.LEFT);

        Assertions.assertEquals(Heading.EAST, current.getHeading(), "LEFT option should turn player left (SOUTH -> EAST)");
        Assertions.assertNull(board.getSelectedOption(), "Selected option should be consumed by executeNextStep()");
        Assertions.assertEquals(Phase.ACTIVATION, board.getPhase(), "Game should be back in activation phase after choosing an option");
        Assertions.assertEquals(board.getPlayer(1), board.getCurrentPlayer(), "Game should have advanced to next player after executing option");

    }

    @Test
    void testPhaseShifting(){
        //TODO implement
    }

    @Test
    void test_executePrograms_nonInteractiveCommands(){
        //clear current cards from setup to round 1
        Board board = gameController.board;
        for (int p = 0; p < board.getPlayersNumber(); p++) {
            Player current = board.getPlayer(p);
            for (int i = 0; i < Player.NO_REGISTERS; i++) {
                current.getProgramField(i).setCard(null);
            }
        }
        // sets player 1 to a known position with a known heading
        Player player = board.getPlayer(0);
        player.setSpace(board.getSpace(3, 3));
        player.setHeading(Heading.NORTH);

        //create specific commandCards
        player.getProgramField(0).setCard(new CommandCard(Command.RIGHT));
        player.getProgramField(1).setCard(new CommandCard(Command.FORWARD));
        player.getProgramField(2).setCard(new CommandCard(Command.FAST_FORWARD));
        player.getProgramField(3).setCard(new CommandCard(Command.LEFT));
        player.getProgramField(4).setCard(new CommandCard(Command.BACK));

        //prepare the board and execute the registers with specific commands
        board.setPhase(Phase.ACTIVATION);
        board.setStep(0);
        board.setCurrentPlayer(player);


        int before = board.getGameCounter(); //stores the gamecounter for later checks

        gameController.executePrograms();
//checks if the player  has moved to the correct position in round 1 after all the specific commands has been played
        Assertions.assertEquals(board.getSpace(6, 4), player.getSpace());
        Assertions.assertEquals(Heading.NORTH, player.getHeading());
        Assertions.assertEquals(Phase.PROGRAMMING, board.getPhase()); //checks if phase has returned to programming phase

        //Resets card registers for round 2 of testing
        for (int p = 0; p < board.getPlayersNumber(); p++) {
            Player current = board.getPlayer(p);
            for (int i = 0; i < Player.NO_REGISTERS; i++) {
                current.getProgramField(i).setCard(null);
            }
        }
        // add UTURN commandcard
        player.getProgramField(0).setCard(new CommandCard(Command.UTURN));

        //prepare the board and execute the registers with specific command and executes
        board.setPhase(Phase.ACTIVATION);
        board.setStep(0);
        board.setCurrentPlayer(player);

        gameController.executePrograms();

        Assertions.assertEquals(Heading.SOUTH, player.getHeading()); //checks if player has made a UTURN
        Assertions.assertTrue(board.getGameCounter() >= before + 6);
        Assertions.assertEquals(Phase.PROGRAMMING, board.getPhase()); //checks if phase has returned to programming phase
    }

    /**
     * Helper function to activate conveyors on a given space.
     * @param space
     */
    private void activateConveyors(Space space) {
        if (space == null || space.getPlayer() == null) { //check if space or player is null at given space
            return; //return
        }

        Player player = space.getPlayer(); //determine space

        for (FieldAction fieldAction : space.getActions()) { //call action on given space
            if (fieldAction.doAction(gameController, space)) { //if field action returns true
                activateConveyors(player.getSpace()); //recursive statement.
                return;
            }
        }
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
