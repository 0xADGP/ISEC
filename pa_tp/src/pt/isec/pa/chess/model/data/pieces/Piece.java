package pt.isec.pa.chess.model.data.pieces;

import pt.isec.pa.chess.model.data.Board;

import java.io.Serializable;
import java.util.HashSet;
import java.util.Set;

abstract public class Piece implements Serializable {
    static final long serialVersionUID = 1L;
    protected char cDefinition;
    protected PieceColor PieceColor;
    protected Position pPos;
    protected Board board;
    protected boolean bHasMoved;

    Piece(char cDefinition, Board board, PieceColor PieceColor, char cColumn, int iRow) {
        this.cDefinition = (PieceColor == PieceColor.WHITE) ? Character.toUpperCase(cDefinition) : Character.toLowerCase(cDefinition);
        this.PieceColor = PieceColor;
        this.pPos = new Position(cColumn, iRow);
        this.board = board;
        this.bHasMoved = false;
    }

    abstract public Set<Position> getPossibleMoves();

    public void setPosition(Position pos) {
        this.pPos = pos;
    }

    public char getDefinition() {
        return cDefinition;
    }

    public PieceColor getColor() {
        return PieceColor;
    }

    public Position getPosition() {
        return pPos;
    }

    public void setBoard(Board board) {
        this.board = board;
    }

    @Override
    public String toString() {
        return this.getDefinition() + String.valueOf(pPos.getColumn()) + pPos.getRow();
    }

    protected boolean isPossibleMove(Position pos) {
        return pos.getColumn() >= 'a' && pos.getColumn() <= 'h' && pos.getRow() >= 1 && pos.getRow() <= 8;
    }

    protected Set<Position> getLinearMoves(int [][]moves) {
        Set<Position> possibleMoves = new HashSet<>();

        for (int[] move : moves) {
            for (int i = 1; i <= 8; i++) {
                char cCol = (char) (pPos.getColumn() + move[0] * i);
                int iRow = pPos.getRow() + move[1] * i;

                if (cCol < 'a' || cCol > 'h' || iRow < 1 || iRow > 8) {
                    break;
                }

                Position newPos = new Position(cCol, iRow);
                Piece pieceAtNewPos = board.getPiece(newPos);

                if (pieceAtNewPos != null) {
                    if (pieceAtNewPos.getColor() == this.PieceColor) {
                        break;
                    } else {
                        possibleMoves.add(newPos);
                        break;
                    }
                }

                possibleMoves.add(newPos);
            }
        }
        return possibleMoves;
    }

    public void setHasMoved(boolean hasMoved) {
        this.bHasMoved = hasMoved;
    }

    public boolean hasMoved() {
        return this.bHasMoved;
    }

}