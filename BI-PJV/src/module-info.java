module thedrake {
    requires javafx.fxml;
    requires javafx.controls;
    requires java.desktop;

    opens thedrake.ui;
    opens main_screen;
}