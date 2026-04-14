/*
 *  This file is part of the initial project provided for the
 *  course "Project in Software Development (02362)" held at
 *  DTU Compute at the Technical University of Denmark.
 *
 *  Copyright (C) 2019, 2020: Ekkart Kindler, ekki@dtu.dk
 *
 *  This software is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this project; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
package dk.dtu.compute.se.pisd.roborally.controller;

import dk.dtu.compute.se.pisd.roborally.exceptions.ImpossibleMoveException;
import dk.dtu.compute.se.pisd.roborally.model.*;
import org.jetbrains.annotations.NotNull;

import java.util.Iterator;
import java.util.List;

/**
 * ...
 *
 * @author Ekkart Kindler, ekki@dtu.dk
 *
 */
public class GameController {

    final public Board board;

    public GameController(@NotNull Board board) {
        this.board = board;
    }

    /**
     * This method moves the current player to a non-occupied space, and is triggered by a user
     * clicking on set space. This method is not a part of the game, and should be removed.
     *
     * @param space the space to which the current player should move
     * @author Tokemeister, Friisma, KaspeDKK, Simon, Thomas, Rasbas
     *
     */
    public void moveCurrentPlayerToSpace(@NotNull Space space) {

        if (space.getPlayer() != null)
            throw new IllegalStateException("Space is already occupied. MISTER!"); // defensive programming
        if (board.getCurrentPlayer() == null)
            throw new IllegalStateException("current player is null?!"); // very defensive programming

        Player curr = board.getCurrentPlayer(); // get player

        int count = board.getGameCounter(); // get count locally

        curr.setSpace(space); // set current player to assigned space
        count++; // increment local
        int next = (board.getPlayerNumber(curr) + 1) % board.getPlayersNumber(); // get next player with wraparound using %
        board.setCurrentPlayer(board.getPlayer(next)); // set next player

        board.setGameCounter(count); // increment the actual count.

    }


    /**
     * Moves a player recursively according to the rules of Robo rally.
     * <p>
     * moveToSpace uses our implementation of the getNeighbour to throw an error if the move is not legal.
     * I.E the destination is behind a wall.
     * if the move is illegal, we throw ImpossibleMoveException.
     *
     * @param nextSpace the wanted space.
     * @param player    initial mover (can also be a pusher).
     * @param heading   initial moving direction - doesnt change.
     *
     */
    public void moveToSpace(@NotNull Space nextSpace, @NotNull Player player, @NotNull Heading heading) throws ImpossibleMoveException {
        try {
            if (board.getNeighbour(player.getSpace(), heading) == null) { //getNeighbour checks for walls - returns null if no neighbor
                throw new ImpossibleMoveException("illegal move");
            } else {
                if (nextSpace.getPlayer() != null) {
                    Space nextNextSpace = board.getNeighbour(nextSpace, heading); //the space after the next space
                    if (nextNextSpace == null) {
                        throw new ImpossibleMoveException("Illegal move!");
                    }
                    Player neighborPlayer = nextSpace.getPlayer();
                    moveToSpace(nextNextSpace, neighborPlayer, heading); //recursive case - the moveToSpace is called again
                }
                player.setSpace(nextSpace); //standard case - nextSpace is just free.

            }
        } catch (ImpossibleMoveException e) {
            System.err.println(e); //print custom message
            e.printStackTrace(); //print stacktrace
        }
    }

    // XXX A6c
    public void startProgrammingPhase() {
        board.setPhase(Phase.PROGRAMMING);
        board.setCurrentPlayer(board.getPlayer(0));
        board.setStep(0);

        for (int i = 0; i < board.getPlayersNumber(); i++) {
            Player player = board.getPlayer(i);
            if (player != null) {
                for (int j = 0; j < Player.NO_REGISTERS; j++) {
                    CommandCardField field = player.getProgramField(j);
                    field.setCard(null);
                    field.setVisible(true);
                }
                for (int j = 0; j < Player.NO_CARDS; j++) {
                    CommandCardField field = player.getCardField(j);
                    field.setCard(generateRandomCommandCard());
                    field.setVisible(true);
                }
            }
        }
    }

    //self-explanatory
    private CommandCard generateRandomCommandCard() {
        Command[] commands = Command.values();
        int random = (int) (Math.random() * commands.length);
        return new CommandCard(commands[random]);
    }

    /**
     * This method initiates the ACTIVATION PHASE and is called through the GUI.
     */
    public void finishProgrammingPhase() {
        makeProgramFieldsInvisible();
        makeProgramFieldsVisible(0);
        board.setPhase(Phase.ACTIVATION); //initiates activation phase.
        board.setCurrentPlayer(board.getPlayer(0));
        board.setStep(0); //reset steps
    }

    // Enables programming fields - self-explanatory.
    private void makeProgramFieldsVisible(int register) {
        if (register >= 0 && register < Player.NO_REGISTERS) {
            for (int i = 0; i < board.getPlayersNumber(); i++) {
                Player player = board.getPlayer(i);
                CommandCardField field = player.getProgramField(register);
                field.setVisible(true);
            }
        }
    }

    //Disables programming fields - self-explanatory.
    private void makeProgramFieldsInvisible() {
        for (int i = 0; i < board.getPlayersNumber(); i++) {
            Player player = board.getPlayer(i);
            for (int j = 0; j < Player.NO_REGISTERS; j++) {
                CommandCardField field = player.getProgramField(j);
                field.setVisible(false);
            }
        }
    }

    /**
     * disables step-mode, and then executes all the commands in the given order.
     */
    public void executePrograms() {
        board.setStepMode(false);
        continuePrograms();
    }

    /**
     * enables step-mode, and then executes the command step-wise.
     */
    public void executeStep() {
        board.setStepMode(true);
        continuePrograms();
    }

    /**
     * This method is what separates the step-modes,
     */
    private void continuePrograms() {
        do {
            executeNextStep();
        } while (board.getPhase() == Phase.ACTIVATION && !board.isStepMode());
    }

    /**
     * The method will execute the next step in the correct order from the cards played by the players -
     * switching between them. It is the method that simulates the "ACTIVATION phase". The method also checks if there are
     * any FieldActions in the current space.
     *
     * The method also instantiates the PLAYER_INTERACTION phase, based on whether the
     * obtained card is interactive.
     *
     * @author Tokemeister, Friisma, KaspeDKK, Simon, Thomas, Rasbas
     *
     */
    private void executeNextStep() {
        Player currentPlayer = board.getCurrentPlayer();
        if (board.getPhase() == Phase.ACTIVATION && currentPlayer != null) { //Guard for activation phase - should not happen
            int step = board.getStep(); //represents registers.
            if (step >= 0 && step < Player.NO_REGISTERS) { // Guard for OB
                CommandCard card = currentPlayer.getProgramField(step).getCard(); //obtain card
                if (card != null) {
                    Command command = card.command;
                    if (command.isInteractive()) { //case of interactive card
                        Command selectedOption = board.getSelectedOption();
                        if (selectedOption == null) {
                            board.setPhase(Phase.PLAYER_INTERACTION); //shifts to player interactive phase.
                            return;
                        } else {
                            board.setSelectedOption(null);
                            executeCommand(currentPlayer, selectedOption);
                        }
                    } else { //case of a non-interactive card
                        executeCommand(currentPlayer, command);
                    }
                }

                int nextPlayerNumber = board.getPlayerNumber(currentPlayer) + 1;
                if (nextPlayerNumber < board.getPlayersNumber()) {
                    board.setCurrentPlayer(board.getPlayer(nextPlayerNumber));
                } else {
                    step++; //increment step.
                    if (step < Player.NO_REGISTERS) { //case of more steps.
                        makeProgramFieldsVisible(step);
                        board.setStep(step);
                        board.setCurrentPlayer(board.getPlayer(0));
                    } else {
                        for (int i = 0; i < board.getPlayersNumber(); i++) {
                            Player player = board.getPlayer(i);
                            Space playerSpace = player.getSpace();

                            if (playerSpace != null && playerSpace.getActions() != null) { //if there are any fieldActions on the given space.
                                List<FieldAction> actions = playerSpace.getActions();

                                for (FieldAction action : actions) { //execute implementation of fieldAction
                                    if (action.doAction(this, playerSpace)) {
                                        executeNextStep(); // keeps repeating until its false and no more valid field actions exist
                                    }
                                }
                            }
                        }
                        if (board.getPhase() != Phase.FINISHED) { //This guard ensures the game cant continue when we change the phase to finished.
                            startProgrammingPhase();
                        }
                    }
                }
            } else {
                // this should not happen
                assert false;
            }
        } else {
            // this should not happen
            assert false;
        }
    }

    /**
     * This method will set the given command "option" to the option that the player chose. In this case it can be (LEFT / RIGHT)
     * It will then set phase back to activation and continue the execution of the command cards.
     *
     * @param option
     */
    public void executeOptionAndContinue(Command option) {
        board.setSelectedOption(option); // sets the given commands "option" to the option that the player chose
        board.setPhase(Phase.ACTIVATION);
        continuePrograms();
    }

    /**
     * The method will execute the commands from the cards played by the player.
     *
     * @param player  is the current player
     * @param command is the command from the card played by the player.
     * @author Tokemeister, Friisma, KaspeDKK, Simon, Thomas, Rasbas
     */
    private void executeCommand(@NotNull Player player, Command command) {
        board.setGameCounter(board.getGameCounter() + 1);
        if (player.board == board && command != null) {
            switch (command) { //goes through each command and calls the movement respectively.
                case FORWARD:
                    this.moveForward(player);
                    break;
                case RIGHT:
                    this.turnRight(player);
                    break;
                case LEFT:
                    this.turnLeft(player);
                    break;
                case FAST_FORWARD:
                    this.fastForward(player);
                    break;
                case BACK:
                    this.back(player);
                    break;
                case UTURN:
                    this.uTurn(player);
                    break;
                default:
            }
        }
    }

    /**
     * Moves a player forward according to the MoveToSpace implementation.
     * The other movement commands are built on the moveForward
     *
     * @param player - current player.
     */
    public void moveForward(@NotNull Player player) {
        Space currentSpace = player.getSpace(); // current position
        Heading heading = player.getHeading(); // player heading

        Space nextSpace = board.getNeighbour(currentSpace, heading); // the space the player wants to move into
        if (nextSpace != null) {
            try {
                moveToSpace(nextSpace, player, heading);// sets the new position
            } catch (ImpossibleMoveException e) {
                throw new RuntimeException(e);
            }
        }
    }


    public void fastForward(@NotNull Player player) {
        moveForward(player);
        moveForward(player);
    }

    public void turnRight(@NotNull Player player) {
        Heading heading = player.getHeading();
        player.setHeading(heading.next()); //next = +90 deg.
    }

    public void turnLeft(@NotNull Player player) {
        Heading heading = player.getHeading();
        player.setHeading(heading.prev()); //prev = -90 deg.
    }

    public void uTurn(@NotNull Player player) {
        turnLeft(player);
        turnLeft(player); // 90+90 = 180 degrees
    }

    public void back(@NotNull Player player) {
        uTurn(player);
        moveForward(player);
        uTurn(player);
    }

    /**
     * Sets game winner in the model, and changes phase to finished
     *
     * @param player
     */
    public void setWinner(Player player){
        if (player != null) {
            board.setWinner(player); // points to logic / board
            board.setPhase(Phase.FINISHED); //changes phase
        }
    }


}
