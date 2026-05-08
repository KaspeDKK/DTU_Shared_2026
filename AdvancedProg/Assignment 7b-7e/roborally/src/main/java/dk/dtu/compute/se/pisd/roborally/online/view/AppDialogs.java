package dk.dtu.compute.se.pisd.roborally.online.view;

import dk.dtu.compute.se.pisd.roborally.online.controller.OnlineController;
import dk.dtu.compute.se.pisd.roborally.online.model.Game;

import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Modality;
import javafx.stage.Stage;
import org.springframework.web.client.HttpClientErrorException;

public class AppDialogs {

    private OnlineController onlineController;

    /**
     *
     * @param onlineController of the app window / app view on the user's current display
     */
    public AppDialogs(OnlineController onlineController) {
        this.onlineController = onlineController;
    }

    /** sign in Dialog buttons. Sign in by inputting your name (which is uniquely identified) in a signIn dialog
     */

    public void signIn() {
        Stage stage = new Stage();

        Text text = new Text("Sign in as user for Online RoboRally with a user name.");
        TextField userName = new TextField();

        Button cancel = new Button("Cancel");
        cancel.setOnAction(e -> stage.close());
        Button register = new Button("Sign in");
        register.setOnAction(
                e -> {
                    String name = userName.getText();
                    if (name.length() >= 4) {
                        stage.close();
                        onlineController.signIn(name);
                    }
                }
        );
        HBox buttons = new HBox(cancel, register);

        VBox vbox = new VBox(text, userName, buttons);

        Scene scene = new Scene(vbox);
        stage.setTitle("Sign in for Online RoboRally");
        stage.setScene(scene);
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setResizable(false);
        stage.sizeToScene();
        stage.show();
    }

    /** sign up Dialog buttons. Sign up by inputting a non-existing name in a signUp dialog
     *
     */
    public void signUp() {
        Stage stage = new Stage();

        Text text = new Text("Sign up for Online RoboRally");
        TextField userName = new TextField();

        Button cancel = new Button("Cancel");
        cancel.setOnAction(e -> stage.close());

        Button register = new Button("Sign up");
        register.setOnAction(
                e -> {
                    String name = userName.getText();
                    stage.close();
                    onlineController.signUp(name);
                }
        );

        HBox buttons = new HBox(cancel, register);

        VBox vbox = new VBox(text, userName, buttons);
        Scene scene = new Scene(vbox);
        stage.setTitle("Sign up for Online RoboRally");
        stage.setScene(scene);
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setResizable(false);
        stage.sizeToScene();
        stage.show();

    }

    // Done Assignment 7c you might want to implement a dialog for a SingUp or
    //      registering a new user.

    /** Dialog buttons for event of "new game" button in an online game.
     *  When pushing "new game" button, then a createNewGame dialog will appear and ask you for the name of the game, the minimum player count and maximum player count.
     *  If minimum player number input < 2, then catch the exception thrown from the backend, and display an error message
     *  If maximum player number input > 6, then catch the exception thrown from the backend, and display an error message
     */

    public void createNewGame() {
        Stage stage = new Stage();

        Text text = new Text("Add the needed data for your a new game.");

        Label labelGameName = new Label("Name:");
        TextField gameName = new TextField();
        HBox nameBox = new HBox(labelGameName, gameName);

        Label minLabel = new Label("min:");
        TextField min = new TextField();
        Label maxLabel = new Label("max:");
        TextField max = new TextField();
        HBox minMaxBox = new HBox(minLabel, min, maxLabel, max);


        Button cancel = new Button("Cancel");
        cancel.setOnAction(e -> stage.close());
        Button create = new Button("Create");
        create.setOnAction(
                e -> {
                    try {
                        Game game = new Game();
                        game.setName(gameName.getText());
                        game.setMinPlayers(Integer.parseInt(min.getText()));
                        game.setMaxPlayers(Integer.parseInt(max.getText()));
                        onlineController.createGame(game);
                        stage.close();
                    } catch (HttpClientErrorException.Conflict ex) {
                        Text errorText = new Text("There was a problem creating the game: " + ex.getMessage());
                        VBox vbox = new VBox(text, nameBox, minMaxBox, errorText, cancel);
                        Scene scene = new Scene(vbox);
                        stage.setScene(scene);
                    }
                }
        );
        HBox buttons = new HBox(cancel, create);

        VBox vbox = new VBox(text, nameBox, minMaxBox, buttons);

        Scene scene = new Scene(vbox);
        stage.setTitle("Create New Online Game");
        stage.setScene(scene);
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setResizable(false);
        stage.sizeToScene();
        stage.show();
    }

}
