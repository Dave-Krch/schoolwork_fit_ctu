package thedrake;

import java.io.PrintWriter;
import java.io.Serializable;

public class Board implements JSONSerializable {

    private final int dimension;

    private final BoardTile[][] board;

    // Konstruktor. Vytvoří čtvercovou hrací desku zadaného rozměru, kde všechny dlaždice jsou prázdné, tedy BoardTile.EMPTY
    public Board(int dimension) {
        this.dimension = dimension;

        board = new BoardTile[dimension][dimension];

        for(int i = 0; i < dimension; i++) {
            for(int j = 0; j < dimension; j++) {
                this.board[i][j] = BoardTile.EMPTY;
            }
        }
    }

    // Rozměr hrací desky
    public int dimension() {
        return dimension;
    }

    // Vrací dlaždici na zvolené pozici.
    public BoardTile at(TilePos pos) {
        return board[pos.i()][pos.j()];
    }

    // Vytváří novou hrací desku s novými dlaždicemi. Všechny ostatní dlaždice zůstávají stejné
    public Board withTiles(TileAt... ats) {
        Board out = new Board(dimension);

        //naklonuje board
        for(int i = 0; i < dimension; i++) {
            out.board[i] = this.board[i].clone();
        }

        //vytvori novy
        for (TileAt at : ats) {
            out.board[at.pos.i()][at.pos.j()] = at.tile;
        }

        return out;
    }

    // Vytvoří instanci PositionFactory pro výrobu pozic na tomto hracím plánu
    public PositionFactory positionFactory() {
        return new PositionFactory(dimension);
    }

    public static class TileAt {
        public final BoardPos pos;
        public final BoardTile tile;

        public TileAt(BoardPos pos, BoardTile tile) {
            this.pos = pos;
            this.tile = tile;
        }
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.printf("{" + "\"dimension\":" + dimension + ",\"tiles\":[");

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                this.board[j][i].toJSON(writer);
                if((i == dimension - 1) && ( j == dimension - 1)){
                    break;
                }
                writer.printf(",");
            }
        }

        writer.printf("]" + "}");
    }
}

