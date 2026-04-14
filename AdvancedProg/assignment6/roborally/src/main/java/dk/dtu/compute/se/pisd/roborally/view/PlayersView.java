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
import dk.dtu.compute.se.pisd.roborally.model.Board;
import dk.dtu.compute.se.pisd.roborally.model.Player;
import javafx.scene.control.TabPane;

import dk.dtu.compute.se.pisd.roborally.model.Phase;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.TabPane;
import javafx.scene.layout.VBox;
import org.jetbrains.annotations.NotNull;

/**
 * ...
 *
 * @author Ekkart Kindler, ekki@dtu.dk
 *
 */
public class PlayersView extends TabPane implements ViewObserver {

    private Board board;

    private PlayerView[] playerViews;

    private TabPane tabPane;

    private VBox buttonPanel;

    private Button finishButton;
    private Button executeButton;
    private Button stepButton;

    private GameController gameController;

    public PlayersView(GameController gameController) {
            board = gameController.board;

            this.setTabClosingPolicy(TabClosingPolicy.UNAVAILABLE);

            finishButton = new Button("Finish Programming");
            finishButton.setOnAction(e -> gameController.finishProgrammingPhase());

            executeButton = new Button("Execute Program");
            executeButton.setOnAction(e -> gameController.executePrograms());

            stepButton = new Button("Execute Current Register");
            stepButton.setOnAction(e -> gameController.executeStep());

            buttonPanel = new VBox(finishButton, executeButton, stepButton);
            buttonPanel.setAlignment(Pos.CENTER_LEFT);
            buttonPanel.setSpacing(3.0);

            playerViews = new PlayerView[board.getPlayersNumber()];
            for (int i = 0; i < board.getPlayersNumber(); i++) {
                playerViews[i] = new PlayerView(gameController, board.getPlayer(i));
                this.getTabs().add(playerViews[i]);
            }

            board.attach(this);
            update(board);
    }

    @Override
    public void updateView(Subject subject) {
        if (subject == board) {
            Player current = board.getCurrentPlayer();
            int currentIndex = board.getPlayerNumber(current);
            this.getSelectionModel().select(currentIndex);

            for (PlayerView playerView : playerViews) {
                playerView.getSidePanel().getChildren().remove(buttonPanel);
            }

            if (board.getPhase() != Phase.PLAYER_INTERACTION) {
                playerViews[currentIndex].getSidePanel().getChildren().clear();
                playerViews[currentIndex].getSidePanel().getChildren().add(buttonPanel);

                switch (board.getPhase()) {
                    case INITIALISATION:
                        finishButton.setDisable(true);
                        executeButton.setDisable(false);
                        stepButton.setDisable(true);
                        break;

                    case PROGRAMMING:
                        finishButton.setDisable(false);
                        executeButton.setDisable(true);
                        stepButton.setDisable(true);
                        break;

                    case ACTIVATION:
                        finishButton.setDisable(true);
                        executeButton.setDisable(false);
                        stepButton.setDisable(false);
                        break;

                    case FINISHED:
                        finishButton.setDisable(true);
                        executeButton.setDisable(true);
                        stepButton.setDisable(true);
                        break;

                    default:
                        finishButton.setDisable(true);
                        executeButton.setDisable(true);
                        stepButton.setDisable(true);
                        break;
                }
            }
        }
    }
}