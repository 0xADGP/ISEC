package pt.isec.pa.chess.ui;

import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import pt.isec.pa.chess.model.ChessGameManager;
import pt.isec.pa.chess.ui.res.ImageManager;
import javafx.scene.image.Image;
import pt.isec.pa.chess.ui.res.SoundManager;

import java.util.*;
import static java.lang.Double.min;


public class CanvasBoard extends Canvas {
    private final ChessGameManager data;
    private final int numRows;
    private final int numCols;
    private String selectedPos = null;
    private final RootPane parent;
    private Set<String> possibleMoves = new HashSet<>();

    public CanvasBoard(ChessGameManager data, RootPane parent) {
        this.data = data;
        this.numRows = data.getBoardRows();
        this.numCols = data.getBoardCols();
        this.parent = parent;
        createViews();
        registerHandlers();
        update();
    }

    private void createViews() {
    }

    private void registerHandlers() {
        data.addPropertyChangeListener(ChessGameManager.PROP_BOARD_UPDATE, evt -> {
            if (evt.getOldValue() instanceof String oldGame && evt.getNewValue() instanceof String newGame) {
                String[] oldGameTokens = oldGame.trim().split(",");
                String[] newGameTokens = newGame.trim().split(",");
                Character pieceXd = null;
                String fromXd = null, toXd = null;

                Arrays.sort(oldGameTokens);
                Arrays.sort(newGameTokens);

                Map<Character, Integer> oldCounts = new HashMap<>();
                for (String token : oldGameTokens) {
                    if (!token.isEmpty()) {
                        char piece = Character.toUpperCase(token.charAt(0));
                        oldCounts.put(piece, oldCounts.getOrDefault(piece, 0) + 1);
                    }
                }

                Map<Character, Integer> newCounts = new HashMap<>();
                for (String token : newGameTokens) {
                    if (!token.isEmpty()) {
                        char piece = Character.toUpperCase(token.charAt(0));
                        newCounts.put(piece, newCounts.getOrDefault(piece, 0) + 1);
                    }
                }

                for (Map.Entry<Character, Integer> entry : newCounts.entrySet()) {
                    char piece = entry.getKey();
                    int newCount = entry.getValue();
                    int oldCount = oldCounts.getOrDefault(piece, 0);
                    if (piece != 'P' && newCount > oldCount) {
                        SoundManager.play("promotion.mp3");
                        return;
                    }
                }

                for (int i = 0; i < Math.min(oldGameTokens.length, newGameTokens.length); i++) {
                    if (!oldGameTokens[i].equals(newGameTokens[i])) {
                        fromXd = oldGameTokens[i];
                        toXd = newGameTokens[i];
                        pieceXd = oldGameTokens[i].charAt(0);
                        break;
                    }
                }

                if (pieceXd != null && fromXd != null && toXd != null) {
                    Character upperPieceXd = Character.toUpperCase(pieceXd);

                    if (upperPieceXd == 'K') {
                        char fromCol = fromXd.charAt(1);
                        char toCol = toXd.charAt(1);
                        int colDiff = Math.abs(toCol - fromCol);
                        if (colDiff == 2) {
                            SoundManager.play("castling.mp3");
                            return;
                        }
                    }

                    SoundManager.playSequence(pieceXd + fromXd.substring(1) + toXd.substring(1));
                }
            }

            update();
        });

        widthProperty().addListener((obs, oldVal, newVal) -> update());
        heightProperty().addListener((obs, oldVal, newVal) -> update());
        this.setOnMouseClicked(this::handleClick);
    }

    private void update() {
        GraphicsContext gc = getGraphicsContext2D();
        gc.clearRect(0, 0, getWidth(), getHeight());
        drawBoard(gc);
        drawPieces(gc);
    }

    private void handleClick(MouseEvent e) {
        possibleMoves.clear();
        double sizeUser = min(getWidth(), getHeight());
        double offsetX = (getWidth() - sizeUser) / 2;
        double offsetY = (getHeight() - sizeUser) / 2;
        double marginX = sizeUser * 0.08;
        double marginY = sizeUser * 0.08;
        double boardWidth = sizeUser - 2 * marginX;
        double boardHeight = sizeUser - 2 * marginY;
        double cellWidth = boardWidth / numCols;
        double cellHeight = boardHeight / numRows;
        double mouseX = e.getX();
        double mouseY = e.getY();

        if (mouseX < offsetX + marginX || mouseX > offsetX + marginX + boardWidth || mouseY < offsetY + marginY || mouseY > offsetY + marginY + boardHeight) {
            //ModelLog.getInstance().log("Click outside the board area.");
            return;
        }

        int col = (int) ((mouseX - offsetX - marginX) / cellWidth);
        int row = (int) ((mouseY - offsetY - marginY) / cellHeight);

        char colLetter = (char) ('a' + col);
        int boardRow = numRows - row;

        String clicked = String.valueOf(colLetter) + boardRow;

        if (selectedPos == null) {
            selectedPos = clicked;

            if (parent.isLearningModeActive() && parent.isShowMovesActive()) {
                String movesStr = data.getPossibleMovesFromPieceAt(selectedPos);
                possibleMoves = new HashSet<>(Arrays.asList(movesStr.replaceAll("[\\[\\]\\s]", "").split(",")));
            } else {
                possibleMoves.clear();
            }
            update();
        } else {

            if (data.willNeedPromotion(selectedPos, clicked)) {
                //System.out.println("[DEBUG] Promotion triggered for move: " + selectedPos + " -> " + clicked);
                char chosen = parent.showPromotionDialog();
                boolean moved = data.movePiece(selectedPos, clicked);
                if (moved) {
                    data.promotePawn(clicked, chosen);
                    update();
                }
            } else {
                data.movePiece(selectedPos, clicked);
            }

            selectedPos = null;
            possibleMoves.clear();
            update();
        }
    }


    private void drawBoard(GraphicsContext gc) {
        double sizeUser = min(getWidth(), getHeight());
        double offsetX = (getWidth() - sizeUser) / 2;
        double offsetY = (getHeight() - sizeUser) / 2;
        double marginX = sizeUser * 0.08;
        double marginY = sizeUser * 0.08;
        double boardWidth = sizeUser - 2 * marginX;
        double boardHeight = sizeUser - 2 * marginY;
        double cellWidth = boardWidth / numCols;
        double cellHeight = boardHeight / numRows;

        gc.setFont(Font.font(cellHeight * 0.4));
        Color lightColor = Color.web("#fceee9");
        Color darkColor = Color.web("#944b45");

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                boolean Light = (row + col) % 2 == 0;
                double x = offsetX + marginX + col * cellWidth;
                double y = offsetY + marginY + row * cellHeight;
                gc.setFill(Light ? lightColor : darkColor);
                gc.fillRect(x, y, cellWidth, cellHeight);

                String pos = "" + (char) ('a' + col) + (numRows - row);
                if (possibleMoves.contains(pos)) {
                    double circleRadius = Math.min(cellWidth, cellHeight) * 0.2;
                    double cx = x + cellWidth / 2;
                    double cy = y + cellHeight / 2;

                    gc.setFill(Color.web("#88888888"));
                    gc.fillOval(cx - circleRadius, cy - circleRadius, circleRadius * 2, circleRadius * 2);
                }
            }
        }

        for (int col = 0; col < numCols; col++) {
            char colLabel = (char) ('a' + col);
            double x = offsetX + marginX + col * cellWidth + cellWidth / 2 - 5;
            double y = offsetY + marginY + boardHeight + cellHeight * 0.6;
            gc.setFill(Color.BLACK);
            gc.fillText(String.valueOf(colLabel), x, y);
        }

        for (int row = 0; row < numRows; row++) {
            String rowLabel = String.valueOf(numRows - row);
            double y = offsetY + marginY + row * cellHeight + cellHeight / 2 + 5;
            gc.fillText(rowLabel, offsetX + marginX - cellWidth * 0.4, y);
            gc.fillText(rowLabel, offsetX + marginX + boardWidth + cellWidth * 0.1, y);
        }
        drawPieces(gc);
    }

    private void drawPieces(GraphicsContext gc) {
        double sizeUser = min(getWidth(), getHeight());
        double offsetX = (getWidth() - sizeUser) / 2;
        double offsetY = (getHeight() - sizeUser) / 2;
        double marginX = sizeUser * 0.08;
        double marginY = sizeUser * 0.08;
        double boardWidth = sizeUser - 2 * marginX;
        double boardHeight = sizeUser - 2 * marginY;
        double cellWidth = boardWidth / numCols;
        double cellHeight = boardHeight / numRows;

        String[] tokens = data.getBoardPiecesString().split(",");

        for (String token : tokens) {
            token = token.trim();
            if (token.length() < 3) continue;

            char type = token.charAt(0);
            boolean isWhite = Character.isUpperCase(type);
            String posStr = token.substring(1, 3).toLowerCase();
            int col = posStr.charAt(0) - 'a';
            int row = numRows - Character.getNumericValue(posStr.charAt(1));

            String pieceName = switch (Character.toUpperCase(type)) {
                case 'K' -> "king";
                case 'Q' -> "queen";
                case 'R' -> "rook";
                case 'B' -> "bishop";
                case 'N' -> "knight";
                case 'P' -> "pawn";
                default -> null;
            };

            if (pieceName != null) {
                Image img = ImageManager.getImage("pieces/" + pieceName + (isWhite ? "W" : "B") + ".png");
                if (img != null) {
                    double x = offsetX + marginX + col * cellWidth + cellWidth * 0.1;
                    double y = offsetY + marginY + row * cellHeight + cellHeight * 0.1;
                    gc.drawImage(img, x, y, cellWidth * 0.8, cellHeight * 0.8);
                }
            }
        }
    }
}
