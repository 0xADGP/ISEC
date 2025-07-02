package pt.isec.pa.chess.model.data.pieces;

import pt.isec.pa.chess.model.data.Board;

public enum PieceFactory {
    KING((color, board, column, row) -> new King(color, board, column, row)),
    QUEEN((color, board, column, row) -> new Queen(color, board, column, row)),
    BISHOP((color, board, column, row) -> new Bishop(color, board, column, row)),
    KNIGHT((color, board, column, row) -> new Knight(color, board, column, row)),
    ROOK((color, board, column, row) -> new Rook(color, board, column, row)),
    PAWN((color, board, column, row) -> new Pawn(color, board, column, row));

    private final PieceCreator creator;

    PieceFactory(PieceCreator creator) {
        this.creator = creator;
    }

    public Piece createPiece(PieceColor color, Board board, char column, int row) {
        if (column < 'a' || column > 'h' || row < 1 || row > 8)
            return null;
        return creator.create(color, board, column, row);
    }

    @FunctionalInterface
    private interface PieceCreator {
        Piece create(PieceColor color, Board board, char column, int row);
    }


    //segundo metodo
    public static Piece PieceFactoryFromText(String textRepresentation, Board board) {
        if (textRepresentation == null || textRepresentation.isEmpty()) {
            return null;
        }

        boolean hasNotMoved = false;
        String pieceData = textRepresentation;

        if (textRepresentation.endsWith("*")) {
            hasNotMoved = true;
            pieceData = textRepresentation.substring(0, textRepresentation.length() - 1);
        }

        char pieceType = pieceData.charAt(0);
        char column = pieceData.charAt(1);
        int row;

        try {
            row = Integer.parseInt(pieceData.substring(2));
        } catch (NumberFormatException e) {
            return null;
        }

        //ve cor peça
        PieceColor ePieceColor = Character.isUpperCase(pieceType) ? PieceColor.WHITE : PieceColor.BLACK;

        char normalizedType = Character.toUpperCase(pieceType);

        Piece piece;

        // Cria peça tendo em conta o tipo
        switch (normalizedType) {
            case 'K':
                piece = KING.createPiece(ePieceColor, board, column, row);
                if (piece != null && !hasNotMoved) {
                    ((King) piece).setHasMoved(true); //hasmoved=false por default
                }
                break;
            case 'Q':
                piece = QUEEN.createPiece(ePieceColor, board, column, row);
                break;
            case 'R':
                piece = ROOK.createPiece(ePieceColor, board, column, row);
                if (piece != null && !hasNotMoved) {
                    ((Rook) piece).setHasMoved(true); //hasmoved=false por default
                }
                break;
            case 'B':
                piece = BISHOP.createPiece(ePieceColor, board, column, row);
                break;
            case 'N':
                piece = KNIGHT.createPiece(ePieceColor, board, column, row);
                break;
            case 'P':
                piece = PAWN.createPiece(ePieceColor, board, column, row);
                break;
            default:
                return null;
        }

        return piece;
    }
}