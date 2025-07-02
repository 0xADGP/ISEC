package pt.isec.pa.chess.model;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import pt.isec.pa.chess.model.data.pieces.PieceColor;

import java.io.File;

import static org.junit.jupiter.api.Assertions.*;

public class ChessGameTest {
    private ChessGameManager gameManager;

    @BeforeEach
    void setUp() {
        gameManager = new ChessGameManager();
    }

    @Test
    void testBoardInitialization() {
        assertEquals(32, gameManager.getBoardPiecesString().split(",").length, "Board should have 32 pieces after default initialization");

        String boardState = gameManager.getBoardPiecesString();
        System.out.println("Initial board state: " + boardState);
        assertTrue(boardState.contains("Ke1"), "White king should be at e1");
        assertTrue(boardState.contains("pd7"), "Black pawn should be at d7");
    }

    @Test
    void testPieceMovement() {
        boolean moveResult = gameManager.movePiece("e2", "e4");
        System.out.println("Move e2 to e4 result: " + moveResult);
        assertTrue(moveResult, "Pawn move from e2 to e4 should be successful");

        String boardState = gameManager.getBoardPiecesString();
        System.out.println("Board state after move: " + boardState);
        assertTrue(boardState.contains("Pe4"), "Pawn should be at e4 after move");
        assertFalse(boardState.contains("Pe2"), "Pawn should no longer be at e2");
        assertEquals(PieceColor.BLACK, gameManager.getcCurrentPlayer(), "Current player should switch to black");
    }

    @Test
    void testCheckmateDetection() {
        boolean move1 = gameManager.movePiece("f2", "f4");
        System.out.println("Move f2 to f4: " + move1);
        boolean move2 = gameManager.movePiece("e7", "e5");
        System.out.println("Move e7 to e5: " + move2);
        boolean move3 = gameManager.movePiece("g2", "g4");
        System.out.println("Move g2 to g4: " + move3);
        boolean move4 = gameManager.movePiece("d8", "h4");
        System.out.println("Move d8 to h4: " + move4);
        String boardState = gameManager.getBoardPiecesString();
        System.out.println("Board state after moves: " + boardState);

        boolean isCheckmate = gameManager.bIsCheckmate(PieceColor.WHITE);
        System.out.println("Is checkmate: " + isCheckmate);
        assertTrue(isCheckmate, "White should be in checkmate");
        assertEquals(PieceColor.BLACK, gameManager.getWinner(), "Black should be the winner");
        assertTrue(gameManager.isGameOver(), "Game should be over");
    }

    @Test
    void testUndoFunctionality() {
        String initialState = gameManager.getBoardPiecesString();
        gameManager.movePiece("e2", "e4");
        assertTrue(gameManager.hasUndo(), "Undo should be available after a move");

        gameManager.undo();
        String afterUndoState = gameManager.getBoardPiecesString();
        assertEquals(initialState, afterUndoState, "Board state should match initial state after undo");
        assertEquals(PieceColor.WHITE, gameManager.getcCurrentPlayer(), "Current player should revert to white");
    }

    @Test
    void testPawnPromotion() {
        gameManager.cgGame.loadBoardStateFromString("Pe7,Ke1,ke5,pd7", PieceColor.WHITE);
        String boardState = gameManager.getBoardPiecesString();
        System.out.println("Board state after load: " + boardState);

        boolean needsPromotion = gameManager.willNeedPromotion("e7", "e8");
        System.out.println("Will need promotion e7 to e8: " + needsPromotion);
        assertTrue(needsPromotion, "Pawn move to e8 should require promotion");

        boolean moveResult = gameManager.movePiece("e7", "e8");
        System.out.println("Move e7 to e8 result: " + moveResult);
        assertTrue(moveResult, "Pawn move to e8 should be successful");

        gameManager.promotePawn("e8", 'Q');
        boardState = gameManager.getBoardPiecesString();
        System.out.println("Board state after promotion: " + boardState);
        assertTrue(boardState.contains("Qe8"), "Pawn should be promoted to queen at e8");
    }

    @Test
    void testGameSerialization() {
        gameManager.setPlayerName(1, "Sigma");
        gameManager.setPlayerName(2, "Beta");
        boolean moveResult = gameManager.movePiece("e2", "e4");
        System.out.println("Move e2 to e4: " + moveResult);
        String boardState = gameManager.getBoardPiecesString();
        System.out.println("SBoard state: " + boardState);

        String tempFile = "test_game.dat";
        gameManager.save(tempFile);

        boolean moveResult2 = gameManager.movePiece("e7", "e6");
        System.out.println("Move e7 to e6: " + moveResult2);

        boolean moveResult3 = gameManager.movePiece("e4", "e5");
        System.out.println("Move e4 to e5: " + moveResult3);

        ChessGameManager newGameManager = new ChessGameManager();
        newGameManager.open(tempFile);
        String loadedBoardState = newGameManager.getBoardPiecesString();
        System.out.println("LBoard state: " + loadedBoardState);

        assertEquals("Sigma", newGameManager.getPlayerName(PieceColor.WHITE), "Player 1 name should be Sigma");
        assertEquals("Beta", newGameManager.getPlayerName(PieceColor.BLACK), "Player 2 name should be Beta");
        assertTrue(loadedBoardState.contains("Pe4"), "Pawn should be at e4 in loaded game");
        assertEquals(PieceColor.BLACK, newGameManager.getcCurrentPlayer(), "Current player should be black");

        new File(tempFile).delete();
    }
}