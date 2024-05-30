package thedrake.ui;

import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import main_screen.MainApp;
import thedrake.PlayingSide;

public class VictoryScreen extends GridPane {
    public VictoryScreen(PlayingSide side) {
        this.getStyleClass().add("victory-screen");

        Label victoryLabel = new Label(side.toString() + " victory!");
        victoryLabel.getStyleClass().add("victory-label");

        Button newGameButton = new Button("New game");
        newGameButton.getStyleClass().add("button");
        newGameButton.setOnMouseClicked(mouseEvent -> newGame());

        Button titleScreenButton = new Button("Title screen");
        titleScreenButton.getStyleClass().add("button");
        titleScreenButton.setOnMouseClicked(mouseEvent -> titleScreen());

        add(victoryLabel, 0, 0);
        add(newGameButton, 0, 1);
        add(titleScreenButton, 0, 2);

        getStylesheets().add(getClass().getResource("styles.css").toExternalForm());
    }

    private void newGame() {
        Stage stage = (Stage) getScene().getWindow();
        TheDrakeApp app = new TheDrakeApp();
        try {
            app.start(stage);
        } catch (Exception e) {
            System.err.println("Chyba pri zmene sceny");
        }
    }

    private void titleScreen() {
        Stage stage = (Stage) getScene().getWindow();
        MainApp app = new MainApp();
        try {
            app.start(stage);
        } catch (Exception e) {
            System.err.println("Chyba pri zmene sceny");
        }
    }
}
