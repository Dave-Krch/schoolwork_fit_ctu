package thedrake.ui;


import thedrake.GameState;
import thedrake.Move;

public interface TileViewContext {

    GameState getGameState();

    void tileViewSelected(BoardTileView boardTileView);

    void stackViewSelected(StackTileView stackTileView);

    void executeMove(Move move);
}
