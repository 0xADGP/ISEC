package pt.isec.pa.chess.model;

import pt.isec.pa.chess.model.data.pieces.PieceColor;
import pt.isec.pa.chess.model.memento.CareTaker;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.*;
import java.util.Set;

/**
 * Facade class that manages the core functionality of the chess game.
 * Handles user interactions such as moving pieces, saving/loading games,
 * importing/exporting partial games, and maintaining game state.
 * Uses PropertyChangeSupport to notify UI components of updates.
 *
 * @author Antonio Pedorso, Luis Duarte, Carolina Veloso
 * @version 1.0.0
 * @see ChessGame
 * @see ModelLog
 */

public class ChessGameManager {
    public static final String PROP_BOARD_UPDATE = "boardUpdate";
    public static final String PROP_GAME_STATE = "gameState";
    private String oldGameState = null;

    ChessGame cgGame;
    private CareTaker careTaker;
    private PropertyChangeSupport pcs;


    /**
     * Constructs a new ChessGameManager instance.
     * Initializes the internal ChessGame object, sets up the CareTaker for undo/redo support,
     * and prepares the property change support for UI notification.
     */
    public ChessGameManager() {
        cgGame = new ChessGame();
        careTaker = new CareTaker(cgGame);
        pcs = new PropertyChangeSupport(this);
    }

    /**
     * Adds piece.
     */
    public boolean addPiece(String sPiece) {
        try {
            boolean worked = cgGame.addPiece(sPiece);
            pcs.firePropertyChange(PROP_BOARD_UPDATE, null, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
            return worked;
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error adding piece: " + ex.getMessage());
            return false;
        }
    }

    /**
     * Adds a listener for property change events.
     *
     * @param property Property name to listen to
     * @param listener Listener to be notified
     */
    public void addPropertyChangeListener(String property, PropertyChangeListener listener) {
        pcs.addPropertyChangeListener(property, listener);
    }

    /**
     * Saves the current game state to a file using serialization.
     *
     * @param sFileName The file path to save the game
     */
    public void save(String sFileName) {
        try {
            cgGame.save(sFileName);
            ModelLog.getInstance().log("Game saved to: " + sFileName);
        } catch (IOException e) {
            ModelLog.getInstance().log("Error saving game: " + e.getMessage());
        }
    }

    /**
     * Imports a partial game from a formatted file.
     *
     * @param sFileName The file to import
     */
    public void importa(String sFileName) {
        try {
            careTaker.reset();
            oldGameState = cgGame.getBoardPiecesString();
            cgGame.importa(sFileName);
            //careTaker.save();
            pcs.firePropertyChange(PROP_BOARD_UPDATE, null, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
            ModelLog.getInstance().log("Game imported from: " + sFileName);
        } catch (IOException | RuntimeException e) {
            ModelLog.getInstance().log("Error importing game: " + e.getMessage());
        }
    }

    /**
     * Exports the current game state to a formatted file.
     *
     * @param sFileName The destination file name
     */
    public void exporta(String sFileName) {
        try {
            cgGame.exporta(sFileName);
            ModelLog.getInstance().log("Game exported to: " + sFileName);
        } catch (IOException | RuntimeException e) {
            ModelLog.getInstance().log("Error exporting game: " + e.getMessage());
        }
    }

    /**
     * Opens a previously saved game.
     *
     * @param sFileName File to load from
     */
    public void open(String sFileName) {
        try {
            careTaker.reset();
            oldGameState = cgGame.getBoardPiecesString();
            cgGame.open(sFileName);
            //careTaker.save();
            pcs.firePropertyChange(PROP_BOARD_UPDATE, null, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
            ModelLog.getInstance().log("Game loaded from: " + sFileName);
        } catch (IOException | ClassNotFoundException e) {
            ModelLog.getInstance().log("Error opening game: " + e.getMessage());
        }
    }


    /**
     * Moves a piece from one position to another.
     *
     * @param SoldPos Source position (e.g., "e2")
     * @param SnewPos Destination position (e.g., "e4")
     * @return true if the move was successful, false otherwise
     */
    public boolean movePiece(String SoldPos, String SnewPos) {
        try {
            oldGameState = cgGame.getBoardPiecesString();
            careTaker.save();
            boolean pieceMoved = cgGame.movePiece(SoldPos, SnewPos);
            if (!pieceMoved) {
                careTaker.removeLast();
            }
            pcs.firePropertyChange(PROP_BOARD_UPDATE, oldGameState, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
            return pieceMoved;
        } catch (IllegalArgumentException ex) {
            ModelLog.getInstance().log("Error: " + ex.getMessage());
            return false;
        }
    }

    /**
     * Checks whether a pawn move from the given position to the destination would result in a promotion.
     *
     * @param from the starting position of the piece (e.g., "e7")
     * @param to   the destination position (e.g., "e8")
     * @return true if the move leads to a promotion, false otherwise
     */
    public boolean willNeedPromotion(String from, String to) {
        return cgGame.willNeedPromotion(from, to);
    }

    /**
     * Promotes a pawn at the specified position to the selected type.
     *
     * @param position  Position of the pawn
     * @param pieceType Piece type to promote to (Q, R, B, N)
     */
    public void promotePawn(String position, char pieceType) {
        try {
            oldGameState = cgGame.getBoardPiecesString();
            //careTaker.save();
            cgGame.promotePawn(position, pieceType);
            pcs.firePropertyChange(PROP_BOARD_UPDATE, oldGameState, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
        } catch (IllegalArgumentException ex) {
            ModelLog.getInstance().log("Error promoting pawn: " + ex.getMessage());
        }
    }

    /**
     * Starts an empty game.
     */
    public void startEmpty() {
        try {
            cgGame.loadEmptyGame();
            careTaker.reset();
            pcs.firePropertyChange(PROP_BOARD_UPDATE, null, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error starting empty game: " + ex.getMessage());
        }
    }

    /**
     * Resets the game to the initial state.
     */
    public void restart() {
        try {
            cgGame.restart();
            careTaker.reset();
            pcs.firePropertyChange(PROP_BOARD_UPDATE, null, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error restarting game: " + ex.getMessage());
        }
    }

    /**
     * Sets the name of a player.
     *
     * @param player Player number (1 or 2)
     * @param name   Player's name
     */
    public void setPlayerName(int player, String name) {
        try {
            cgGame.setPlayerName(player, name);
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
            ModelLog.getInstance().log("Player " + player + " set to: " + name);
        } catch (IllegalArgumentException ex) {
            ModelLog.getInstance().log("Error setting player name:: " + ex.getMessage());
        }
    }

    /**
     * Gets the name of the player of the specified color.
     *
     * @param eColor Piece color
     * @return Player's name
     */
    public String getPlayerName(PieceColor eColor) {
        try {
            return cgGame.getPlayerName(eColor);
        } catch (IllegalArgumentException ex) {
            ModelLog.getInstance().log("Error getting player name: " + ex.getMessage());
        }
        return null;
    }

    /**
     * Gets the possible moves for the piece at the given position.
     *
     * @param pos Board position
     * @return List of possible target positions
     */
    public String getPossibleMovesFromPieceAt(String pos) {
        try {
            Set<String> moves = cgGame.getPossibleMovesFromPieceAt(pos);
            return moves.toString();
        } catch (IllegalArgumentException ex) {
            ModelLog.getInstance().log("Error getting possible moves: " + ex.getMessage());
            return "[]";
        }
    }


    /**
     * Returns a string representation of the current board.
     *
     * @return Board string
     */
    public String getBoardPiecesString() {
        try {
            return cgGame.getBoardPiecesString();
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error getting board pieces: " + ex.getMessage());
            return "";
        }
    }

    /**
     * Returns the color of the current player.
     *
     * @return Current player's color
     */
    public PieceColor getcCurrentPlayer() {
        try {
            return cgGame.getcCurrentPlayer();
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error getting current player: " + ex.getMessage());
            return null;
        }
    }

    /**
     * Returns the color of the opponent player.
     *
     * @return Opponent player's color
     */
    public PieceColor getcOpponent() {
        try {
            return cgGame.getcOpponent();
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error getting opponent player: " + ex.getMessage());
            return null;
        }
    }

    /**
     * Checks whether the specified player is in checkmate.
     *
     * @param color Player color
     * @return true if checkmate, false otherwise
     */
    public boolean bIsCheckmate(PieceColor color) {
        try {
            return cgGame.bIsCheckmate(color);
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error checking checkmate: " + ex.getMessage());
            return false;
        }
    }

    /**
     * Returns the winner of the game, if any.
     *
     * @return Winner's color
     */
    public PieceColor getWinner() {
        try {
            return cgGame.getWinner();
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error getting winner: " + ex.getMessage());
            return null;
        }
    }

    /**
     * Checks if the game is over.
     *
     * @return true if game over, false otherwise
     */
    public boolean isGameOver() {
        try {
            return cgGame.isGameOver();
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error checking game over: " + ex.getMessage());
            return false;
        }
    }

    /**
     * Returns debug info about the game.
     *
     * @return Debug string
     */
    public String debug() {
        StringBuilder sb = new StringBuilder();
        sb.append(cgGame.sPlayer1).append(",");
        sb.append(cgGame.sPlayer2).append(",");
        sb.append(cgGame.cCurrentPlayer).append(",");
        sb.append(cgGame.getBoardPiecesString());
        return sb.toString();
    }

    /**
     * Gets the board column count.
     *
     * @return Number of columns
     */
    public int getBoardCols() {
        try {
            return cgGame.getBoardSize();
        } catch (RuntimeException e) {
            ModelLog.getInstance().log("Error getting board columns: " + e.getMessage());
        }
        return -1;
    }

    /**
     * Gets the board row count.
     *
     * @return Number of rows
     */
    public int getBoardRows() {
        try {
            return cgGame.getBoardSize();
        } catch (RuntimeException e) {
            ModelLog.getInstance().log("Error getting board rows: " + e.getMessage());
        }
        return -1;
    }


    /**
     * Undoes the last move.
     *
     * @return true if undo was successful
     */
    public boolean undo() {
        try {
            if (careTaker.hasUndo()) {
                careTaker.undo();
                ModelLog.getInstance().log("Undo performed");
            }
            pcs.firePropertyChange(PROP_BOARD_UPDATE, null, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
            return true;
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error undoing move: " + ex.getMessage());
            return false;
        }
    }

    /**
     * Redoes the last undone move.
     *
     * @return true if redo was successful
     */
    public boolean redo() {
        try {
            if (careTaker.hasRedo()) {
                careTaker.redo();
                ModelLog.getInstance().log("Redo performed");
            }
            pcs.firePropertyChange(PROP_BOARD_UPDATE, null, cgGame.getBoardPiecesString());
            pcs.firePropertyChange(PROP_GAME_STATE, null, null);
            return true;
        } catch (Exception ex) {
            ModelLog.getInstance().log("Error redoing move: " + ex.getMessage());
            return false;
        }
    }

    /**
     * Checks if an undo operation is available.
     *
     * @return true if undo is available
     */
    public boolean hasUndo() {
        return careTaker.hasUndo();
    }

    /**
     * Checks if a redo operation is available.
     *
     * @return true if redo is available
     */
    public boolean hasRedo() {
        return careTaker.hasRedo();
    }
}
