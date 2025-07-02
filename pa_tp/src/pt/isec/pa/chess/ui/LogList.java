package pt.isec.pa.chess.ui;

import javafx.scene.control.ListView;

import javafx.geometry.Insets;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import pt.isec.pa.chess.model.ModelLog;

public class LogList extends BorderPane {
    private ListView<String> listView;

    public LogList() {
        createViews();
        registerHandlers();
        update();
    }

    private void createViews() {
        listView = new ListView<>();
        this.setCenter(listView);

        Button btnClear = new Button("Clear Logs");
        btnClear.setOnAction(e -> {
            ModelLog.getInstance().reset();
            listView.getItems().clear();
        });

        HBox bottomBox = new HBox(btnClear);
        bottomBox.setPadding(new Insets(5));
        this.setBottom(bottomBox);
    }

    private void registerHandlers() {
        ModelLog.getInstance().addPropertyChangeListener(ModelLog.PROP_NEW_LOG, evt -> {
            listView.getItems().add((String) evt.getNewValue());
        });
    }

    private void update() {
    }

    public void log(String message) {
        listView.getItems().add(message);
    }

    public void clearLogs() {
        listView.getItems().clear();
    }
}
