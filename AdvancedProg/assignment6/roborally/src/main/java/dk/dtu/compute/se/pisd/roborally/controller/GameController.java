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

import dk.dtu.compute.se.pisd.roborally.model.*;
import org.jetbrains.annotations.NotNull;

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
     * clicking on set space.
     *
     * @author Tokemeister, Pomfriis, KaspeDKK, Simon, Thomas, Rasbas
     *
     * @param space the space to which the current player should move
     *
     */
    public void moveCurrentPlayerToSpace(@NotNull Space space)  {

        if (space.getPlayer() != null) throw new IllegalStateException("Space is already occupied. MISTER!"); // defensive programming
        if (board.getCurrentPlayer() == null) throw new IllegalStateException("current player is null?!"); // very defensive programming

        Player curr = board.getCurrentPlayer(); // get player


        int count = board.getGameCounter(); // get count locally
        
        curr.setSpace(space); // set current player to assigned space
        count++; // increment local
        int next = (board.getPlayerNumber(curr) + 1) % board.getPlayersNumber(); // get next player with wraparound using %
        board.setCurrentPlayer(board.getPlayer(next)); // set next player

        board.setGameCounter(count); // increment the actual count.

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

    // XXX A6c
    private CommandCard generateRandomCommandCard() {
        Command[] commands = Command.values();
        int random = (int) (Math.random() * commands.length);
        return new CommandCard(commands[random]);
    }

    // XXX A6c
    public void finishProgrammingPhase() {
        makeProgramFieldsInvisible();
        makeProgramFieldsVisible(0);
        board.setPhase(Phase.ACTIVATION);
        board.setCurrentPlayer(board.getPlayer(0));
        board.setStep(0);
    }

    // XXX A6c
    private void makeProgramFieldsVisible(int register) {
        if (register >= 0 && register < Player.NO_REGISTERS) {
            for (int i = 0; i < board.getPlayersNumber(); i++) {
                Player player = board.getPlayer(i);
                CommandCardField field = player.getProgramField(register);
                field.setVisible(true);
            }
        }
    }

    // XXX A6c
    private void makeProgramFieldsInvisible() {
        for (int i = 0; i < board.getPlayersNumber(); i++) {
            Player player = board.getPlayer(i);
            for (int j = 0; j < Player.NO_REGISTERS; j++) {
                CommandCardField field = player.getProgramField(j);
                field.setVisible(false);
            }
        }
    }

    // XXX A6c
    public void executePrograms() {
        board.setStepMode(false);
        continuePrograms();
    }

    // XXX A6c
    public void executeStep() {
        board.setStepMode(true);
        continuePrograms();
    }

    // XXX A6c
    private void continuePrograms() {
        do {
            executeNextStep();
        } while (board.getPhase() == Phase.ACTIVATION && !board.isStepMode());
    }

    // XXX A6c
    // TODO A6d: add the execution of the field actions at the right
    //      place in this method
    // TODO A6e: implement the execution af an interactive card to
    //     this method (e.g. by switching to the PLAYER_INTERACTION phase
    //     at the right point)
    private void executeNextStep() {
        Player currentPlayer = board.getCurrentPlayer();
        if (board.getPhase() == Phase.ACTIVATION && currentPlayer != null) {
            int step = board.getStep();
            if (step >= 0 && step < Player.NO_REGISTERS) {
                CommandCard card = currentPlayer.getProgramField(step).getCard();
                if (card != null) {
                    Command command = card.command;
                    executeCommand(currentPlayer, command);
                }
                int nextPlayerNumber = board.getPlayerNumber(currentPlayer) + 1;
                if (nextPlayerNumber < board.getPlayersNumber()) {
                    board.setCurrentPlayer(board.getPlayer(nextPlayerNumber));
                } else {
                    step++;
                    if (step < Player.NO_REGISTERS) {
                        makeProgramFieldsVisible(step);
                        board.setStep(step);
                        board.setCurrentPlayer(board.getPlayer(0));
                    } else {
                        startProgrammingPhase();
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

    /** For the current player to execute a command. Commands are RoboRally programming cards.
     *
     * @param player current player that is executing a command
     * @param command command to be executed by the current player
     */
    private void executeCommand(@NotNull Player player, Command command) {
        if (player != null && player.board == board && command != null) {
            // XXX This is a very simplistic way of dealing with some basic cards and
            //     their execution. This should eventually be done in a more elegant way
            //     (this concerns the way cards are modelled as well as the way they are executed).

            switch (command) {
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
                // DONE A6c: add the cases for the new commands BACK and UTURN to
                //     this case statement.
                default:
                    // DO NOTHING (for now)//
            }
        }
    }

    // DONE A6c: implement this method

    /** Moves the player one space forward in the heading direction
     * when executing a FORWARD command in {@code executeCommand}
     *
     * @param player this player that is moving forward
     */
    public void moveForward(@NotNull Player player) {
        Space currentSpace = player.getSpace(); // current position
        Heading heading = player.getHeading(); // player heading
        Space nextSpace = board.getNeighbour(currentSpace,heading); // the space the player wants to move into
        if (nextSpace != null){
            player.setSpace(nextSpace); // sets the new position
        }
    }

    /** Moves the player executing a FAST_FORWARD command
     * two spaces forward in the heading direction in {@code executeCommand}
     *
     * @param player this player that is moving forward twice
     */

    // DONE A6c: implement this method
    public void fastForward(@NotNull Player player) {
        moveForward(player);
        moveForward(player);
    }

    /** Turns the player's heading direction 90 degrees to the right
     * when executing a RIGHT command in {@code executeCommand}
     *
     * @param player this player that is changing heading direction
     */

    // DONE A6c: implement this method
    public void turnRight(@NotNull Player player) {
        Heading heading = player.getHeading();
        player.setHeading(heading.next());
    }

    /** Turns the player's heading direction 90 degrees to the left
     * when executing a LEFT command in {@code executeCommand}
     *
     * @param player this player that changes heading direction
     */

    // DONE A6c: implement this method
    public void turnLeft(@NotNull Player player) {
        Heading heading = player.getHeading();
        player.setHeading(heading.prev());
    }

    /** Turns the player's heading 180 degrees, when
     * executing a UTURN command in {@code executeCommand}
     *
     * @param player this player that is changing heading direction
     */

    // DONE A6c: Add two methods for the new commands BACK and UTURN here.

    public void uTurn(@NotNull Player player){
        turnLeft(player);
        turnLeft(player);
    }

    /** Moves the player backwards from the heading direction, when
     * executing a BACK command in {@code executeCommand}
     *
     * @param player this player that turning right
     */

    public void back(@NotNull Player player){
        uTurn(player);
        moveForward(player);
        uTurn(player);
    }
    /**
     * A method called when no corresponding controller operation is implemented yet.
     * This should eventually be removed.
     */
    public void notImplemented() {
        // XXX just for now to indicate that the actual method is not yet implemented
        assert false;
    }

}
