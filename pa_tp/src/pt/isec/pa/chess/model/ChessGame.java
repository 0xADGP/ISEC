package pt.isec.pa.chess.model;

import pt.isec.pa.chess.model.data.Board;
import pt.isec.pa.chess.model.data.pieces.*;
import pt.isec.pa.chess.model.memento.IMemento;
import pt.isec.pa.chess.model.memento.IOriginator;


import java.io.*;
import java.util.*;

/**
 * Core logic of the chess game. Maintains the board state, current player, game rules, checkmate detection, and player management.
 * Handles moves, pawn promotion, game loading/saving, and turn control.
 * <p>
 * Implements {@link Serializable} for saving/loading full game state and {@link IOriginator} for memento pattern support.
 *
 * @author Antonio Pedorso, Luis Duarte, Carolina Veloso
 * @version 1.0.0
 * @see Board
 * @see Piece
 * @see PieceColor
 * @see PieceFactory
 * @see ModelLog
 */

public class ChessGame implements Serializable, IOriginator {
    @Serial
    static final long serialVersionUID = 1L;

    Piece lastMovedPiece;
    Position lastMoveFrom;
    Position lastMoveTo;
    Board board;
    PieceColor cCurrentPlayer = PieceColor.WHITE;
    PieceColor cWinner = null;
    String sPlayer1 = null, sPlayer2 = null;

    /**
     * Private implementation of a memento object that captures a serialized snapshot of the ChessGame.
     */
    /**
     * Internal class implementing the Memento design pattern to capture and restore the state of a ChessGame.
     * Stores a serialized snapshot of the game and lightweight metadata (board state and current player).
     */
    private static class MyMemento implements IMemento, Serializable {
        private final String boardState;
        private final PieceColor currentPlayer;
        private final byte[] snapshot;

        /**
         * Creates a new memento based on the current state of the ChessGame.
         * Serializes the full object to allow complete restoration later.
         *
         * @param base the ChessGame instance to capture
         */
        MyMemento(ChessGame base) {
            this.boardState = base.getBoardPiecesString();
            this.currentPlayer = base.getcCurrentPlayer();
            try (ByteArrayOutputStream baos = new ByteArrayOutputStream(); ObjectOutputStream oos = new ObjectOutputStream(baos)) {
                oos.writeObject(base);
                this.snapshot = baos.toByteArray();
            } catch (IOException e) {
                throw new RuntimeException("Failed to create memento snapshot", e);
            }
        }

        /**
         * Gets a textual representation of the board state at the time of capture.
         *
         * @return a string representing the board state
         */
        public String getBoardState() {
            return boardState;
        }

        /**
         * Gets the current player at the time of capture.
         *
         * @return the {@link PieceColor} of the player who was to move
         */
        public PieceColor getCurrentPlayer() {
            return currentPlayer;
        }

        /**
         * Returns a deep copy of the ChessGame as it was when the memento was created.
         * Uses object deserialization to reconstruct the full state.
         *
         * @return a reconstructed ChessGame object, or null if deserialization fails
         */
        @Override
        public Object getSnapshot() {
            if (snapshot == null) return null;
            try (ByteArrayInputStream bais = new ByteArrayInputStream(snapshot); ObjectInputStream ois = new ObjectInputStream(bais)) {
                return ois.readObject();
            } catch (Exception e) {
                return null;
            }
        }
    }

    /**
     * Saves the current game state as a memento for undo functionality.
     *
     * @return an {@link IMemento} object containing the game state snapshot
     */
    @Override
    public IMemento save() {
        return new MyMemento(this);
    }

    /**
     * Restores the game state from a given memento.
     *
     * @param memento the {@link IMemento} object containing the game state to restore
     */
    @Override
    public void restore(IMemento memento) {
        if (memento instanceof MyMemento m) {
            loadBoardStateFromString(m.getBoardState(), m.getCurrentPlayer());
            cWinner = null;
            lastMovedPiece = null;
            lastMoveFrom = null;
            lastMoveTo = null;
            for (Piece piece : board.getPieces()) {
                piece.setBoard(board);
            }
        }
    }

    /**
     * Constructs a new ChessGame instance with a standard 8x8 chessboard.
     * Initializes the board with the standard chess setup and sets the current player to white.
     */
    public ChessGame() {
        board = new Board();
        board.clearBoard();
        board.initializeBoard();

    }

    /**
     * Add piece.
     */
    public boolean addPiece(String sPiece) {
        return board.addPiece(sPiece);
    }

    /**
     * Returns the size of the chessboard (number of rows or columns).
     *
     * @return the size of the board (e.g., 8 for a standard chessboard)
     */
    public int getBoardSize() {
        return board.getBoardSize();
    }

    /**
     * Sets the name of the specified player.
     *
     * @param player the player number (1 for white, 2 for black)
     * @param name   the name to assign to the player
     * @throws RuntimeException if the player number is invalid (not 1 or 2)
     */
    public void setPlayerName(int player, String name) {
        if (player < 1 || player > 2) {
            throw new RuntimeException("tried to set invalid player");
            //System.out.println("Invalid player number");
            //return;
        }
        if (player == 1) {
            sPlayer1 = name;
        } else {
            sPlayer2 = name;
        }
    }

    /**
     * Returns the name of the player associated with the specified color.
     *
     * @param eColor the color of the player ({@link PieceColor#WHITE} or {@link PieceColor#BLACK})
     * @return the name of the player, or null if not set
     */
    public String getPlayerName(PieceColor eColor) {
        if (eColor == PieceColor.WHITE) {
            return sPlayer1;
        } else {
            return sPlayer2;
        }
    }

    /**
     * Provides a debug representation of the game state, including the board
     * and current player information.
     *
     * @return a string containing debug information about the game state
     */
    public String debug() {
        //
        StringBuilder sb = new StringBuilder();
        sb.append("\n--------------GAME DEBUG INFO------------------\n");
        sb.append("Current player: " + cCurrentPlayer);
        sb.append("\n");
        sb.append(board.debug());
        sb.append("\n");


        sb.append("Winner: " + cWinner);
        sb.append("\n");
        return sb.toString();
    }

    /**
     * Starts a new game by resetting the board to the initial chess setup.
     */
    public void start() {
        board.clearBoard();
        board.initializeBoard();
    }

    /**
     * Returns the current board pieces as a string representation.
     *
     * @return a string representing the pieces on the board
     */
    public String getBoardPiecesString() {
        return board.printPieces();
    }

    /**
     * Gets the color of the current player.
     *
     * @return the current player's color ({@link PieceColor#WHITE} or {@link PieceColor#BLACK})
     */
    public PieceColor getcCurrentPlayer() {
        return cCurrentPlayer;
    }

    /**
     * Returns the opposing player's color.
     *
     * @return the opponent's color ({@link PieceColor#WHITE} or {@link PieceColor#BLACK})
     */
    public PieceColor getcOpponent() {
        return cCurrentPlayer == PieceColor.WHITE ? PieceColor.BLACK : PieceColor.WHITE;
    }

    /**
     * Gets the valid moves for a piece at the specified position.
     * Returns an empty set if the position is invalid, empty, or the piece
     * does not belong to the current player.
     *
     * @param pos the position of the piece (e.g., "e2")
     * @return a set of valid move positions as strings (e.g., {"e3", "e4"})
     * @throws IllegalArgumentException if the position is null
     */
    public Set<String> getPossibleMovesFromPieceAt(String pos) {
        if (pos == null) throw new IllegalArgumentException("Position can't be null");

        Position p = new Position(pos);
        Piece piece = board.getPiece(p);
        if (piece == null || piece.getColor() != cCurrentPlayer) return new HashSet<>();

        Set<String> result = new HashSet<>();
        Position originalPos = piece.getPosition();

        for (Position move : piece.getPossibleMoves()) {

            // movimentos castling King
            boolean isCastling = piece instanceof King && Math.abs(move.getColumn() - originalPos.getColumn()) == 2;

            if (isCastling) {
                if (!isValidCastling(piece, originalPos, move)) {
                    continue;
                }
            }


            Piece captured = board.getPiece(move);
            Piece backup = captured;

            // Simulate move
            board.removePiece(originalPos);
            if (captured != null) board.removePiece(move);

            piece.setPosition(move);
            board.addPiece(move, piece);


            if (!isCastling && !isInCheck(cCurrentPlayer)) {
                result.add("" + move.getColumn() + move.getRow());
            } else if (isCastling) {
                result.add("" + move.getColumn() + move.getRow());
            }

            // Undo
            board.removePiece(move);
            piece.setPosition(originalPos);
            board.addPiece(originalPos, piece);
            if (backup != null) board.addPiece(move, backup);
        }

        // EN PASSANT
        if (piece instanceof Pawn && lastMovedPiece instanceof Pawn && lastMoveFrom != null && lastMoveTo != null) {
            int dir = piece.getColor() == PieceColor.WHITE ? 1 : -1;

            if (Math.abs(lastMoveTo.getRow() - lastMoveFrom.getRow()) == 2 && lastMovedPiece.getColor() != piece.getColor() && lastMoveTo.getRow() == piece.getPosition().getRow() && Math.abs(lastMoveTo.getColumn() - piece.getPosition().getColumn()) == 1) {

                Position enPassantTarget = new Position(lastMoveTo.getColumn(), lastMoveTo.getRow() + dir);

                board.removePiece(originalPos);
                board.removePiece(lastMoveTo);
                piece.setPosition(enPassantTarget);
                board.addPiece(enPassantTarget, piece);

                if (!isInCheck(cCurrentPlayer)) {
                    result.add("" + enPassantTarget.getColumn() + enPassantTarget.getRow());
                }

                board.removePiece(enPassantTarget);
                piece.setPosition(originalPos);
                board.addPiece(originalPos, piece);
                board.addPiece(lastMoveTo, lastMovedPiece);
            }
        }


        return result;
    }


    /**
     * Checks whether a castling move is valid for the given king.
     * Returns false if the king or rook has already moved, if there are pieces
     * between them, or if the king is in check, passes through check, or lands in check.
     *
     * @param king the king piece attempting to castle
     * @param from the current position of the king
     * @param to   the target castling position of the king
     * @return true if the castling move is valid, false otherwise
     */
    private boolean isValidCastling(Piece king, Position from, Position to) {
        if (!(king instanceof King) || king.hasMoved() || isInCheck(king.getColor())) {
            return false;
        }

        boolean isKingside = to.getColumn() > from.getColumn();
        char rookCol = isKingside ? 'h' : 'a';
        Position rookPos = new Position(rookCol, from.getRow());
        Piece rook = board.getPiece(rookPos);

        if (rook == null || !(rook instanceof Rook) || rook.getColor() != king.getColor() || rook.hasMoved()) {
            return false;
        }

        int colStep = isKingside ? 1 : -1;
        char startFreeCol = (char) (from.getColumn() + colStep);
        char endFreeCol = isKingside ? 'g' : 'b';
        for (char col = startFreeCol; col != endFreeCol + colStep; col += colStep) {
            if (!board.isEmpty(new Position(col, from.getRow()))) {
                return false;
            }
        }
        char kingEndCol = isKingside ? 'g' : 'c';
        for (char col = from.getColumn(); col != kingEndCol + (isKingside ? 1 : -1); col += colStep) {
            Position pos = new Position(col, from.getRow());
            for (Piece enemy : board.getAllPieces()) {
                if (enemy.getColor() != king.getColor() && enemy.getPossibleMoves().contains(pos)) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Returns the winner of the game, if determined.
     *
     * @return the color of the winning player ({@link PieceColor#WHITE} or {@link PieceColor#BLACK}), or null if the game is not over or ended in a draw
     */
    public PieceColor getWinner() {
        return cWinner;
    }

    /**
     * Checks if the game has ended due to checkmate, stalemate, or a missing king.
     *
     * @return true if the game is over, false otherwise
     */
    public boolean isGameOver() {
        if (board.getPieces().size() == 0) return false;
        boolean bWhiteKing = false;
        boolean bBlackKing = false;

        for (Piece p : board.getPieces()) {
            if (Character.toUpperCase(p.getDefinition()) == 'K') {
                if (p.getColor() == PieceColor.WHITE) {
                    bWhiteKing = true;
                } else if (p.getColor() == PieceColor.BLACK) {
                    bBlackKing = true;
                }
            }
        }

        if (!bWhiteKing || !bBlackKing) {
            return false;
        }

        if (!bWhiteKing) {
            cWinner = PieceColor.BLACK;
            return true;
        } else if (!bBlackKing) {
            cWinner = PieceColor.WHITE;
            return true;
        }

        // checkmate
        if (bIsCheckmate(PieceColor.WHITE)) {
            cWinner = PieceColor.BLACK;
            return true;
        } else if (bIsCheckmate(PieceColor.BLACK)) {
            cWinner = PieceColor.WHITE;
            return true;
        }

        // Check for stalemate
        if (!isInCheck(cCurrentPlayer) && !hasLegalMoves(cCurrentPlayer)) {
            cWinner = null;
            return true;
        }

        if (board.getPieces().size() == 2) {
            boolean onlyKings = true;
            for (Piece p : board.getPieces()) {
                if (Character.toUpperCase(p.getDefinition()) != 'K') {
                    onlyKings = false;
                    break;
                }
            }
            if (onlyKings) {
                cWinner = null;
                return true;
            }
        }

        return false;
    }

    /**
     * Checks whether the specified player has at least one legal move available.
     * Iterates through all pieces of the given color and evaluates their possible moves.
     *
     * @param color the color of the player to check ({@link PieceColor#WHITE} or {@link PieceColor#BLACK})
     * @return true if at least one legal move exists, false otherwise
     */
    private boolean hasLegalMoves(PieceColor color) {
        for (Piece piece : board.getAllPieces()) {
            if (piece.getColor() == color) {
                String pos = "" + piece.getPosition().getColumn() + piece.getPosition().getRow();
                if (!getPossibleMovesFromPieceAt(pos).isEmpty()) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Determines if the specified player's king is currently under threat.
     * This means at least one enemy piece has a legal move that could capture the king.
     *
     * @param color The color of the player to check (WHITE or BLACK)
     * @return true if the player's king is in check, false otherwise
     */
    private boolean isInCheck(PieceColor color) {
        Position kingPos = null;

        for (Piece piece : new ArrayList<>(board.getAllPieces())) {
            if (Character.toUpperCase(piece.getDefinition()) == 'K' && piece.getColor() == color) {
                kingPos = piece.getPosition();
                break;
            }
        }

        if (kingPos == null) return false; // king already captured

        for (Piece enemy : new ArrayList<>(board.getAllPieces())) {
            if (enemy.getColor() != color && enemy.getPossibleMoves().contains(kingPos)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Checks if a player is in checkmate.
     *
     * @param color Player's color
     * @return true if in checkmate
     */
    public boolean bIsCheckmate(PieceColor color) {
        if (!isInCheck(color)) {
            return false;
        }

        List<Piece> piecesCopy = new ArrayList<>(board.getAllPieces());

        for (Piece piece : piecesCopy) {
            if (piece.getColor() != color) continue;

            String pos = "" + piece.getPosition().getColumn() + piece.getPosition().getRow();
            Set<String> moves = getPossibleMovesFromPieceAt(pos);

            for (String moveStr : moves) {
                Position move = new Position(moveStr);
                Position originalPos = piece.getPosition();
                Piece captured = board.getPiece(move);
                Piece backup = captured;

                board.removePiece(originalPos);
                if (captured != null) board.removePiece(move);

                piece.setPosition(move);
                board.addPiece(move, piece);

                boolean isPromotion = willNeedPromotion(pos, moveStr);
                List<Piece> promotedPieces = new ArrayList<>();
                if (isPromotion) {
                    promotedPieces.add(PieceFactory.QUEEN.createPiece(piece.getColor(), board, move.getColumn(), move.getRow()));
                    promotedPieces.add(PieceFactory.ROOK.createPiece(piece.getColor(), board, move.getColumn(), move.getRow()));
                    promotedPieces.add(PieceFactory.BISHOP.createPiece(piece.getColor(), board, move.getColumn(), move.getRow()));
                    promotedPieces.add(PieceFactory.KNIGHT.createPiece(piece.getColor(), board, move.getColumn(), move.getRow()));
                }

                boolean stillInCheck;
                if (!isPromotion) {
                    stillInCheck = isInCheck(color);
                } else {
                    stillInCheck = true;
                    for (Piece promoted : promotedPieces) {
                        board.removePiece(move);
                        board.addPiece(move, promoted);
                        boolean check = isInCheck(color);
                        if (!check) {
                            stillInCheck = false;
                            break;
                        }
                    }
                }

                board.removePiece(move);
                piece.setPosition(originalPos);
                board.addPiece(originalPos, piece);
                if (backup != null) board.addPiece(move, backup);

                if (!stillInCheck) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * Promotes a pawn at the specified position to a new piece type.
     *
     * @param position  the position of the pawn to promote (e.g., "e8")
     * @param pieceType the type of piece to promote to ('Q', 'R', 'B', or 'N')
     * @throws IllegalArgumentException if the piece type is invalid or no pawn is at the position
     */
    public void promotePawn(String position, char pieceType) {
        //System.out.println("Promotion received: " + pieceType + " at " + position);

        char column = position.charAt(0);
        int row = Character.getNumericValue(position.charAt(1));

        if (column < 'a' || column > 'h' || row < 1 || row > 8) return;

        Position pos = new Position(column, row);

        Piece pawn = board.getPiece(pos);
        if (!(pawn instanceof Pawn)) {
            //System.out.println("No pawn found at promotion position.");
            return;
        }

        board.removePiece(pos);
        PieceColor color = pawn.getColor();

        char normalizedType = Character.toUpperCase(pieceType);
        Piece newPiece = switch (normalizedType) {
            case 'Q' -> PieceFactory.QUEEN.createPiece(color, board, column, row);
            case 'R' -> PieceFactory.ROOK.createPiece(color, board, column, row);
            case 'B' -> PieceFactory.BISHOP.createPiece(color, board, column, row);
            case 'N' -> PieceFactory.KNIGHT.createPiece(color, board, column, row);
            default -> throw new IllegalArgumentException("Invalid piece: " + pieceType);
        };

        if (newPiece != null) {
            board.addPiece(pos, newPiece);
            //System.out.println("Promotion done: " + newPiece.getClass().getSimpleName());
            ModelLog.getInstance().log("Promotion done: " + newPiece.getClass().getSimpleName());
        } else {
            //System.out.println("Promotion failed: piece not created.");
        }
        //System.out.println("Promotion done: " + newPiece.getClass().getSimpleName());
        //System.out.println("[DEBUG] board after promotion: " + board.printPieces());
    }


    /**
     * Checks if a move will result in a pawn promotion for the piece at the specified position.
     *
     * @param SoldPos the starting position of the piece (e.g., "e7")
     * @param SnewPos the destination position (e.g., "e8")
     * @return true if the move will require a pawn promotion, false otherwise
     */
    public boolean willNeedPromotion(String SoldPos, String SnewPos) {
        Position oldPos = new Position(SoldPos);
        Position newPos = new Position(SnewPos);
        Piece piece = board.getPiece(oldPos);

        if (!(piece instanceof Pawn)) return false;

        return (piece.getColor() == PieceColor.WHITE && newPos.getRow() == 8) || (piece.getColor() == PieceColor.BLACK && newPos.getRow() == 1);
    }


    /**
     * Moves a piece from one position to another, if the move is valid.
     * Handles special moves like castling and en passant.
     *
     * @param SoldPos the starting position of the piece (e.g., "e2")
     * @param SnewPos the destination position (e.g., "e4")
     * @return true if the move was successful, false if invalid or game is over
     */
    public boolean movePiece(String SoldPos, String SnewPos) {
        if (isGameOver()) return false;

        Position newPos = new Position(SnewPos);
        Position oldPos = new Position(SoldPos);

        Piece pMover = board.getPiece(oldPos);
        if (pMover == null || pMover.getColor() != cCurrentPlayer) return false;

        Set<String> legalMoves = getPossibleMovesFromPieceAt(SoldPos);
        if (!legalMoves.contains(SnewPos)) return false;

        ModelLog.getInstance().log(cCurrentPlayer + " moved piece to: " + newPos);
        boolean isCastling = false;

        // Castling
        Position rookOldPos = null, rookNewPos = null;
        if (pMover instanceof King && Math.abs(newPos.getColumn() - oldPos.getColumn()) == 2) {
            isCastling = true;
            if (newPos.getColumn() > oldPos.getColumn()) {
                rookOldPos = new Position('h', oldPos.getRow());
                rookNewPos = new Position('f', oldPos.getRow());
            } else {
                rookOldPos = new Position('a', oldPos.getRow());
                rookNewPos = new Position('d', oldPos.getRow());
            }
        }


        // Remove peça capturada
        Piece target = board.getPiece(newPos);
        if (target != null && target.getColor() != pMover.getColor()) {
            board.removePiece(newPos);
        }

        // EN PASSANT
        if (pMover instanceof Pawn && lastMovedPiece instanceof Pawn) {
            if (Math.abs(newPos.getColumn() - oldPos.getColumn()) == 1 && target == null) {
                if (pMover.getColor() == PieceColor.WHITE && oldPos.getRow() == 5 && newPos.getRow() == 6) {
                    Position capturedPos = new Position(newPos.getColumn(), 5);
                    Piece maybeCaptured = board.getPiece(capturedPos);
                    if (maybeCaptured == lastMovedPiece && maybeCaptured.getColor() != pMover.getColor()) {
                        board.removePiece(capturedPos);
                    }
                } else if (pMover.getColor() == PieceColor.BLACK && oldPos.getRow() == 4 && newPos.getRow() == 3) {
                    Position capturedPos = new Position(newPos.getColumn(), 4);
                    Piece maybeCaptured = board.getPiece(capturedPos);
                    if (maybeCaptured == lastMovedPiece && maybeCaptured.getColor() != pMover.getColor()) {
                        board.removePiece(capturedPos);
                    }
                }
            }
        }

        board.removePiece(oldPos);


        pMover.setPosition(newPos);
        board.addPiece(newPos, pMover);

        // Mover rook castling
        if (isCastling) {
            Piece rook = board.getPiece(rookOldPos);
            if (rook != null && rook instanceof Rook) {
                board.removePiece(rookOldPos);
                rook.setPosition(rookNewPos);
                board.addPiece(rookNewPos, rook);
                rook.setHasMoved(true);
            }
        }


        pMover.setHasMoved(true);
        cCurrentPlayer = getcOpponent();
        lastMovedPiece = pMover;
        lastMoveFrom = new Position(oldPos.getColumn(), oldPos.getRow());
        lastMoveTo = new Position(newPos.getColumn(), newPos.getRow());
        isGameOver();

        return true;

    }

    /**
     * Removes a piece from the specified position on the board.
     *
     * @param pos the position of the piece to remove (e.g., "e4")
     * @throws IllegalArgumentException if the position is null or invalid
     */
    public void removePiece(String pos) {
        // FIXME WILL WE USE THIS IN BOARD EDITOR? IF NOT JUST DELETE IT
        if (pos == null) throw new IllegalArgumentException("Position can't be null");

        char[] split = pos.toCharArray();

        if (split[0] < 'a' || split[0] > 'h' || (int) split[1] < 1 || (int) split[1] > 8) {
            throw new IllegalArgumentException("Invalid position: " + split[0] + (int) split[1]);
        }

        Position p = new Position(pos);
        board.removePiece(p);
    }

    /**
     * Empty game.
     */
    public void loadEmptyGame() {
        cCurrentPlayer = PieceColor.WHITE;
        cWinner = null;
        board = new Board();
        board.initializeBoard(new String[0]);
    }


    /**
     * Loads a board state from a string representation and sets the current player.
     *
     * @param state         the string representation of the board state
     * @param currentPlayer the color of the current player ({@link PieceColor#WHITE} or {@link PieceColor#BLACK})
     */
    public void loadBoardStateFromString(String state, PieceColor currentPlayer) {
        if (state == null || state.isEmpty()) {
            board = new Board();
            board.initializeBoard();
            cCurrentPlayer = PieceColor.WHITE;
            return;
        }

        state = state.replaceAll("\\R", "");
        state = state.replaceAll("\\s", "");
        state = state.trim();

        String[] tokens = state.split(",");
        if (tokens.length == 0) {
            board = new Board();
            board.initializeBoard();
            cCurrentPlayer = currentPlayer;
            return;
        }

        board = new Board();
        board.initializeBoard(tokens);
        cCurrentPlayer = currentPlayer;

        for (Piece piece : board.getPieces()) {
            piece.setBoard(board);
        }
    }

    /**
     * Imports a partial game state from a file, including the current player and piece positions.
     *
     * @param sFileName the name of the file to import
     * @throws IOException      if an I/O error occurs
     * @throws RuntimeException if the file format or content is invalid
     */
    public void importa(String sFileName) throws IOException {
        board.clearBoard();
        try (DataInputStream dis = new DataInputStream(new BufferedInputStream(new FileInputStream(sFileName)))) {
            byte[] data = dis.readAllBytes();

            String line = new String(data).replaceAll("\\R", "");
            line = line.replaceAll("\\s", "");
            line = line.trim();

            //System.out.println(line);

            if (line.isEmpty()) { //fich vazio
                return;
            }

            String[] tokens = line.split(",");
            if (tokens.length < 2) {//invalido
                return;
            }

            PieceColor Color;
            try {
                Color = PieceColor.valueOf(tokens[0].toUpperCase());
            } catch (IllegalArgumentException e) {
                throw new RuntimeException("color " + tokens[0] + " invalid");
            }

            for (int i = 1; i < tokens.length; i++) {
                String token = tokens[i].trim();

                if (!token.matches("(?i)[kqrbnp][a-h][1-8]\\*?")) {
                    throw new RuntimeException("piece " + token + " invalid");
                }
            }

            cCurrentPlayer = Color;
            board = new Board();

            board.initializeBoard(Arrays.copyOfRange(tokens, 1, tokens.length));

            for (Piece piece : board.getPieces()) {
                piece.setBoard(board);
            }
        }
    }

    /**
     * Exports the current game state to a file, including the current player and piece positions.
     *
     * @param sFileName the name of the file to export to
     * @throws IOException if an I/O error occurs
     */
    public void exporta(String sFileName) throws IOException {
        StringBuilder sb = new StringBuilder();
        sb.append(cCurrentPlayer).append(",");
        sb.append(getBoardPiecesString());

        try (DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(sFileName)))) {
            dos.write(sb.toString().getBytes());
        }
    }

    /**
     * Loads a full game state from a serialized file.
     *
     * @param sFileName the name of the file to load
     * @throws IOException            if an I/O error occurs
     * @throws ClassNotFoundException if the serialized object cannot be deserialized
     */
    public void open(String sFileName) throws IOException, ClassNotFoundException {

        ChessGame loaded = ChessGameSerialization.load(sFileName);

        if (loaded != null) {
            this.board = loaded.board;
            this.cCurrentPlayer = loaded.cCurrentPlayer;
            this.cWinner = loaded.cWinner;
            this.sPlayer1 = loaded.sPlayer1;
            this.sPlayer2 = loaded.sPlayer2;

            for (Piece piece : board.getPieces()) {
                piece.setBoard(board);
            }

        } else {
            board = new Board();
            board.initializeBoard();

            for (Piece piece : board.getPieces()) {
                piece.setBoard(board);
            }

            cCurrentPlayer = PieceColor.WHITE;
            cWinner = null;
        }
    }


    /**
     * Saves the current game state to a serialized file.
     *
     * @param sFileName the name of the file to save to
     * @throws IOException if an I/O error occurs
     */
    public void save(String sFileName) throws IOException {
        ChessGameSerialization.save(sFileName, this);
    }

    /**
     * Restarts the game, reinitializing the board and setting the current player to white.
     */
    public void restart() {
        cCurrentPlayer = PieceColor.WHITE;
        cWinner = null;
        board = new Board();
        board.initializeBoard();
    }

    /*public void showAllPossiveMoves() {
        for (Piece piece : board.getPieces()) {
            if (piece.getColor() == cCurrentPlayer) {
                System.out.println("Piece " + piece.getColor() + " " + piece.getPosition());
                for (Position move : piece.getPossibleMoves()) {
                    System.out.println("Move " + move.toString());
                }
            }
        }
    }*/
}