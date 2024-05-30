package thedrake.ui;

import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.image.ImageView;

import thedrake.BoardPos;
import thedrake.Move;
import thedrake.Tile;



public class BoardTileView extends Pane implements TileView {

    private Tile tile;

    private final TileBackgrounds backgrounds = new TileBackgrounds();

    private final Border selectBorder = new Border(new BorderStroke(Color.BLACK, BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(3)));

    private final ImageView moveImage;

    private final BoardPos position;

    private final TileViewContext context;

    private Move move;

    public BoardTileView(Tile tile, BoardPos boardPos, TileViewContext context) {
        this.tile = tile;
        this.position = boardPos;
        this.context = context;

        setPrefSize(100, 100);

        update();

        setOnMouseClicked(event -> onClick());

        moveImage = new ImageView(getClass().getResource("/assets/move.png").toString());
        moveImage.setVisible(false);
        getChildren().add(moveImage);
    }

    public void setTile(Tile tile) {
        this.tile = tile;
        update();
    }

    private void onClick() {
        if(move != null)
            context.executeMove(move);
        else if(tile.hasTroop())
            select();
    }

    private void select() {
        setBorder(selectBorder);
        context.tileViewSelected(this);
    }

    @Override
    public void unSelect() {
        setBorder(null);
    }

    public void update() {
        setBackground(backgrounds.get(tile));
    }

    public BoardPos position() {
        return position;
    }

    public void setMove(Move move) {
        this.move = move;
        moveImage.setVisible(true);
    }

    public void clearMove() {
        this.move = null;
        moveImage.setVisible(false);
    }
}
