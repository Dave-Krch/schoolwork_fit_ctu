package thedrake.ui;

import javafx.scene.image.Image;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.layout.Pane;
import thedrake.GameState;
import thedrake.PlayingSide;

public class PlayerOnTurnPane extends Pane {

    Background orange;
    Background blue;

    public PlayerOnTurnPane() {
        setMaxSize(100,100);
        setMinSize(100,100);

        Image img_orange = new Image(getClass().getResourceAsStream("/assets/onTurnO.png"));
        orange = new Background(new BackgroundImage(img_orange, null, null, null, null));

        Image img_blue = new Image(getClass().getResourceAsStream("/assets/onTurnB.png"));
        blue = new Background(new BackgroundImage(img_blue, null, null, null, null));

    }

    public void update(GameState state) {
        if(state.sideOnTurn() == PlayingSide.ORANGE)
            setBackground(orange);
        else
            setBackground(blue);
    }

}
