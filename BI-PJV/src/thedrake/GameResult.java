package thedrake;

import java.io.PrintWriter;

public enum GameResult implements JSONSerializable {
    VICTORY, DRAW, IN_PLAY;

    @Override
    public void toJSON(PrintWriter writer) {
        switch (this) {
            case VICTORY:
                writer.printf("\"VICTORY\"");
                break;
            case DRAW:
                writer.printf("\"DRAW\"");
                break;
            case IN_PLAY:
                writer.printf("\"IN_PLAY\"");
                break;
        }
    }
}
