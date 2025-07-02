package pt.isec.pa.chess.model.data.pieces;

import pt.isec.pa.chess.model.data.Board;

import java.util.HashSet;
import java.util.Set;

public class Pawn extends Piece {
    private boolean bHasMoved;

    public Pawn(PieceColor PieceColor, Board board, char cColumn, int iRow) {
        super('P', board, PieceColor, cColumn, iRow);
        this.bHasMoved = (PieceColor == PieceColor.WHITE && iRow == 2) || (PieceColor == PieceColor.BLACK && iRow == 7) ? false : true;
    }

    public Set<Position> getPossibleMoves() {
        Set<Position> possibleMoves = new HashSet<>();

        int direction = (PieceColor == PieceColor.WHITE) ? 1 : -1;
        char col = pPos.getColumn();
        int row = pPos.getRow();

        if ((row + direction) <= 8 && (row + direction) >= 1) {
            Position oneStep = new Position(col, row + direction);
            if (isPossibleMove(oneStep) && board.getPiece(oneStep) == null) {
                possibleMoves.add(oneStep);

                if ((row + 2 * direction) <= 8 && (row + 2 * direction) >= 1) {
                    Position twoSteps = new Position(col, row + 2 * direction);
                    if (!bHasMoved && isPossibleMove(twoSteps) && board.getPiece(twoSteps) == null) {
                        possibleMoves.add(twoSteps);
                    }
                }

            }
        }

        char colLeft = (char) (col - 1);
        if (colLeft >= 'a' && colLeft <= 'h' && (row + direction) <= 8 && (row + direction) >= 1) {
            Position diagLeft = new Position(colLeft, row + direction);
            if (isPossibleMove(diagLeft)) {
                Piece targetLeft = board.getPiece(diagLeft);
                if (targetLeft != null && targetLeft.getColor() != this.PieceColor) {
                    possibleMoves.add(diagLeft);
                }
            }
        }

        char colRight = (char) (col + 1);
        if (colRight <= 'h' && colRight >= 'a' && (row + direction) <= 8 && (row + direction) >= 1) {
            Position diagRight = new Position(colRight, row + direction);
            if (isPossibleMove(diagRight)) {
                Piece targetRight = board.getPiece(diagRight);
                if (targetRight != null && targetRight.getColor() != this.PieceColor) {
                    possibleMoves.add(diagRight);
                }
            }
        }

        return possibleMoves;
    }

    public boolean hasMoved() {
        return bHasMoved;
    }

    public void setHasMoved(boolean hasMoved) {
        this.bHasMoved = hasMoved;
    }

    public boolean isPromotionPosition() {
        return (PieceColor == PieceColor.WHITE && pPos.getRow() == 8) || (PieceColor == PieceColor.BLACK && pPos.getRow() == 1);
    }
}