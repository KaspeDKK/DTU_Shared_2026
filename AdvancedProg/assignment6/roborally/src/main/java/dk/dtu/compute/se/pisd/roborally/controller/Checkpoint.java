package dk.dtu.compute.se.pisd.roborally.controller;

import dk.dtu.compute.se.pisd.roborally.model.Player;
import dk.dtu.compute.se.pisd.roborally.model.Space;
import org.jetbrains.annotations.NotNull;

/**
 * This class represents the checkpoint from Roborally
 *
 * @author Tokemeister, Friisma, KaspeDKK, SimoXSwagger, UngeRas, Thomas
 */

public class Checkpoint extends FieldAction {

    public Integer number;

    public boolean doAction(@NotNull GameController gameController, @NotNull Space space) {
        // TODO A6d: needs to be implemented
        // ...
        if (gameController.board.getCurrentPlayer() != null){
            Player currP = gameController.board.getCurrentPlayer();
             //if player did not reach the specific checkpoint yet.
                if (currP.getCheckpoint() == this.number-1){
                currP.setCheckpoint(this.number); //increment num of checkpoints visited
                return true;
            }
        }
        return false;
    }

    public void setNumber(Integer number){this.number = number;};

}


