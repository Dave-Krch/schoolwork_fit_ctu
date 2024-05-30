package thedrake.ui;

import javafx.geometry.Pos;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.layout.*;
import javafx.scene.image.Image;
import javafx.scene.layout.Background;
import thedrake.*;

import java.util.List;


public class BoardView extends StackPane implements TileViewContext{

    private GameState gameState;

    private TileView selected;

    private ValidMoves validMoves;

    private PlayerOnTurnPane playerOnTurnPane;

    private StackTileView blueStack;
    private StackTileView orangeStack;

    private GridPane mainGrid;
    private GridPane board;
    private GridPane blueArmy;
    private GridPane orangeArmy;

    public BoardView(GameState gameState) {
        this.gameState = gameState;
        validMoves = new ValidMoves(this.gameState);

        PositionFactory positionFactory = gameState.board().positionFactory();

        // BoardView is a stack view so we add mainGrid as a grid for the main screen
        getChildren().add(mainGrid = new GridPane());

        // main game board
        mainGrid.add(board = new GridPane(), 1, 1);

        // pane showing who is on turn
        mainGrid.add(playerOnTurnPane = new PlayerOnTurnPane(), 0, 1);
        playerOnTurnPane.update(gameState);

        // ----- grids representing stacks and captured units for both players -----
        mainGrid.add(orangeArmy = new GridPane(), 1, 0);
        mainGrid.add(blueArmy = new GridPane(), 1, 2);

        orangeArmy.add(createSimplePane("/assets/orangeStack.png"), 0, 0);
        orangeArmy.add(orangeStack = new StackTileView(this, PlayingSide.ORANGE), 1, 0);
        orangeArmy.add(createSimplePane("/assets/orangeCaptured.png"), 2, 0);

        blueArmy.add(createSimplePane("/assets/blueStack.png"), 0, 0);
        blueArmy.add(blueStack = new StackTileView(this, PlayingSide.BLUE), 1, 0);
        blueArmy.add(createSimplePane("/assets/blueCaptured.png"), 2, 0);
        // ---------------------------------------------------------------------------

        setMinSize(1500, 800);
        setAlignment(Pos.CENTER);

        mainGrid.setAlignment(Pos.CENTER);

        setupGridPane(orangeArmy);
        setupGridPane(blueArmy);
        setupGridPane(board);

        for(int y = 0; y < positionFactory.dimension(); y++) {
            for(int x = 0; x < positionFactory.dimension(); x++) {
                BoardPos pos = positionFactory.pos(x, positionFactory.dimension() - y - 1);
                board.add(new BoardTileView(gameState.tileAt(pos), pos, this), x, y);
            }
        }
    }

    // visual settings for nodes
    private void setupGridPane(GridPane gridPane) {
        gridPane.setHgap(5);
        gridPane.setVgap(5);
        gridPane.setPadding(new Insets(15));
        gridPane.setAlignment(Pos.CENTER);
    }

    private void updateTiles() {
        for(Node node : board.getChildren()) {
            BoardTileView boardTileView = (BoardTileView) node;
            boardTileView.setTile(gameState.tileAt(boardTileView.position()));
            boardTileView.update();
        }
    }

    @Override
    public GameState getGameState() {
        return gameState;
    }

    @Override
    public void tileViewSelected(BoardTileView boardTileView) {
        if(selected != null && selected != boardTileView)
            selected.unSelect();

        selected = boardTileView;

        clearMoves();
        showMoves(validMoves.boardMoves(boardTileView.position()));
    }

    @Override
    public void stackViewSelected(StackTileView stackTileView){
        if(selected != null && selected != stackTileView)
            selected.unSelect();

        selected = stackTileView;

        clearMoves();
        showMoves(validMoves.movesFromStack());
    }

    private void checkVictory() {
        if(gameState.result() == GameResult.VICTORY) {
            if(gameState.army(PlayingSide.BLUE).boardTroops().leaderPosition() == TilePos.OFF_BOARD)
                victory(PlayingSide.ORANGE);
            else if (gameState.army(PlayingSide.ORANGE).boardTroops().leaderPosition() == TilePos.OFF_BOARD)
                victory(PlayingSide.BLUE);
        }
        else if(validMoves.allMoves().isEmpty()) {
            victory(gameState.armyNotOnTurn().side());
        }
    }

    private void victory(PlayingSide side) {
        getChildren().add(new VictoryScreen(side));
    }

    @Override
    public void executeMove(Move move) {
        selected.unSelect();
        selected = null;

        clearMoves();

        gameState = move.execute(gameState);
        validMoves = new ValidMoves(gameState);

        playerOnTurnPane.update(gameState);
        updateTiles();
        updateCaptured();
        updateStack();
        checkVictory();
    }

    private void showMoves(List<Move> moves) {
        for(Move move : moves) {
            tileViewAt(move.target()).setMove(move);
        }
    }

    private void clearMoves() {
        for(Node node : board.getChildren()) {
            BoardTileView boardTileView = (BoardTileView) node;
            boardTileView.clearMove();
        }
    }

    private BoardTileView tileViewAt(BoardPos target) {
        int index = ( gameState.board().dimension() - 1 - target.j()) * 4 + target.i();
        return (BoardTileView) board.getChildren().get(index);
    }

    private void updateCaptured() {
        int blue_captured_count = blueArmy.getChildren().size() - 3;
        if( blue_captured_count < gameState.army(PlayingSide.BLUE).captured().size()) {
            blueArmy.add(new CapturedTroopView(gameState.army(PlayingSide.BLUE).captured().getLast(), PlayingSide.ORANGE) ,blue_captured_count + 3, 0);
        }

        int orange_captured_count = orangeArmy.getChildren().size() - 3;
        if( orange_captured_count < gameState.army(PlayingSide.ORANGE).captured().size()) {
            orangeArmy.add(new CapturedTroopView(gameState.army(PlayingSide.ORANGE).captured().getLast(), PlayingSide.BLUE) ,orange_captured_count + 3, 0);
        }
    }

    private Pane createSimplePane(String imagePath) {
        Pane out = new Pane();
        out.setPrefSize(100, 100);

        Image img = new Image(getClass().getResourceAsStream(imagePath));
        Background bg = new Background(new BackgroundImage(img, null, null, null, null));

        out.setBackground(bg);

        return out;
    }

    private void updateStack() {
        orangeStack.update();
        blueStack.update();
    }

}
