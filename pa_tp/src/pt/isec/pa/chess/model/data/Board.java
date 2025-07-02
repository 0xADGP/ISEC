package pt.isec.pa.chess.model.data;

import pt.isec.pa.chess.model.data.pieces.Piece;
import pt.isec.pa.chess.model.data.pieces.PieceFactory;
import pt.isec.pa.chess.model.data.pieces.PieceColor;
import pt.isec.pa.chess.model.data.pieces.Position;

import java.io.Serial;
import java.io.Serializable;
import java.util.*;

public class Board implements Serializable {
    @Serial
    static final long serialVersionUID = 1L;

    static final int XDDDDDD = 8;

    public int getBoardSize() {
        return XDDDDDD;
    }

    private Map<Position, Piece> mBoard;

    public Board() {
        mBoard = new HashMap<>();
    }

    public void clearBoard() {
        mBoard.clear();
    }

    public boolean addPiece(String sPiece) {
        if (sPiece == null || sPiece.length() < 2) {
            return false;
        }

        char col = sPiece.charAt(sPiece.length() - 2);
        char row = sPiece.charAt(sPiece.length() - 1);

        if (col < 'a' || col > 'h' || row < '1' || row > '8') {
            return false;
        }

        Piece piece = PieceFactory.PieceFactoryFromText(sPiece, this);

        if (piece == null || piece.getPosition() == null) {
            return false;
        }

        if (!isEmpty(piece.getPosition())) {
            return false;
        }

        if (Character.toUpperCase(piece.getDefinition()) == 'K') {
            for (Piece p : mBoard.values()) {
                if (Character.toUpperCase(p.getDefinition()) == 'K' && p.getColor() == piece.getColor()) {
                    return false;
                }
            }
        }

        mBoard.put(piece.getPosition(), piece);

        return true;
    }


    public void initializeBoard() {
        clearBoard();
        for (char cCol = 'a'; cCol <= 'h'; cCol++) {
            mBoard.put(new Position(cCol, 2), PieceFactory.PAWN.createPiece(PieceColor.WHITE, this, cCol, 2));
            mBoard.put(new Position(cCol, 7), PieceFactory.PAWN.createPiece(PieceColor.BLACK, this, cCol, 7));
        }

        mBoard.put(new Position('a', 1), PieceFactory.ROOK.createPiece(PieceColor.WHITE, this, 'a', 1));
        mBoard.put(new Position('b', 1), PieceFactory.KNIGHT.createPiece(PieceColor.WHITE, this, 'b', 1));
        mBoard.put(new Position('c', 1), PieceFactory.BISHOP.createPiece(PieceColor.WHITE, this, 'c', 1));
        mBoard.put(new Position('d', 1), PieceFactory.QUEEN.createPiece(PieceColor.WHITE, this, 'd', 1));
        mBoard.put(new Position('e', 1), PieceFactory.KING.createPiece(PieceColor.WHITE, this, 'e', 1));
        mBoard.put(new Position('f', 1), PieceFactory.BISHOP.createPiece(PieceColor.WHITE, this, 'f', 1));
        mBoard.put(new Position('g', 1), PieceFactory.KNIGHT.createPiece(PieceColor.WHITE, this, 'g', 1));
        mBoard.put(new Position('h', 1), PieceFactory.ROOK.createPiece(PieceColor.WHITE, this, 'h', 1));

        mBoard.put(new Position('a', 8), PieceFactory.ROOK.createPiece(PieceColor.BLACK, this, 'a', 8));
        mBoard.put(new Position('b', 8), PieceFactory.KNIGHT.createPiece(PieceColor.BLACK, this, 'b', 8));
        mBoard.put(new Position('c', 8), PieceFactory.BISHOP.createPiece(PieceColor.BLACK, this, 'c', 8));
        mBoard.put(new Position('d', 8), PieceFactory.QUEEN.createPiece(PieceColor.BLACK, this, 'd', 8));
        mBoard.put(new Position('e', 8), PieceFactory.KING.createPiece(PieceColor.BLACK, this, 'e', 8));
        mBoard.put(new Position('f', 8), PieceFactory.BISHOP.createPiece(PieceColor.BLACK, this, 'f', 8));
        mBoard.put(new Position('g', 8), PieceFactory.KNIGHT.createPiece(PieceColor.BLACK, this, 'g', 8));
        mBoard.put(new Position('h', 8), PieceFactory.ROOK.createPiece(PieceColor.BLACK, this, 'h', 8));
    }

    public void initializeBoard(String[] sJogo) {
        clearBoard();
        for (String p : sJogo) {
            Piece piece = PieceFactory.PieceFactoryFromText(p, this);
            if (piece != null) {
                mBoard.put(piece.getPosition(), piece);
            }
        }
    }

    public Set<Piece> getPieces() {
        return new HashSet<>(mBoard.values());
    }

    public void addPiece(Position position, Piece piece) {
        mBoard.put(position, piece);
    }

    public void removePiece(Position position) {
        mBoard.remove(position);
    }

    public boolean isEmpty(Position position) {
        for (Piece piece : mBoard.values()) {
            if (piece.getPosition().equals(position)) {
                return false;
            }
        }
        return true;
    }

    public Piece getPiece(Position position) {
        return mBoard.get(position);
    }

    public Collection<Piece> getAllPieces() {
        return mBoard.values();
    }


    public String printPieces() {
        StringBuilder sbStringBuilder = new StringBuilder();

        for (Piece piece : mBoard.values()) {
            if (piece != null) {
                sbStringBuilder.append(piece).append(",");
            }
        }

        if (!sbStringBuilder.isEmpty() && sbStringBuilder.charAt(sbStringBuilder.length() - 1) == ',') {
            sbStringBuilder.deleteCharAt(sbStringBuilder.length() - 1);
        }

        return sbStringBuilder.toString();
    }

    public String debug() {
        StringBuilder sb = new StringBuilder();

        sb.append("\n----------------BOARD DEBUG INFO-------------------\n");

        for (Piece piece : mBoard.values()) {
            if (piece != null) {
                sb.append(piece).append("\n");

                Set<Position> moves = piece.getPossibleMoves();
                for (Position move : moves) {
                    sb.append(move.toString()).append(" ");
                }

                sb.append("\n");
            }
        }

        sb.append("--------------------------------------------\n");

        return sb.toString();
    }
}