package main_screen;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import thedrake.ui.TheDrakeApp;

public class MainController {

    @FXML
    public BorderPane border_pane;
    @FXML
    public Button singleplayer_btn;
    @FXML
    public Button local_multiplayer_btn;
    @FXML
    public Button online_multiplayer_btn;
    @FXML
    public Button exit_btn;
    @FXML
    public ImageView logo_img;

    @FXML
    protected void onExitButtonClick(ActionEvent event) {
        Stage stage = (Stage) singleplayer_btn.getScene().getWindow();
        stage.close();
    }

    @FXML
    public void onLocalMultiplayerButtonClick(ActionEvent event) {
        Stage stage = (Stage) local_multiplayer_btn.getScene().getWindow();
        TheDrakeApp app = new TheDrakeApp();
        try {
            app.start(stage);
        } catch (Exception e) {
            System.err.println("Chyba pri zmene sceny");
        }
    }

}