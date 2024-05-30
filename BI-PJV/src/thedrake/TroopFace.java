package thedrake;

import java.io.PrintWriter;

public enum TroopFace implements JSONSerializable {
    AVERS,
    REVERS;

    @Override
    public void toJSON(PrintWriter writer) {
        switch (this) {
            case AVERS:
                writer.printf("\"AVERS\"");
                break;
            case REVERS:
                writer.printf("\"REVERS\"");
                break;
        }
    }
}
