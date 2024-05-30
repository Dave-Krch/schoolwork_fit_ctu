package thedrake.ui;

import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import thedrake.*;

public class StackTileView extends Pane implements TileView {

    private final TileBackgrounds backgrounds = new TileBackgrounds();

    private final Border selectBorder = new Border(new BorderStroke(Color.BLACK, BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(3)));

    TileViewContext context;

    PlayingSide side;

    StackTileView(TileViewContext context, PlayingSide side) {
        this.context = context;
        this.side = side;

        setPrefSize(100, 100);

        update();

        setOnMouseClicked(event -> onClick());
    }

    private void onClick() {
        if(!context.getGameState().army(side).stack().isEmpty() && context.getGameState().sideOnTurn() == side)
            select();
    }

    private void select(){
        setBorder(selectBorder);
        context.stackViewSelected(this);
    }

    @Override
    public void unSelect(){
        setBorder(null);
    }

    public void update() {
        setBackground(backgrounds.getTroop(
                context.getGameState().army(side).stack().getFirst(),
                side,
                TroopFace.AVERS
        ));
    }
}
