package dk.dtu.compute.se.pisd.roborally.controller;

import dk.dtu.compute.se.pisd.roborally.model.Board;
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
    private Boolean isFinal = false;

    /**
     * doAction excecutes the action of the space accordingly.
     *
     * This doAction method gives the player a checkpoint.
     *
     * @param gameController the gameController of the respective game
     * @param space the space this action should be executed for
     * @return boolean if the conditions of the checkpoint is met.
     */
    public boolean doAction(@NotNull GameController gameController, @NotNull Space space) {
        if (space.getPlayer() != null){
            Player currP = space.getPlayer();
             //if player did not reach the specific checkpoint yet.
                if (currP.getCheckpoint() == this.number-1){
                currP.setCheckpoint(this.number); //increment num of checkpoints visited
                    if (this.isFinal){  //win condition
                    gameController.setWinner(currP);
                    }
                return true;
            }
        }
        return false;
    }

    public void setNumber(Integer number){this.number = number;};

    public void setIsFinal(boolean isFinal){this.isFinal = isFinal;}


}


