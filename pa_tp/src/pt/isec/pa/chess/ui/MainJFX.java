package pt.isec.pa.chess.ui;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import pt.isec.pa.chess.model.ChessGameManager;

public class MainJFX extends Application {
    private ChessGameManager gameManager;
    private RootPane root;
    private Stage logStage;
    //private Stage NewStage;

    @Override
    public void init() throws Exception {
        super.init();
        gameManager = new ChessGameManager();
    }

    @Override
    public void start(Stage stage) {
        createMainStage(stage);
        /*NewStage = new Stage();
        createMainStage(NewStage);
        */

        logStage = new Stage();
        createLogStage(logStage, stage.getX() + stage.getWidth(), stage.getY());

        stage.setOnCloseRequest(e -> logStage.close());
        logStage.setOnCloseRequest(e -> stage.close());
    }

    private void createMainStage(Stage stage) {
        root = new RootPane(gameManager);
        Scene scene = new Scene(root, 600, 700);
        stage.setScene(scene);
        stage.setTitle("PA Chess Game");
        stage.setResizable(true);
        stage.show();
    }

    private void createLogStage(Stage stage, double x, double y) {
        LogList logList = new LogList();
        Scene scene = new Scene(logList, 300, 400);
        stage.setScene(scene);
        stage.setTitle("Chess Logs");
        stage.setX(x);
        stage.setY(y);
        stage.show();

        root.setLogWindow(logList);
    }
}
