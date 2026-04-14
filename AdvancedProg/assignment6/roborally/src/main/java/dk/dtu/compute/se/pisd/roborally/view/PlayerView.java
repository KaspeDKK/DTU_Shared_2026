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
package dk.dtu.compute.se.pisd.roborally.view;

import dk.dtu.compute.se.pisd.designpatterns.observer.Subject;
import dk.dtu.compute.se.pisd.roborally.controller.GameController;
import dk.dtu.compute.se.pisd.roborally.model.Command;
import dk.dtu.compute.se.pisd.roborally.model.CommandCardField;
import dk.dtu.compute.se.pisd.roborally.model.Phase;
import dk.dtu.compute.se.pisd.roborally.model.Player;
import javafx.geometry.Pos;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Tab;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import org.jetbrains.annotations.NotNull;

/**
 * ...
 *
 * @author Ekkart Kindler, ekki@dtu.dk
 *
 */
public class PlayerView extends Tab implements ViewObserver {

    private Player player;

    private VBox top;

    private Label programLabel;
    private GridPane programPane;
    private Label cardsLabel;
    private GridPane cardsPane;
    private Label checkpointLabel;

    private CardFieldView[] programCardViews;
    private CardFieldView[] cardViews;

    private VBox sidePanel;
    private VBox playerInteractionPanel;

    private VBox defaultSidePanelContent;

    private GameController gameController;

    public PlayerView(@NotNull GameController gameController, @NotNull Player player) {
        super(player.getName());
        this.setStyle("-fx-text-base-color: " + player.getColor() + ";");

        top = new VBox();
        this.setContent(top);

        this.gameController = gameController;
        this.player = player;

        programLabel = new Label("Program");

        programPane = new GridPane();
        programPane.setVgap(2.0);
        programPane.setHgap(2.0);
        programCardViews = new CardFieldView[Player.NO_REGISTERS];
        for (int i = 0; i < Player.NO_REGISTERS; i++) {
            CommandCardField cardField = player.getProgramField(i);
            if (cardField != null) {
                programCardViews[i] = new CardFieldView(gameController, cardField);
                programPane.add(programCardViews[i], i, 0);
            }
        }

        sidePanel = new VBox();
        sidePanel.setAlignment(Pos.CENTER_LEFT);
        sidePanel.setSpacing(3.0);

        playerInteractionPanel = new VBox();
        playerInteractionPanel.setAlignment(Pos.CENTER_LEFT);
        playerInteractionPanel.setSpacing(3.0);

        programPane.add(sidePanel, Player.NO_REGISTERS, 0);

        cardsLabel = new Label("Command Cards");
        cardsPane = new GridPane();
        cardsPane.setVgap(2.0);
        cardsPane.setHgap(2.0);
        cardViews = new CardFieldView[Player.NO_CARDS];
        for (int i = 0; i < Player.NO_CARDS; i++) {
            CommandCardField cardField = player.getCardField(i);
            if (cardField != null) {
                cardViews[i] = new CardFieldView(gameController, cardField);
                cardsPane.add(cardViews[i], i, 0);
            }
        }

        checkpointLabel = new Label("Number of reached checkpoints: " + player.getCheckpoint());

        top.getChildren().add(programLabel);
        top.getChildren().add(programPane);
        top.getChildren().add(cardsLabel);
        top.getChildren().add(cardsPane);
        top.getChildren().add(checkpointLabel);


        // DONE A6d: a label for the status of this player could be added here
        //     for showing the number of achieved checkpoints (etc).

        if (player.board != null) {
            player.board.attach(this);
            update(player.board);
        }
    }

    public VBox getSidePanel() {
        return sidePanel;
    }

    public void setDefaultSidePanelContent(@NotNull VBox content) {
        this.defaultSidePanelContent = content;
        if (player.board.getPhase() != Phase.PLAYER_INTERACTION) {
            sidePanel.getChildren().clear();
            sidePanel.getChildren().add(defaultSidePanelContent);
        }
    }

    @Override
    public void updateView(Subject subject) {
        if (player.board.getPhase() == Phase.FINISHED){


            //Show the modal, but only once - hence the boolean
            if(!player.board.getGameOverMessageIsShown()) {
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setTitle("Game Over");
                alert.setHeaderText("Winner");
                alert.setContentText(player.board.getWinner().getName() + " won the game!");
                alert.showAndWait();

                player.board.setGameOverMessageIsShown(true); //Sets message shown to true.
            }

            player.board.setWinner(null);
            return;
        }
        if (subject == player.board) {
            checkpointLabel.setText("Number of reached checkpoints: " + player.getCheckpoint()); // number of checkpoints reached for each player.
            player.attach(this);
            for (int i = 0; i < Player.NO_REGISTERS; i++) {
                CardFieldView cardFieldView = programCardViews[i];
                if (cardFieldView != null) {
                    if (player.board.getPhase() == Phase.PROGRAMMING ) {
                        cardFieldView.setBorder(CardFieldView.BORDER_DEFAULT);
                    } else {
                        if (i < player.board.getStep()) {
                            cardFieldView.setBorder(CardFieldView.BORDER_DONE);
                        } else if (i == player.board.getStep()) {
                            if (player.board.getCurrentPlayer() == player) {
                                cardFieldView.setBorder(CardFieldView.BORDER_ACTIVE);
                            } else if (player.board.getPlayerNumber(player.board.getCurrentPlayer()) > player.board.getPlayerNumber(player)) {
                                cardFieldView.setBorder(CardFieldView.BORDER_DONE);
                            } else {
                                cardFieldView.setBorder(CardFieldView.BORDER_READY);
                            }
                        } else {
                            cardFieldView.setBorder(CardFieldView.BORDER_DEFAULT);
                        }
                    }
                }
            }

            // DONE A6e: these buttons should be shown only when there is
            // an interactive command card, and the buttons should represent
            // the player's choices of the interactive command card. The
            // following is just a mockup showing two options

            if (player.board.getPhase() == Phase.PLAYER_INTERACTION) {
                sidePanel.getChildren().clear();
                sidePanel.getChildren().add(playerInteractionPanel);

                playerInteractionPanel.getChildren().clear();

                if (player.board.getCurrentPlayer() == player) {
                    Button optionButton = new Button("Turn Right");
                    optionButton.setOnAction(e -> gameController.executeOptionAndContinue(Command.RIGHT));
                    playerInteractionPanel.getChildren().add(optionButton);

                    optionButton = new Button("Turn Left");
                    optionButton.setOnAction(e -> gameController.executeOptionAndContinue(Command.LEFT));
                    playerInteractionPanel.getChildren().add(optionButton);
                }
            } else {
                playerInteractionPanel.getChildren().clear();

                if (defaultSidePanelContent != null) {
                    sidePanel.getChildren().clear();
                    sidePanel.getChildren().add(defaultSidePanelContent);
                }

                if (player.board.getPhase() == Phase.FINISHED) {
                    cardsPane.setDisable(true);
                    programPane.setDisable(true);
                } else {
                    cardsPane.setDisable(false);
                    programPane.setDisable(false);
                }
            }

        }
    }

}
