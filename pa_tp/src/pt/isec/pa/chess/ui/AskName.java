package pt.isec.pa.chess.ui;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import javafx.stage.Modality;
import pt.isec.pa.chess.model.ChessGameManager;

public class AskName extends Stage {
    ChessGameManager data;
    TextField tfPlayer1, tfPlayer2;
    Button btnConfirm, btnCancel;

    public AskName(ChessGameManager data) {
        this.data = data;
        this.initModality(Modality.APPLICATION_MODAL);
        this.setTitle("Player Names");
        createViews();
        registerHandlers();
        update();
    }

    private void createViews() {
        Label lblPlayer1 = new Label("White Player:");
        tfPlayer1 = new TextField();
        tfPlayer1.setPrefWidth(200);

        Label lblPlayer2 = new Label("Black Player:");
        tfPlayer2 = new TextField();
        tfPlayer2.setPrefWidth(200);

        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.add(lblPlayer1, 0, 0);
        grid.add(tfPlayer1, 1, 0);
        grid.add(lblPlayer2, 0, 1);
        grid.add(tfPlayer2, 1, 1);

        btnConfirm = new Button("Start Game");
        btnCancel = new Button("Cancel");

        HBox btns = new HBox(10, btnCancel, btnConfirm);
        btns.setAlignment(Pos.CENTER_RIGHT);

        VBox root = new VBox(15, grid, btns);
        root.setPadding(new Insets(20));

        Scene scene = new Scene(root);
        this.setScene(scene);
        this.setResizable(false);
    }

    private void registerHandlers() {
        btnCancel.setOnAction(e -> this.close());
        btnConfirm.setOnAction(e -> {
            String player1Name = tfPlayer1.getText().trim();
            String player2Name = tfPlayer2.getText().trim();

            if (player1Name.isEmpty() || player2Name.isEmpty()) {

                Alert alert = new Alert(Alert.AlertType.ERROR);
                alert.setTitle("Invalid Names");
                alert.setHeaderText("Player names are required");
                alert.setContentText("Please enter names for both players.");
                alert.showAndWait();
                return;
            }

            //data.restart();
            if (player1Name.equals(player2Name)) {
                player2Name = player2Name.concat("2");
            }
            data.setPlayerName(1, player1Name);
            data.setPlayerName(2, player2Name);
            this.close();
        });

        tfPlayer2.setOnAction(e -> btnConfirm.fire());

        tfPlayer1.setOnAction(e -> tfPlayer2.requestFocus());
    }

    private void update() {
    }
}
