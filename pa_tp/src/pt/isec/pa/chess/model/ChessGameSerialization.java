package pt.isec.pa.chess.model;

import com.sun.webkit.Timer;

import java.io.*;

public class ChessGameSerialization {
    @Serial
    static final long serialVersionUID = 1L;

    private ChessGameSerialization() {
    }

    public static void save(String filename, ChessGame o) {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(filename))) {
            oos.writeObject(o);
        } catch (Exception e) {
            //System.err.println("Erro ao guardar o jogo");
            ModelLog.getInstance().log("Erro ao guardar o jogo");
        }
    }

    public static ChessGame load(String filename) {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(filename))) {
            return (ChessGame) ois.readObject();
        } catch (Exception e) {
            //System.err.println("Erro ao carregar o jogo");
            ModelLog.getInstance().log("Erro ao carregar o jogo");
        }
        return null;
    }
}
