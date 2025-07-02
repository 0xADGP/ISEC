package pt.isec.pa.chess.model.data.pieces;

import pt.isec.pa.chess.model.data.Board;

import java.util.HashSet;
import java.util.Set;

public class Knight extends Piece {
    public Knight(PieceColor PieceColor, Board board, char cColumn, int iRow) {
        super('N', board, PieceColor, cColumn, iRow);
    }

    public Set<Position> getPossibleMoves() {
        Set<Position> possibleMoves = new HashSet<>();

        int[][] moves = {{-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {2, 1}, {2, -1}};

        for (int[] move : moves) {
            char cCol = (char) (pPos.getColumn() + move[0]);
            int iRow = pPos.getRow() + move[1];

            if (cCol >= 'a' && cCol <= 'h' && iRow >= 1 && iRow <= 8) {
                Position newPos = new Position(cCol, iRow);
                Piece pieceAtNewPos = board.getPiece(newPos);
                if (pieceAtNewPos != null) {
                    if (pieceAtNewPos.getColor() == this.PieceColor) {
                        continue;
                    }
                }
                possibleMoves.add(newPos);
            }
        }

        return possibleMoves;
    }
}