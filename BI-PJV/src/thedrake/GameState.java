package thedrake;

import java.io.PrintWriter;
import java.io.Serializable;
import java.util.Optional;

public class GameState implements JSONSerializable {
    private final Board board;
    private final PlayingSide sideOnTurn;
    private final Army blueArmy;
    private final Army orangeArmy;
    private final GameResult result;

    public GameState(
            Board board,
            Army blueArmy,
            Army orangeArmy) {
        this(board, blueArmy, orangeArmy, PlayingSide.BLUE, GameResult.IN_PLAY);
    }

    public GameState(
            Board board,
            Army blueArmy,
            Army orangeArmy,
            PlayingSide sideOnTurn,
            GameResult result) {
        this.board = board;
        this.sideOnTurn = sideOnTurn;
        this.blueArmy = blueArmy;
        this.orangeArmy = orangeArmy;
        this.result = result;
    }

    public Board board() {
        return board;
    }

    public PlayingSide sideOnTurn() {
        return sideOnTurn;
    }

    public GameResult result() {
        return result;
    }

    public Army army(PlayingSide side) {
        if (side == PlayingSide.BLUE) {
            return blueArmy;
        }

        return orangeArmy;
    }

    public Army armyOnTurn() {
        return army(sideOnTurn);
    }

    public Army armyNotOnTurn() {
        if (sideOnTurn == PlayingSide.BLUE)
            return orangeArmy;

        return blueArmy;
    }

    public Tile tileAt(TilePos pos) {
        Optional<TroopTile> res;
        res = blueArmy.boardTroops().at(pos);

        if (res.isPresent()) {
            return res.get();
        }

        res = orangeArmy.boardTroops().at(pos);
        if (res.isPresent()) {
            return res.get();
        }

        return board.at(pos);
    }

    private boolean canStepFrom(TilePos origin) {
        if(result != GameResult.IN_PLAY) {
            return false;
        }

        if(armyOnTurn().boardTroops().isPlacingGuards() /*|| !armyNotOnTurn().boardTroops().isLeaderPlaced()*/) {
            return false;
        }

        Optional<TroopTile> res;
        res = armyOnTurn().boardTroops().at(origin);

        return res.isPresent();
    }

    private boolean canStepTo(TilePos target) {
        if(result != GameResult.IN_PLAY || target == TilePos.OFF_BOARD) {
            return false;
        }

        return tileAt(target).canStepOn();

    }

    private boolean canCaptureOn(TilePos target) {
        if(result != GameResult.IN_PLAY || target == TilePos.OFF_BOARD) {
            return false;
        }

        Optional<TroopTile> res;
        res = armyNotOnTurn().boardTroops().at(target);

        return res.isPresent();
    }

    public boolean canStep(TilePos origin, TilePos target) {
        return canStepFrom(origin) && canStepTo(target);
    }

    public boolean canCapture(TilePos origin, TilePos target) {
        return canStepFrom(origin) && canCaptureOn(target);
    }

    public boolean canPlaceFromStack(TilePos target) {
        if(result != GameResult.IN_PLAY || armyOnTurn().stack().isEmpty() || target == TilePos.OFF_BOARD || !canStepTo(target)) {
            return false;
        }

        //neni polozeny leader
        if(!armyOnTurn().boardTroops().isLeaderPlaced()) {
            if(sideOnTurn() == PlayingSide.BLUE) {
                return target.row() == 1;
            }
            else if(sideOnTurn() == PlayingSide.ORANGE) {
                return target.row() == board.dimension();
            }
        }

        if(armyOnTurn().boardTroops().isPlacingGuards()) {
            for(TilePos neighbour: armyOnTurn().boardTroops().leaderPosition().neighbours()) {
                if(target.equals(neighbour)) {
                    return true;
                }
            }
            return false;
        }

        for(BoardPos troop: armyOnTurn().boardTroops().troopPositions()) {
            for(BoardPos neighbour: troop.neighbours()) {
                if(target.equals(neighbour)) {
                    return true;
                }
            }
        }

        return false;
    }

    public GameState stepOnly(BoardPos origin, BoardPos target) {
        if (canStep(origin, target))
            return createNewGameState(
                    armyNotOnTurn(),
                    armyOnTurn().troopStep(origin, target), GameResult.IN_PLAY);

        throw new IllegalArgumentException();
    }

    public GameState stepAndCapture(BoardPos origin, BoardPos target) {
        if (canCapture(origin, target)) {
            Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
            GameResult newResult = GameResult.IN_PLAY;

            if (armyNotOnTurn().boardTroops().leaderPosition().equals(target))
                newResult = GameResult.VICTORY;

            return createNewGameState(
                    armyNotOnTurn().removeTroop(target),
                    armyOnTurn().troopStep(origin, target).capture(captured), newResult);
        }

        throw new IllegalArgumentException();
    }

    public GameState captureOnly(BoardPos origin, BoardPos target) {
        if (canCapture(origin, target)) {
            Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
            GameResult newResult = GameResult.IN_PLAY;

            if (armyNotOnTurn().boardTroops().leaderPosition().equals(target))
                newResult = GameResult.VICTORY;

            return createNewGameState(
                    armyNotOnTurn().removeTroop(target),
                    armyOnTurn().troopFlip(origin).capture(captured), newResult);
        }

        throw new IllegalArgumentException();
    }

    public GameState placeFromStack(BoardPos target) {
        if (canPlaceFromStack(target)) {
            return createNewGameState(
                    armyNotOnTurn(),
                    armyOnTurn().placeFromStack(target),
                    GameResult.IN_PLAY);
        }

        throw new IllegalArgumentException();
    }

    public GameState resign() {
        return createNewGameState(
                armyNotOnTurn(),
                armyOnTurn(),
                GameResult.VICTORY);
    }

    public GameState draw() {
        return createNewGameState(
                armyOnTurn(),
                armyNotOnTurn(),
                GameResult.DRAW);
    }

    private GameState createNewGameState(Army armyOnTurn, Army armyNotOnTurn, GameResult result) {
        if (armyOnTurn.side() == PlayingSide.BLUE) {
            return new GameState(board, armyOnTurn, armyNotOnTurn, PlayingSide.BLUE, result);
        }

        return new GameState(board, armyNotOnTurn, armyOnTurn, PlayingSide.ORANGE, result);
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.printf("{" + "\"result\":");
        result.toJSON(writer);
        writer.printf("," + "\"board\":");
        board.toJSON(writer);
        writer.printf("," + "\"blueArmy\":");
        blueArmy.toJSON(writer);
        writer.printf("," + "\"orangeArmy\":");
        orangeArmy.toJSON(writer);
        writer.printf("}");
    }
}
