package thedrake;

import java.io.PrintWriter;

public enum PlayingSide implements JSONSerializable {
    ORANGE,
    BLUE;

    @Override
    public String toString() {
        if(this.equals(ORANGE))
            return "ORANGE";
        return "BLUE";
    }

    @Override
    public void toJSON(PrintWriter writer) {
        switch (this) {
            case ORANGE:
                writer.printf("\"ORANGE\"");
                break;
            case BLUE:
                writer.printf("\"BLUE\"");
                break;
        }
    }
}
