package thedrake.ui;

import javafx.scene.layout.Pane;
import thedrake.PlayingSide;
import thedrake.Troop;
import thedrake.TroopFace;

public class CapturedTroopView extends Pane {

    public CapturedTroopView(Troop troop, PlayingSide side) {
        setPrefSize(100, 100);
        setBackground(new TileBackgrounds().getTroop(troop, side, TroopFace.AVERS));
    }
}
