package pt.isec.pa.chess.model.data.pieces;

import pt.isec.pa.chess.model.data.Board;


import java.util.HashSet;
import java.util.Set;

public class Bishop extends Piece {
    public Bishop(PieceColor PieceColor, Board board, char cColumn, int iRow) {
        super('B', board, PieceColor, cColumn, iRow);
    }

    public Set<Position> getPossibleMoves() {
        Set<Position> possibleMoves = new HashSet<>();

        int[][] moves = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

        return getLinearMoves(moves);
    }
}