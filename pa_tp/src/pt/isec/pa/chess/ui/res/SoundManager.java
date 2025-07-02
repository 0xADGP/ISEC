package pt.isec.pa.chess.ui.res;

import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.util.Duration;

import java.util.List;

public class SoundManager {
    private static MediaPlayer mp;
    private static boolean enabled = false;

    private SoundManager() {
    }

    public static void setEnabled(boolean value) {
        enabled = value;
    }

    public static boolean play(String filename) {
        if (!enabled) return false;
        try {
            var url = SoundManager.class.getResource("sounds/en/" + filename);
            if (url == null) return false;
            String path = url.toExternalForm();
            Media music = new Media(path);
            stop();
            mp = new MediaPlayer(music);
            mp.setStartTime(Duration.ZERO);
            mp.setStopTime(music.getDuration());
            mp.setAutoPlay(true);
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    public static void stop() {
        if (mp != null) {
            mp.stop();
            mp.dispose();
            mp = null;
        }
    }

    public static void playSequence(String move) {
        if (!enabled) return;
        if (move == null || move.length() < 5) return;

        char pieceChar = move.charAt(0);
        String fromCol = String.valueOf(move.charAt(1));
        String fromRow = String.valueOf(move.charAt(2));
        String toCol = String.valueOf(move.charAt(3));
        String toRow = String.valueOf(move.charAt(4));

        String pieceSound = switch (Character.toLowerCase(pieceChar)) {
            case 'k' -> "king.mp3";
            case 'q' -> "queen.mp3";
            case 'r' -> "rook.mp3";
            case 'b' -> "bishop.mp3";
            case 'n' -> "knight.mp3";
            case 'p' -> "pawn.mp3";
            default -> null;
        };

        List<String> sounds = new java.util.ArrayList<>();
        if (pieceSound != null) sounds.add(pieceSound);
        sounds.add(fromCol + ".mp3");
        sounds.add(fromRow + ".mp3");
        sounds.add(toCol + ".mp3");
        sounds.add(toRow + ".mp3");

        playNextInSequence(sounds, 0);
    }

    private static void playNextInSequence(List<String> sounds, int index) {
        if (index >= sounds.size()) return;

        String filename = sounds.get(index);
        var url = SoundManager.class.getResource("sounds/en/" + filename);
        if (url == null) {
            playNextInSequence(sounds, index + 1);
            return;
        }

        String path = url.toExternalForm();
        Media music = new Media(path);
        stop();
        mp = new MediaPlayer(music);
        mp.setOnEndOfMedia(() -> {
            stop();
            playNextInSequence(sounds, index + 1);
        });
        mp.setAutoPlay(true);
    }
}
