package pt.isec.pa.chess.model.data.pieces;

import pt.isec.pa.chess.model.data.Board;

import java.util.HashSet;
import java.util.Set;

public class King extends Piece {
    private boolean bHasMoved;

    public King(PieceColor PieceColor, Board board, char cColumn, int iRow) {
        super('K', board, PieceColor, cColumn, iRow);
        this.bHasMoved = (PieceColor == PieceColor.WHITE && iRow == 1 && cColumn == 'e') || (PieceColor == PieceColor.BLACK && iRow == 8 && cColumn == 'e') ? false : true;
    }

    @Override
    public Set<Position> getPossibleMoves() {
        Set<Position> possibleMoves = new HashSet<>();

        int[][] moves = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

        for (int[] move : moves) {
            char cCol = (char) (pPos.getColumn() + move[0]);
            int iRow = pPos.getRow() + move[1];

            if (cCol >= 'a' && cCol <= 'h' && iRow >= 1 && iRow <= 8) {
                Position newPos = new Position(cCol, iRow);
                Piece pieceAtNewPos = board.getPiece(newPos);

                if (pieceAtNewPos == null || pieceAtNewPos.getColor() != this.PieceColor) {
                    possibleMoves.add(newPos);
                }
            }
        }

        if (!bHasMoved) {
            possibleMoves.add(new Position((char) (pPos.getColumn() + 2), pPos.getRow()));
            possibleMoves.add(new Position((char) (pPos.getColumn() - 2), pPos.getRow()));
        }


        return possibleMoves;
    }

    @Override
    public boolean hasMoved() {
        return bHasMoved;
    }

    @Override
    public void setHasMoved(boolean hasMoved) {
        this.bHasMoved = hasMoved;
    }

    @Override
    public String toString() {
        return super.toString() + (bHasMoved ? "" : "*");
    }
}