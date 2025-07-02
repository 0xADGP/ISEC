package pt.isec.pa.chess.model.data.pieces;

import java.io.Serializable;
import java.util.Objects;

public class Position implements Serializable {
    private char cColumn;
    private int iRow;

    public Position(String pos) {
        if (pos.length() != 2) {
            throw new IllegalArgumentException("Invalid position: " + pos);
        }
        char col = pos.charAt(0);
        char rowChar = pos.charAt(1);
        if (col < 'a' || col > 'h' || !Character.isDigit(rowChar) || rowChar < '1' || rowChar > '8') {
            throw new IllegalArgumentException("Invalid position: " + pos);
        }
        this.cColumn = col;
        this.iRow = Character.getNumericValue(rowChar);
    }

    public Position(char cColumn, int iRow) {
        if (cColumn < 'a' || cColumn > 'h' || iRow < 1 || iRow > 8) {
            throw new IllegalArgumentException("Invalid position: " + cColumn + iRow);
        }
        this.cColumn = cColumn;
        this.iRow = iRow;
    }

    public void setColumn(char cColumn) {
        if (cColumn < 'a' || cColumn > 'h') {
            throw new IllegalArgumentException("Invalid column: " + cColumn);
        }
        this.cColumn = cColumn;
    }

    public void setRow(int iRow) {
        if (iRow < 1 || iRow > 8) {
            throw new IllegalArgumentException("Invalid row: " + iRow);
        }
        this.iRow = iRow;
    }

    public char getColumn() {
        return cColumn;
    }

    public int getRow() {
        return iRow;
    }

    @Override
    public String toString() {
        return "(" + cColumn + "," + iRow + ")";
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        Position position = (Position) o;
        return cColumn == position.cColumn && iRow == position.iRow;
    }

    @Override
    public int hashCode() {
        return Objects.hash(cColumn, iRow);
    }
}