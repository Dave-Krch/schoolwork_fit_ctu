package thedrake;

import java.io.PrintWriter;
import java.util.*;

public class BoardTroops implements JSONSerializable {
    private final PlayingSide playingSide;
    private final Map<BoardPos, TroopTile> troopMap;
    private final TilePos leaderPosition;
    private final int guards;

    public BoardTroops(PlayingSide playingSide) {
        this.playingSide = playingSide;

        troopMap = Collections.emptyMap();

        leaderPosition = TilePos.OFF_BOARD;

        guards = 0;
    }

    public BoardTroops(
            PlayingSide playingSide,
            Map<BoardPos, TroopTile> troopMap,
            TilePos leaderPosition,
            int guards) {
        this.playingSide = playingSide;
        this.troopMap = new HashMap<>(troopMap);
        this.leaderPosition = leaderPosition;
        this.guards = guards;
    }

    public Optional<TroopTile> at(TilePos pos) {
        if (pos == null) {
            return Optional.empty();
        }

        TroopTile troopTile = troopMap.get(pos);
        return Optional.ofNullable(troopTile);
    }

    public PlayingSide playingSide() {
        return playingSide;
    }

    public TilePos leaderPosition() {
        return leaderPosition;
    }

    public int guards() {
        return guards;
    }

    public boolean isLeaderPlaced() {
        return leaderPosition != TilePos.OFF_BOARD;
    }

    public boolean isPlacingGuards() {
        return (isLeaderPlaced() && guards < 2);
    }

    public Set<BoardPos> troopPositions() {
        return troopMap.keySet();
    }

    public BoardTroops placeTroop(Troop troop, BoardPos target) {

        if(troopMap.containsKey(target))
            throw new IllegalArgumentException("Troop already on given tile");

        TilePos leader_out = this.leaderPosition();
        int guards_out = this.guards();

        if(!isLeaderPlaced())
            leader_out = target;

        if(isPlacingGuards())
            guards_out ++;

        BoardTroops out = new BoardTroops(this.playingSide, this.troopMap, leader_out, guards_out);

        out.troopMap.put(target, new TroopTile(troop, this.playingSide, TroopFace.AVERS));

        return out;
    }

    public BoardTroops troopStep(BoardPos origin, BoardPos target) {
        if(isPlacingGuards() || !isLeaderPlaced())
            throw new IllegalStateException("Player in placing guards or leader phase");
        if(!troopMap.containsKey(origin) || troopMap.containsKey(target))
            throw new IllegalArgumentException("Empty origin or taken target");

        TilePos leader_out = this.leaderPosition();

        if(leader_out.equals(origin))
            leader_out = target;

        BoardTroops out = new BoardTroops(this.playingSide, this.troopMap, leader_out, this.guards);

        out.troopMap.put(target, troopMap.get(origin).flipped());
        out.troopMap.remove(origin);

        return out;
    }

    public BoardTroops troopFlip(BoardPos origin) {
        if (!isLeaderPlaced()) {
            throw new IllegalStateException(
                    "Cannot move troops before the leader is placed.");
        }

        if (isPlacingGuards()) {
            throw new IllegalStateException(
                    "Cannot move troops before guards are placed.");
        }

        if (!at(origin).isPresent())
            throw new IllegalArgumentException();

        Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);
        TroopTile tile = newTroops.remove(origin);
        newTroops.put(origin, tile.flipped());

        return new BoardTroops(playingSide(), newTroops, leaderPosition, guards);
    }

    public BoardTroops removeTroop(BoardPos target) {
        if(isPlacingGuards() || !isLeaderPlaced())
            throw new IllegalStateException("Player in placing guards or leader phase");
        if(!troopMap.containsKey(target))
            throw new IllegalArgumentException("Empty target tile");

        TilePos leader_out = this.leaderPosition();

        if (leader_out.equalsTo(target.i(), target.j()))
            leader_out = TilePos.OFF_BOARD;

        BoardTroops out = new BoardTroops(this.playingSide, this.troopMap, leader_out, this.guards);

        out.troopMap.remove(target);

        return out;
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.printf("{" + "\"side\":");
        playingSide.toJSON(writer);
        writer.printf("," + "\"leaderPosition\":");
        leaderPosition.toJSON(writer);
        writer.printf("," + "\"guards\":" + guards);
        writer.printf("," + "\"troopMap\":{");

        List<Map.Entry<BoardPos, TroopTile>> sortedEntries = new ArrayList<>(troopMap.entrySet());
        sortedEntries.sort(Comparator.comparing(entry -> entry.getKey().toString()));

        for(Map.Entry<BoardPos, TroopTile> entry : sortedEntries) {
            writer.printf("\"" + entry.getKey().toString() + "\":");
            entry.getValue().toJSON(writer);
            if (entry != sortedEntries.get(sortedEntries.size() - 1))
                writer.printf(",");
        }

        writer.printf("}}");
    }
}
